// wpkg_sync manages libraries installation and update
// Copyright (C) 2004 Ace / Melting Pot
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "std_afx.h"

#include <nel/misc/debug.h>
#include <nel/misc/path.h>

#include <curl/curl.h>

#include "server.h"
#include "wpkg_sync.h"
#include "wpkg_sync_dlg.h"

using namespace std;
using namespace NLMISC;

void createPackageList(vector<CPackageSum> &packageNames);

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


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//struct DlFile
//{
//	string filename;
//	FILE *stream;
//};
//
//static int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
//{
//	DlFile *out=(DlFile *)stream;
//	if(out && !out->stream)
//	{
//		/* open file for writing */
//		out->stream = fopen(out->filename.c_str(), "wb");
//		if(!out->stream) return -1; /* failure, can't open file to write */
//	}
//	return fwrite(buffer, size, nmemb, out->stream);
//}

int my_progress_func(void *foo, double t, double d, double ultotal, double ulnow)
{
	double pour1 = t!=0.0?d*100.0/t:0.0;
	double pour2 = ultotal!=0.0?ulnow*100.0/ultotal:0.0;
	changeSplash("Progression : dl %s / %s (%5.02f %%) ul %s / %s (%5.02f %%)\n", NLMISC::bytesToHumanReadable((uint32)d).c_str(), NLMISC::bytesToHumanReadable((uint32)t).c_str(), pour1, NLMISC::bytesToHumanReadable((uint32)ulnow).c_str(), NLMISC::bytesToHumanReadable((uint32)ultotal).c_str(), pour2);
	return 0;
}

void downloadFile(const string &src, const string &dst)
{
	string uri = ConfigFile.getVar("Source").asString();

	nlassert(!uri.empty());
	
	if(uri[uri.size()-1] != '/')
		uri += '/';

	uri += src;

	nldebug("Downloading '%s' to '%s'", uri.c_str(), dst.c_str());

	CURL *curl;
	CURLcode res;
//	DlFile dlfile;
//	dlfile.filename = dst;
//	dlfile.stream = NULL;

	FILE *fp = fopen(dst.c_str(), "wb");
	nlassert(fp);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	nlassert(curl);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
	curl_easy_setopt(curl, CURLOPT_URL, uri);
	curl_easy_setopt(curl, CURLOPT_FILE, fp);
	res = curl_easy_perform(curl);

	long r;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &r);

	curl_easy_cleanup(curl);
	
	if(CURLE_FTP_COULDNT_RETR_FILE == res)
	{
		// file not found, delete it
		NLMISC::CFile::deleteFile(dst.c_str());
		return;
	}

	if(CURLE_OK != res) nlerror("curl error : %d", res);
	
//	if(dlfile.stream) fclose(dlfile.stream);

	fclose(fp);
	curl_global_cleanup();

	if(r == 404)
	{
		// file not found, delete it
		NLMISC::CFile::deleteFile(dst.c_str());
		return;
	}	
}

void uploadFile(const string &src, const string &dst)
{
	string uri = ConfigFile.getVar("Upload").asString();
	
	nlassert(!uri.empty());
	
	if(uri[uri.size()-1] != '/')
		uri += '/';
	
	uri += dst;
	
	nldebug("Uploading '%s' to '%s'", src.c_str(), uri.c_str());

	CURL *curl;
	CURLcode res;
	FILE *fp;

	fp = fopen(src.c_str(), "rb");
	nlassert(fp);
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	nlassert(curl);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE);
	curl_easy_setopt(curl, CURLOPT_URL, uri);
	curl_easy_setopt(curl, CURLOPT_INFILE, fp);
	curl_easy_setopt(curl, CURLOPT_INFILESIZE, NLMISC::CFile::getFileSize(src));
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if(CURLE_OK != res) nlerror("curl error : %d", res);
	fclose(fp);
	curl_global_cleanup();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uploadPackage(const string &sumFileName)
{
	if(ConfigFile.getVar("Upload").asString().empty())
		return;

	string packageName = NLMISC::CFile::getFilenameWithoutExtension(sumFileName);

	openSplash("Uploading package '%s'", packageName.c_str());

	nlassert(NLMISC::CFile::fileExists(sumFileName));
	string packageFileName(NLMISC::CFile::getPath(sumFileName) + packageName +".tgz");
	nlassert(NLMISC::CFile::fileExists(packageFileName));

	uploadFile(packageFileName, NLMISC::CFile::getFilename(packageFileName));

	// get lastest summaries
	updatePackageList();
	vector<CPackageSum> packageNames;
	getPackageList(packageNames);

	// remove the old summary if already exist
	for(uint i = 0; i < packageNames.size(); i++)
	{
		if(packageNames[i].name() == packageName)
		{
			packageNames.erase(packageNames.begin()+i);
			break;
		}
	}

	// add the new summary
	FILE *fp=fopen(sumFileName.c_str(), "rb");
	nlassert(fp);
	string desc = getLine(fp);
	fclose(fp);
	packageNames.push_back(CPackageSum(desc));

	createPackageList(packageNames);

	NLMISC::CFile::deleteFile(packageFileName);
	NLMISC::CFile::deleteFile(sumFileName);

	closeSplash();
}

void createPackageList(vector<CPackageSum> &packageNames)
{
	string dest = LocalRepository + "summaries";
	
	FILE *fp = fopen(dest.c_str(), "wb");
	nlassert(fp);
	for(uint i = 0; i < packageNames.size(); i++)
	{
		fprintf(fp, "%s\n", packageNames[i].toString().c_str());
	}
	fclose(fp);
	uploadFile(dest, "summaries");
}

void getPackageList(vector<CPackageSum> &packageNames)
{
	string dest = LocalRepository + "summaries";

	FILE *fp = fopen(dest.c_str(), "rb");
	if(!fp) return;
	while(true)
	{
		string desc = getLine(fp);
		if(feof(fp)) break;
		packageNames.push_back(CPackageSum(desc));
	}
	fclose(fp);
}

void updatePackageList()
{
	openSplash("Updating package list...");
	downloadFile("summaries", LocalRepository+"summaries");
	closeSplash();
}

void downloadPackage(const std::string &packageName)
{
	openSplash("Downloading Package '%s'...", packageName.c_str());
	string packageFileName = packageName + ".tgz";
	downloadFile(packageFileName, LocalRepository + "tmp/" + packageFileName);
	closeSplash();
}
