
#include "stdafx.h"

#include <Wininet.h>
#include <io.h>
#include <process.h>
#include <direct.h>
#include <sys/utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <nel/misc/debug.h>
#include <nel/misc/path.h>

#include "server.h"
#include "wpkg_sync.h"
#include "wpkg_sync_dlg.h"

using namespace std;
using namespace NLMISC;

static HINTERNET RootInternet = 0;
static HINTERNET FtpInternet = 0;


void throwError(const string &head);
void uploadFile(const string &source, const string &dest);
void downloadFile(const string &source, const string &dest);

CPackageSum::CPackageSum(string name, string version, string description, string maintainer)
{
	Name = name;
	Version = version;
	time(&Date);
	Description = description;
	Maintainer = maintainer;
	Init = true;
}
	
CPackageSum::CPackageSum(string raw)
{
	vector<string> res;
	explode(raw, "|", res);
	nlassert(res.size() == 5);
	Name = res[0];
	Version = res[1];
	Date = atoi(res[2].c_str());
	Description = res[3];
	Maintainer = res[4];
	Init = true;
}

string CPackageSum::toString()
{
	nlassert(Init);
	return NLMISC::toString("%s|%s|%u|%s|%s", Name.c_str(), Version.c_str(), Date, Description.c_str(), Maintainer.c_str());
}



// ftp://login:password@server/path
void decodeURI(const string &uri, string &server, string &login, string &password, string &path)
{
	server = "";
	login = "";
	password = "";
	path = "";
	
	if(uri.empty())
		nlerror("empty uri");

	if(uri.find("ftp://") != 0)
		nlerror("uri must start with ftp://");

	string::size_type posserv = 6;

	string::size_type posat = uri.find("@", 6);
	if(posat != string::npos)
	{
		string::size_type pos2p = uri.find(":", 6);
		if(pos2p != string::npos)
		{
			// there's a login and pass
			login = uri.substr(6, pos2p - 6);
			password = uri.substr(pos2p + 1, posat - pos2p - 1);
		}
		else
		{
			// there's only a login
			login = uri.substr(6, posat - 6);
		}
		posserv = posat+1;
	}
	
	string::size_type posslash = uri.find("/", posserv);
	if(posslash != string::npos)
	{
		server = uri.substr(posserv, posslash - posserv);
		path = uri.substr(posslash + 1);
	}
	else
	{
		server = uri.substr(posserv);
	}

//	nlinfo("'%s' '%s' '%s' '%s' ", uri.c_str(), server.c_str(), login.c_str(), path.c_str());
}

void goToSumDirectory()
{
	if(FtpSetCurrentDirectory(FtpInternet, "sum") == FALSE)
	{
		nlinfo("sum directory doesn't exist, create it");
		if(FtpCreateDirectory(FtpInternet, "sum") == FALSE)
		{
			throwError ("FtpCreateDirectory()");
		}
		if(FtpSetCurrentDirectory(FtpInternet, "sum") == FALSE)
		{
			throwError ("FtpSetCurrentDirectory()");
		}
	}
}

void uploadPackage(const string &packageFileName)
{
	if(ConfigFile.getVar("Upload").asString().empty())
		return;

	openSplash("Uploading package '%s'", packageFileName.c_str());

	nlassert(NLMISC::CFile::fileExists(packageFileName));
	string sumFileName(NLMISC::CFile::getPath(packageFileName) + NLMISC::CFile::getFilenameWithoutExtension(packageFileName)+".sum");
	nlassert(NLMISC::CFile::fileExists(sumFileName));
	openConnection(ConfigFile.getVar("Upload").asString());
	uploadFile(packageFileName, NLMISC::CFile::getFilename(packageFileName));
	goToSumDirectory();
	uploadFile(sumFileName, NLMISC::CFile::getFilename(sumFileName));
	closeConnection();
	NLMISC::CFile::deleteFile(packageFileName);
	NLMISC::CFile::deleteFile(sumFileName);

	closeSplash();
	
	updatePackageList();
}

void getPackageList(vector<CPackageSum> &packageNames)
{
	string dest = LocalRepository + "sum";

	vector<string> filenames;
	CPath::getPathContent(dest, true, false, true, filenames);
	for(uint i = 0; i < filenames.size(); i++)
	{
		FILE *fp = fopen(filenames[i].c_str(), "rb");
		if(fp == NULL)
		{
			packageNames.push_back(filenames[i] + " <can't open the summary>");
		}
		else
		{
			string desc = getLine(fp);
			packageNames.push_back(CPackageSum(desc));
			fclose(fp);
		}
	}
}

void updatePackageList()
{
	openSplash("Updating package list...");
	
	openSplash("Getting server directory list...");

	openConnection(ConfigFile.getVar("Source").asString());
	goToSumDirectory();
	WIN32_FIND_DATA wfd;
	HINTERNET res = FtpFindFirstFile(FtpInternet, NULL, &wfd, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD, 0);
	if(res == NULL)
	{
		if(GetLastError() == ERROR_NO_MORE_FILES)
		{
			closeConnection();
			return;
		}
		else
		{
			throwError("FtpFindFirstFile()");
		}
	}

	vector<string> filenames;
	while(true)
	{
		filenames.push_back(wfd.cFileName);
		if(!InternetFindNextFile(res, &wfd))
		{
			if(GetLastError() == ERROR_NO_MORE_FILES)
			{
				break;
			}
			else
			{
				throwError("InternetFindNextFile()");
			}
		}
	}
	InternetCloseHandle(res);

	closeSplash();

	for(uint i = 0; i < filenames.size(); i++)
	{
		openSplash("Getting package information '%s'", filenames[i].c_str());
		string dest = LocalRepository + "sum/" + filenames[i];
		downloadFile(filenames[i], dest);
		closeSplash();
	}

	closeConnection();

	closeSplash();
}

void downloadPackage(const std::string &packageName)
{
	openSplash("Downloading Package '%s'...", packageName.c_str());
	openConnection(ConfigFile.getVar("Source").asString());
	string packageFileName = packageName + ".tgz";
	downloadFile(packageFileName, LocalRepository + "tmp/" + packageFileName);
	closeConnection();
	closeSplash();
}

void openConnection(const string &uri)
{
	string server, login, password, path;
	decodeURI(uri, server, login, password, path);

	if(!RootInternet)
	{
		nldebug("Opening root connection");
		RootInternet = InternetOpen("wpkg_sync", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
		if(!RootInternet)
		{
			closeConnection();
			throwError("InternetOpen()");
		}
	}

	if(!FtpInternet)
	{
		DWORD pass = INTERNET_FLAG_PASSIVE;
		if(ConfigFile.exists("PassiveMode") && ConfigFile.getVar("PassiveMode").asInt() == 0)
			pass = 0;

		nldebug("Opening ftp connection to '%s' with login '%s'", server.c_str(), login.c_str());
		FtpInternet = InternetConnect(RootInternet, server.c_str(),
			INTERNET_DEFAULT_FTP_PORT, login.c_str(), password.c_str(), INTERNET_SERVICE_FTP, pass, 0);

		if(!FtpInternet)
		{
			DWORD res = GetLastError();
			closeConnection();
			throwError("InternetConnect()");
		}
	}

	if(path.empty())
	{
		nldebug("Using default ftp directory");
	}
	else
	{
		nldebug("Setting ftp directory to '%s'", path.c_str());
		if(FtpSetCurrentDirectory(FtpInternet, path.c_str()) == FALSE)
			throwError ("FtpSetCurrentDirectory()");
	}
}

void closeConnection()
{
	nldebug("Closing connection");
	if(FtpInternet)
	{
		InternetCloseHandle(FtpInternet);
		FtpInternet = 0;
	}
}

void downloadFile(const string &source, const string &dest)
{
	nldebug("Downloading '%s' to '%s'", source.c_str(), dest.c_str());

	nlassert(RootInternet != 0);
	nlassert(FtpInternet != 0);

	if(FtpGetFile(FtpInternet, source.c_str(), dest.c_str(), FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 0) == FALSE)
		throwError ("FtpGetFile()");
}

void uploadFile(const string &source, const string &dest)
{
	nldebug("Uploading '%s' to '%s'", source.c_str(), dest.c_str());

	nlassert(RootInternet != 0);
	nlassert(FtpInternet != 0);

	if(FtpPutFile(FtpInternet, source.c_str(), dest.c_str(), FTP_TRANSFER_TYPE_BINARY, 0) == FALSE)
		throwError ("FtpPutFile(): ");
}

void throwError(const string &head)
{
	string res = head + " ";
	TCHAR szTemp[356] = "", *szBuffer=NULL, *szBufferFinal = NULL;
	DWORD  dwIntError , dwLength = 0;
	char strName[256]="";
	
	DWORD dError = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_HMODULE, GetModuleHandle("wininet.dll"),dError,0, (LPSTR)strName,256,NULL);
	wsprintf(szTemp, "error code: %d\nMessage: %s\n", dError, strName);
	
	if(dError == ERROR_INTERNET_EXTENDED_ERROR)
	{
		InternetGetLastResponseInfo(&dwIntError, NULL, &dwLength);
		if(dwLength)
		{
			if(!(szBuffer = (TCHAR *) LocalAlloc(LPTR, dwLength)))
			{
				lstrcat(szTemp, TEXT("Unable to allocate memory to display Internet error code. Error code: "));
				lstrcat(szTemp, TEXT(_itoa(GetLastError(), szBuffer, 10)));
				lstrcat(szTemp, TEXT("\n"));
				throw Exception(szTemp);
			}
			if(!InternetGetLastResponseInfo (&dwIntError, (LPTSTR) szBuffer, &dwLength))
			{
				lstrcat(szTemp, TEXT("Unable to get Internet error. Error code: "));
				lstrcat(szTemp, TEXT(_itoa(GetLastError(), szBuffer, 10)));
				lstrcat(szTemp, TEXT("\n") );
				throw Exception(szTemp);
			}
			if(!(szBufferFinal = (TCHAR *) LocalAlloc ( LPTR,  
				(strlen (szBuffer) +strlen (szTemp) + 1)  ) )  )
			{
				lstrcat(szTemp, TEXT( "Unable to allocate memory. Error code: "));
				lstrcat(szTemp, TEXT(_itoa (GetLastError(), szBuffer, 10)));
				lstrcat(szTemp, TEXT("\n"));
				throw Exception(szTemp);
			}
			lstrcpy(szBufferFinal, szTemp);
			lstrcat(szBufferFinal, szBuffer);
			LocalFree(szBuffer);
			res += szBufferFinal;
			LocalFree(szBufferFinal);
		}
	}
	else
	{
		res += szTemp;
	}
	throw Exception(res);
}
