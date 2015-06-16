
// FileEncryptionDlg.h : header file
//

#pragma once
#include "afxeditbrowsectrl.h"


// CFileEncryptionDlg dialog
class CFileEncryptionDlg : public CDialogEx
{
// Construction
public:
	CFileEncryptionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FILEENCRYPTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCEditBrowseCtrl InputFileBrowseControl;
	CMFCEditBrowseCtrl OutputFileBrowseControl;
	afx_msg void OnBnClickedButtonEncrypt();
	afx_msg void OnBnClickedButtonDecrypt();
};
