// ApSelDialog.cpp : implementation file
//

#include "stdafx.h"
#include "testDialog.h"
#include "ApSelDialog.h"
#include "afxdialogex.h"


// CApSelDialog dialog

IMPLEMENT_DYNAMIC(CApSelDialog, CDialogEx)

CApSelDialog::CApSelDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CApSelDialog::IDD, pParent)
{

}

CApSelDialog::~CApSelDialog()
{
}

void CApSelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CApSelDialog, CDialogEx)
END_MESSAGE_MAP()


// CApSelDialog message handlers
