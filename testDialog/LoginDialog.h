#pragma once
#include "afxwin.h"


// CLoginDialog dialog

class CtestDialogDlg;

class CLoginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog(CWnd* pParent = NULL, CtestDialogDlg* p = NULL);   // standard constructor
	virtual ~CLoginDialog();

// Dialog Data
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool IsPhoneValid(CString& phoneNumberStr);
	CtestDialogDlg* _parent;

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_loginUsernameEdit;
	CEdit m_loginPasswordEdit;
	CEdit m_usernameEdit;
	CEdit m_passwordEdit;
	CEdit m_passwordConfirmEdit;
	CEdit m_familyNameEdit;
	CEdit m_firstNameEdit;
	CEdit m_mailEdit;
	CEdit m_phoneEdit;
	CListBox m_phoneList;
	bool m_bLoggedIn;
	afx_msg void OnBnClickedPhoneaddBtn();
	afx_msg void OnBnClickedLoginBtn();
	afx_msg void OnBnClickedRegisterBtn();
	bool GetLoginHash(CString& usernameStr, CString& passwordStr, CString& hashStr);
	bool TryLogin(CString& username, CString& passwordStr);
	afx_msg void OnIdcancel();
	afx_msg void OnIdok();
	afx_msg void OnEnChangePasswdConfirmEdit();
};
