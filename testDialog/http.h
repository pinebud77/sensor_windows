#pragma once

#include <afxinet.h>
#include <afxsock.h>

class CHttpSession : public CInternetSession
{
	// Construction
public:
	CHttpSession(LPCTSTR szAgentName,
		DWORD dwContext,
		DWORD dwAccessType);

	virtual ~CHttpSession();
	void CommonConstruct();

	// Operations
public:
	void SetStatus(LPCTSTR fmt, ...);
	void SetStatusWnd(CWnd* pWnd)     { m_pStatusWnd = pWnd; }

	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpSession)
	//}}AFX_VIRTUAL
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus,
		LPVOID lpvStatusInformation,
		DWORD dwStatusInformationLength);

	// Generated message map functions
	//{{AFX_MSG(CHttpSession)
	//}}AFX_MSG

	// Attributes
protected:
	CWnd* m_pStatusWnd;
};

bool encode_char_str(char * input, char * output);
bool decode_char_str(char* input, char * output);
void decode_file(CFile* DataFile, CHttpFile* HttpFile);
int HttpCall(const TCHAR* ServerAddress,
	int ServerPort,
	const TCHAR* ServerProcess,
	const TCHAR* QueryString,
	const char* DataBlock,
	CFile* DataFile,
	CWnd* pStatusWnd);