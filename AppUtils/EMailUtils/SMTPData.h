#pragma once
#include <afxtempl.h>
class AFX_EXT_CLASS CSMTPData
{
public:
	CString GetBoundaryString();
	BOOL AddAttachments(CString strFilePath,CString& strError);
	int GetNumberRecipents();
	CString GetSenderName();
	void SetSenderName(CString strSenderName);
	CString GetReplyTo();
	void SetReplyTo( CString strReplyTo );
	CString GetSubject();
	void SetSubject( CString strSubject );
	BOOL GetPasswordRequired();
	void SetPasswordRequired( BOOL bPasswordRequired );
	CString GetPassword();
	void SetPassword( CString strPassword );
	CString GetUserId();
	void SetUserId( CString strUserId );
	CString GetMailServerAddress();
	void SetMailServerAddress( CString strMailServerAddress );
	BOOL Validate( CString& strErrMsg, int& nErrId );
	CString GetMessage();
	void SetMessage( CString strMessage ,BOOL bText=TRUE, BOOL bHtml=FALSE);
	CString GetRecipient();
	void SetRecipient( CString strRecipient );
	CString GetSenderId();
	void SetSenderId( CString strSenderId );
	BOOL GetEMailAlert();
	void SetEmailAlert( BOOL bEmailAlert );
	CString GetHeader( );
	void SetHeader( CString strHeader, BOOL bEnableMime=TRUE);
	void SetReceiverId( CString strReceiverId );
	CString GetReceiverId();
	void SetRecieverName( CString strRecieverName );
	CString GetRecieverName();
	int GetPort();
	void SetPort( int nPort );
	CSMTPData();
	virtual ~CSMTPData();

	enum ENUM_SMTP_VALIDATION
	{
		SMTP_VALIDATION_SUCCESS = 100,
		SMTP_VALIDATION_ERROR_SERVER_ADDRESS_REQUIRED,
		SMTP_VALIDATION_ERROR_SENDER_ID_REQUIRED,
		SMTP_VALIDATION_ERROR_RECEIVER_ID_REQUIRED,
		SMTP_VALIDATION_ERROR_USER_ID_REQUIRED,
		SMTP_VALIDATION_ERROR_PASSWORD_REQUIRED
	};

private:
	BOOL	m_bEmailAlert;
	BOOL	m_bPasswordRequired;
	CString m_strRecipient;
	CString m_strPassword;
	CString m_strUserId;
	CString m_strMailServerAddress;
	CString m_strMessage;
	CString m_strSenderId;
	CString m_strSenderName;
	CString m_strSubject;
	CString m_strReplyTo;
	CString m_strHeader;
	CString m_strBoundary;
	CString m_strReceiverId;
	CString m_strRecieverName;
	int m_nPort;
};
