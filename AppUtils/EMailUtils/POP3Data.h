#pragma once
#include "afxcoll.h"
#include "..\..\AppUtils\WinUtils.h"
#include "PostService.h"

enum REQUEST_TYPE
{
	REQUEST_TYPE_POST_UNDEFINED = 0,
	REQUEST_TYPE_POST_UP,
	REQUEST_TYPE_POST_DOWN
};

enum ORDER_STATUS
{
	ORDER_STATUS_UNDEFINED = 0,
	ORDER_STATUS_PENDING_RETRIEVAL,
	ORDER_STATUS_RETRIEVED,
	ORDER_STATUS_FILED,
	ORDER_STATUS_IGNORED,
	ORDER_STATUS_PENDING_CLOSE,
	ORDER_STATUS_CLOSED
};

class AFX_EXT_CLASS CPOP3Data
{
public:
	CPOP3Data(void);
	virtual ~CPOP3Data(void);

private:
	CWinUtils m_WinUtils;
	CPtrList m_lstServices;
	void DeleteAllServices();
	BOOL GetServiceItem( int nServiceId, int nRequestType, CPostService*& pRefPostService );
	

public:
	BOOL AddServiceToList( CPostService PostService );
	BOOL RemoveServiceItem( CString strServiceId );
	CPtrList& GetServiceList();

public:
	void SetOrderStatus( int nOrderStatus );
	int GetOrderStatus();

	void SetRequestId( CString strRequestId );
	CString GetRequestId();

	void SetRequestType( int nRequestType );
	int GetRequestType();

	void SetMessageNum( int nMessageNum );
	int GetMessageNum();

	void SetMessageSize( long lMessageSize );
	long GetMessageSize();

	void SetMessageRaw( CString strMessageRaw );
	CString GetMessageRaw();

	void SetEmailHeader( CString strEmailHeader );
	CString GetEmailHeader();

	void SetEmailInfo( CString strEmailInfo );
	CString GetEmailInfo();

	void SetSenderEmail( CString strSenderEmail );
	CString GetSenderEmail();

	void SetRecipient( CString strRecipient );
	CString GetRecipient();

	void SetSendDateTime( CString strSendDateTime );
	CString GetSendDateTime();

	void SetSubject( CString strSubject );
	CString GetSubject();

	void SetPlainTextBody( CString strTextPlainBody );
	CString GetPlainTextBody();

	void SetIsValidPostRequest( BOOL bIsValidPostRequest );
	BOOL GetIsValidPostRequest();

	void SetCompanyId( int nCompanyId );
	int GetCompanyId();

	void SetBrokerId( int nBrokerId );
	int GetBrokerId();

	void SetAgentId( int nAgentId );
	int GetAgentId();

	void SetLocationId( int nLocationId );
	int GetLocationId();

	void SetBrokerName( CString strBrokerName );
	CString GetBrokerName();

	void SetBrokerNameNew( CString strBrokerNameNew );
	CString GetBrokerNameNew();

	void SetAgentName( CString strAgentNameNew );
	CString GetAgentName();

	void SetAgentNameNew( CString strAgentNameNew );
	CString GetAgentNameNew();

	void SetAgentEmail( CString strAgentEmailNew );
	CString GetAgentEmail();

	void SetAgentEmailNew( CString strAgentEmailNew );
	CString GetAgentEmailNew();

	void SetAgentPhone( CString strAgentPhoneNew );
	CString GetAgentPhone();

	void SetAgentPhoneNew( CString strAgentPhoneNew );
	CString GetAgentPhoneNew();

	void SetAddress( CString strAddressNew );
	CString GetAddress();

	void SetAddressNew( CString strAddressNew );
	CString GetAddressNew();

	void SetCity( CString strCityNew );
	CString GetCity();

	void SetCityNew( CString strCityNew );
	CString GetCityNew();

	void SetInstructions( CString strInstructions );
	CString GetInstructions();

	void SetInstructionsNew( CString strInstructionsNew );
	CString GetInstructionsNew();

	void SetStorageIndicator( CString strStorageIndicatorNew );
	int GetStorageIndicator();

	void SetStorageIndicatorNew( CString strStorageIndicatorNew );
	int GetStorageIndicatorNew();

	void SetPanelPhone( CString strPanelPhone );
	CString GetPanelPhone();

	void SetMapArea( CString strMapArea );
	CString GetMapArea();

	void SetGrid( CString strGrid );
	CString GetGrid();

	void SetCrossStreet( CString strCrossStreet );
	CString GetCrossStreet();

	void SetLotNumber( CString strLotNumber );
	CString GetLotNumber();

	void SetExpirationDate( CString strExpirationDate );
	CString GetExpirationDate();

	void SetDirections( CString strDirections );
	CString GetDirections();

	void SetRider1( CString strRider1 );
	CString GetRider1();

	void SetRider2( CString strRider2 );
	CString GetRider2();

	void SetRider3( CString strRider3 );
	CString GetRider3();

	void SetRider4( CString strRider4 );
	CString GetRider4();

	void SetRider5( CString strRider5 );
	CString GetRider5();

	void SetRider6( CString strRider6 );
	CString GetRider6();

	void SetPostColor( CString strPostColor );
	CString GetPostColor();

	void SetLatitude( CString strLatitude );
	CString GetLatitude();

	void SetLongitude( CString strLongitude );
	CString GetLongitude();

	void SetRequest( CString strRequest );
	CString GetRequest();

private:
	int m_nOrderStatus;
	CString m_strRequestId;
	int m_nRequestType;
	int m_nMessageNum;
	long m_lMessageSize;
	CString m_strMessageRaw;
	CString m_strEmailHeader;
	CString m_strEmailInfo;
	CString m_strPlainTextBody;
	CString m_strSenderEmail;
	CString m_strRecipient;
	CString m_strSubject;
	CString m_strSendDateTime;
	BOOL m_bIsValidPostRequest;

	// REQUEST FORM
private:
	int m_nCompanyId;
	int m_nBrokerId;
	int m_nAgentId;
	int m_nLocationId;

	CString m_strRequest;
	
	// POST DOWNS
private:
	CString m_strBrokerName;
	CString m_strAgentName;
public:
	CString m_strAgentEmail;
	CString m_strAgentPhone;
	CString m_strAddress;
	CString m_strCity;
	CString m_strInstructions;
	int m_nStorageIndicator;
private:
	CString m_strBrokerNameNew;
	CString m_strAgentNameNew;
	CString m_strAgentEmailNew;
	CString m_strAgentPhoneNew;
	CString m_strAddressNew;
	CString m_strCityNew;
	CString m_strInstructionsNew;
	int m_nStorageIndicatorNew;

	// POST UPS
public:
	CString m_strDirections;
	CString m_strMapArea;
    CString m_strGrid;
	CString m_strPanelPhone;
	CString m_strCrossStreet;
	CString m_strPostColor;
private:
    CString m_strLotNumber;
    CString m_strExpirationDate;
    CString m_strRider1;
    CString m_strRider2;
    CString m_strRider3;
    CString m_strRider4;
	CString m_strRider5;
    CString m_strRider6;
    CString m_strNext;
	CString m_strLatitude;
	CString m_strLongitude;

	

public:
	CString m_strFieldEndDelimiter;

    CString m_strRequestPostUpDelimiter;
    CString m_strRequestPostUpCompanyDelimiter;
    CString m_strRequestPostUpAgentNameDelimiter;
    CString m_strRequestPostUpAgentEmailDelimiter;
    CString m_strRequestPostUpAgentPhoneDelimiter;
    CString m_strRequestPostUpPanelPhoneDelimiter;
    CString m_strRequestPostUpStorageIndicatorDelimiter;
    CString m_strRequestPostUpSignAddressDelimiter;
    CString m_strRequestPostUpSignCityDelimiter;
    CString m_strRequestPostUpAreaMapDelimiter;
    CString m_strRequestPostUpGridDelimiter;
    CString m_strRequestPostUpCrossStreetDelimiter;
    CString m_strRequestPostUpLotNumberDelimiter;
    CString m_strRequestPostUpExpirationDateDelimiter;
    CString m_strRequestPostUpInstructionsDelimiter;
    CString m_strRequestPostUpDirectionsDelimiter;
    CString m_strRequestPostUpTextField4Delimiter;
    CString m_strRequestPostUpTextFieldDelimiter;
    CString m_strRequestPostUpTextField2Delimiter;
    CString m_strRequestPostUpTextField3Delimiter;
    CString m_strRequestPostUpTextField5Delimiter;
    CString m_strRequestPostUpTextField6Delimiter;
    CString m_strRequestPostUpTextField7Delimiter;
    CString m_strRequestPostUpNextDelimiter;

	CString m_strRequestPostDownAgentEmailDelimiter;
	CString m_strRequestPostDownAgentNameDelimiter;
	CString m_strRequestPostDownAgentPhoneDelimiter;
	CString m_strRequestPostDownCompanyNameDelimiter;
	CString m_strRequestPostDownAddressDelimiter;
	CString m_strRequestPostDownCityDelimiter;
	CString m_strRequestPostDownInstructionsDelimiter;
	CString m_strRequestPostDownStoreIndicatorDelimiter;
};

