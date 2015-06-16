#pragma once


// CPassphraseDlg dialog

class CPassphraseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassphraseDlg)

public:
	CPassphraseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassphraseDlg();

// Dialog Data
	enum { IDD = IDD_PASSPHRASE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CString m_Passphrase;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_Passphrase;
};
