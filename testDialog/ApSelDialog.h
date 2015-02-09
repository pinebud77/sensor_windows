#pragma once


// CApSelDialog dialog

class CApSelDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CApSelDialog)

public:
	CApSelDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CApSelDialog();

// Dialog Data
	enum { IDD = IDD_AP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
