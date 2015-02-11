#pragma once
#include "afxwin.h"

class CtestDialogDlg;

// CFinishDialog dialog

class CFinishDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFinishDialog)

public:
	CFinishDialog(CWnd* pParent = NULL, CtestDialogDlg * p = NULL);   // standard constructor
	virtual ~CFinishDialog();
	CtestDialogDlg * _parent;

// Dialog Data
	enum { IDD = IDD_FINISH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIdcancel();
	afx_msg void OnIdok();
	CEdit m_nameEdit;
	afx_msg void OnBnClickedFinishBtn();
	CEdit m_reportPeriodEdit;
	CEdit m_alarmPeriodEdit;
	CEdit m_tRangeLowEdit;
	CEdit m_tRangeHighEdit;
	CEdit m_hRangeLowEdit;
	CEdit m_hRangeHighEdit;
	bool GetLoginHash(CString& usernameStr, CString& passwordStr, CString& secureHashStr);
	virtual BOOL OnInitDialog();
};
