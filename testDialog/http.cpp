#include "stdafx.h"
#include "http.h"


bool encode_char_str(const char * input, char* output)
{
	int i, max, oi;
	unsigned short asc;
	char c;
	char buffer[10];
	
	max = strlen(input);
	oi = 0;
	for (i = 0; i < max; i++) {
		c = input[i];
		asc = c;
		if (asc>47 && asc < 58) {
			output[oi++] = c;
			output[oi] = '\0';
		} else if (asc>64 && asc < 91) {
			output[oi++] = c;
			output[oi] = '\0';
		} else if (asc >96 && asc < 123) {
			output[oi++] = c;
			output[oi] = '\0';
		} else if (asc == 32) {
			output[oi++] = '+';
			output[oi] = '\0';
		} else {
			sprintf_s(buffer, "%%%2x", asc);
			int iv = buffer[1];
			if (iv == 32) {
				buffer[1] = '0';
			}
			strcat_s(output, 300, buffer);
			oi += strlen(buffer);
			output[oi] = '\0';
		}
	}

	return true;
}

bool decode_char_str(const char* input, char* output) {
	char buffer[10];
	int i, max, oi;
	unsigned short asc;
	unsigned char c;

	max = strlen(input);
	oi = 0;
	for (i = 0; i < max;) {
		c = input[i];
		asc = c;
		if (asc == 37) {
			strncpy_s(buffer, &input[i + 1], 2);
			i += 3;
			sscanf_s(buffer, "%2x", &asc);
			output[oi++] = (unsigned char)asc;
		} else if (asc == 43) {
			output[oi++] = ' ';
			i++;
		} else {
			output[oi++] = c;
			i++;
		}
	}
	output[oi] = '\0';

	return true;
}

void decode_file(CFile* DataFile, CHttpFile* HttpFile)
{
	char buffer[10];
	int i, max;
	unsigned short asc;
	unsigned char c;
	max = (unsigned int)HttpFile->GetLength();
	//	HttpFile->SeekToBegin();
	for (i = 0; i<max; i++)
	{
		HttpFile->Read(&c, 1);
		asc = c;
		if (asc == 37)
		{
			HttpFile->Read(buffer, 2);
			sscanf_s(buffer, "%2x", &asc);
			DataFile->Write(&asc, 1);
		}
		else if (asc == 43)
		{
			c = (char)32;
			DataFile->Write(&c, 1);
		}
		else
		{
			DataFile->Write(&c, 1);
		}
	}
	return;
}

int HttpCall(const TCHAR* ServerAddress,
	int ServerPort,
	const TCHAR* ServerProcess,
	const TCHAR* QueryString,
	const char* DataBlock,
	CFile* DataFile,
	CWnd* pStatusWnd)
{
	long lread;
	ULONGLONG max;
	DWORD ServiceType = INTERNET_SERVICE_HTTP;
	char lf[3];
	CHttpConnection* pHttpConnection = NULL;

	sprintf_s(lf, "%c%c", char(13), char(10));

	DWORD AccessType = PRE_CONFIG_INTERNET_ACCESS;
	
	CHttpSession* pHttpSession = new CHttpSession(_T("HttpCall"), 1, AccessType);
	pHttpSession->SetStatusWnd(pStatusWnd);

	pHttpConnection = pHttpSession->GetHttpConnection(ServerAddress, ServerPort, _T("anonymous"), _T(""));
	CHttpFile* pHttpFile = NULL;
	DWORD HttpRequestFlags;

	CString Process, Query;
	Process = ServerProcess;
	Query = QueryString;
	
	if (Query.GetLength() > 0) {
		Process += '?' + Query;
	}

	TRY{
		HttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE;

		DWORD TotalLen;
		TotalLen = strlen(DataBlock);

		if (TotalLen > 0) {
			pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,
				Process, NULL, 1, NULL, (LPCTSTR)"1.0", HttpRequestFlags);
		}
		else {
			pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,
				Process, NULL, 1, NULL, (LPCTSTR)"1.0", HttpRequestFlags);
		}

		CString strHeaders = _T("Connection: close\r\n");
		strHeaders += _T("Content-Type: application/x-www-form-urlencoded\r\n");
		CString lenStr;
		lenStr.Format(_T("%d"), TotalLen);
		strHeaders += _T("Content-Length: ");
		strHeaders += lenStr;
		strHeaders += _T("\r\n\r\n");
		pHttpFile->AddRequestHeaders(strHeaders.GetBuffer());
		strHeaders.ReleaseBuffer();

		if (TotalLen > 0) {
			pHttpFile->SendRequestEx(TotalLen, HSR_INITIATE, 1);
			pHttpFile->Write(DataBlock, strlen(DataBlock));
			pHttpFile->EndRequest();
		}
		else {
			pHttpFile->SendRequest();
		}
		max = pHttpFile->GetLength();
	} CATCH_ALL(e) {
		AfxMessageBox(_T("Connection to Server Failed"));
	}
	END_CATCH_ALL

		DWORD dwRet;
	pHttpFile->QueryInfoStatusCode(dwRet);

	DataFile->SeekToBegin();
	CString strRetBufLen;
	pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strRetBufLen);
	max = _ttol(strRetBufLen);
	if (max <= 0) {
		max = pHttpFile->GetLength();
	}

	lread = 1000;
	char c[1000];
	while (lread > 0)
	{
		lread = pHttpFile->Read(c, 1000);
		if (lread > 0) DataFile->Write(c, lread);
	}

	pHttpFile->Close();
	pHttpConnection->Close();
	pHttpSession->Close();
	delete pHttpFile;
	delete pHttpConnection;
	delete pHttpSession;

	return dwRet;
}


#define BUFFER_SIZE 4095

/////////////////////////////////////////////////////////////////////////////
// CHttpSession

CHttpSession::CHttpSession(LPCTSTR szAgentName = NULL,
	DWORD dwContext = 1,
	DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG)//,
	//LPCTSTR pstrProxyName = NULL, 
	//LPCTSTR pstrProxyBypass = NULL, 
	//DWORD dwFlags = 0 )
	:CInternetSession(szAgentName,
	dwContext,
	dwAccessType,
	NULL,//pstrProxyName, 
	NULL,//pstrProxyBypass, 
	0)//dwFlags) 
{
	CommonConstruct();
}

CHttpSession::~CHttpSession()
{
}

void CHttpSession::CommonConstruct()
{
	m_pStatusWnd = NULL;
	try {
		EnableStatusCallback(TRUE);
	}
	catch (...)
	{
	}
}
// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHttpSession, CInternetSession)
	//{{AFX_MSG_MAP(CHttpSession)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CHttpSession member functions

void CHttpSession::OnStatusCallback(DWORD dwContext,
	DWORD dwInternetStatus,
	LPVOID lpvStatusInformation,
	DWORD dwStatusInformationLength)
{
	// Status callbacks need thread-state protection. 
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CString str;

	switch (dwInternetStatus)
	{
	case INTERNET_STATUS_RESOLVING_NAME:
		str.Format(_T("Resolving name for %s"), lpvStatusInformation);
		break;

	case INTERNET_STATUS_NAME_RESOLVED:
		str.Format(_T("Resolved name for %s"), lpvStatusInformation);
		break;

	case INTERNET_STATUS_HANDLE_CREATED:
		//str.Format("Handle %8.8X created", hInternet);
		break;

	case INTERNET_STATUS_CONNECTING_TO_SERVER:
	{
		str.Format(_T("Connecting to socket address "));
	}
	break;

	case INTERNET_STATUS_REQUEST_SENT:
		str.Format(_T("Request sent"));
		break;

	case INTERNET_STATUS_SENDING_REQUEST:
		str.Format(_T("Sending request..."));
		break;

	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		str.Format(_T("Connected to socket address"));
		break;

	case INTERNET_STATUS_RECEIVING_RESPONSE:
		return;
		str.Format(_T("Receiving response..."));
		break;

	case INTERNET_STATUS_RESPONSE_RECEIVED:
		str.Format(_T("Response received"));
		break;

	case INTERNET_STATUS_CLOSING_CONNECTION:
		str.Format(_T("Closing the connection to the server"));
		break;

	case INTERNET_STATUS_CONNECTION_CLOSED:
		str.Format(_T("Connection to the server closed"));
		break;

	case INTERNET_STATUS_HANDLE_CLOSING:
		return;
		str.Format(_T("Handle closed"));
		break;

	case INTERNET_STATUS_REQUEST_COMPLETE:
		// See the CInternetSession constructor for details on INTERNET_FLAG_ASYNC.
		// The lpvStatusInformation parameter points at an INTERNET_ASYNC_RESULT 
		// structure, and dwStatusInformationLength contains the final completion 
		// status of the asynchronous function. If this is ERROR_INTERNET_EXTENDED_ERROR, 
		// the application can retrieve the server error information by using the 
		// Win32 function InternetGetLastResponseInfo. See the ActiveX SDK for more 
		// information about this function. 
		if (dwStatusInformationLength == sizeof(INTERNET_ASYNC_RESULT))
		{
			INTERNET_ASYNC_RESULT* pResult = (INTERNET_ASYNC_RESULT*)lpvStatusInformation;
			str.Format(_T("Request complete, dwResult = %8.8X, dwError = %8.8X"),
				pResult->dwResult, pResult->dwError);
		}
		else
			str.Format(_T("Request complete"));
		break;

	case INTERNET_STATUS_CTL_RESPONSE_RECEIVED:
	case INTERNET_STATUS_REDIRECT:
	default:
		str.Format(_T("Unknown status: %d"), dwInternetStatus);
		break;
	}

	SetStatus(str);

	TRACE(_T("CHttpSession::OnStatusCallback: %s\n"), str);
}

void CHttpSession::SetStatus(LPCTSTR fmt, ...)
{
	/*
	va_list args;
	TCHAR buffer[512];

	va_start(args, fmt);
	_vstprintf(buffer, fmt, args);
	va_end(args);

	TRACE1("CHttpSession::SetStatus: %s\n", buffer);

	if (m_pStatusWnd)
	{
		m_pStatusWnd->SetWindowText(buffer);
		m_pStatusWnd->RedrawWindow();
	}
	*/
}

