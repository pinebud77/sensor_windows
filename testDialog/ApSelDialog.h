#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CApSelDialog dialog

class CtestDialogDlg;

class CApSelDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CApSelDialog)

public:
	CApSelDialog(CWnd* pParent = NULL, CtestDialogDlg * p=NULL);   // standard constructor
	virtual ~CApSelDialog();

// Dialog Data
	enum { IDD = IDD_AP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIdcancel();
	afx_msg void OnIdok();
	CEdit m_ApNameEdit;
	CEdit m_ApPasswordEdit;
	CComboBoxEx m_SecurityCB;
	afx_msg void OnBnClickedSetapBtn();
	afx_msg void OnBnClickedScanapBtn();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnSelchangeSecurityCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void OnApConnect();
	void OnApConnectFail();
	CtestDialogDlg * _parent;
	CButton m_SetApBtn;
	CButton m_ScanApBtn;
	CProgressCtrl m_ProgressCtrl;
};
