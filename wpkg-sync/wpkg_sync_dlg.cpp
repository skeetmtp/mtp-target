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

#include <vector>
#include <deque>
#include <string>

#include <nel/misc/path.h>

#include "wpkg_sync.h"
#include "wpkg_sync_dlg.h"
#include "server.h"
#include "make_package.h"
#include "splash_dlg.h"

using namespace std;
using namespace NLMISC;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWPKGSyncDlg dialog

CWPKGSyncDlg::CWPKGSyncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWPKGSyncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWPKGSyncDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWPKGSyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWPKGSyncDlg)
	DDX_Control(pDX, IDC_LIST_PACKAGES, ListPackages);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWPKGSyncDlg, CDialog)
	//{{AFX_MSG_MAP(CWPKGSyncDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MAKE_PACKAGE, OnMakePackage)
	ON_BN_CLICKED(IDC_UPLOAD_PACKAGE, OnUploadPackage)
	ON_BN_CLICKED(IDC_UPDATE_PACKAGE_LIST, OnUpdatePackageList)
	ON_BN_CLICKED(IDC_GET_PACKAGES, OnGetPackages)
	ON_BN_CLICKED(IDC_REMOVE_PACKAGES, OnRemovePackages)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWPKGSyncDlg message handlers

void CWPKGSyncDlg::refreshPackageList()
{
	vector<CPackageSum> packages;
	getPackageList(packages);
	
	getInstalledPackages(packages);
	
	ListPackages.DeleteAllItems();

	if(packages.size()>0)
	{
		for(uint i = 0; i < packages.size(); i++)
		{
			CString strItem;

//			string date;
//			_tzset();
//			char tmpbuf[128];
//			_strdate(tmpbuf);
//			date = tmpbuf;
//			date += " ";
//			_strtime(tmpbuf);
//			date += tmpbuf;
			
			time_t t = packages[i].date();
			string date = asctime(localtime(&t));
			date = date.substr(0, date.size()-1);

			LVITEM lvi;
			// Insert the first item
			lvi.mask = LVIF_TEXT;
			strItem = packages[i].name().c_str();
			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			int idx = ListPackages.InsertItem(&lvi);

nlinfo("'%s' '%s' '%s' '%s'", packages[i].name().c_str(), packages[i].localVersion().c_str(), packages[i].version().c_str(), packages[i].description().c_str());

			if(packages[i].localVersion().empty())
			{
				// package not installed
				strItem = "";
			}
			else if(packages[i].localVersion() == packages[i].version())
			{
				// package already installed
				// !! if you change the name Installed, you must also change in the function OnGetPackage()
				strItem = "Installed";
			}
			else
			{
				// package already installed but a different version is available
				// !! if you change the name Installed, you must also change in the function OnGetPackage()
				strItem = "Old";
			}

			lvi.iItem = idx;

			lvi.iSubItem = 1;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			ListPackages.SetItem(&lvi);

			strItem = packages[i].localVersion().c_str();
			lvi.iSubItem = 2;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			ListPackages.SetItem(&lvi);

			strItem = packages[i].version().c_str();
			lvi.iSubItem = 3;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			ListPackages.SetItem(&lvi);
			
			strItem = date.c_str();
			lvi.iSubItem = 4;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			ListPackages.SetItem(&lvi);

			strItem = packages[i].description().c_str();
			lvi.iSubItem = 5;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			ListPackages.SetItem(&lvi);

			strItem = packages[i].maintainer().c_str();
			lvi.iSubItem = 6;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			ListPackages.SetItem(&lvi);
		}
	}
	else
	{
		static recur = false;
		if(!recur && IDYES == MessageBox("There's no package list, do you want to update the package list from server?", "Info", MB_YESNO | MB_ICONQUESTION))
		{
			updatePackageList();
			recur = true;
			refreshPackageList();
			recur = false;
		}
	}
	ListPackages.SetColumnWidth(0, LVSCW_AUTOSIZE);
	ListPackages.SetColumnWidth(1, LVSCW_AUTOSIZE);
	ListPackages.SetColumnWidth(2, LVSCW_AUTOSIZE);
	ListPackages.SetColumnWidth(3, LVSCW_AUTOSIZE);
	ListPackages.SetColumnWidth(4, LVSCW_AUTOSIZE);
	ListPackages.SetColumnWidth(5, LVSCW_AUTOSIZE);
	ListPackages.SetColumnWidth(6, LVSCW_AUTOSIZE);
}

BOOL CWPKGSyncDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	ListPackages.InsertColumn(0, _T("Package"), LVCFMT_LEFT, -1);
	ListPackages.InsertColumn(1, _T("Status"), LVCFMT_LEFT, -1);
	ListPackages.InsertColumn(2, _T("LocalVersion"), LVCFMT_LEFT, -1);
	ListPackages.InsertColumn(3, _T("NewVersion"), LVCFMT_LEFT, -1);
	ListPackages.InsertColumn(4, _T("Package Date"), LVCFMT_LEFT, -1);
	ListPackages.InsertColumn(5, _T("Description"), LVCFMT_LEFT, -1);
	ListPackages.InsertColumn(6, _T("Maintainer"), LVCFMT_LEFT, -1);
	
	refreshPackageList();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	if(ConfigFile.getVar("Upload").asString().empty())
	{
		CWnd *btn = GetDlgItem(IDC_UPLOAD_PACKAGE);
		btn->ShowWindow(FALSE);
		btn = GetDlgItem(IDC_MAKE_PACKAGE);
		btn->ShowWindow(FALSE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWPKGSyncDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWPKGSyncDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWPKGSyncDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWPKGSyncDlg::OnMakePackage()
{
	static char BASED_CODE szFilter[] = "Package Description Files (*.wpkg)|*.wpkg|All Files (*.*)|*.*||";

	CFileDialog fd(TRUE, "wpkg", NULL, OFN_HIDEREADONLY, szFilter, this);
	if(fd.DoModal() == IDOK)
	{
		string str(fd.GetPathName().GetBuffer(0));
		makePackage(str);
	}
}

void CWPKGSyncDlg::OnUploadPackage()
{
	static char BASED_CODE szFilter[] = "Package Files (*.sum)|*.sum|All Files (*.*)|*.*||";
	
	CFileDialog fd(TRUE, "sum", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, szFilter, this);
	if(fd.DoModal() == IDOK)
	{
		string str(fd.GetPathName().GetBuffer(0));
		uploadPackage(str);
		refreshPackageList();
	}
}

void CWPKGSyncDlg::OnUpdatePackageList() 
{
	updatePackageList();
	refreshPackageList();
}

void CWPKGSyncDlg::OnGetPackages()
{
	POSITION p = ListPackages.GetFirstSelectedItemPosition();
	
	openSplash("Getting Packages...");
	
	while (p)
	{
		int nSelected = ListPackages.GetNextSelectedItem(p);
		// Do something with item nSelected
		
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		ListPackages.GetItem(&lvi);

		string pname = lvi.pszText;

		lvi.iSubItem = 1;
		ListPackages.GetItem(&lvi);

		string status = lvi.pszText;

		if(status != "Installed")
		{
			downloadPackage(pname);

			if(status == "Old")
				removePackage(pname);
			
			installPackage(pname);
		}
	}
	refreshPackageList();

	closeSplash();
}

void CWPKGSyncDlg::OnRemovePackages() 
{
	if(IDNO == MessageBox("Are you sure to you want to delete these packages?", "Delete", MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	POSITION p = ListPackages.GetFirstSelectedItemPosition();
	
	while (p)
	{
		int nSelected = ListPackages.GetNextSelectedItem(p);
		// Do something with item nSelected
		
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		ListPackages.GetItem(&lvi);
		
		string pname = lvi.pszText;
		
		lvi.iSubItem = 1;
		ListPackages.GetItem(&lvi);
		
		string status = lvi.pszText;
		
		if(!status.empty())
		{
			removePackage(pname);
		}
	}
	refreshPackageList();
}


static deque<string> SplashStack;
static CSplashDialog	*Splash = NULL;

void changeSplash(const char *format, ...)
{
	if(!Splash) return;
	char *msg;
	NLMISC_CONVERT_VARGS (msg, format, 256);
	string newmsg;
	if(!SplashStack.empty())
	{
		newmsg = SplashStack.back() + "\n\r";
	}
	newmsg += msg;
	Splash->GetDlgItem(IDC_SPLASHTEXT)->SetWindowText(newmsg.c_str());
}

void openSplash(const char *format, ...)
{
	char *msg;
	NLMISC_CONVERT_VARGS (msg, format, 256);

	if(!Splash)
	{
		Splash = new CSplashDialog();
		Splash->Create(IDD_SPLASHDIALOG, NULL);
		Splash->ShowWindow(SW_SHOW);
	}

	string newmsg;
	if(!SplashStack.empty())
	{
		newmsg = SplashStack.back() + "\n\r";
	}
	newmsg += msg;
	SplashStack.push_back(newmsg);
	Splash->GetDlgItem(IDC_SPLASHTEXT)->SetWindowText(newmsg.c_str());
	nlinfo(msg);
}

void closeSplash()
{
	SplashStack.pop_back();
	if(SplashStack.empty())
	{
		Splash->DestroyWindow();
		delete Splash;
		Splash = NULL;
	}
	else
	{
		Splash->GetDlgItem(IDC_SPLASHTEXT)->SetWindowText(SplashStack.back().c_str());
	}
}

string getLine(FILE *fp)
{
	nlassert(fp);
	char line[1024];
	fgets(line, 1024, fp);
	int size = strlen(line);
	if(size >= 1 && line[size-1] == '\n')
	{
		if(size >= 2 && line[size-2] == '\r')
		{
			line[size-2] = '\0';
		}
		else
		{
			line[size-1] = '\0';
		}
	}
	return line;
}
