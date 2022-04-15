#pragma once
#include "utils.h"
#include <afxinet.h>
#include "WebServerParams.h"

class AFX_EXT_CLASS CWebServerUtils : public CUtils
{
public:
	CWebServerUtils(void);
	virtual ~CWebServerUtils(void);

public:
	void UrlEncodeData( CString& strData );

public:
	BOOL SendRequestToWebServer( CString strServerName, CString strTarget, CString strXMLToSend, CString& strResponse );
	BOOL SendRequestToWebServer( CWebServerParams& WebServerParams );
	BOOL UploadFileToFtpServer( CString& strErrMsg, CString strServerIP, CString strUserName, CString strPassword, CString strFileName, CString strOriginFilePath, CString strDestinationFilePath = CString( "/" ), int nPort = 21, BOOL bPassive = FALSE );

private:
	void DeleteServerUtilPointers( CHttpConnection*& pHttpConnection, CHttpFile*& pHttpFile );
};

