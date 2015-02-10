
// testDialogDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "testDialog.h"
#include "testDialogDlg.h"
#include "afxdialogex.h"
#include "LoginDialog.h"
#include "UartDialog.h"
#include "ApSelDialog.h"
#include "ScanDialog.h"
#include "FinishDialog.h"

#include "Serial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDialogDlg 대화 상자



CtestDialogDlg::CtestDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pLoginDlg = NULL;
	m_pUartDlg = NULL;
	m_pSerial = NULL;
	m_pApSelDlg = NULL;
	m_pScanDlg = NULL;
	m_pFinishDlg = NULL;
}

void CtestDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestDialogDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CtestDialogDlg 메시지 처리기

BOOL CtestDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if (NULL == m_pLoginDlg) {
		m_pLoginDlg = new CLoginDialog(this, this);
		m_pLoginDlg->Create(CLoginDialog::IDD, this);
	}
	m_pLoginDlg->ShowWindow(SW_SHOWNORMAL);
	m_pLoginDlg->SetFocus();

	if (NULL == m_pUartDlg) {
		m_pUartDlg = new CUartDialog(this, this);
		m_pUartDlg->Create(CUartDialog::IDD, this);
	}

	if (NULL == m_pApSelDlg) {
		m_pApSelDlg = new CApSelDialog(this, this);
		m_pApSelDlg->Create(CApSelDialog::IDD, this);
	}

	if (NULL == m_pScanDlg) {
		m_pScanDlg = new CScanDialog(this, this);
		m_pScanDlg->Create(CScanDialog::IDD, this);
	}

	if (NULL == m_pFinishDlg) {
		m_pFinishDlg = new CFinishDialog(this, this);
		m_pFinishDlg->Create(CFinishDialog::IDD, this);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CtestDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CtestDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CtestDialogDlg::~CtestDialogDlg()
{
	delete m_pLoginDlg;
	delete m_pUartDlg;
	delete m_pApSelDlg;
	delete m_pSerial;
	delete m_pScanDlg;
	delete m_pFinishDlg;
}

void CtestDialogDlg::NextProcess(CDialogEx * pChild)
{
	if (pChild == m_pLoginDlg) {
		SetWindowText(_T("센서 포트 검색"));
		m_pLoginDlg->ShowWindow(SW_HIDE);
		m_pUartDlg->ShowWindow(SW_SHOWNORMAL);
		m_pUartDlg->SetFocus();
	}
	else if (pChild == m_pUartDlg) {
		SetWindowText(_T("AP 정보 입력"));
		m_pUartDlg->ShowWindow(SW_HIDE);
		m_pApSelDlg->ShowWindow(SW_SHOWNORMAL);
		m_pApSelDlg->SetFocus();
	}
	else if (pChild == m_pApSelDlg) {
		SetWindowText(_T("센서노드 정보 입력"));
		m_pApSelDlg->ShowWindow(SW_HIDE);
		m_pFinishDlg->ShowWindow(SW_SHOWNORMAL);
		m_pFinishDlg->SetFocus();
	}
	else if (pChild == m_pScanDlg) {
		SetWindowText(_T("센서노드 정보 입력"));
		m_pScanDlg->ShowWindow(SW_HIDE);
		m_pFinishDlg->ShowWindow(SW_SHOWNORMAL);
		m_pFinishDlg->SetFocus();
	}
	else {

	}
}


void CtestDialogDlg::ToScanWindow()
{
	SetWindowText(_T("AP 스캔"));
	m_pApSelDlg->ShowWindow(SW_HIDE);
	m_pScanDlg->ShowWindow(SW_NORMAL);
	m_pScanDlg->SetFocus();
}
