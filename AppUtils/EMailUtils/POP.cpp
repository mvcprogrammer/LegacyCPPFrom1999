#include "stdafx.h"
#include "POP.h"

CPOP::CPOP(void)
{
	m_nPostUpRequestId = 0;
	m_nPostDownRequestId = 0;
}


CPOP::~CPOP(void)
{
}

BOOL CPOP::GetMailFromFile( CPOP3DataList& POP3DataList, CEdit* pStatusBar, CString& strErrMsg )
{
	CFileFind FileFinder;
	CString strFilePath = "C:\\Emails\\Current\\";
	BOOL bFileExists = FileFinder.FindFile(_T( strFilePath + "*.txt"));

	while( bFileExists )
	{
	   bFileExists = FileFinder.FindNextFile();

	   CString strFileName = strFilePath + (LPCTSTR)FileFinder.GetFileName();

	   CStdioFile ReadingFile;

	   if( !( m_TextFileUtils.OpenFileForReading( &ReadingFile, strFileName )))
	   {
		   strErrMsg = "Failed to open file: " + strFileName;
		   return FALSE;
	   }

	   CString strLine;
	   CString strEmail;

	   while( m_TextFileUtils.ReadNextLineFromFile( &ReadingFile, strLine ))
	   {
			strLine += "\n";
			strEmail += strLine;
	   }

	   CPOP3Data* pPOP3Data = new CPOP3Data();

	   pPOP3Data->SetOrderStatus( ORDER_STATUS_FILED );

	   pPOP3Data->SetMessageRaw( strEmail );
	   SetPostServiceRequestId( pPOP3Data, (( LPCTSTR )FileFinder.GetFileName()), strErrMsg );

	   POP3DataList.AddPostRequestData( pPOP3Data );

	   strEmail = "";	  
	}

	return TRUE;
}

BOOL CPOP::SetPostServiceRequestId( CPOP3Data* pPOP3Data, CString strFileName, CString& strErrMsg )
{
	strFileName = strFileName.Mid( 0, strFileName.GetLength() - 4 );
	pPOP3Data->SetRequestId( strFileName );

	return TRUE;
}

BOOL CPOP::GetMail( CPOP3DataList& POP3DataList, CString strPOPServerAddress, int nPort, CString strUserId, CString strPassword, CEdit* pStatusBar, CString& strErrMsg )
{
	pStatusBar->SetWindowTextA( "Connecting to mail server..." );

	CSocket POPSocket;

	if( !( ConnectPOPSocket( &POPSocket, strPOPServerAddress, nPort, strErrMsg )))
		return FALSE;

	pStatusBar->SetWindowTextA( "Connected. Sending username and password..." );

	if( !( SendPOPUserId( &POPSocket, strUserId, strErrMsg )))
		return FALSE;

	if( !( SendPOPUserPassword( &POPSocket, strPassword, strErrMsg )))
		return FALSE;

	int nNumOfMessages = 0;
	int nBufferSizeRequirement = 0;

	pStatusBar->SetWindowTextA( "Checking mail..." );

	if( !( SendSTATCommand( &POPSocket, nNumOfMessages, nBufferSizeRequirement, strErrMsg )))
		return FALSE;

	if( nNumOfMessages == 0 )
	{
		SendQUITCommand( &POPSocket, strErrMsg );
		return TRUE;
	}

	if( !( SendLISTCommand( &POPSocket, nNumOfMessages, POP3DataList, strErrMsg )))
		return FALSE;

	if( !( GetEmailMessages( POPSocket, POP3DataList, pStatusBar, strErrMsg )))
		return FALSE;

	SendQUITCommand( &POPSocket, strErrMsg );

	return TRUE;
}

BOOL CPOP::ConnectPOPSocket( CSocket* pPOPSocket, CString strPOPServerAddress, int nPort, CString& strErrMsg )
{
	CBuffer ConnectResponseBuffer;
	CString strPOPCommandResponse;

	if( !( m_SocketUtils.CreateTalkSocket( pPOPSocket, strPOPServerAddress, nPort )))
	{
		strErrMsg = "Failed to connect to mail server.";
		return FALSE;
	}

	while( !( GotAllData( &ConnectResponseBuffer )))
	{
		CBuffer ReceiveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &ReceiveBuffer )))
		{
			strErrMsg = "Failure receiving initial data.";
			return FALSE;
		}

		ConnectResponseBuffer.AddDataToBuffer( ReceiveBuffer.GetBuffer(), ReceiveBuffer.GetBufferUsed());
	}
	
	strPOPCommandResponse = CString( ConnectResponseBuffer.GetBuffer(), ConnectResponseBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strPOPCommandResponse )))
	{
		strErrMsg = "Failed to connect to POP3 Server. " + strPOPCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP::SendPOPUserId( CSocket* pPOPSocket, CString strUserId, CString& strErrMsg )
{
	CString strPOPCommand;
	CString strPOPCommandResponse;
	
	strPOPCommand = "USER " + strUserId + "\r\n";

	if( !( m_SocketUtils.SocketSend( pPOPSocket, strPOPCommand.GetBuffer(), strPOPCommand.GetLength())))
	{
		strErrMsg = "Failed to send user command.";
		return FALSE;
	}

	CBuffer UserIdResponseBuffer;

	while( !( GotAllData( &UserIdResponseBuffer )))
	{
		CBuffer ReceiveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &ReceiveBuffer )))
		{
			strErrMsg = "Failed to receive user command response.";
			return FALSE;
		}

		UserIdResponseBuffer.AddDataToBuffer( ReceiveBuffer.GetBuffer(), ReceiveBuffer.GetBufferUsed());
	}

	strPOPCommandResponse = CString( UserIdResponseBuffer.GetBuffer(), UserIdResponseBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strPOPCommandResponse )))
	{
		strErrMsg = "User logon failed. " + strPOPCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP::SendPOPUserPassword( CSocket* pPOPSocket, CString strUserPassword, CString& strErrMsg )
{
	CString strPOPCommand;
	CString strPOPCommandResponse;
	
	strPOPCommand = "PASS " + strUserPassword + "\r\n";

	if( !( m_SocketUtils.SocketSend( pPOPSocket, strPOPCommand.GetBuffer(), strPOPCommand.GetLength())))
	{
		strErrMsg = "Failure sending user password.";
		return FALSE;
	}

	CBuffer PasswordResponseBuffer;

	while( !( GotAllData( &PasswordResponseBuffer )))
	{
		CBuffer ReceiveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &ReceiveBuffer )))
		{
			strErrMsg = "User Password Failed.";
			return FALSE;
		}

		PasswordResponseBuffer.AddDataToBuffer( ReceiveBuffer.GetBuffer(), ReceiveBuffer.GetBufferUsed());
	}

	strPOPCommandResponse = CString( PasswordResponseBuffer.GetBuffer(), PasswordResponseBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strPOPCommandResponse )))
	{
		strErrMsg = "User password failed. " + strPOPCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP::SendSTATCommand( CSocket* pPOPSocket, int& nNumOfMessages, int& nBufferSizeRequirement, CString& strErrMsg )
{
	CString strPOPCommand;
	CString strPOPCommandResponse;
	
	strPOPCommand = "STAT\r\n";

	if( !( m_SocketUtils.SocketSend( pPOPSocket, strPOPCommand.GetBuffer(), strPOPCommand.GetLength())))
	{
		strErrMsg = "Failed to send STAT command.";
		return FALSE;
	}

	CBuffer STATCommandResponseBuffer;

	while( !( GotAllData( &STATCommandResponseBuffer )))
	{
		CBuffer ReceiveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &ReceiveBuffer )))
		{
			strErrMsg = "Failure retrieving STAT command response";
			return FALSE;
		}

		STATCommandResponseBuffer.AddDataToBuffer( ReceiveBuffer.GetBuffer(), ReceiveBuffer.GetBufferUsed());
	}

	strPOPCommandResponse = CString( STATCommandResponseBuffer.GetBuffer(), STATCommandResponseBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strPOPCommandResponse )))
	{
		strErrMsg = "STAT command failed. " + strPOPCommandResponse;
		return FALSE;
	}

	if( !( GetNumOfMessages( strPOPCommandResponse, nNumOfMessages, nBufferSizeRequirement )))
	{
		strErrMsg = "Failed to get number of messages.";
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP::SendLISTCommand( CSocket* pPOPSocket, int nNumOfMessages, CPOP3DataList& POP3DataList, CString& strErrMsg )
{
	CString strPOPCommand;
	CString strPOPCommandResponse;
	
	strPOPCommand = "LIST\r\n";

	if( !( m_SocketUtils.SocketSend( pPOPSocket, strPOPCommand.GetBuffer(), strPOPCommand.GetLength())))
	{
		strErrMsg = "Failed to send LIST command.";
		return FALSE;
	}

	CBuffer LISTCommandResponseBuffer;

	while( !( GotAllData( &LISTCommandResponseBuffer, TRUE )))
	{
		CBuffer ReceiveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &ReceiveBuffer )))
		{
			strErrMsg = "Failure retrieving LIST command response";
			return FALSE;
		}

		LISTCommandResponseBuffer.AddDataToBuffer( ReceiveBuffer.GetBuffer(), ReceiveBuffer.GetBufferUsed());
	}

	strPOPCommandResponse = CString( LISTCommandResponseBuffer.GetBuffer(), LISTCommandResponseBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strPOPCommandResponse )))
	{
		strErrMsg = "LIST command failed. " + strPOPCommandResponse;
		return FALSE;
	}

	if( !( BuildRETRList( strPOPCommandResponse, nNumOfMessages, POP3DataList, strErrMsg )))
		return FALSE;

	return TRUE;
}

BOOL CPOP::SendRETRCommand( CSocket* pPOPSocket, int nMessageId, long lMessageSize, CString& strRETRCommandResponse, CString& strErrMsg, CEdit* pStatusBar )
{
	CString strPOPCommand;
	
	strPOPCommand = "RETR " + m_SocketUtils.NumToStr( nMessageId ) + "\r\n";

	if( !( m_SocketUtils.SocketSend( pPOPSocket, strPOPCommand.GetBuffer(), strPOPCommand.GetLength())))
	{
		strErrMsg = "Failed to send LIST command.";
		return FALSE;
	}

	CBuffer RETRCommandResponseBuffer( lMessageSize );

	while( !( GotAllData( &RETRCommandResponseBuffer, TRUE )))
	{
		CBuffer ReceiveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &ReceiveBuffer )))
		{
			strErrMsg = "Failure retrieving RETR command response";
			return FALSE;
		}

		RETRCommandResponseBuffer.AddDataToBuffer( ReceiveBuffer.GetBuffer(), ReceiveBuffer.GetBufferUsed());
	}

	strRETRCommandResponse = CString( RETRCommandResponseBuffer.GetBuffer(), RETRCommandResponseBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strRETRCommandResponse )))
	{
		strErrMsg = "RETR command failed. " + strRETRCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP::SendDELECommand( CSocket* pPOPSocket, int nMessageId, CString& strErrMsg )
{
	CString strPOPCommand;
	CString strPOPCommandResponse;

	strPOPCommand = "DELE " + m_SocketUtils.NumToStr( nMessageId ) + "\r\n";

	if( !( m_SocketUtils.SocketSend( pPOPSocket, strPOPCommand.GetBuffer(), strPOPCommand.GetLength())))
	{
		strErrMsg = "Failed to send DELETE command.";
		return FALSE;
	}

	CBuffer DELECommandBuffer;

	while( !( GotAllData( &DELECommandBuffer )))
	{
		CBuffer RecieveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &RecieveBuffer )))
		{
			strErrMsg = "Failure retrieving DELE command response.";
			return FALSE;
		}

		DELECommandBuffer.AddDataToBuffer( RecieveBuffer.GetBuffer(), RecieveBuffer.GetBufferUsed());
	}

	strPOPCommandResponse = CString( DELECommandBuffer.GetBuffer(), DELECommandBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strPOPCommandResponse )))
	{
		strErrMsg = "DELE command failed. " + strPOPCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP::SendQUITCommand( CSocket* pPOPSocket, CString& strErrMsg )
{
	CString strPOPCommand;
	CString strPOPCommandResponse;
	
	strPOPCommand = "QUIT\r\n";

	if( !( m_SocketUtils.SocketSend( pPOPSocket, strPOPCommand.GetBuffer(), strPOPCommand.GetLength())))
	{
		strErrMsg = "Failed to send QUIT command.";
		return FALSE;
	}

	CBuffer QUITCommandBuffer;

	while( !( GotAllData( &QUITCommandBuffer )))
	{
		CBuffer ReceiveBuffer;

		if( !( m_SocketUtils.SocketReceive( pPOPSocket, &ReceiveBuffer )))
		{
			strErrMsg = "Failure retrieving QUIT command response";
			return FALSE;
		}

		QUITCommandBuffer.AddDataToBuffer( ReceiveBuffer.GetBuffer(), ReceiveBuffer.GetBufferUsed());
	}

	strPOPCommandResponse = CString( QUITCommandBuffer.GetBuffer(), QUITCommandBuffer.GetBufferUsed());

	if( !( IsValidPOPResponse( strPOPCommandResponse )))
	{
		strErrMsg = "QUIT command failed. " + strPOPCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP::GetNumOfMessages( CString strPOPCommandResponse, int& nNumOfMessages, int& nBufferSizeRequirement )
{
	CUtils Utils;
	CString strFirstDelimiter = "+OK ";

	int nEndDelimiter = strPOPCommandResponse.Find( " ", strFirstDelimiter.GetLength());

	if( nEndDelimiter < strFirstDelimiter.GetLength())
		return FALSE;

	CString strNumOfMessages = strPOPCommandResponse.Mid( strFirstDelimiter.GetLength(), nEndDelimiter - strFirstDelimiter.GetLength());
	Utils.StringToInt( strNumOfMessages, nNumOfMessages );

	if( nNumOfMessages > 0 )
	{
		strPOPCommandResponse = strPOPCommandResponse.Mid( nEndDelimiter + 1 );
		nBufferSizeRequirement = Utils.StringToInt( strPOPCommandResponse );
	}

	return TRUE;
}

BOOL CPOP::BuildRETRList( CString strLISTCommandResponse, int nNumOfMessages, CPOP3DataList& POP3DataList, CString& strErrMsg )
{
	CString strBeginningMessageDelimiter = "+OK \r\n";
	CString strDataDelimiter = " ";
	CString strMessageDelimiter = "\r\n";
	CString strMultiLineMessageDelimiter = "\r\n.\r\n";
	int nBegChop = 0;
	int nEndChop = 0;
	int nDelimiterNotFound = -1;

	nBegChop += strBeginningMessageDelimiter.GetLength();

	for( int nCurDataMessage = 0; nCurDataMessage < nNumOfMessages; nCurDataMessage++ )
	{
		nEndChop = strLISTCommandResponse.Find( strDataDelimiter, nBegChop );

		if( nEndChop == nDelimiterNotFound )
		{
			strErrMsg = "Error detected while parsing RETR List";
			return FALSE;
		}

		int nMessageId = m_SocketUtils.StringToInt( strLISTCommandResponse.Mid( nBegChop, nEndChop - nBegChop ));

		ASSERT( nMessageId == ( nCurDataMessage + 1 ));

		nBegChop = nEndChop + strDataDelimiter.GetLength();

		nEndChop = strLISTCommandResponse.Find( strMessageDelimiter, nBegChop );

		int nMessageLength = m_SocketUtils.StringToInt( strLISTCommandResponse.Mid( nBegChop, nEndChop - nBegChop ));

		CPOP3Data* pPOP3Data = new CPOP3Data();
		pPOP3Data->SetMessageNum( nMessageId );
		pPOP3Data->SetMessageSize( nMessageLength );
		pPOP3Data->SetOrderStatus( ORDER_STATUS_PENDING_RETRIEVAL );

		POP3DataList.AddPostRequestData( pPOP3Data );

		nBegChop = nEndChop + strMessageDelimiter.GetLength();
	}

	return TRUE;
}

BOOL CPOP::GotAllData( CBuffer* pBuffer, BOOL IsMultiLineData )
{
	CString strMessageDelimiter = "\r\n";
	CString strMultiLineMessageDelimiter = "\r\n.\r\n";

	CString strMessageFromServer( pBuffer->GetBuffer(), pBuffer->GetBufferUsed());
	
	if( strMessageFromServer.GetLength() < strMessageDelimiter.GetLength())
		return FALSE;

	if( IsMultiLineData )
		return( strMessageFromServer.Mid(( strMessageFromServer.GetLength() - strMultiLineMessageDelimiter.GetLength()), strMultiLineMessageDelimiter.GetLength()) == strMultiLineMessageDelimiter );
	else
		return( strMessageFromServer.Mid(( strMessageFromServer.GetLength() - strMessageDelimiter.GetLength()), strMessageDelimiter.GetLength()) == strMessageDelimiter );
}

BOOL CPOP::IsValidPOPResponse( CString strPOPResponse )
{
	return( strPOPResponse.Find( "+OK", 0 ) == 0 );
}

BOOL CPOP::GetEmailMessages( CSocket& POPSocket, CPOP3DataList& POP3DataList, CEdit* pStatusBar, CString& strErrMsg )
{
	POSITION posDataList = POP3DataList.GetHeadPosition();
	int nNumMessages = 0;

	while( posDataList != NULL )
	{
		pStatusBar->SetWindowTextA( "Retrieved " + m_SocketUtils.NumToStr( ++nNumMessages ) + " Message(s)..." );

		CPOP3Data* pPOP3Data = ( CPOP3Data* )POP3DataList.GetNext( posDataList );

		if( pPOP3Data->GetOrderStatus() == ORDER_STATUS_FILED )
			continue;

		CString strRETRCommandResponse;

		if( !( SendRETRCommand( &POPSocket, pPOP3Data->GetMessageNum(), pPOP3Data->GetMessageSize(), strRETRCommandResponse, strErrMsg, pStatusBar )))
			return FALSE;

		strRETRCommandResponse.Replace( CString( "\"" ), "" );
		strRETRCommandResponse.Replace( "'", "" );

		pPOP3Data->SetMessageRaw( strRETRCommandResponse );

		if( !( SendDELECommand( &POPSocket, pPOP3Data->GetMessageNum(), strErrMsg )))
			return FALSE;

		pPOP3Data->SetOrderStatus( ORDER_STATUS_RETRIEVED );
	}

	return TRUE;
}

BOOL CPOP::ParseEmailMessages( CPOP3DataList& POP3DataList, CEdit* pStatusBar, CString& strErrMsg )
{
	POSITION posDataList = POP3DataList.GetHeadPosition();
	int nCurrentMessage = 0;

	while( posDataList != NULL )
	{
		CPOP3Data* pPOP3Data = ( CPOP3Data* )POP3DataList.GetNext( posDataList );

		if( !( ParseEmailHeader( pPOP3Data->GetMessageRaw(), pPOP3Data, strErrMsg )))
			return FALSE;

		if( !( SetIsValidPostRequest( pPOP3Data, strErrMsg )))
			return FALSE;

		if( !( pPOP3Data->GetIsValidPostRequest()))
			continue;

		pStatusBar->SetWindowTextA( "Parsing " + m_SocketUtils.NumToStr( ++nCurrentMessage ) + " Message(s)..." );

		if( !( ParseEmailPlainText( pPOP3Data->GetMessageRaw(), pPOP3Data, strErrMsg )))
			return FALSE;

		if( !( ParseSunshineRequest( pPOP3Data, strErrMsg )))
			return FALSE;

		if( !( POP3DataList.FileRequestItem( pPOP3Data, strErrMsg )))
			return FALSE;
	}

	return TRUE;
}

BOOL CPOP::ParseEmailHeader( CString strEmailMessage, CPOP3Data* pPOP3Data, CString& strErrMsg )
{
	CString strHeaderEndDelimiter = "\r\n\r\n";
	int nBegChop = 0;
	int nEndChop = strEmailMessage.Find( strHeaderEndDelimiter );

	if( nEndChop == -1 )
	{
		strErrMsg = "Failed to find Email header info.";
		return FALSE;
	}

	pPOP3Data->SetEmailHeader( strEmailMessage.Mid( nBegChop, nEndChop ));

	if( !( ParseEmailSender( pPOP3Data, strErrMsg )))
		return FALSE;

	if( !( ParseSendDateTime( pPOP3Data, strErrMsg )))
		return FALSE;

	if( !( ParseRecipient( pPOP3Data, strErrMsg )))
		return FALSE;

	if( !( ParseSubject( pPOP3Data, strErrMsg )))
		return FALSE;

	return TRUE;
}

BOOL CPOP::SetIsValidPostRequest( CPOP3Data* pPOP3Data, CString& strErrMsg )
{
	CString strHeader = pPOP3Data->GetEmailHeader();

	if( strHeader.Find( "\r\nIn-reply-to:" ) >= 0 )
	{
		pPOP3Data->SetIsValidPostRequest( FALSE );
	}
	else if(( pPOP3Data->GetSubject() != "Form Submission Post Up" ) && ( pPOP3Data->GetSubject() != "Form Submission Post Down" ))
	{
		pPOP3Data->SetIsValidPostRequest( FALSE );
	}
	else
	{
		pPOP3Data->SetIsValidPostRequest( TRUE );
	}
		
	return TRUE;
}

BOOL CPOP::ParseEmailSender( CPOP3Data* pPOP3Data, CString& strErrMsg )
{
	CString strSenderDataStart = "\r\nFrom:";
	CString strSenderDataEnd = "\r\n";

	CString strEmailHeader = pPOP3Data->GetEmailHeader();

	int nSenderFieldStart = strEmailHeader.Find( strSenderDataStart );

	if( nSenderFieldStart < 0 )
	{
		int nSenderFieldStart = strEmailHeader.Find( "\r\nFROM:" );

		if( nSenderFieldStart < 0 )
		{
			strErrMsg = "Failed to find sender field start.";
			return FALSE;
		}
	}

	nSenderFieldStart += strSenderDataStart.GetLength();

	int nSenderFieldEnd = strEmailHeader.Find( strSenderDataEnd, nSenderFieldStart );

	if( nSenderFieldEnd < 0 )
	{
		strErrMsg = "Failed to find sender field end.";
		return FALSE;
	}

	CString strSenderEmail = strEmailHeader.Mid( nSenderFieldStart, ( nSenderFieldEnd - nSenderFieldStart ));

	strSenderEmail.Trim();
	pPOP3Data->SetSenderEmail( strSenderEmail );

	return TRUE;
}

BOOL CPOP::ParseSendDateTime( CPOP3Data* pPOP3Data, CString& strErrMsg )
{
	CString strSendDateTimeStart = "\r\nDate:";
	CString strSendDateTimeEnd = "\r\n";

	CString strEmailHeader = pPOP3Data->GetEmailHeader();

	int nSendDateTimeFieldStart = strEmailHeader.Find( strSendDateTimeStart );
	
	if( nSendDateTimeFieldStart < 0 )
	{
		nSendDateTimeFieldStart = strEmailHeader.Find( "\r\nDATE:" );

		if( nSendDateTimeFieldStart < 0 )
			return TRUE;
	}

	nSendDateTimeFieldStart += strSendDateTimeStart.GetLength();

	int nSendDateTimeFieldEnd = strEmailHeader.Find( strSendDateTimeEnd, nSendDateTimeFieldStart );

	if( nSendDateTimeFieldEnd < 0 )
	{
		strErrMsg = "Failed to find Date Time Field End.";
		return FALSE;
	}

	CString strSendDateTime = strEmailHeader.Mid( nSendDateTimeFieldStart, ( nSendDateTimeFieldEnd - nSendDateTimeFieldStart ));

	strSendDateTime.Trim();
	pPOP3Data->SetSendDateTime( strSendDateTime );

	return TRUE;
}

BOOL CPOP::ParseRecipient( CPOP3Data* pPOP3Data, CString& strErrMsg )
{
	CString strRecipientStart = "\r\nTo:";
	CString strRecipientEnd = "\r\n";

	CString strEmailHeader = pPOP3Data->GetEmailHeader();

	int nRecipientFieldStart = strEmailHeader.Find( strRecipientStart );

	if( nRecipientFieldStart < 0 )
	{
		nRecipientFieldStart = strEmailHeader.Find( "\r\nTO:" );

		if( nRecipientFieldStart < 0 )
		{
			strErrMsg = "Failed to find Recipient Field Start.";
			return FALSE;
		}
	}

	nRecipientFieldStart += strRecipientStart.GetLength();
	int nRecipientFieldEndDelimiter = strEmailHeader.Find( strRecipientEnd, nRecipientFieldStart );

	if( nRecipientFieldEndDelimiter < 0 )
	{
		strErrMsg = "Failed to find Recipient Field End.";
		return FALSE;
	}

	CString strRecipient = strEmailHeader.Mid( nRecipientFieldStart, ( nRecipientFieldEndDelimiter - nRecipientFieldStart ));

	strRecipient.Trim();
	pPOP3Data->SetRecipient( strRecipient );

	return TRUE;
}

BOOL CPOP::ParseSubject( CPOP3Data* pPOP3Data, CString& strErrMsg )
{
	CString strSubjectStart = "\r\nSubject:";
	CString strSubjectEnd = "\r\n";

	CString strEmailHeader = pPOP3Data->GetEmailHeader();

	int nSubjectStart = strEmailHeader.Find( strSubjectStart );

	if( nSubjectStart < 0 )
	{
		nSubjectStart = strEmailHeader.Find( "\r\nSUBJECT:" );

		strErrMsg = "Failed to find subject start.";
		return FALSE;
	}

	nSubjectStart += strSubjectStart.GetLength();
	int nSubjectEnd = strEmailHeader.Find( strSubjectEnd, nSubjectStart );

	if( nSubjectEnd < 0 )
	{
		strErrMsg = "Failed to find Subject end.";
		return FALSE;
	}

	CString strSubject = strEmailHeader.Mid( nSubjectStart, ( nSubjectEnd - nSubjectStart ));

	strSubject.Trim();
	pPOP3Data->SetSubject( strSubject );

	return TRUE;
}

BOOL CPOP::ParseEmailPlainText( CString strEmailMessage, CPOP3Data* pPOP3Data, CString& strErrMsg )
{
	CString strBodyStart = "\r\n\r\n";
	CString strBodyEnd = "\r\n.\r\n";

	CString strEmailRaw = pPOP3Data->GetMessageRaw();

	int nBodyStart = strEmailRaw.Find( strBodyStart );

	if( nBodyStart < 0 )
	{
		strErrMsg = "Failed to find email body.";
		return FALSE;
	}

	nBodyStart += strBodyStart.GetLength();
	
	int nBodyEnd = strEmailRaw.Find( strBodyEnd, nBodyStart );

	if( nBodyEnd < 0 )
	{
		strErrMsg = "Failed to find email body end.";
		return FALSE;
	}

	CString strEmailBody = strEmailRaw.Mid( nBodyStart, ( nBodyEnd - nBodyStart ));
	pPOP3Data->SetPlainTextBody( strEmailBody );

	return TRUE;
}

BOOL CPOP::ParseSunshineRequest( CPOP3Data*& pPOP3Data, CString& strErrMsg )
{
	if( pPOP3Data->GetSubject() == "Form Submission Post Up" )
	{
		pPOP3Data->SetRequestType( REQUEST_TYPE_POST_UP );

		CPostService PostService( "Up-Post", 56, 18.0 );
		pPOP3Data->AddServiceToList( PostService );

		return ParseSunshinePostUpRequest( pPOP3Data, strErrMsg );
		
	}
	else if( pPOP3Data->GetSubject() == "Form Submission Post Down" )
	{
		pPOP3Data->SetRequestType( REQUEST_TYPE_POST_DOWN );
		CPostService PostService( "Down 1st Post", 0, 0 );
		pPOP3Data->AddServiceToList( PostService );

		return ParseSunshinePostDownRequest( pPOP3Data, strErrMsg );
	}
	else
	{
		ASSERT( FALSE );
		strErrMsg = "Unhandled Post Request Type detected.";
		return FALSE;
	}
}

BOOL CPOP::ParseSunshinePostUpRequest( CPOP3Data*& pPOP3Data, CString& strErrMsg )
{
	CString strRequest;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpDelimiter, strRequest, strErrMsg )))
		return FALSE;

	pPOP3Data->SetRequest( strRequest );

	CString strBroker;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpCompanyDelimiter, strBroker, strErrMsg )))
		return FALSE;

	pPOP3Data->SetBrokerName( strBroker );

	CString strAgentName;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpAgentNameDelimiter, strAgentName, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAgentName( strAgentName );

	CString strAgentEmail;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpAgentEmailDelimiter, strAgentEmail, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAgentEmail( strAgentEmail );

	CString strAgentPhone;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpAgentPhoneDelimiter, strAgentPhone, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAgentPhone( strAgentPhone );

	CString strPanelPhone;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpPanelPhoneDelimiter, strPanelPhone, strErrMsg )))
		return FALSE;

	pPOP3Data->SetPanelPhone( strPanelPhone );

	CString strStorageIndicator;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpStorageIndicatorDelimiter, strStorageIndicator, strErrMsg )))
		return FALSE;

	pPOP3Data->SetStorageIndicator( strStorageIndicator );

	CString strAddress;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpSignAddressDelimiter, strAddress, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAddress( strAddress );

	CString strCity;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpSignCityDelimiter, strCity, strErrMsg )))
		return FALSE;

	pPOP3Data->SetCity( strCity );

	CString strMapArea;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpAreaMapDelimiter, strMapArea, strErrMsg )))
		return FALSE;

	pPOP3Data->SetMapArea( strMapArea );

	CString strGrid;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpGridDelimiter, strGrid, strErrMsg )))
		return FALSE;

	pPOP3Data->SetGrid( strGrid );

	CString strCrossStreet;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpCrossStreetDelimiter, strCrossStreet, strErrMsg )))
		return FALSE;

	pPOP3Data->SetCrossStreet( strCrossStreet );

	CString strLotNumber;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpLotNumberDelimiter, strLotNumber, strErrMsg )))
		return FALSE;

	pPOP3Data->SetLotNumber( strLotNumber );

	CString strExpirationDate;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpExpirationDateDelimiter, strExpirationDate, strErrMsg )))
		return FALSE;

	pPOP3Data->SetExpirationDate( strExpirationDate );

	CString strInstructions;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpInstructionsDelimiter, strInstructions, strErrMsg )))
		return FALSE;

	pPOP3Data->SetInstructions( strInstructions );

	CString strDirections;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpDirectionsDelimiter, strDirections, strErrMsg )))
		return FALSE;

	pPOP3Data->SetDirections( strDirections );

	CString strRider1;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpTextField4Delimiter, strRider1, strErrMsg )))
		return FALSE;

	pPOP3Data->SetRider1( strRider1 );

	CString strRider2;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpTextField2Delimiter, strRider2, strErrMsg )))
		return FALSE;

	pPOP3Data->SetRider2( strRider2 );

	CString strRider3;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpTextField3Delimiter, strRider3, strErrMsg )))
		return FALSE;

	pPOP3Data->SetRider3( strRider3 );

	CString strRider4;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpTextField5Delimiter, strRider4, strErrMsg )))
		return FALSE;

	pPOP3Data->SetRider4( strRider4 );

	CString strRider5;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpTextField6Delimiter, strRider5, strErrMsg )))
		return FALSE;

	pPOP3Data->SetRider5( strRider5 );

	CString strRider6;
	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostUpTextField7Delimiter, strRider6, strErrMsg )))
		return FALSE;

	pPOP3Data->SetRider6( strRider6 );

	return TRUE;
}

BOOL CPOP::ParseSunshinePostDownRequest( CPOP3Data*& pPOP3Data, CString& strErrMsg )
{
	CString strFieldData;

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownAgentEmailDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAgentEmail( strFieldData );

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownAgentNameDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAgentName( strFieldData );

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownAgentPhoneDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAgentPhone( strFieldData );

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownCompanyNameDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetBrokerName( strFieldData );

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownAddressDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetAddress( strFieldData );

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownCityDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetCity( strFieldData );

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownInstructionsDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetInstructions( strFieldData );

	if( !( ParseSunshineField( pPOP3Data->GetPlainTextBody(), pPOP3Data->m_strRequestPostDownStoreIndicatorDelimiter, strFieldData, strErrMsg )))
		return FALSE;

	pPOP3Data->SetStorageIndicator( strFieldData );

	return TRUE;
}

BOOL CPOP::ParseSunshineField( CString strSunshineDataRaw, CString strFieldDelimiter, CString& strSunshineFieldData, CString& strErrMsg )
{
	CString strEndDelimiter = "\r\n";
	int nBeginChop = strSunshineDataRaw.Find( strFieldDelimiter );

	if( nBeginChop == -1 )
	{
		strErrMsg = "Failed to find field: " + strFieldDelimiter;
		return FALSE;
	}

	nBeginChop += strFieldDelimiter.GetLength();

	int nEndChop = strSunshineDataRaw.Find( strEndDelimiter, nBeginChop );

	if( nEndChop == -1 )
	{
		strErrMsg = "Failed to find ending delimiter for field: " + strFieldDelimiter;
		return FALSE;
	}

	strSunshineFieldData = strSunshineDataRaw.Mid( nBeginChop, ( nEndChop - nBeginChop ));
	strSunshineFieldData.TrimLeft();

	return TRUE;
}

