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

#if !defined(AFX_WPKG_SYNC_DLG_H__4E6598FE_426D_4206_A1D3_AD75BC44F359__INCLUDED_)
#define AFX_WPKG_SYNC_DLG_H__4E6598FE_426D_4206_A1D3_AD75BC44F359__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#include "resource.h"

void changeSplash(const char *format, ...);
void openSplash(const char *format, ...);
void closeSplash();
std::string getLine(FILE *fp);


/////////////////////////////////////////////////////////////////////////////
// CWPKGSyncDlg dialog

class CWPKGSyncDlg : public CDialog
{
// Construction
public:
	CWPKGSyncDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWPKGSyncDlg)
	enum { IDD = IDD_WPKG_SYNC_DIALOG };
	CListCtrl	ListPackages;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWPKGSyncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWPKGSyncDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMakePackage();
	afx_msg void OnUploadPackage();
	afx_msg void OnUpdatePackageList();
	afx_msg void OnGetPackages();
	afx_msg void OnRemovePackages();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void refreshPackageList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WPKG_SYNC_DLG_H__4E6598FE_426D_4206_A1D3_AD75BC44F359__INCLUDED_)
