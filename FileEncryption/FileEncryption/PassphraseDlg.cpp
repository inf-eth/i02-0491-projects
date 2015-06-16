// PassphraseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileEncryption.h"
#include "PassphraseDlg.h"
#include "afxdialogex.h"

extern CString Passphrase;

// CPassphraseDlg dialog

IMPLEMENT_DYNAMIC(CPassphraseDlg, CDialogEx)

CPassphraseDlg::CPassphraseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassphraseDlg::IDD, pParent)
	, m_Passphrase(_T("Enter passphrase"))
{

	m_Passphrase = _T("");
}

CPassphraseDlg::~CPassphraseDlg()
{
}

void CPassphraseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_PASSPHRASE, m_Passphrase);
	DDX_Text(pDX, IDC_EDIT_PASSPHRASE, m_Passphrase);
	DDV_MaxChars(pDX, m_Passphrase, 127);
}


BEGIN_MESSAGE_MAP(CPassphraseDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPassphraseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPassphraseDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPassphraseDlg message handlers


void CPassphraseDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if (m_Passphrase == "")
	{
		AfxMessageBox(L"Please enter a valid passphrase.", MB_ICONSTOP);
		return;
	}
	else
	{
		Passphrase = m_Passphrase;
		CDialogEx::OnOK();
	}
	UpdateData(false);
}


void CPassphraseDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
