#include "StdAfx.h"
#include "..\..\AppUtils\Buffer.h"
#include "Base64Coder.h"
#include ".\smtp.h"

CSMTP::CSMTP(void)
{
}

CSMTP::~CSMTP(void)
{
}

BOOL CSMTP::SendEmail( CSMTPData* pSMTPData, CString& strErrMsg )
{
	CSocket SMTPSocket;

	if( !( ConnectSMTPSocket( SMTPSocket, pSMTPData, strErrMsg )))
		return FALSE;

	if( !( SendEHLOCommand( SMTPSocket, strErrMsg )))
		return FALSE;

	if( !( SendAUTHCommand( SMTPSocket, strErrMsg )))
		return FALSE;

	if( !( SendLogin( SMTPSocket, pSMTPData, strErrMsg )))
		return FALSE;

	if( !( SendPassword( SMTPSocket, pSMTPData, strErrMsg )))
		return FALSE;

	if( !( SendSenderEmail( SMTPSocket, pSMTPData, strErrMsg )))
		return FALSE;

	if( !( SendReciepient( SMTPSocket, pSMTPData, strErrMsg )))
		return FALSE;

	if( !( SendDataCommand( SMTPSocket, strErrMsg )))
		return FALSE;

	if( !( SendData( SMTPSocket, pSMTPData, strErrMsg )))
		return FALSE;

	if( !( SendQUITCommand( SMTPSocket, strErrMsg )))
		return FALSE;

	return TRUE;
}

BOOL CSMTP::ConnectSMTPSocket( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg )
{
	if( !( m_SocketUtils.CreateTalkSocket( &SMTPSocket, pSMTPData->GetMailServerAddress(), pSMTPData->GetPort())))
	{
		strErrMsg = "Failed to connect to mail server.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer)))
	{
		strErrMsg = "Failure receiving initial data.";
		return FALSE;
	}

	
	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to initial connection request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "220" )
	{
		strErrMsg = "Invalid response to initial connection request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendEHLOCommand( CSocket& SMTPSocket, CString& strErrMsg )
{
	CString strCommand = "EHLO admin862a5d570\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strCommand.GetBuffer(), strCommand.GetLength())))
	{
		strErrMsg = "Failed to send EHLO command.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive EHLO command response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to hello request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "250" )
	{
		strErrMsg = "Invalid response to hello request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendAUTHCommand( CSocket& SMTPSocket, CString& strErrMsg )
{
	CString strSMTPCommand = "AUTH LOGIN\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strSMTPCommand.GetBuffer(), strSMTPCommand.GetLength())))
	{
		strErrMsg = "Failed to send EHLO command.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	while( TRUE )
	{
		CBuffer RecieveBuffer;

		if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &RecieveBuffer )))
		{
			strErrMsg = "Failed to receive AUTH LOGIN command response.";
			return FALSE;
		}

		ResponseBuffer.AddDataToBuffer( RecieveBuffer.GetBuffer(), RecieveBuffer.GetBufferUsed());

		CString strResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

		if( strResponse.Find( "334 " ) >= 0 )
			break;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to AUTH LOGIN request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "334" )
	{
		CString strAckResponse = "334";

		int nAckPosition = strCommandResponse.Find( "334 " );

		if( nAckPosition == -1 )
		{
			strErrMsg = "Invalid response to Authorization Request: " + strCommandResponse;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSMTP::SendLogin( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg )
{
	CBase64Coder Coder;
	Coder.Encode( pSMTPData->GetUserId());
	CString strLoginMsg( Coder.EncodedMessage());
	strLoginMsg += "\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strLoginMsg.GetBuffer(), strLoginMsg.GetLength())))
	{
		strErrMsg = "Failed to send Login Info.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive Login Message response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to login request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "334" )
	{
		strErrMsg = "Invalid response to Login Request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendPassword( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg )
{
	CBase64Coder Coder;
	Coder.Encode( pSMTPData->GetPassword());
	CString strLoginMsg( Coder.EncodedMessage());
	strLoginMsg += "\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strLoginMsg.GetBuffer(), strLoginMsg.GetLength())))
	{
		strErrMsg = "Failed to send Password Info.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive Password Message response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to password request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "235" )
	{
		strErrMsg = "Invalid response to Password Request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendSenderEmail( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg )
{
	CString strSMTPCommand = "MAIL FROM: " + pSMTPData->GetSenderId() + "\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strSMTPCommand.GetBuffer(), strSMTPCommand.GetLength())))
	{
		strErrMsg = "Failed to send Sender Email command.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive AUTH LOGIN command response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to Sender Email request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "250" )
	{
		strErrMsg = "Invalid response to Sender Email Request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendReciepient( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg )
{
	CString strSMTPCommand = "RCPT TO: <" + pSMTPData->GetRecipient() + ">\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strSMTPCommand.GetBuffer(), strSMTPCommand.GetLength())))
	{
		strErrMsg = "Failed to send Sender Email command.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive AUTH LOGIN command response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to Sender Email request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "250" )
	{
		strErrMsg = "Invalid response to Sender Email Request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendDataCommand( CSocket& SMTPSocket, CString& strErrMsg )
{
	CString strSMTPCommand = "DATA\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strSMTPCommand.GetBuffer(), strSMTPCommand.GetLength())))
	{
		strErrMsg = "Failed to send Data command.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive Data command response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to Data command request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "354" )
	{
		strErrMsg = "Invalid response to Data command Request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendData( CSocket& SMTPSocket, CSMTPData* pSMTPData, CString& strErrMsg )
{
	CString strCommand = "REPLY TO: " + pSMTPData->GetReplyTo() + "\r\n";
	strCommand += "FROM: \"" + pSMTPData->GetSenderName() + "\" <" + pSMTPData->GetSenderId() + ">\r\n";
	strCommand += "TO: \"" + pSMTPData->GetRecieverName() + "\" <" + pSMTPData->GetRecipient() + ">\r\n";
	strCommand += "SUBJECT: " + pSMTPData->GetSubject() + "\r\n";
	
	
	CString strSMTPDateTime;
	
	if( !( GetSMTPDateTime( strSMTPDateTime )))
	{
		strErrMsg = "Failed to get SMTP Date/Time";
		return FALSE;
	}
	
	strCommand += "Date: " + strSMTPDateTime + "\r\n\r\n";
	strCommand += pSMTPData->GetMessage() + "\r\n.\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strCommand.GetBuffer(), strCommand.GetLength())))
	{
		strErrMsg = "Failed to send Data command.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive Data command response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to Data request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "250" )
	{
		strErrMsg = "Invalid response to Data Request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::SendQUITCommand( CSocket& SMTPSocket, CString& strErrMsg )
{
	CString strSMTPCommand = "QUIT\r\n";

	if( !( m_SocketUtils.SocketSend( &SMTPSocket, strSMTPCommand.GetBuffer(), strSMTPCommand.GetLength())))
	{
		strErrMsg = "Failed to send QUIT command.";
		return FALSE;
	}

	CBuffer ResponseBuffer;

	if( !( m_SocketUtils.SocketReceive( &SMTPSocket, &ResponseBuffer )))
	{
		strErrMsg = "Failed to receive QUIT command response.";
		return FALSE;
	}

	CString strCommandResponse( ResponseBuffer.GetBuffer(), ResponseBuffer.GetBufferUsed());

	if( strCommandResponse.GetLength() < 3 )
	{
		strErrMsg = "Invalid response to QUIT request: " + strCommandResponse;
		return FALSE;
	}

	if( strCommandResponse.Mid( 0, 3 ) != "221" )
	{
		strErrMsg = "Invalid response to QUIT Request: " + strCommandResponse;
		return FALSE;
	}

	return TRUE;
}

BOOL CSMTP::GetSMTPDateTime( CString& strSMTPDateTime )
{
	COleDateTime datCurDate = COleDateTime::GetCurrentTime();
	
	CString strDay;

	switch( datCurDate.GetDayOfWeek())
	{
	case 1:
		strDay = "Sun";
		break;

	case 2:
		strDay = "Mon";
		break;

	case 3:
		strDay = "Tue";
		break;

	case 4:
		strDay = "Wed";
		break;

	case 5:
		strDay = "Thr";
		break;

	case 6:
		strDay = "Fri";
		break;

	case 7:
		strDay = "Sat";
		break;

	default:
		return FALSE;
	}

	strSMTPDateTime += strDay + ", ";

	strSMTPDateTime += m_WinUtils.NumToStr( datCurDate.GetDay()) + " ";

	CString strMonth;

	switch( datCurDate.GetMonth())
	{
	case 1:
		strMonth = "Jan";
		break;

	case 2:
		strMonth = "Feb";
		break;

	case 3:
		strMonth = "Mar";
		break;

	case 4:
		strMonth = "Apr";
		break;

	case 5:
		strMonth = "May";
		break;

	case 6:
		strMonth = "Jun";
		break;

	case 7:
		strMonth = "Jul";
		break;

	case 8:
		strMonth = "Aug";
		break;

	case 9:
		strMonth = "Sep";
		break;

	case 10:
		strMonth = "Oct";
		break;

	case 11:
		strMonth = "Nov";
		break;

	case 12:
		strMonth = "Dec";
		break;

	default:
		return FALSE;
	}

	strSMTPDateTime += strMonth + " ";

	strSMTPDateTime += m_WinUtils.NumToStr( datCurDate.GetYear()) + " ";

	strSMTPDateTime += m_WinUtils.NumToStr( datCurDate.GetHour()) + ":" + m_WinUtils.NumToStr( datCurDate.GetMinute()) + ":" + m_WinUtils.NumToStr( datCurDate.GetSecond()) + " ";

	strSMTPDateTime += "-0400";

	return TRUE;
}