#pragma once
#include "utils.h"

class AFX_EXT_CLASS CWinUtils : public CUtils
{
public:
	long GetNextDay( void );
	BOOL GetUUID( CString& strUUID );
	CString Trim( CString& strText );
	CString MakeDbText( CString strText );
	double GetAbsolute( double dNumber );
	CString MakeFixedLengthString( CString strString, int nLength, int nFillerType );
	BOOL IsCurrency(CString strCurrency, CString& strCurrencyFormatted );
	long GetCurrentDate(void);
	long GetCurrentTime();
	CString FormatDate( long lDate, int nFormat = 1 );
	CString FormatTime( long lTime, int nFormat = 1 );
	CString GetCurrentDateFormatted( int nFormat = 1);
	CString GetCurrentTimeFormatted( int nFormat = 1);
	CWinUtils(CString strLogPath);
	CWinUtils();
	virtual ~CWinUtils();
};

