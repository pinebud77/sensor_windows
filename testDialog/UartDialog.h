#pragma once
#include "afxcmn.h"


// CUartDialog dialog
class CtestDialogDlg;

class CUartDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CUartDialog)

public:
	CUartDialog(CWnd* pParent = NULL, CtestDialogDlg * p = NULL);   // standard constructor
	virtual ~CUartDialog();

// Dialog Data
	enum { IDD = IDD_DETECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CtestDialogDlg* _parent;

	DECLARE_MESSAGE_MAP()
	CProgressCtrl m_Progress;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
