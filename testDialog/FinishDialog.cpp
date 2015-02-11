// FinishDialog.cpp : implementation file
//

#include "stdafx.h"
#include "testDialog.h"
#include "FinishDialog.h"
#include "afxdialogex.h"

#include <Windows.h>
#include <wincrypt.h>

#include "testDialogDlg.h"
#include "http.h"

// CFinishDialog dialog

IMPLEMENT_DYNAMIC(CFinishDialog, CDialogEx)

CFinishDialog::CFinishDialog(CWnd* pParent /*=NULL*/, CtestDialogDlg * p)
	: CDialogEx(CFinishDialog::IDD, pParent)
{
	_parent = p;
}

CFinishDialog::~CFinishDialog()
{
}

void CFinishDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SENSORNAME_EDIT, m_nameEdit);
	DDX_Control(pDX, IDC_REPORTPERIOD_EDIT, m_reportPeriodEdit);
	DDX_Control(pDX, IDC_ALARMPERIOD_EDIT, m_alarmPeriodEdit);
	DDX_Control(pDX, IDC_TRANGELOW_EDIT, m_tRangeLowEdit);
	DDX_Control(pDX, IDC_ETRANGEHIGH_EDIT, m_tRangeHighEdit);
	DDX_Control(pDX, IDC_HRANGELOW_EDIT, m_hRangeLowEdit);
	DDX_Control(pDX, IDC_HRANGEHIGH_EDIT, m_hRangeHighEdit);
}


BEGIN_MESSAGE_MAP(CFinishDialog, CDialogEx)
	ON_COMMAND(IDCANCEL, &CFinishDialog::OnIdcancel)
	ON_COMMAND(IDOK, &CFinishDialog::OnIdok)
	ON_BN_CLICKED(IDC_FINISH_BTN, &CFinishDialog::OnBnClickedFinishBtn)
END_MESSAGE_MAP()


// CFinishDialog message handlers


void CFinishDialog::OnIdcancel()
{
	// TODO: Add your command handler code here
}


void CFinishDialog::OnIdok()
{
	// TODO: Add your command handler code here
}


void CFinishDialog::OnBnClickedFinishBtn()
{
	// TODO: Add your control notification handler code here
	int tLowThreshold, tHighThreshold;
	int hLowThreshold, hHighThreshold;
	int alarmPeriod, reportPeriod;
	CString strTemp;
	char dataBuffer[1000] = "";
	char itemBuffer[300] = "";
	char encBuffer[300] = "";
	bool bError = false;
	CString strName;
	CWnd* pWndRet = GetDlgItem(IDC_LOGIN_BTN);
	CString hashResultStr;
	CMemFile MemFile;
	char return_buffer[500];
	CString returnedData;
	int retVal;

	m_nameEdit.GetWindowText(strName);
	if (strName == _T("")) {
		MessageBox(_T("센서 이름을 설정하지 않았습니다."), _T("에러"), MB_ICONERROR | MB_OK);
		return;
	}

	WideCharToMultiByte(CP_UTF8, 0, strName, -1, itemBuffer, 300, NULL, NULL);
	encode_char_str(itemBuffer, encBuffer);
	sprintf_s(dataBuffer, "name=%s", encBuffer);

	GetLoginHash(_parent->m_strUsername, _parent->m_strPassword, hashResultStr);

	WideCharToMultiByte(CP_UTF8, 0, _parent->m_strUsername, -1, itemBuffer, 300, NULL, NULL);
	encode_char_str(itemBuffer, encBuffer);
	sprintf_s(itemBuffer, "&username=%s", encBuffer);
	strcat_s(dataBuffer, itemBuffer);

	WideCharToMultiByte(CP_UTF8, 0, _parent->m_strPassword, -1, itemBuffer, 300, NULL, NULL);
	encode_char_str(itemBuffer, encBuffer);
	sprintf_s(itemBuffer, "&password=%s", encBuffer);
	strcat_s(dataBuffer, itemBuffer);

	WideCharToMultiByte(CP_UTF8, 0, hashResultStr, -1, itemBuffer, 300, NULL, NULL);
	encode_char_str(itemBuffer, encBuffer);
	sprintf_s(itemBuffer, "&secure_key=%s", encBuffer);
	strcat_s(dataBuffer, itemBuffer);

	WideCharToMultiByte(CP_UTF8, 0, _parent->m_strMac, -1, itemBuffer, 300, NULL, NULL);
	encode_char_str(itemBuffer, encBuffer);
	sprintf_s(itemBuffer, "&mac_address=%s", encBuffer);
	strcat_s(dataBuffer, itemBuffer);

	m_tRangeLowEdit.GetWindowText(strTemp);
	if (strTemp == _T("")) {
		sprintf_s(itemBuffer, "&t_low=N");
		tLowThreshold = -10000;
	}
	else {
		tLowThreshold = _ttoi(strTemp);
		sprintf_s(itemBuffer, "&t_low=%d", tLowThreshold);
	}
	strcat_s(dataBuffer, itemBuffer);

	m_tRangeHighEdit.GetWindowText(strTemp);
	if (strTemp == _T("")) {
		sprintf_s(itemBuffer, "&t_high=N");
		tHighThreshold = 10000;
	}
	else {
		tHighThreshold = _ttoi(strTemp);
		sprintf_s(itemBuffer, "&t_high=%d", tHighThreshold);
	}
	strcat_s(dataBuffer, itemBuffer);

	m_hRangeLowEdit.GetWindowText(strTemp);
	if (strTemp == _T("")) {
		sprintf_s(itemBuffer, "&h_low=N");
		hLowThreshold = -10000;
	}
	else {
		hLowThreshold = _ttoi(strTemp);
		sprintf_s(itemBuffer, "&h_low=%d", hLowThreshold);
	}
	strcat_s(dataBuffer, itemBuffer);


	m_hRangeHighEdit.GetWindowText(strTemp);
	if (strTemp == _T("")) {
		sprintf_s(itemBuffer, "&h_high=N");
		hHighThreshold = 10000;
	}
	else {
		hHighThreshold = _ttoi(strTemp);
		sprintf_s(itemBuffer, "&h_high=%d", hHighThreshold);
	}
	strcat_s(dataBuffer, itemBuffer);


	m_reportPeriodEdit.GetWindowText(strTemp);
	if (strTemp == _T("")) {
		MessageBox(_T("센서 리포트 주기를 설정하지 않았습니다."), _T("에러"), MB_ICONERROR | MB_OK);
		return;
	}
	else {
		reportPeriod = _ttoi(strTemp);
		reportPeriod *= 60;
		sprintf_s(itemBuffer, "&report_period=%d", reportPeriod);
	}
	strcat_s(dataBuffer, itemBuffer);


	m_alarmPeriodEdit.GetWindowText(strTemp);
	if (strTemp == _T("")) {
		sprintf_s(itemBuffer, "&alarm_period=N");
	}
	else {
		alarmPeriod = _ttoi(strTemp);
		alarmPeriod *= 60;
		sprintf_s(itemBuffer, "&alarm_period=%d", alarmPeriod);
	}
	strcat_s(dataBuffer, itemBuffer);

	if (tLowThreshold >= tHighThreshold) {
		MessageBox(_T("온도 범위가 잘못되었습니다."), _T("에러"), MB_ICONERROR | MB_OK);
		return;
	}

	if (hLowThreshold >= hHighThreshold) {
		MessageBox(_T("습도 범위가 잘못되었습니다."), _T("에러"), MB_ICONERROR | MB_OK);
		return;
	}


	retVal = HttpCall(_T(SERVER_ADDRESS),
		80,
		_T("/sensor/appnode/"),
		_T(""),
		dataBuffer,
		&MemFile,
		pWndRet
		);
	ULONGLONG DataLen = MemFile.GetLength();
	MemFile.SeekToBegin();
	if (DataLen < 500 - 1) {
		MemFile.Read(return_buffer, (UINT)DataLen);
		return_buffer[DataLen] = '\0';
	}
	else {
		MemFile.Read(return_buffer, (UINT)500);
		return_buffer[500 - 1] = '\0';
	}

	CString resultStr = CA2W(return_buffer);
	returnedData.ReleaseBuffer();
	if (resultStr == _T("+suck=")) {
		_parent->NextProcess(this);
	}
	else {
		MessageBox(_T("센서 노드 업데이트/등록이 실패하였습니다."), _T("에러"), MB_ICONERROR | MB_OK);
	}
}


bool CFinishDialog::GetLoginHash(CString& usernameStr, CString& passwordStr, CString& hashResultStr)
{
	DWORD dwStatus = 0;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	CString hashStr;
	DWORD cbHash = 0;
	BYTE rgbHash[40];
	CHAR rgbDigits[] = "0123456789abcdef";
	CHAR utfBuffer[400];

	hashStr = _T("hello") + usernameStr + passwordStr + _T("application");

	WideCharToMultiByte(CP_UTF8, 0, hashStr, -1, utfBuffer, 400, NULL, NULL);

	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		return false;
	}

	if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
	{
		CryptReleaseContext(hProv, 0);
		return false;
	}

	if (!CryptHashData(hHash, (const BYTE *)utfBuffer, strlen(utfBuffer), NULL)) {
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		return false;
	}

	cbHash = 40;

	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
		for (DWORD i = 0; i < cbHash; i++) {
			hashResultStr += rgbDigits[rgbHash[i] >> 4];
			hashResultStr += rgbDigits[rgbHash[i] & 0xf];
		}
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return true;
}


BOOL CFinishDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_reportPeriodEdit.SetWindowText(_T("10"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
