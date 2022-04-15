#pragma once
#include "..\SocketUtils.h"
#include "..\WinUtils.h"
#include "SMTPData.h"

class AFX_EXT_CLASS CSMTP
{
public:
	CSMTP(void);
	virtual ~CSMTP(void);
	CSocketUtils m_SocketUtils;
	CWinUtils m_WinUtils;
	BOOL SendEmail( CSMTPData* pSMTPData, CString& strErrMsg );
	BOOL SendMailWithAuth(CSMTPData* pSMTPData, CString& strErrMsg);
	BOOL ConnectSMTPSocket( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg );
	BOOL SendEHLOCommand( CSocket& SMTPSocket, CString& strErrMsg );
	BOOL SendAUTHCommand( CSocket& SMTPSocket, CString& strErrMsg );
	BOOL SendLogin( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg );
	BOOL SendPassword( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg );
	BOOL SendSenderEmail( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg );
	BOOL SendReciepient( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg );
	BOOL SendDataCommand( CSocket& SMTPSocket, CString& strErrMsg );
	BOOL SendData( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg );
	BOOL SendQUITCommand( CSocket& SMTPSocket, CString& strErrMsg );
	BOOL GetSMTPDateTime( CString& strSMTPDateTime );
};
