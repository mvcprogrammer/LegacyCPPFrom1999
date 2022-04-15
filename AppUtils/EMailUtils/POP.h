#pragma once
#include "..\SocketUtils.h"
#include "..\..\AppUtils\Buffer.h"
#include "..\..\AppUtils\TextFileUtils.h"
#include "..\..\AppUtils\WinUtils.h"
#include "POP3DataList.h"

class AFX_EXT_CLASS CPOP
{
public:
	CPOP(void);
	virtual ~CPOP(void);
	BOOL GetMailFromFile( CPOP3DataList& POP3DataList, CEdit* pStatusBar, CString& strErrMsg );
	BOOL GetMail( CPOP3DataList& POP3DataList, CString strPOPServerAddress, int nPort, CString strUserId, CString strPassword, CEdit* pStatusBar, CString& strErrMsg );
	BOOL ParseEmailMessages( CPOP3DataList& POP3DataList, CEdit* pStatusMessage, CString& strErrMsg );
	BOOL GetNumOfMessages( CString strPOPCommandResponse, int& nNumOfMessages, int& nBufferSizeRequirement );

private:
	CSocketUtils m_SocketUtils;
	CTextFileUtils m_TextFileUtils;
	CWinUtils m_WinUtils;
	int m_nPostUpRequestId;
	int m_nPostDownRequestId;

private:
	BOOL ConnectPOPSocket( CSocket* pPOPSocket, CString strPOPServerAddress, int nPort, CString& strErrMsg );
	BOOL SendPOPUserId( CSocket* pPOPSocket, CString strUserId, CString& strErrMsg );
	BOOL SendPOPUserPassword( CSocket* pPOPSocket, CString strUserPassword, CString& strErrMsg );
	BOOL SendSTATCommand( CSocket* pPOPSocket, int& nNumOfMessages, int& nBufferSizeRequirement, CString& strErrMsg );
	BOOL SendLISTCommand( CSocket* pPOPSocket, int nNumOfMessages, CPOP3DataList& POP3DataList, CString& strErrMsg );
	BOOL SendRETRCommand( CSocket* pPOPSocket, int nMessageId, long lMessageSize, CString& strRETRCommandResponse, CString& strErrMsg, CEdit* pStatusBar );
	BOOL SendDELECommand( CSocket* pPOPSocket, int nMessageId, CString& strErrMsg );
	BOOL SendQUITCommand( CSocket* pPOPSocket, CString& strErrMsg );
	BOOL GotAllData( CBuffer* pBuffer, BOOL IsMultiLineData = FALSE );
	BOOL IsValidPOPResponse( CString strPOPResponse );
	BOOL IsValidMultiLinePOPResponse( CString strPOPCommandResponse );
	BOOL BuildRETRList( CString strLISTCommandResponse, int nNumOfMessages, CPOP3DataList& POP3DataList, CString& strErrMsg );
	BOOL GetEmailMessages( CSocket& POPSocket, CPOP3DataList& POP3DataList, CEdit* pStatusBar, CString& strErrMsg );
	BOOL ParseEmailHeader( CString strEmailMessage, CPOP3Data* pPOP3Data, CString& strErrMsg );
	BOOL ParseEmailSender( CPOP3Data* pPOP3Data, CString& strErrMsg );
	BOOL ParseRecipient( CPOP3Data* pPOP3Data, CString& strErrMsg );
	BOOL ParseSendDateTime( CPOP3Data* pPOP3Data, CString& strErrMsg );
	BOOL ParseSubject( CPOP3Data* pPOP3Data, CString& strErrMsg );
	BOOL ParseEmailPlainText( CString strEmailMessage, CPOP3Data* pPOP3Data, CString& strErrMsg );
	BOOL ParseSunshineField( CString strSunshineDataRaw, CString strFieldDelimiter, CString& strSunshineFieldData, CString& strErrMsg );
	BOOL ParseSunshineRequest( CPOP3Data*& pPOP3Data, CString& strErrMsg );
	BOOL ParseSunshinePostUpRequest( CPOP3Data*& pPOP3Data, CString& strErrMsg );
	BOOL ParseSunshinePostDownRequest( CPOP3Data*& pPOP3Data, CString& strErrMsg );
	BOOL SetIsValidPostRequest( CPOP3Data* pPOP3Data, CString& strErrMsg );
	BOOL SetPostServiceRequestId( CPOP3Data* pPOP3Data, CString strFileName, CString& strErrMsg );
};

