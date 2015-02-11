#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CScanDialog dialog

class CtestDialogDlg;

class CScanDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CScanDialog)

public:
	CScanDialog(CWnd* pParent = NULL, CtestDialogDlg* p = NULL);   // standard constructor
	virtual ~CScanDialog();
	CtestDialogDlg * _parent;

// Dialog Data
	enum { IDD = IDD_SCAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedScanconnectBtn();
	afx_msg void OnBnClickedRescanBtn();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CListCtrl m_apListCtrl;
	CEdit m_passwordEdit;
	CImageList m_LargeImg;
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	CProgressCtrl m_connectProgress;
	void ScanAp();
	void ConnectAp();
	CButton m_connectBtn;
	CButton m_rescanBtn;
	void EnableControls(bool bEnable);
	void AddSsid(char * ssid, int rssi, int security);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLvnItemchangedApscanList(NMHDR *pNMHDR, LRESULT *pResult);
	void OnApConnect();
	void OnApConnectFail();
};
