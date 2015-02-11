// ScanDialog.cpp : implementation file
//

#include "stdafx.h"
#include "testDialog.h"
#include "ScanDialog.h"
#include "afxdialogex.h"

#include "Serial.h"
#include "testDialogDlg.h"

#define MAX_INIT_STRING_LEN 300
#define SET_OR_SCAN_MESSAGE "Press 'a' to set AP settings, Press 's' to scan APs : \r\n"
#define SSID_MESSAGE "SSID : "
#define SECURITY_MESSAGE "Security : "
#define PASSWORD_MESSAGE "Password : "
#define CONNECTING_MESSAGE "connecting to AP.. \r\n"
#define CONNECTED_MESSAGE "AP connected 1\r\n"
#define CONNECTION_FAIL_MESSAGE "AP connected 1\r\n"
#define SCAN_FINISH_MESSAGE "Scan result : \r\n"
#define WRITE_EEPROM_MESSAGE "Writing EEPROM\r\n"
#define READ_EEPROM_MESSAGE "reading EEPROM..\r\n"
#define MAC_MESSAGE "mac: 08-00-00-59-2d-26\r\n"

// CScanDialog dialog

IMPLEMENT_DYNAMIC(CScanDialog, CDialogEx)

CScanDialog::CScanDialog(CWnd* pParent /*=NULL*/, CtestDialogDlg * p)
	: CDialogEx(CScanDialog::IDD, pParent)
{
	_parent = p;
}

CScanDialog::~CScanDialog()
{
}

void CScanDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_APSCAN_LIST, m_apListCtrl);
	DDX_Control(pDX, IDC_SCANPASSWD_EDIT, m_passwordEdit);
	DDX_Control(pDX, IDC_PROGRESS1, m_connectProgress);
	DDX_Control(pDX, IDC_SCANCONNECT_BTN, m_connectBtn);
	DDX_Control(pDX, IDC_RESCAN_BTN, m_rescanBtn);
}


BEGIN_MESSAGE_MAP(CScanDialog, CDialogEx)
	ON_BN_CLICKED(IDC_SCANCONNECT_BTN, &CScanDialog::OnBnClickedScanconnectBtn)
	ON_BN_CLICKED(IDC_RESCAN_BTN, &CScanDialog::OnBnClickedRescanBtn)
//	ON_WM_CREATE()
//ON_WM_CREATE()
ON_WM_SHOWWINDOW()
ON_NOTIFY(LVN_ITEMCHANGED, IDC_APSCAN_LIST, &CScanDialog::OnLvnItemchangedApscanList)
END_MESSAGE_MAP()


// CScanDialog message handlers

static UINT ConnectMonitor(LPVOID pParam)
{
	CScanDialog* pDlg = (CScanDialog*)pParam;
	char buffer[MAX_INIT_STRING_LEN];
	bool bConnected = true;
	int readBytes;
	CSerial * pSerial = pDlg->_parent->m_pSerial;
	unsigned int mac[6];

	readBytes = pSerial->ReadBytesOrWait(buffer, strlen(CONNECTED_MESSAGE), 1000000);
	if (readBytes != strlen(CONNECTED_MESSAGE)) {
		bConnected = false;
	}
	else if (strncmp(CONNECTED_MESSAGE, buffer, strlen(CONNECTED_MESSAGE))) {
		bConnected = false;
	}

	readBytes = pSerial->ReadBytesOrWait(buffer, strlen(MAC_MESSAGE), 1000000);
	if (readBytes != strlen(MAC_MESSAGE)) {
		bConnected = false;
	}

	if (bConnected) {
		buffer[readBytes] = '\0';
		sscanf_s(buffer, "mac: %x-%x-%x-%x-%x-%x\r\n",
			&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

		pDlg->_parent->m_strMac.Format(_T("%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x"),
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		pDlg->OnApConnect();
	}
	else {
		pDlg->OnApConnectFail();
	}

	return 0;
}

void CScanDialog::OnBnClickedScanconnectBtn()
{
	// TODO: Add your control notification handler code here
	CSerial* pSerial = _parent->m_pSerial;
	char buffer[MAX_INIT_STRING_LEN];
	CString apNameStr, passwordStr, securityStr;
	char apNameBuffer[100], passwordBuffer[100], securityBuffer[100];
	bool bFound = false;

	int index;
	POSITION pos = m_apListCtrl.GetFirstSelectedItemPosition();

	if (pos == NULL) {
		return;
	}
	else {
		index = m_apListCtrl.GetNextSelectedItem(pos);
	}

	apNameStr = m_apListCtrl.GetItemText(index, 0);
	securityStr = m_apListCtrl.GetItemText(index, 1);
	
	m_passwordEdit.GetWindowText(passwordStr);
	
	WideCharToMultiByte(CP_UTF8, 0, apNameStr, -1, apNameBuffer, 100, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, passwordStr, -1, passwordBuffer, 100, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, securityStr, -1, securityBuffer, 100, NULL, NULL);

	bFound = true;
	do {
		int readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SET_OR_SCAN_MESSAGE), 1000);
		if (strlen(SET_OR_SCAN_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SET_OR_SCAN_MESSAGE, buffer, strlen(SET_OR_SCAN_MESSAGE))) {
			bFound = false;
			break;
		}

		pSerial->SendData("a", 1);

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SSID_MESSAGE), 1000);
		if (strlen(SSID_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SSID_MESSAGE, buffer, strlen(SSID_MESSAGE))) {
			bFound = false;
			break;
		}

		pSerial->SendData(apNameBuffer, strlen(apNameBuffer));
		pSerial->SendData("\r\n", 2);

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SECURITY_MESSAGE), 1000);
		if (strlen(SECURITY_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SECURITY_MESSAGE, buffer, strlen(SECURITY_MESSAGE))) {
			bFound = false;
			break;
		}

		pSerial->SendData(securityBuffer, 1);
		pSerial->SendData("\r\n", 2);

		if (securityBuffer[0] != '0') {
			readBytes = pSerial->ReadBytesOrWait(buffer, strlen(PASSWORD_MESSAGE), 1000);
			if (strlen(PASSWORD_MESSAGE) != readBytes) {
				bFound = false;
				break;
			}
			else if (strncmp(PASSWORD_MESSAGE, buffer, strlen(PASSWORD_MESSAGE))) {
				bFound = false;
				break;
			}

			pSerial->SendData(passwordBuffer, strlen(passwordBuffer));
			pSerial->SendData("\r\n", 2);
		}

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(CONNECTING_MESSAGE), 1000);
		if (strlen(CONNECTING_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(CONNECTING_MESSAGE, buffer, strlen(CONNECTING_MESSAGE))) {
			bFound = false;
			break;
		}
	} while (false);

	if (!bFound) {
		MessageBox(_T("센서노드와의 통신에 실패했습니다."), _T("에러"), MB_ICONERROR | MB_OK);
		return;
	}

	EnableControls(false);

	//Start connection check thread
	AfxBeginThread(ConnectMonitor, (LPVOID)this);
}


void CScanDialog::OnBnClickedRescanBtn()
{
	// TODO: Add your control notification handler code here
	ScanAp();
}


//int CScanDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	return 0;
//}


//int CScanDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  Add your specialized creation code here
//	
//
//	return 0;
//}


BOOL CScanDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 100;
	lvColumn.pszText = _T("SSID");
	m_apListCtrl.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 100;
	lvColumn.pszText = _T("Security");
	m_apListCtrl.InsertColumn(1, &lvColumn);

	m_LargeImg.Create(IDB_AP_BITMAP, 32, 1, RGB(255, 255, 245));
	m_apListCtrl.SetImageList(&m_LargeImg, LVSIL_NORMAL);

	LONG mListStyle = GetWindowLong(m_apListCtrl.m_hWnd, GWL_STYLE);

	LVITEM lvItem;
	int nItem;
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("Hydra");
	lvItem.iImage = 0;
	nItem = m_apListCtrl.InsertItem(&lvItem);

	mListStyle &= ~LVS_TYPEMASK;
	mListStyle |= LVS_ICON;
	SetWindowLong(m_apListCtrl, GWL_STYLE, mListStyle);

	m_connectProgress.SetMarquee(false, 50);
	m_connectProgress.ShowWindow(SW_HIDE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

static UINT ScanWorker (LPVOID pParam)
{
	CScanDialog * pDlg = (CScanDialog *)pParam;
	bool bFound = true;
	char buffer[1000];
	char lineBuffer[100];
	char ssidBuffer[100];
	int rssi;
	int security;
	char lineIndex = 0;
	int valueIndex = 0;
	CSerial * pSerial = pDlg->_parent->m_pSerial;
	int readBytes;
	CMemFile memFile;

	do {
		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SET_OR_SCAN_MESSAGE), 1000);
		if (strlen(SET_OR_SCAN_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SET_OR_SCAN_MESSAGE, buffer, strlen(SET_OR_SCAN_MESSAGE))) {
			bFound = false;
			break;
		}

		pSerial->SendData("s", 1);

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SCAN_FINISH_MESSAGE), 15000);
		if (strlen(SCAN_FINISH_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SCAN_FINISH_MESSAGE, buffer, strlen(SCAN_FINISH_MESSAGE))) {
			bFound = false;
			break;
		}

		do {
			readBytes = pSerial->ReadBytesOrWait(buffer, 1000, 1000);
			memFile.Write(buffer, readBytes);
		} while (readBytes != 0);

		memFile.SeekToBegin();
		
		char c;
		valueIndex = 0;
		lineIndex = 0;
		while (memFile.Read(&c, 1)) {
			if (c == '\n')
				continue;
			if (c == '\r') {
				if (valueIndex == 0) {
					//SSID
					strncpy_s(ssidBuffer, lineBuffer, 100);
					lineIndex = 0;
				}
				else if (valueIndex == 1) {
					rssi = atoi(lineBuffer);
					lineIndex = 0;
				}
				else {
					security = atoi(lineBuffer);
					pDlg->AddSsid(ssidBuffer, rssi, security);
					lineIndex = 0;
				}
				valueIndex++;
				if (valueIndex > 2) {
					valueIndex = 0;
				}
				continue;
			}
			lineBuffer[lineIndex++] = c;
			lineBuffer[lineIndex] = '\0';
		}
		pSerial->SendData(buffer, 1);	//ugly ugly
	} while (false);

	if (!bFound) {
		AfxMessageBox(_T("센서 노드와의 통신에 이상이 생겼습니다."), MB_OK | MB_ICONERROR);
	}

	pDlg->EnableControls(true);

	return 0;
}


void CScanDialog::ScanAp()
{
	int i, nCount;

	EnableControls(false);

	nCount = m_apListCtrl.GetItemCount();
	for (i = 0; i < nCount; i++) {
		m_apListCtrl.DeleteItem(0);
	}

	AfxBeginThread(ScanWorker, (LPVOID)this);

}


void CScanDialog::ConnectAp()
{
}


void CScanDialog::EnableControls(bool bEnable)
{
	if (bEnable) {
		m_apListCtrl.EnableWindow(true);
		m_passwordEdit.EnableWindow(true);
		m_connectBtn.EnableWindow(true);
		m_rescanBtn.EnableWindow(true);

		m_connectProgress.SetMarquee(false, 50);
		m_connectProgress.ShowWindow(SW_HIDE);
	}
	else {
		m_apListCtrl.EnableWindow(false);
		m_passwordEdit.EnableWindow(false);
		m_connectBtn.EnableWindow(false);
		m_rescanBtn.EnableWindow(false);

		m_connectProgress.SetMarquee(true, 50);
		m_connectProgress.ShowWindow(SW_SHOWNORMAL);
	}
}


void CScanDialog::AddSsid(char * ssid, int rssi, int security)
{
	LVITEM lvItem;
	CString strItem;

	int nItem;
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	strItem = CA2W(ssid);
	lvItem.pszText = (LPTSTR)(LPCTSTR)(strItem);
	if (rssi > -50) 
		lvItem.iImage = 0;
	else if (rssi > -75)
		lvItem.iImage = 1;
	else
		lvItem.iImage = 2;
	if (security == 0)
		lvItem.iImage += 3;

	nItem = m_apListCtrl.InsertItem(&lvItem);

	strItem.Format(_T("%d"), security);
	lvItem.iSubItem = 1;
	lvItem.pszText = (LPTSTR)(LPCTSTR)(strItem);
	m_apListCtrl.SetItem(&lvItem);
}


void CScanDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if (bShow) {
		ScanAp();
	}
}


void CScanDialog::OnLvnItemchangedApscanList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	// TODO: Add your control notification handler code here

	int index;
	POSITION pos = m_apListCtrl.GetFirstSelectedItemPosition();

	if (pos == NULL) {
		return;
	}
	else {
		index = m_apListCtrl.GetNextSelectedItem(pos);
	}
	
	CString securityStr = m_apListCtrl.GetItemText(index, 1);
	if (securityStr == _T("0")){
		m_passwordEdit.EnableWindow(false);
	}
	else{
		m_passwordEdit.EnableWindow(true); 
	}
}


void CScanDialog::OnApConnect()
{
	_parent->NextProcess(this);
}


void CScanDialog::OnApConnectFail()
{
	MessageBox(_T("AP 접속에 실패했습니다."), _T("에러"), MB_ICONERROR | MB_OK);
	EnableControls(true);
}
