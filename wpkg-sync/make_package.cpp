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

#include <io.h>
#include <fcntl.h>
#include <libtar.h>
#include <zlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include <nel/misc/debug.h>
#include <nel/misc/algo.h>
#include <nel/misc/path.h>
#include <nel/misc/config_file.h>

#include "make_package.h"
#include "wpkg_sync_dlg.h"
#include "wpkg_sync.h"
#include "server.h"

using namespace std;
using namespace NLMISC;

static int gzopen_frontend(char *pathname, int oflags, int mode)
{
	char *gzoflags;
	gzFile gzf;
	int fd;

	switch (oflags & 0x3)
	{
	case O_WRONLY:
		gzoflags = "wb";
		break;
	case O_RDONLY:
		gzoflags = "rb";
		break;
	default:
	case O_RDWR:
		errno = EINVAL;
		return -1;
	}

	fd = open(pathname, oflags, mode);
	if (fd == -1)
		return -1;

	//	if ((oflags & O_CREAT) && fchmod(fd, mode))
	//		return -1;

	gzf = gzdopen(fd, gzoflags);
	if (!gzf)
	{
		errno = ENOMEM;
		return -1;
	}

	return (int)gzf;
}

static tartype_t gztype = { (openfunc_t) gzopen_frontend, (closefunc_t) gzclose,
(readfunc_t) gzread, (writefunc_t) gzwrite
};


FILE *FilesList = 0;

void addFile(TAR *t, const string &src, const string &dst)
{
	nlassert(FilesList);
	fprintf(FilesList, "%s\n", dst.c_str());
	nlinfo("Adding File '%s'", dst.c_str());
	if (tar_append_file(t, (char*)src.c_str(), (char*)dst.c_str()) != 0)
	{
		nlerror("tar_append_file(\"%s\", \"%s\"): %s", src.c_str(), dst.c_str(), strerror(errno));
		tar_close(t);
	}
}

void addFiles(TAR *t, CConfigFile::CVar &var, const string &srcDir, const string &dstDir)
{
	nlassert(srcDir.size()>0 && srcDir[srcDir.size()-1]=='/');
	nlassert(dstDir.size()>0 && dstDir[dstDir.size()-1]=='/');
	nlassert(t);

	for(sint i = 0; i < var.size(); i++)
	{
		string srcSub, dstSub;
		string::size_type pos = var.asString(i).find("|");
		if(pos != string::npos)
		{
			srcSub = var.asString(i).substr(0, pos);
			dstSub = var.asString(i).substr(pos+1);
		}
		else
		{
			srcSub = var.asString(i);
			dstSub = NLMISC::CFile::getFilename(var.asString(i));
		}

		string src = srcDir + srcSub;
		string dst = dstDir + dstSub;
		string srcPath, dstPath;
		string srcFile, dstFile;

		srcPath = NLMISC::CFile::getPath(src);
		if(srcPath[srcPath.size()-1] != '/') srcPath += "/";
		srcFile = NLMISC::CFile::getFilename(src);
		dstPath = NLMISC::CFile::getPath(dst);
		if(dstPath[dstPath.size()-1] != '/') dstPath += "/";
		dstFile = NLMISC::CFile::getFilename(dst);

		nlinfo("'%s' '%s' -> '%s' '%s'", srcPath.c_str(), srcFile.c_str(), dstPath.c_str(), dstFile.c_str());

		string wc;
		if(srcFile.find("?") != string::npos || srcFile.find("*") != string::npos)
		{
			// there's wildcard
			wc = srcFile;
			srcFile = "";
			dstFile = "";
		}

		if(NLMISC::CFile::isDirectory(srcPath+srcFile))
		{
			string srcFull = srcPath+srcFile;
			if(srcFull[srcFull.size()-1] != '/') srcFull += "/";

			if(pos == string::npos)
			{
				dstPath = dstDir;
				dstFile = "";
			}

			string dstFull = dstPath+dstFile;
			if(dstFull[dstFull.size()-1] != '/') dstFull += "/";
			vector<string> res;
			CPath::getPathContent(srcFull, true, false, true, res);
			for(uint i = 0; i < res.size(); i++)
			{
				string final = res[i];
				if(final.find(srcFull) != string::npos)
				{
					final = final.substr(srcFull.size());
				}

				nlinfo("'%s'->'%s%s'", res[i].c_str(), dstFull.c_str(), final.c_str());

				if(!wc.empty() && !testWildCard(final,wc))
				{
					nlinfo("skip file '%s' because doesn't match '%s'", final.c_str(), wc.c_str());
					continue;
				}
				addFile(t, res[i], dstFull + final);
			}
		}
		else
		{
			nlinfo("'%s%s'->'%s%s'", srcPath.c_str(), srcFile.c_str(), dstPath.c_str(), dstFile.c_str());
			addFile(t, srcPath+srcFile, dstPath+dstFile);
		}
	}
}

void makePackage(const string &packageDescriptionFileName)
{
	string packageName = NLMISC::CFile::getFilenameWithoutExtension(packageDescriptionFileName);

	nlinfo ("Making package '%s'", packageName.c_str());

	openSplash("Making package %s", packageName.c_str());
	
	string path = CPath::getFullPath(NLMISC::CFile::getPath(packageDescriptionFileName));

	CConfigFile cf;
	cf.load(packageDescriptionFileName);

	TAR *t;

	string tarFileName(packageName + ".tgz");
	if(NLMISC::CFile::fileExists(tarFileName))
		NLMISC::CFile::deleteFile(tarFileName);

	if(tar_open(&t, (char*)tarFileName.c_str(), &gztype, O_WRONLY | O_CREAT, 0644, TAR_VERBOSE | TAR_GNU) == -1)
		nlerror("tar_open(): %s", strerror(errno));

	string listFileName(LocalRepository + "tmp/" + packageName + ".list");
	nlassert(!FilesList);
	FilesList = fopen(listFileName.c_str(), "wb");
	if(!FilesList)
	{
		nlerror("Can't create list file");
		return;
	}

	fprintf(FilesList, "%s\n", cf.getVar("Version").asString().c_str());

	if(cf.exists("Includes")) addFiles(t, cf.getVar("Includes"), path, "include/");
	if(cf.exists("Libraries")) addFiles(t, cf.getVar("Libraries"), path, "lib/");
	if(cf.exists("Binaries")) addFiles(t, cf.getVar("Binaries"), path, "bin/");

	fclose(FilesList);
	FilesList = 0;
	if(tar_append_file(t, (char*)listFileName.c_str(), (char*)string("list/"+NLMISC::CFile::getFilename(listFileName)).c_str()) != 0)
	{
		nlerror("tar_append_file(\"%s\", \"%s\"): %s", listFileName.c_str(), listFileName.c_str(), strerror(errno));
		tar_close(t);
		return;
	}

	NLMISC::CFile::deleteFile(listFileName);

	if (tar_append_eof(t) != 0)
	{
		nlerror("tar_append_eof(): %s", strerror(errno));
		tar_close(t);
		return;
	}

	if (tar_close(t) != 0)
	{
		nlerror("tar_close(): %s", strerror(errno));
		return;
	}

	string sumFileName(packageName+".sum");
	if(NLMISC::CFile::fileExists(sumFileName))
		NLMISC::CFile::deleteFile(sumFileName);
	
	CPackageSum ps(packageName, cf.getVar("Version").asString(), cf.getVar("Description").asString(), cf.getVar("Maintainer").asString());

	FILE *fp = fopen(sumFileName.c_str(), "wb");
	nlassert(fp);
	fputs(ps.toString().c_str(), fp);
	fclose(fp);

	closeSplash();
}

void installPackage(const std::string &packageName)
{
	openSplash("Installing package '%s'", packageName.c_str());
	
	string packageFileName = LocalRepository + "tmp/" + packageName + ".tgz";

	TAR *t;

	if(tar_open(&t, (char*)packageFileName.c_str(), &gztype, O_RDONLY, 0, TAR_VERBOSE | TAR_GNU) == -1)
		nlerror("tar_open(): %s", strerror(errno));

	if(tar_extract_all(t, (char*)LocalRepository.c_str()) != 0)
		nlerror("tar_extract_all(): %s", strerror(errno));

	if(tar_close(t) != 0)
		nlerror("tar_close(): %s", strerror(errno));

	NLMISC::CFile::deleteFile(packageFileName);
	
	closeSplash();
}

void removePackage(const std::string &packageName)
{
	openSplash("Removing package '%s'", packageName.c_str());

	string listFileName = LocalRepository + "list/" + packageName + ".list";
	
	FILE *fp = fopen(listFileName.c_str(), "rb");
	if(!fp)
	{
		nlerror("Can't open list file for package %s (%s)", packageName.c_str(), listFileName.c_str());
		return;
	}

	// skip version number
	string line = getLine(fp);

	while(true)
	{
		line = getLine(fp);
		if(feof(fp)) break;

		string file = LocalRepository + line;

		NLMISC::CFile::deleteFile(file);
	}

	fclose(fp);

	NLMISC::CFile::deleteFile(listFileName);

	closeSplash();
}


void getInstalledPackages(vector<CPackageSum> &packages)
{
	vector<string> res;
	CPath::getPathContent(LocalRepository+"list/", true, false, true, res);
	for(uint i = 0; i < res.size(); i++)
	{
		string packageName = NLMISC::CFile::getFilenameWithoutExtension(res[i]);

		for(uint j = 0; j < packages.size(); j++)
		{
			if(packages[j].name() == packageName)
			{
				FILE *fp = fopen(res[i].c_str(), "rb");
				nlassert(fp);
				string ver = getLine(fp);
				fclose(fp);
				packages[j].localVersion(ver);
				break;
			}
		}
	}
}
