
// testDialogDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"


class CLoginDialog;
class CUartDialog;
class CApSelDialog;
class CScanDialog;
class CFinishDialog;

class CSerial;

// CtestDialogDlg 대화 상자
class CtestDialogDlg : public CDialogEx
{
// 생성입니다.
public:
	CtestDialogDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TESTDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel2();
	CTabCtrl m_tabCtrl;
	~CtestDialogDlg();
	void NextProcess(CDialogEx* pChild);
	CSerial* m_pSerial;
	CLoginDialog *m_pLoginDlg;
	CUartDialog * m_pUartDlg;
	CApSelDialog *m_pApSelDlg;
	CScanDialog * m_pScanDlg;
	CFinishDialog * m_pFinishDlg;
	void ToScanWindow();
	CString m_strMac;
	CString m_strUsername;
	CString m_strPassword;
};
