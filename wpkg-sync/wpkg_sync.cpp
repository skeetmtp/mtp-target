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

#include "wpkg_sync.h"
#include "wpkg_sync_dlg.h"

#include <io.h>
#include <fcntl.h>
#include <libtar.h>
#include <zlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include <nel/misc/path.h>
#include <nel/misc/debug.h>
#include <nel/misc/config_file.h>

#include "server.h"
#include "make_package.h"

using namespace std;
using namespace NLMISC;

CConfigFile ConfigFile;
std::string LocalRepository;

/////////////////////////////////////////////////////////////////////////////
// CWPKGSyncApp

BEGIN_MESSAGE_MAP(CWPKGSyncApp, CWinApp)
	//{{AFX_MSG_MAP(CWPKGSyncApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWPKGSyncApp construction

CWPKGSyncApp::CWPKGSyncApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWPKGSyncApp object

CWPKGSyncApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWPKGSyncApp initialization

void createDirectory(const string &path)
{
	if(!NLMISC::CFile::isDirectory(path))
	{
		NLMISC::CFile::createDirectory(path);
		if(!NLMISC::CFile::isDirectory(path))
		{
			MessageBox(NULL, toString("Can't create directory %s", path.c_str()).c_str(), "Configuration file Error", MB_ICONERROR | MB_OK);
			exit(EXIT_FAILURE);
		}
	}
}

BOOL CWPKGSyncApp::InitInstance()
{
	ConfigFile.load("wpkg-sync.cfg");

	if(!ConfigFile.exists("Source") || ConfigFile.getVar("Source").asString().empty())
	{
		MessageBox(NULL, "You must setup the Source variable in your wpkg_sync.cfg with a valid ftp uri", "Configuration file Error", MB_ICONERROR | MB_OK);
		exit(EXIT_FAILURE);
	}

	if(!ConfigFile.exists("LocalRepository") || ConfigFile.getVar("LocalRepository").asString().empty())
	{
		MessageBox(NULL, "You must setup the LocalRepository variable in your wpkg_sync.cfg", "Configuration file Error", MB_ICONERROR | MB_OK);
		exit(EXIT_FAILURE);
	}

	LocalRepository = CPath::standardizePath(ConfigFile.getVar("LocalRepository").asString());

	createDirectory(LocalRepository);
	createDirectory(LocalRepository+"tmp");
	createDirectory(LocalRepository+"list");

//	downloadFile("libtar.tgz", "libtar.tgz");
//	uploadFile("ok.ff", "ok.ff");

	//makePackage("c:/code/external/stlport.wpkg");
	//makePackage("C:\\code\\external\\libxml2-2.4.26\\libxml2.wpkg");
	//uploadPackage("libtar");

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CWPKGSyncDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
