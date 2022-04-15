#include "StdAfx.h"
#include "WebServerUtils.h"


CWebServerUtils::CWebServerUtils(void)
{
}

CWebServerUtils::~CWebServerUtils(void)
{
}

BOOL CWebServerUtils::UploadFileToFtpServer( CString& strErrMsg, CString strServerIP, CString strUserName, CString strPassword, CString strFileName, CString strOriginFilePath, CString strDestinationFilePath, int nPort, BOOL bPassive )
{
	CString strCallingFunction = "CWebServerUtils::UploadFileToFtpServer";

	CFileFind FileFinder;

	if( !( FileFinder.FindFile( strOriginFilePath + strFileName )))
	{
		strErrMsg = "File not found.";
		SendAdminAlert( strErrMsg, strCallingFunction );
		return FALSE;
	}

	CInternetSession InternetSession;
	CFtpConnection* pFtpConnection = NULL;

	try
	{
		pFtpConnection = InternetSession.GetFtpConnection( strServerIP, strUserName, strPassword, nPort, bPassive );

		if( pFtpConnection == NULL )
		{
			strErrMsg = "Invalid FTP connection pointer detected.";
			SendAdminAlert( strErrMsg, strCallingFunction );
			return FALSE;
		}

		if( !( pFtpConnection->PutFile( strOriginFilePath + strFileName, strDestinationFilePath + strFileName )))
		{
			strErrMsg = "Failed to put the file on the remote server.";
			InternetSession.Close();
			pFtpConnection->Close();

			if( pFtpConnection != NULL )
				delete pFtpConnection;

			pFtpConnection = NULL;

			return FALSE;
		}

		pFtpConnection->Command( "QUIT" );
	}
	catch( CInternetException* pException )
	{
		TCHAR szErr[1024];
		pException->GetErrorMessage(szErr, 1024);
		SendAdminAlert( CString( szErr ), strCallingFunction );
		strErrMsg = CString( szErr );
		pException->Delete();

		if( pFtpConnection != NULL )
			delete pFtpConnection;

		pFtpConnection = NULL;
		return FALSE;
	}

	pFtpConnection->Close();
	InternetSession.Close();

	if( pFtpConnection != NULL )
		delete pFtpConnection;

	pFtpConnection = NULL;
	return TRUE;
}

BOOL CWebServerUtils::SendRequestToWebServer( CString strServerName, CString strTarget, CString strXMLToSend, CString& strResponse )
{
	CString strCallingFunction = "CWebServerUtils::SendRequestToServer()";

	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;

	try
	{
		CInternetSession InternetSession( _T("neal@sunnybeachsoftware.com"), 1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE );
		pHttpConnection = InternetSession.GetHttpConnection( _T( strServerName ), INTERNET_FLAG_NO_AUTO_REDIRECT, 80, NULL, NULL );

		if( pHttpConnection == NULL )
		{
			SendAdminAlert( "Invalid HttpConnection pointer detected.", strCallingFunction );
			return FALSE;
		}

		DWORD dwHttpRequestFlags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD;
		pHttpFile = pHttpConnection->OpenRequest( _T("POST"), _T( strTarget ), NULL, 1, NULL, NULL, dwHttpRequestFlags );

		if( pHttpFile == NULL )
		{
			SendAdminAlert( "Invalid HttpFile pointer detected.", strCallingFunction );
			DeleteServerUtilPointers( pHttpConnection, pHttpFile );
			return FALSE;
		}

		CString strHeader = "Content-Type: text/xml; charset=utf-8\nContent-Length:" + NumToStr( strXMLToSend.GetLength());
		pHttpFile->AddRequestHeaders( strHeader );

		if( !( pHttpFile->SendRequest(( LPCTSTR )strHeader, ( DWORD )strHeader.GetLength(), ( LPVOID )( LPCTSTR )strXMLToSend, ( DWORD )strXMLToSend.GetLength())))
		{
			SendAdminAlert( "Failure sending Http request.", strCallingFunction );
			DeleteServerUtilPointers( pHttpConnection, pHttpFile );
			return FALSE;
		}

		DWORD dwRet = 0;
		pHttpFile->QueryInfoStatusCode(dwRet);

		if( dwRet != HTTP_STATUS_OK )
		{
			SendAdminAlert( "HTTP status error: " + NumToStr(( long )dwRet ), strCallingFunction );
			DeleteServerUtilPointers( pHttpConnection, pHttpFile );
			return FALSE;
		}

		CString strLine;

		while( pHttpFile->ReadString( strLine ))
		{
			strResponse += strLine;
		}

		pHttpFile->Close();
		pHttpConnection->Close();
		InternetSession.Close();

		DeleteServerUtilPointers( pHttpConnection, pHttpFile );
	}
	catch( CInternetException* pException )
	{
		TCHAR szErr[1024];
		pException->GetErrorMessage(szErr, 1024);
		SendAdminAlert( CString( szErr ), strCallingFunction );
		DeleteServerUtilPointers( pHttpConnection, pHttpFile );
		pException->Delete();
		return FALSE;
	}

	return TRUE;
}

BOOL CWebServerUtils::SendRequestToWebServer( CWebServerParams& WebServerParams )
{
	CString strCallingFunction = "CWebServerUtils::SendRequestToSecureWebServer()";

	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;

	try
	{
		CInternetSession InternetSession( _T( WebServerParams.GetUserAgent()), 1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE );
		
		if( WebServerParams.GetIsSecure())
			pHttpConnection = InternetSession.GetHttpConnection( _T( WebServerParams.GetWebServerAddress()), INTERNET_DEFAULT_HTTPS_PORT, 443, NULL, NULL );
		else
			pHttpConnection = InternetSession.GetHttpConnection( _T( WebServerParams.GetWebServerAddress()), INTERNET_DEFAULT_HTTP_PORT, 80, NULL, NULL );

		if( pHttpConnection == NULL )
		{
			SendAdminAlert( "Invalid HttpConnection pointer detected.", strCallingFunction );
			return FALSE;
		}

		DWORD dwHttpRequestFlags = NULL;
		
		if( WebServerParams.GetIsSecure())
			dwHttpRequestFlags = INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;

		pHttpFile = pHttpConnection->OpenRequest( _T( WebServerParams.GetHttpVerb()), _T( WebServerParams.GetTarget()), NULL, 1, NULL, NULL, dwHttpRequestFlags );

		if( pHttpFile == NULL )
		{
			SendAdminAlert( "Invalid HttpFile pointer detected.", strCallingFunction );
			DeleteServerUtilPointers( pHttpConnection, pHttpFile );
			return FALSE;
		}

		CString strHeader;

		if( WebServerParams.GetHttpVerb() == "POST" )
		{
			strHeader += WebServerParams.GetContentType();
			pHttpFile->AddRequestHeaders( strHeader );
		}

		if( strHeader.GetLength() > 0 )
		{
			if( !( pHttpFile->SendRequest(( LPCTSTR )strHeader, ( DWORD )strHeader.GetLength(), ( LPVOID )( LPCTSTR )WebServerParams.GetDataToSend(), ( DWORD )WebServerParams.GetDataToSend().GetLength())))
			{
				SendAdminAlert( "Failure sending Http request.", strCallingFunction );
				DeleteServerUtilPointers( pHttpConnection, pHttpFile );
				return FALSE;
			}
		}
		else
		{
			if( !( pHttpFile->SendRequest( NULL, NULL, NULL, NULL )))
			{
				SendAdminAlert( "Failure sending Http request.", strCallingFunction );
				DeleteServerUtilPointers( pHttpConnection, pHttpFile );
				return FALSE;
			}
		}

		DWORD dwRet = 0;
		pHttpFile->QueryInfoStatusCode( dwRet );

		if( dwRet != HTTP_STATUS_OK )
		{
			SendAdminAlert( "HTTP status error: " + NumToStr(( long )dwRet ), strCallingFunction );
			DeleteServerUtilPointers( pHttpConnection, pHttpFile );
			return FALSE;
		}

		CString strResponse;
		CString strLine;

		while( pHttpFile->ReadString( strLine ))
		{
			strResponse += strLine;
		}

		WebServerParams.SetResponse( strResponse );

		CString strRawHeader;
		pHttpFile->QueryInfo( HTTP_QUERY_RAW_HEADERS_CRLF, strRawHeader );

		WebServerParams.SetHeader( strRawHeader );

		pHttpFile->Close();
		pHttpConnection->Close();
		InternetSession.Close();

		DeleteServerUtilPointers( pHttpConnection, pHttpFile );
	}
	catch( CInternetException* pException )
	{
		TCHAR szErr[1024];
		pException->GetErrorMessage(szErr, 1024);
		WebServerParams.SetWebServerErrMsg( CString( szErr ));
		SendAdminAlert( CString( szErr ), strCallingFunction );
		DeleteServerUtilPointers( pHttpConnection, pHttpFile );
		pException->Delete();
		return FALSE;
	}

	return TRUE;
}

void CWebServerUtils::DeleteServerUtilPointers( CHttpConnection*& pHttpConnection, CHttpFile*& pHttpFile )
{
	if( pHttpConnection )
	{
		delete pHttpConnection;
		pHttpConnection = NULL;
	}

	if( pHttpFile )
	{
		delete pHttpFile;
		pHttpFile = NULL;
	}
}

void CWebServerUtils::UrlEncodeData( CString& strData )
{
	strData.Replace( "%", "%25" );
	strData.Replace( "+", "%2B" );
	strData.Replace( ";", "%3B" );
	strData.Replace( "?", "%3F" );
	strData.Replace( "/", "%2F" );
	strData.Replace( ":", "%3A" );
	strData.Replace( "#", "%23" );
	strData.Replace( "&", "%26" );
	strData.Replace( "=", "%3D" );
	strData.Replace( "$", "%24" );
	strData.Replace( ",", "%2C" );
	strData.Replace( " ", "+"   );
	strData.Replace( "<", "%3C" );
	strData.Replace( ">", "%3E" );
	strData.Replace( "~", "%7E" );
	strData.Replace( "\"", "%22" );
	strData.Replace( "'", "%27" );
	strData.Replace( "{", "%7B" );
	strData.Replace( "}", "%7D" );
	strData.Replace( "|", "%7C" );
	strData.Replace( "\\", "%5C" );
	strData.Replace( "^", "%5E" );
	strData.Replace( "[", "%5B" );
	strData.Replace( "]", "%5D" );
	strData.Replace( "`", "%60" );
	strData.Replace( "?", "%3F" );
	strData.Replace( "@", "%40" );
}