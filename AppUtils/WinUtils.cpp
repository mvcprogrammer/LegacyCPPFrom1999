#include "StdAfx.h"
#include "WinUtils.h"

#define SPACES 1
#define ZEROS  2

#define DATE_FORMAT_YYYYMMDD 1
#define DATE_FORMAT_ODBC 2
#define TIME_FORMAT_HHMMSS 1

CWinUtils::CWinUtils()
{

}

CWinUtils::CWinUtils(CString strLogPath)
	: CUtils(strLogPath)
{

}

CWinUtils::~CWinUtils()
{

}

long CWinUtils::GetCurrentDate(void)
{
	COleDateTime datCurDate = COleDateTime::GetCurrentTime();
	return (( datCurDate.GetYear() * 10000 ) + ( datCurDate.GetMonth() * 100 ) + datCurDate.GetDay());
}

long CWinUtils::GetNextDay(void)
{
	COleDateTime datCurDate = COleDateTime::GetCurrentTime();
	COleDateTimeSpan OleDateTimeSpan( 1, 0, 0, 0 );
	datCurDate += OleDateTimeSpan;
	return (( datCurDate.GetYear() * 10000 ) + ( datCurDate.GetMonth() * 100 ) + datCurDate.GetDay());
}

long CWinUtils::GetCurrentTime()
{
	COleDateTime datCurDate = COleDateTime::GetCurrentTime();
	return (( datCurDate.GetHour() * 10000 ) + ( datCurDate.GetMinute() * 100 ) + datCurDate.GetSecond());
}

CString CWinUtils::GetCurrentDateFormatted( int nFormat )
{
	return FormatDate( GetCurrentDate(), nFormat );
}

CString CWinUtils::FormatDate( long lDate, int nFormat )
{
	CString strDate = MakeFixedLengthString( NumToStr( lDate ), 8, ZEROS ); // should be 8, but just to be sure because of parsing

	switch( nFormat )
	{
	case DATE_FORMAT_YYYYMMDD: // 01/27/2008
		strDate = ( strDate.Mid( 4, 2 ) + "/" + strDate.Mid( 6, 2 ) + "/" + strDate.Mid( 0, 4 ));
		break;

	case DATE_FORMAT_ODBC: // {d '2012-08-13'}
		strDate = "{d '" + ( strDate.Mid( 0, 4 ) + "-" + strDate.Mid( 4, 2 )) + "-" + strDate.Mid( 6, 2 ) + "'}";
		break;

	default:
		ASSERT( FALSE );
	}

	return strDate;
}

CString CWinUtils::FormatTime( long lTime, int nFormat )
{
	CString strTime = MakeFixedLengthString( NumToStr( lTime ), 6, ZEROS );

	switch( nFormat )
	{
	case TIME_FORMAT_HHMMSS: // 13:26:54
		strTime = ( strTime.Mid( 0, 2 ) + ":" + strTime.Mid( 2, 2 ) + ":" + strTime.Mid( 4, 2 ));
	break;

	default:
		ASSERT( FALSE );
	}

	return strTime;	
}

CString CWinUtils::GetCurrentTimeFormatted( int nFormat )
{
	return FormatTime( GetCurrentTime(), nFormat );
}

BOOL CWinUtils::GetUUID( CString& strUUID )
{
	GUID guid;
	CoCreateGuid(&guid);

	//strUUID.Format("{%.2X-%.2X-%.2X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}", 
 
	strUUID.Format("%.2X-%.2X-%.2X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X", 
		guid.Data1, guid.Data2, guid.Data3, 
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
		
	return TRUE;
}

BOOL CWinUtils::IsCurrency( CString strCurrency, CString& strCurrencyFormatted )
{
	COleCurrency curMoney;

	try
	{
		if( !( curMoney.ParseCurrency( strCurrency, 0 )))
			return FALSE;

		strCurrencyFormatted = curMoney.Format();
		strCurrencyFormatted.Format( TEXT( "%0.2f" ), _tstof( strCurrencyFormatted ));

	}
	catch( CMemoryException* e )
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);
		SendAdminAlert( szCause, "CWinUtils::IsCurrency( " + strCurrency + " )" );
		e->Delete();
		return FALSE;
	}
	catch( COleException* e )
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);
		SendAdminAlert( szCause, "CWinUtils::IsCurrency( " + strCurrency + " )" );
		e->Delete();
		return FALSE;
	}
	
	return TRUE;
}


CString CWinUtils::MakeDbText( CString strText )
{
	strText.Remove( TCHAR( 39 ));
	return( TCHAR( 39 ) + strText + TCHAR( 39 ));
}

CString CWinUtils::Trim( CString& strText )
{
	strText.TrimRight();
	strText.TrimLeft();
	return strText;
}

double CWinUtils::GetAbsolute( double dNumber )
{
	if( dNumber >= 0 )
		return dNumber;

	return ( dNumber * -1 );
}

CString CWinUtils::MakeFixedLengthString( CString strString, int nLength, int nFillerType )
{
	while( strString.GetLength() < nLength )
	{
		switch( nFillerType )
		{
		case SPACES:
			strString += " ";
			break;

		case ZEROS:
			strString = "0" + strString;
			break;

		default:
			ASSERT( FALSE );
		}
	}

	return strString;
}
