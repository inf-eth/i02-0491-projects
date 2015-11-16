
// FileEncryptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileEncryption.h"
#include "FileEncryptionDlg.h"
#include "PassphraseDlg.h"
#include "afxdialogex.h"
#include <cstring>
#include <openssl/des.h>
#include <openssl/sha.h>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString Passphrase;

void Encrypt(char* Key, char* Msg, char* Res, int size);
void Decrypt(char* Key, char* Msg, char* Res, int size);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileEncryptionDlg dialog




CFileEncryptionDlg::CFileEncryptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileEncryptionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileEncryptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_INPUT_FILE, InputFileBrowseControl);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_OUTPUT_FILE, OutputFileBrowseControl);
}

BEGIN_MESSAGE_MAP(CFileEncryptionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPT, &CFileEncryptionDlg::OnBnClickedButtonEncrypt)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPT, &CFileEncryptionDlg::OnBnClickedButtonDecrypt)
END_MESSAGE_MAP()


// CFileEncryptionDlg message handlers

BOOL CFileEncryptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here
	InputFileBrowseControl.EnableFileBrowseButton();
	OutputFileBrowseControl.EnableFileBrowseButton();

	//InputFileBrowseControl.EnableBrowseButton(true);
	//OutputFileBrowseControl.EnableBrowseButton(true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileEncryptionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileEncryptionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileEncryptionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileEncryptionDlg::OnBnClickedButtonEncrypt()
{
	// TODO: Add your control notification handler code here
	Passphrase = "";
	CString InputFilePath;
	CString OutputFilePath;

	InputFileBrowseControl.GetWindowText(InputFilePath);
	OutputFileBrowseControl.GetWindowText(OutputFilePath);

	if (OutputFilePath == "")
		OutputFilePath = InputFilePath + CString(".enc");


	const size_t StringSize = 1024;
	size_t CharactersConverted = 0;
	char InFilename[StringSize];
	char OutFilename[StringSize];
	wcstombs_s(&CharactersConverted, InFilename, InputFilePath.GetLength()+1, InputFilePath, _TRUNCATE);
	wcstombs_s(&CharactersConverted, OutFilename, OutputFilePath.GetLength()+1, OutputFilePath, _TRUNCATE);

	fstream InFile;
	InFile.open(InFilename, ios::in|ios::binary);
	if (InFile.is_open() == false)
	{
		AfxMessageBox(L"Invalid input file name or path.", MB_ICONSTOP);
		return;
	}

	fstream OutFile;
	OutFile.open(OutFilename, ios::out|ios::binary);
	if (OutFile.is_open() == false)
	{
		AfxMessageBox(L"Invalid output file name or path.", MB_ICONSTOP);
		return;
	}

	CPassphraseDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		const unsigned int MAXBUFFERSIZE = 1024*1024*8;	// 8 MB buffer.
		char* InBuffer = new char[MAXBUFFERSIZE];
		char* OutBuffer = new char[MAXBUFFERSIZE];

		const size_t StringSize = 128;
		size_t CharactersConverted = 0;
		char charPassphrase[StringSize];
		wcstombs_s(&CharactersConverted, (char*)charPassphrase, Passphrase.GetLength()+1, Passphrase, _TRUNCATE);

		char Passphrase_sha256[32];
		char Key[8];
		SHA256((unsigned char*)charPassphrase, strlen(charPassphrase), (unsigned char*)Passphrase_sha256);
		for (int i=0; i<8; i++)
			Key[i] = Passphrase_sha256[i*4];

		InFile.seekg(0, ios::end);
		unsigned int filesize = (unsigned int)InFile.tellg();

		InFile.seekg(ios::beg);
		for (unsigned int i=0; i<filesize/MAXBUFFERSIZE; i++)
		{
			InFile.read(InBuffer, MAXBUFFERSIZE);
			Encrypt(Key, InBuffer, OutBuffer, (int)MAXBUFFERSIZE);
			OutFile.write(OutBuffer, MAXBUFFERSIZE);
		}
		InFile.read(InBuffer, filesize%MAXBUFFERSIZE);
		Encrypt(Key, InBuffer, OutBuffer, (int)(filesize%MAXBUFFERSIZE));
		OutFile.write(OutBuffer, filesize%MAXBUFFERSIZE);
		CString EncryptedMessage = L"File encrypted as " + OutputFilePath;
		AfxMessageBox(EncryptedMessage, MB_ICONINFORMATION);
		delete[] InBuffer;
		delete[] OutBuffer;
	}
	else if (nResponse == IDCANCEL)
	{

	}
	Passphrase = "";
	InFile.close();
	OutFile.close();
}


void CFileEncryptionDlg::OnBnClickedButtonDecrypt()
{
	// TODO: Add your control notification handler code here
	Passphrase = "";
	CString InputFilePath;
	CString OutputFilePath;

	InputFileBrowseControl.GetWindowText(InputFilePath);
	OutputFileBrowseControl.GetWindowText(OutputFilePath);

	if (OutputFilePath == "")
	{
		if (InputFilePath.Find(L".enc") == InputFilePath.GetLength()-4)
			OutputFilePath = InputFilePath.Left(InputFilePath.GetLength()-4);
		else
			OutputFilePath = InputFilePath + CString(".dec");
	}

	const size_t StringSize = 1024;
	size_t CharactersConverted = 0;
	char InFilename[StringSize];
	char OutFilename[StringSize];
	wcstombs_s(&CharactersConverted, InFilename, InputFilePath.GetLength()+1, InputFilePath, _TRUNCATE);
	wcstombs_s(&CharactersConverted, OutFilename, OutputFilePath.GetLength()+1, OutputFilePath, _TRUNCATE);

	fstream InFile;
	InFile.open(InFilename, ios::in|ios::binary);
	if (InFile.is_open() == false)
	{
		AfxMessageBox(L"Invalid input file name or path.", MB_ICONSTOP);
		return;
	}

	fstream OutFile;
	OutFile.open(OutFilename, ios::out|ios::binary);
	if (OutFile.is_open() == false)
	{
		AfxMessageBox(L"Invalid output file name or path.", MB_ICONSTOP);
		return;
	}

	CPassphraseDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		const unsigned int MAXBUFFERSIZE = 1024*1024*8;	// 8 MB buffer.
		char* InBuffer = new char[MAXBUFFERSIZE];
		char* OutBuffer = new char[MAXBUFFERSIZE];

		const size_t StringSize = 128;
		size_t CharactersConverted = 0;
		char charPassphrase[StringSize];
		wcstombs_s(&CharactersConverted, (char*)charPassphrase, Passphrase.GetLength()+1, Passphrase, _TRUNCATE);

		char Passphrase_sha256[32];
		char Key[8];
		SHA256((unsigned char*)charPassphrase, strlen(charPassphrase), (unsigned char*)Passphrase_sha256);
		for (int i=0; i<8; i++)
			Key[i] = Passphrase_sha256[i*4];

		InFile.seekg(0, ios::end);
		unsigned int filesize = (unsigned int)InFile.tellg();

		InFile.seekg(ios::beg);
		for (unsigned int i=0; i<filesize/MAXBUFFERSIZE; i++)
		{
			InFile.read(InBuffer, MAXBUFFERSIZE);
			Decrypt(Key, InBuffer, OutBuffer, (int)MAXBUFFERSIZE);
			OutFile.write(OutBuffer, MAXBUFFERSIZE);
		}
		InFile.read(InBuffer, filesize%MAXBUFFERSIZE);
		Decrypt(Key, InBuffer, OutBuffer, (int)(filesize%MAXBUFFERSIZE));
		OutFile.write(OutBuffer, filesize%MAXBUFFERSIZE);
		CString DecryptedMessage = L"File decrypted as " + OutputFilePath;
		AfxMessageBox(DecryptedMessage, MB_ICONINFORMATION);
		delete[] InBuffer;
		delete[] OutBuffer;
	}
	else if (nResponse == IDCANCEL)
	{

	}
	Passphrase = "";
	InFile.close();
	OutFile.close();
}

void Encrypt(char* Key, char* Msg, char* Res, int size)
{
	int n=0;
	DES_cblock Key2;
	DES_key_schedule schedule;

	// Prepare the key for use with DES_cfb64_encrypt
	memcpy(Key2, Key, 8);
	DES_set_odd_parity( &Key2 );
	DES_set_key_checked( &Key2, &schedule );

	// Encryption occurs here
	DES_cfb64_encrypt((unsigned char*)Msg, (unsigned char*)Res, size, &schedule, &Key2, &n, DES_ENCRYPT);
}


void Decrypt(char* Key, char* Msg, char* Res, int size)
{
	int n=0;

	DES_cblock Key2;
	DES_key_schedule schedule;

	// Prepare the key for use with DES_cfb64_encrypt
	memcpy(Key2, Key,8);
	DES_set_odd_parity(&Key2);
	DES_set_key_checked(&Key2, &schedule);

	// Decryption occurs here
	DES_cfb64_encrypt((unsigned char*)Msg, (unsigned char*)Res, size, &schedule, &Key2, &n, DES_DECRYPT);
}
