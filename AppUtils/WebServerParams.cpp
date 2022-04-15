#include "StdAfx.h"
#include "WebServerParams.h"


CWebServerParams::CWebServerParams(void)
{
	SetIsSecure( FALSE );
	SetUserAgent( "Worlds+Best+Web+Designs" );
	SetVersion( "HTTP/1.1" );
	SetHttpVerb( "GET" );
	SetAcceptTypes( "text/html" );
	SetUserAgent( "" );
	
}

CWebServerParams::~CWebServerParams(void)
{
}

BOOL CWebServerParams::Validate( CString& strErrMsg )
{
	if( GetWebServerAddress().GetLength() == 0 )
	{
		strErrMsg = "Server address required.";
		return FALSE;
	}

	if( GetHttpVerb() == "POST" && GetContentLength() == 0 )
	{
		strErrMsg = "Content type required with a POST command.";
		return FALSE;
	}

	return TRUE;
}

void CWebServerParams::SetIsSecure( BOOL bIsSecure )
{
	m_bIsSecure = bIsSecure;
}

BOOL CWebServerParams::GetIsSecure()
{
	return m_bIsSecure;
}

void CWebServerParams::SetWebServerAddress( CString strWebServerAddress )
{
	m_strWebServerAddress = strWebServerAddress;
}

CString CWebServerParams::GetWebServerAddress()
{
	return m_strWebServerAddress;
}

void CWebServerParams::SetTarget( CString strTarget )
{
	strTarget.Remove( '#' );
	strTarget.Replace( ' ', '+' );
	m_strTarget = strTarget;
}

CString CWebServerParams::GetTarget()
{
	return m_strTarget;
}

void CWebServerParams::SetDataToSend( CString strDataToSend )
{
	m_strDataToSend = strDataToSend;
}

CString CWebServerParams::GetDataToSend()
{
	return m_strDataToSend;
}

void CWebServerParams::SetResponse( CString strResponse )
{
	m_strResponse = strResponse;
}

CString CWebServerParams::GetResponse()
{
	return m_strResponse;
}

void CWebServerParams::SetUserAgent( CString strUserAgent )
{
	m_strUserAgent = strUserAgent;
}

CString CWebServerParams::GetUserAgent()
{
	return m_strUserAgent;
}

void CWebServerParams::SetHttpVerb( CString strHttpVerb )
{
	m_strHttpVerb = strHttpVerb;
}

CString CWebServerParams::GetHttpVerb()
{
	return m_strHttpVerb;
}

void CWebServerParams::SetReferer( CString strReferer )
{
	m_strReferer = strReferer;
}

CString CWebServerParams::GetReferer()
{
	return m_strReferer;
}

void CWebServerParams::SetAcceptTypes( CString strAcceptTypes )
{
	m_strAcceptTypes = strAcceptTypes;
}

CString CWebServerParams::GetAcceptType()
{
	return m_strAcceptTypes;
}

void CWebServerParams::SetVersion( CString strVersion )
{
	m_strVersion = strVersion;
}

CString CWebServerParams::GetVersion()
{
	return m_strVersion;
}

void CWebServerParams::SetWebServerErrMsg( CString strWebServerErrMsg )
{
	m_strWebServerErrMsg = strWebServerErrMsg;
}

CString CWebServerParams::GetWebServerErrMsg()
{
	return m_strWebServerErrMsg;
}

void CWebServerParams::SetContentType( CString strContentType )
{
	m_strContentType = strContentType;
}

CString CWebServerParams::GetContentType()
{
	ASSERT( GetContentLength() > 0 );
	ASSERT( GetHttpVerb() == "POST" );

	CString strContentType = "Content-Type: ";
	strContentType += m_strContentType;
	strContentType += "\nContent-Length: ";
	strContentType += NumToStr( GetContentLength());

	return strContentType;
}

int CWebServerParams::GetContentLength()
{
	return GetDataToSend().GetLength();
}

void CWebServerParams::SetHeader( CString strHeader )
{
	m_strHeader = strHeader;
}

CString CWebServerParams::GetHeader()
{
	return m_strHeader;
}