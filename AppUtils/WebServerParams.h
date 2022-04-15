#pragma once
#include "utils.h"
class AFX_EXT_CLASS CWebServerParams : public CUtils
{
	enum
	{
		WEB_SERVER_REQUEST_TYPE_GET = 1,
		WEB_SERVER_REQUEST_TYPE_POST
	};

public:
	CWebServerParams(void);
	virtual ~CWebServerParams(void);

private:
	BOOL m_bIsSecure;
	CString m_strWebServerAddress;
	CString m_strTarget;
	CString m_strDataToSend;
	CString m_strResponse;
	CString m_strUserAgent;
	CString m_strHttpVerb;
	CString m_strReferer;
	CString m_strAcceptTypes;
	CString m_strVersion;
	CString m_strWebServerErrMsg;
	CString m_strContentType;
	CString m_strHeader;

public:
	BOOL Validate( CString& strErrMsg );
	void SetIsSecure( BOOL bIsSecure );
	BOOL GetIsSecure();
	void SetWebServerAddress( CString strWebServerAddress );
	CString GetWebServerAddress();
	void SetTarget( CString strTarget );
	CString GetTarget();
	void SetDataToSend( CString strDataToSend );
	CString GetDataToSend();
	void SetResponse( CString strResponse );
	CString GetResponse();
	void SetUserAgent( CString strUserAgent );
	CString GetUserAgent();
	void SetHttpVerb( CString strHttpVerb );
	CString GetHttpVerb();
	void SetReferer( CString strReferer );
	CString GetReferer();
	void SetAcceptTypes( CString strAcceptTypes );
	CString GetAcceptType();
	void SetVersion( CString strVersion );
	CString GetVersion();
	void SetWebServerErrMsg( CString strWebServerErrMsg );
	CString GetWebServerErrMsg();
	void SetContentType( CString strContentType );
	CString GetContentType();
	int GetContentLength();
	CString GetHeader();
	void SetHeader( CString strHeader );
};

