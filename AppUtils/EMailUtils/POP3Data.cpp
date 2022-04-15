#include "stdafx.h"
#include "POP3Data.h"


CPOP3Data::CPOP3Data(void)
{
	m_WinUtils.GetUUID( m_strRequestId );
	m_bIsValidPostRequest = FALSE;
	m_nMessageNum = 0;
	m_nRequestType = REQUEST_TYPE_POST_UNDEFINED;
	m_bIsValidPostRequest = FALSE;
	m_nCompanyId = 0;
	m_nBrokerId = 0;
	m_nAgentId = 0;
	m_nLocationId = 0;
	m_nStorageIndicator = FALSE;
	m_nStorageIndicatorNew = FALSE;
	m_nOrderStatus = ORDER_STATUS_UNDEFINED;
	

	m_strFieldEndDelimiter = "\r\n";

	// POST UP REQUESTS
    m_strRequestPostUpDelimiter = "Aup_requestor:";
    m_strRequestPostUpCompanyDelimiter = "Bup_company_name:";
    m_strRequestPostUpAgentNameDelimiter = "Cup_agent_name:";
    m_strRequestPostUpAgentEmailDelimiter = "Dup_agent_email:";
    m_strRequestPostUpAgentPhoneDelimiter = "Eup_agent_phone:";
    m_strRequestPostUpPanelPhoneDelimiter = "Fup_panel_phone:";
    m_strRequestPostUpStorageIndicatorDelimiter = "Gup_storage_indicator:";
    m_strRequestPostUpSignAddressDelimiter = "Iup_address:";
    m_strRequestPostUpSignCityDelimiter = "Jup_city:";
    m_strRequestPostUpAreaMapDelimiter = "Kup_map_area:";
    m_strRequestPostUpGridDelimiter = "Lup_grid:";
    m_strRequestPostUpCrossStreetDelimiter = "Mup_cross_street:";
    m_strRequestPostUpLotNumberDelimiter = "Nup_lot_number:";
    m_strRequestPostUpExpirationDateDelimiter = "Oup_exp_date:";
    m_strRequestPostUpInstructionsDelimiter = "Pup_instructions:";
    m_strRequestPostUpDirectionsDelimiter = "Qup_directions:";
    m_strRequestPostUpTextField4Delimiter = "Rtextfield4:";
    m_strRequestPostUpTextFieldDelimiter = "Stextfield:";
    m_strRequestPostUpTextField2Delimiter = "Ttextfield2:";
    m_strRequestPostUpTextField3Delimiter = "Utextfield3:";
    m_strRequestPostUpTextField5Delimiter = "Vtextfield4:";
    m_strRequestPostUpTextField6Delimiter = "Wtextfield4:";
    m_strRequestPostUpTextField7Delimiter = "Xtextfield4:";
    m_strRequestPostUpNextDelimiter = "YNext:";

	// POST DOWN REQUESTS
	m_strRequestPostDownAgentEmailDelimiter = "agent_email:";
	m_strRequestPostDownAgentNameDelimiter = "agent_name:";
	m_strRequestPostDownAgentPhoneDelimiter = "agent_phone:";
	m_strRequestPostDownCompanyNameDelimiter = "company_name:";
	m_strRequestPostDownAddressDelimiter = "down_address:";
	m_strRequestPostDownCityDelimiter = "down_city:";
	m_strRequestPostDownInstructionsDelimiter = "down_instructions:";
	m_strRequestPostDownStoreIndicatorDelimiter = "storage_indicator:";
}


CPOP3Data::~CPOP3Data(void)
{
	DeleteAllServices();
}

CPtrList& CPOP3Data::GetServiceList()
{
	return m_lstServices;
}

BOOL CPOP3Data::AddServiceToList( CPostService PostService )
{
	CPostService* pPostService = new CPostService( PostService );
	m_lstServices.AddTail(( void* )pPostService );
	return TRUE;
}

BOOL CPOP3Data::GetServiceItem( int nServiceId, int nRequestType, CPostService*& pRefPostService )
{
	POSITION posDataList = m_lstServices.GetHeadPosition();

	while( posDataList != NULL )
	{
		CPostService* pPostService = ( CPostService* )m_lstServices.GetNext( posDataList );
		if( pPostService->GetServiceId() == nServiceId )
		{
			pRefPostService = pPostService;
			return TRUE;
		}
	}

	ASSERT( FALSE );
	return FALSE;
}

BOOL CPOP3Data::RemoveServiceItem( CString strServiceId )
{
	POSITION posDataList = m_lstServices.GetHeadPosition();
	POSITION posPrevDataList = NULL;

	while( posDataList != NULL )
	{
		posPrevDataList = posDataList;
		CPostService* pPostService = ( CPostService* )m_lstServices.GetNext( posDataList );
		if( pPostService->GetServiceId() == strServiceId )
		{
			m_lstServices.RemoveAt( posPrevDataList );
			delete pPostService;
			pPostService = NULL;
			return TRUE;
		}
	}

	ASSERT( FALSE );
	return FALSE;
}

void CPOP3Data::DeleteAllServices()
{
	POSITION posDataList = m_lstServices.GetHeadPosition();

	while( posDataList != NULL )
	{
		CPostService* pPostService = ( CPostService* )m_lstServices.GetNext( posDataList );
		delete pPostService;
		pPostService = NULL;
	}
}


void CPOP3Data::SetOrderStatus( int nOrderStatus )
{
	m_nOrderStatus = nOrderStatus;
}

int CPOP3Data::GetOrderStatus()
{
	return m_nOrderStatus;
}

void CPOP3Data::SetRequestId( CString strRequestId )
{
	m_strRequestId = strRequestId;
}

CString CPOP3Data::GetRequestId()
{
	return m_strRequestId;
}

void CPOP3Data::SetRequestType( int nRequestType )
{
	m_nRequestType = nRequestType;
}

int CPOP3Data::GetRequestType()
{
	return m_nRequestType;
}

void CPOP3Data::SetMessageNum( int nMessageNum )
{
	m_nMessageNum = nMessageNum;
}

int CPOP3Data::GetMessageNum()
{
	return m_nMessageNum;
}

void CPOP3Data::SetMessageSize( long lMessageSize )
{
	m_lMessageSize = lMessageSize;
}

long CPOP3Data::GetMessageSize()
{
	return m_lMessageSize;
}

void CPOP3Data::SetMessageRaw( CString strMessageRaw )
{
	m_strMessageRaw = strMessageRaw;
}

CString CPOP3Data::GetMessageRaw()
{
	return m_strMessageRaw;
}

void CPOP3Data::SetEmailHeader( CString strEmailHeader )
{
	m_strEmailHeader = strEmailHeader;
}

CString CPOP3Data::GetEmailHeader()
{
	return m_strEmailHeader;
}

void CPOP3Data::SetEmailInfo( CString strEmailInfo )
{
	m_strEmailInfo = strEmailInfo;
}

CString CPOP3Data::GetEmailInfo()
{
	return m_strEmailInfo;
}

void CPOP3Data::SetSenderEmail( CString strSenderEmail )
{
	m_strSenderEmail = strSenderEmail;
}

CString CPOP3Data::GetSenderEmail()
{
	return m_strSenderEmail;
}

void CPOP3Data::SetRecipient( CString strRecipient )
{
	m_strRecipient = strRecipient;
}

CString CPOP3Data::GetRecipient()
{
	return m_strRecipient;
}

void CPOP3Data::SetSendDateTime( CString strSendDateTime )
{
	m_strSendDateTime = strSendDateTime;
}

CString CPOP3Data::GetSendDateTime()
{
	return m_strSendDateTime;
}

void CPOP3Data::SetSubject( CString strSubject )
{
	m_strSubject = strSubject;
}

CString CPOP3Data::GetSubject()
{
	return m_strSubject;
}

void CPOP3Data::SetPlainTextBody( CString strTextPlainBody )
{
	m_strPlainTextBody = strTextPlainBody;
}

CString CPOP3Data::GetPlainTextBody()
{
	return m_strPlainTextBody;
}

void CPOP3Data::SetIsValidPostRequest( BOOL bIsValidPostRequest )
{
	m_bIsValidPostRequest = bIsValidPostRequest;
}

BOOL CPOP3Data::GetIsValidPostRequest()
{
	return m_bIsValidPostRequest;
}

// POST REQUESTS
void CPOP3Data::SetCompanyId( int nCompanyId )
{
	m_nCompanyId = nCompanyId;
}

int CPOP3Data::GetCompanyId()
{
	return m_nCompanyId;
}

void CPOP3Data::SetBrokerId( int nBrokerId )
{
	m_nBrokerId = nBrokerId;
}

int CPOP3Data::GetBrokerId()
{
	return m_nBrokerId;
}

void CPOP3Data::SetAgentId( int nAgentId )
{
	m_nAgentId = nAgentId;
}

int CPOP3Data::GetAgentId()
{
	return m_nAgentId;
}

void CPOP3Data::SetLocationId( int nLocationId )
{
	m_nLocationId = nLocationId;
}

int CPOP3Data::GetLocationId()
{
	return m_nLocationId;
}

CString CPOP3Data::GetAgentName()
{
	return m_strAgentName;
}

void CPOP3Data::SetBrokerName( CString strBrokerName )
{
	m_strBrokerName = strBrokerName;
}

CString CPOP3Data::GetBrokerName()
{
	return m_strBrokerName;
}

void CPOP3Data::SetAgentName( CString strAgentName )
{
	m_strAgentName = strAgentName;
}

void CPOP3Data::SetAgentEmail( CString strAgentEmail )
{
	m_strAgentEmail = strAgentEmail;
}

CString CPOP3Data::GetAgentEmail()
{
	return m_strAgentEmail;
}

void CPOP3Data::SetAgentPhone( CString strAgentPhone )
{
	m_strAgentPhone = strAgentPhone;
}

CString CPOP3Data::GetAgentPhone()
{
	return m_strAgentPhone;
}

void CPOP3Data::SetAddress( CString strAddress )
{
	m_strAddress = strAddress;
}

CString CPOP3Data::GetAddress()
{
	return m_strAddress;
}

void CPOP3Data::SetCity( CString strCity )
{
	m_strCity = strCity;
}

CString CPOP3Data::GetCity()
{
	return m_strCity;
}

void CPOP3Data::SetInstructions( CString strInstructions )
{
	m_strInstructions = strInstructions;
}

CString CPOP3Data::GetInstructions()
{
	return m_strInstructions;
}

void CPOP3Data::SetInstructionsNew( CString strInstructionsNew )
{
	m_strInstructionsNew = strInstructionsNew;
}

CString CPOP3Data::GetInstructionsNew()
{
	return m_strInstructionsNew;
}

void CPOP3Data::SetStorageIndicator( CString strStorageIndicator )
{
	m_nStorageIndicator = strStorageIndicator == "Yes";
}

int CPOP3Data::GetStorageIndicator()
{
	return m_nStorageIndicator;
}

void CPOP3Data::SetBrokerNameNew( CString strBrokerNameNew )
{
	m_strBrokerNameNew = strBrokerNameNew;
}

CString CPOP3Data::GetBrokerNameNew()
{
	return m_strBrokerNameNew;
}

void CPOP3Data::SetAgentNameNew( CString strAgentNameNew )
{
	m_strAgentNameNew = strAgentNameNew;
}

CString CPOP3Data::GetAgentNameNew()
{
	return m_strAgentNameNew;
}

void CPOP3Data::SetAgentEmailNew( CString strAgentEmailNew )
{
	m_strAgentEmailNew = strAgentEmailNew;
}

CString CPOP3Data::GetAgentEmailNew()
{
	return m_strAgentEmailNew;
}

void CPOP3Data::SetAgentPhoneNew( CString strAgentPhoneNew )
{
	m_strAgentPhoneNew = strAgentPhoneNew;
}

CString CPOP3Data::GetAgentPhoneNew()
{
	return m_strAgentPhoneNew;
}

void CPOP3Data::SetAddressNew( CString strAddressNew )
{
	m_strAddressNew = strAddressNew;
}

CString CPOP3Data::GetAddressNew()
{
	return m_strAddressNew;
}

void CPOP3Data::SetCityNew( CString strCityNew )
{
	m_strCityNew = strCityNew;
}

CString CPOP3Data::GetCityNew()
{
	return m_strCityNew;
}

void CPOP3Data::SetStorageIndicatorNew( CString strStorageIndicator )
{
	if( strStorageIndicator == "Yes" )
		m_nStorageIndicator = TRUE;
	else if( strStorageIndicator == "No" )
		m_nStorageIndicator = FALSE;
	else
	{
		ASSERT( FALSE );
		m_nStorageIndicator = FALSE;
	}
}

int CPOP3Data::GetStorageIndicatorNew()
{
	return m_nStorageIndicatorNew;
}

void CPOP3Data::SetPanelPhone( CString strPanelPhone )
{
	m_strPanelPhone = strPanelPhone;
}

CString CPOP3Data::GetPanelPhone()
{
	return m_strPanelPhone;
}

void CPOP3Data::SetRequest( CString strRequest )
{
	m_strRequest = strRequest;
}

CString CPOP3Data::GetRequest()
{
	return m_strRequest;
}

void CPOP3Data::SetMapArea( CString strMapArea )
{
	m_strMapArea = strMapArea;
}

CString CPOP3Data::GetMapArea()
{
	return m_strMapArea;
}

void CPOP3Data::SetGrid( CString strGrid )
{
	m_strGrid = strGrid;
}

CString CPOP3Data::GetGrid()
{
	return m_strGrid;
}

void CPOP3Data::SetCrossStreet( CString strCrossStreet )
{
	m_strCrossStreet = strCrossStreet;
}

CString CPOP3Data::GetCrossStreet()
{
	return m_strCrossStreet;
}

void CPOP3Data::SetLotNumber( CString strLotNumber )
{
	m_strLotNumber = strLotNumber;
}

CString CPOP3Data::GetLotNumber()
{
	return m_strLotNumber;
}

void CPOP3Data::SetExpirationDate( CString strExpirationDate )
{
	m_strExpirationDate = strExpirationDate;
}

CString CPOP3Data::GetExpirationDate()
{
	return m_strExpirationDate;
}

void CPOP3Data::SetDirections( CString strDirections )
{
	m_strDirections = strDirections;
}

CString CPOP3Data::GetDirections()
{
	return m_strDirections;
}

void CPOP3Data::SetRider1( CString strRider1 )
{
	m_strRider1 = strRider1;
}

CString CPOP3Data::GetRider1()
{
	return m_strRider1;
}

void CPOP3Data::SetRider2( CString strRider2 )
{
	m_strRider2 = strRider2;
}

CString CPOP3Data::GetRider2()
{
	return m_strRider2;
}

void CPOP3Data::SetRider3( CString strRider3 )
{
	m_strRider3 = strRider3;
}

CString CPOP3Data::GetRider3()
{
	return m_strRider3;
}

void CPOP3Data::SetRider4( CString strRider4 )
{
	m_strRider4 = strRider4;
}

CString CPOP3Data::GetRider4()
{
	return m_strRider4;
}

void CPOP3Data::SetRider5( CString strRider5 )
{
	m_strRider5 = strRider5;
}

CString CPOP3Data::GetRider5()
{
	return m_strRider5;
}

void CPOP3Data::SetRider6( CString strRider6 )
{
	m_strRider6 = strRider6;
}

CString CPOP3Data::GetRider6()
{
	return m_strRider6;
}

void CPOP3Data::SetPostColor( CString strPostColor )
{
	m_strPostColor = strPostColor;
}

CString CPOP3Data::GetPostColor()
{
	return m_strPostColor;
}

void CPOP3Data::SetLatitude( CString strLatitude )
{
	m_strLatitude = strLatitude;
}

CString CPOP3Data::GetLatitude()
{
	return m_strLatitude;
}

void CPOP3Data::SetLongitude( CString strLongitude )
{
	m_strLongitude = strLongitude;
}
CString CPOP3Data::GetLongitude()
{
	return m_strLongitude;
}
