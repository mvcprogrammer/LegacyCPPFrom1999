#include "stdafx.h"
#include ".\smtpdata.h"
#include ".\base64coder.h"

CSMTPData::CSMTPData()
{
	m_bPasswordRequired = FALSE;
	m_bEmailAlert = FALSE;
	m_strBoundary = "bound_string";
	m_nPort = 80;
}

CSMTPData::~CSMTPData()
{
}

void CSMTPData::SetSenderId( CString strSenderId )
{
	m_strSenderId = strSenderId;
}

CString CSMTPData::GetSenderId()
{
	return m_strSenderId;
}

void CSMTPData::SetMessage( CString strMessage , BOOL bText, BOOL bHtml)
{
	m_strMessage = strMessage;
}

CString CSMTPData::GetMessage()
{
	return m_strMessage;
}

BOOL CSMTPData::Validate( CString& strErrMsg, int& nErrId )
{
	if( m_strMailServerAddress.IsEmpty())
	{
		nErrId = SMTP_VALIDATION_ERROR_SERVER_ADDRESS_REQUIRED;
		strErrMsg = "SMTP server address is required. ";
		return FALSE;
	}

	if( m_strSenderId.IsEmpty())
	{
		nErrId = SMTP_VALIDATION_ERROR_SENDER_ID_REQUIRED;
		strErrMsg = "Sender email address is required. ";
		return FALSE;
	}

	if( m_bPasswordRequired )
	{
		if( m_strUserId.IsEmpty())
		{
			nErrId = SMTP_VALIDATION_ERROR_USER_ID_REQUIRED;
			strErrMsg = "If password is required, you must supply a user id.";
			return FALSE;
		}

		if( m_strPassword.IsEmpty())
		{
			nErrId = SMTP_VALIDATION_ERROR_PASSWORD_REQUIRED;
			strErrMsg = "If password is required, you must supply a password.";
			return FALSE;
		}
	}

	return TRUE;
}

void CSMTPData::SetMailServerAddress( CString strMailServerAddress )
{
	m_strMailServerAddress = strMailServerAddress;
}

CString CSMTPData::GetMailServerAddress()
{
	return m_strMailServerAddress;
}

void CSMTPData::SetUserId( CString strUserId )
{
	m_strUserId = strUserId;
}

CString CSMTPData::GetUserId()
{
	return m_strUserId;
}

void CSMTPData::SetPassword( CString strPassword )
{
	m_strPassword = strPassword;
}

CString CSMTPData::GetPassword()
{
	return m_strPassword;
}

void CSMTPData::SetPasswordRequired( BOOL bPasswordRequired )
{
	m_bPasswordRequired = bPasswordRequired;
}

BOOL CSMTPData::GetPasswordRequired()
{
	return m_bPasswordRequired;
}

void CSMTPData::SetEmailAlert( BOOL bEmailAlert )
{
	m_bEmailAlert = bEmailAlert;
}

BOOL CSMTPData::GetEMailAlert()
{
	return m_bEmailAlert;
}

CString CSMTPData::GetRecipient()
{
	return m_strRecipient;
}

void CSMTPData::SetRecipient( CString strRecipient )
{
	m_strRecipient = strRecipient;
}

void CSMTPData::SetSubject( CString strSubject )
{
	m_strSubject = strSubject;
}

CString CSMTPData::GetSubject()
{
	return m_strSubject;
}

void CSMTPData::SetReplyTo( CString strReplyTo )
{
	m_strReplyTo = strReplyTo;
}

CString CSMTPData::GetReplyTo()
{
	return m_strReplyTo;
}	

CString CSMTPData::GetHeader( )
{
	return m_strHeader;
}	

void CSMTPData::SetHeader( CString strHeader, BOOL bEnableMime)
{
	if(!strHeader.IsEmpty())
		m_strHeader = strHeader +"\r\n";
	else
		m_strHeader = "";
	if(bEnableMime)
	{
		m_strHeader += 	"MIME-VERSION: 1.0\r\n";
		m_strHeader += "Content-Type: multipart/mixed; boundary="+m_strBoundary;
	}
}

void CSMTPData::SetSenderName(CString strSenderName)
{
	m_strSenderName = strSenderName;
}

CString CSMTPData::GetSenderName()
{
	return m_strSenderName;
}

void CSMTPData::SetReceiverId( CString strReceiverId )
{
	m_strReceiverId = strReceiverId;
}

CString CSMTPData::GetReceiverId()
{
	return m_strReceiverId;
}

void CSMTPData::SetRecieverName( CString strRecieverName )
{
	m_strRecieverName = strRecieverName;
}

CString CSMTPData::GetRecieverName()
{
	return m_strRecieverName;
}

int CSMTPData::GetPort()
{
	return m_nPort;
}

void CSMTPData::SetPort( int nPort )
{
	m_nPort = nPort;
}

BOOL CSMTPData::AddAttachments(CString strFilePath, CString& strError)
{
	strFilePath.TrimLeft();
	strFilePath.TrimRight();
	if(strFilePath.Find(",") > -1)
	{
		CString strTemp = strFilePath;
		while(strTemp.Find(",") > -1 || strTemp.GetLength() > 0)
		{
			CString strMessage="", strFileName="", strFullPath="";
			if(strTemp.Find(',') > -1)
				strFullPath = strTemp.Left(strTemp.Find(','));
			else
				strFullPath = strTemp;
			strFullPath.TrimLeft();
			strFullPath.TrimRight();
			strFileName = strFullPath.Mid(strFullPath.ReverseFind('\\')+1);
			strMessage += "--"+m_strBoundary+"\r\n";
			strMessage += "Content-Type: application/octet-stream\r\n";
			strMessage += "Content-Transfer-Encoding: base64\r\n";
			strMessage += "Content-Disposition: attachment; filename=\""+strFileName+"\"\r\n\r\n";
			CStdioFile fileTest;
			if(!fileTest.Open(strFullPath,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
			{
				strError = "File Could Not Be Openned";
				return FALSE;
			}
			int iBytesRead; 
			CString strEncoded="";
			CBase64Coder baseEncoder;
			//CString strTemp
			BYTE szBuffer[55];
			do
			{
				iBytesRead = fileTest.Read(szBuffer,54);
				szBuffer[iBytesRead] = 0;
				baseEncoder.Encode(szBuffer,iBytesRead);
				strEncoded += baseEncoder.EncodedMessage();
				strEncoded += "\r\n";
			}while(iBytesRead == 54);
			fileTest.Close();
			strMessage += strEncoded +"\r\n";
			SetMessage(strMessage,FALSE);
			if(strTemp.Find(',') > -1)
				strTemp = strTemp.Mid(strTemp.Find(',')+1);
			else
				break;
		}
	}
	else
	{

		CString strMessage="", strFileName="";
		strFileName = strFilePath.Mid(strFilePath.ReverseFind('\\')+1);
		strMessage += "--"+m_strBoundary+"\r\n";
		strMessage += "Content-Type: application/octet-stream; file="+strFileName+"\r\n";
		strMessage += "Content-Transfer-Encoding: base64\r\n";
		strMessage += "Content-Disposition: attachment; filename=\""+strFileName+"\"\r\n\r\n";
		CStdioFile fileTest;
		if(!fileTest.Open(strFilePath,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
		{
			strError = "File Could Not Be Openned";
			return FALSE;
		}
		int iBytesRead; 
		CString strEncoded="";
		CBase64Coder baseEncoder;
		//CString strTemp
		BYTE szBuffer[55];
		do
		{
			iBytesRead = fileTest.Read(szBuffer,54);
			szBuffer[iBytesRead] = 0;
			baseEncoder.Encode(szBuffer,iBytesRead);
			strEncoded += baseEncoder.EncodedMessage();
			strEncoded += "\r\n";
		}while(iBytesRead == 54);
		fileTest.Close();
//		strEncoded = strEncoded.Left(strEncoded.GetLength() -1);
		strMessage += strEncoded;
		SetMessage(strMessage,FALSE);
	}
	return TRUE;
}

CString CSMTPData::GetBoundaryString()
{
	return m_strBoundary;
}
