// UartDialog.cpp : implementation file
//

#include "stdafx.h"
#include "testDialog.h"
#include "UartDialog.h"
#include "afxdialogex.h"

#include "testDialogDlg.h"
#include "Serial.h"

#define ARDUINO_INIT_STRING  "Press 'c' key to update AP settings : \r\n"
#define MAX_INIT_STRING_LEN	300

// CUartDialog dialog

IMPLEMENT_DYNAMIC(CUartDialog, CDialogEx)

CUartDialog::CUartDialog(CWnd* pParent /*=NULL*/, CtestDialogDlg * p)
	: CDialogEx(CUartDialog::IDD, pParent)
{
	_parent = p;
}

CUartDialog::~CUartDialog()
{
}

void CUartDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}


BEGIN_MESSAGE_MAP(CUartDialog, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDCANCEL, &CUartDialog::OnIdcancel)
	ON_COMMAND(IDOK, &CUartDialog::OnIdok)
END_MESSAGE_MAP()


// CUartDialog message handlers


int CUartDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	return 0;
}

UINT UartFind(LPVOID pParam)
{
	CtestDialogDlg * pParent = (CtestDialogDlg*)pParam;
	CSerial*pSerial = NULL;
	int i;
	char buffer[MAX_INIT_STRING_LEN];
	bool bFound = false;

	if (pParent->m_pSerial != NULL) {
		return 0;
	}

	pSerial = new CSerial();

	for (i = 1; i < 200; i++) {
		if (pSerial->Open(i, 9600)) {
			int readBytes = pSerial->ReadBytesOrWait(buffer, strlen(ARDUINO_INIT_STRING), 10000);
			if (strlen(ARDUINO_INIT_STRING) != readBytes) {
				bFound = false;
			} else if (!strncmp(buffer, ARDUINO_INIT_STRING, strlen(ARDUINO_INIT_STRING))) {
				bFound = true;
				break;
			}
			if (bFound) {
				break;
			}
			pSerial->Close();
		}
	}

	if (bFound) {
		pSerial->SendData("c", 1);
		pParent->m_pSerial = pSerial;
		pParent->NextProcess(pParent->m_pUartDlg);
	}
	else {
		AfxMessageBox(_T("센서노드를 찾지 못했습니다.\r\n창을 닫습니다."), IDOK | MB_ICONERROR);
		pParent->EndDialog(0);
	}

	return 0;
}



void CUartDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if (bShow) {
		m_Progress.SetMarquee(true, 50);
		AfxBeginThread(UartFind, (LPVOID)_parent);
	} else {
		m_Progress.SetMarquee(false, 50);
	}
}


void CUartDialog::OnIdcancel()
{
	// TODO: Add your command handler code here
}


void CUartDialog::OnIdok()
{
	// TODO: Add your command handler code here
}
