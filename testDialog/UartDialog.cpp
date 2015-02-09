// UartDialog.cpp : implementation file
//

#include "stdafx.h"
#include "testDialog.h"
#include "UartDialog.h"
#include "afxdialogex.h"

#include "testDialogDlg.h"
#include "Serial.h"

#define ARDUINO_INIT_STRING  "Press 'c' key to update AP settings :"

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
	int i, j;
	char buffer[300];
	bool bFound = false;

	if (pParent->m_pSerial != NULL) {
		return 0;
	}

	pSerial = new CSerial();

	for (i = 1; i < 200; i++) {
		if (pSerial->Open(i, 9600)) {
			int readBytes;

			for (j = 0; j < 70; j++) {
				readBytes = pSerial->ReadData(buffer, 300);
				if (readBytes) {
					if (readBytes > 300) 
						buffer[300 - 1] = '\0';
					else 
						buffer[readBytes] = '\0';
					if (!strncmp(buffer, ARDUINO_INIT_STRING, strlen(ARDUINO_INIT_STRING))) {
						bFound = true;
					}
					break;
				}
				Sleep(100);
			}
			if (bFound) {
				break;
			}
			pSerial->Close();
		}
	}

	if (bFound) {
		pParent->m_pSerial = pSerial;
		pParent->NextProcess(pParent->m_pUartDlg);
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
