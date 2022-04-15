#include "StdAfx.h"
#include "Utils.h"
#include <direct.h>
#include <atlutil.h>
#include <string.h>

CUtils::CUtils()
{
	m_strLogPath = "UseDefault";
}

CUtils::CUtils(CString strLogPath)
{
	m_strLogPath = strLogPath;
}

CUtils::~CUtils()
{

}

CString CUtils::NumToStr( UINT nNumber )
{
	char pBuffer[ 34 ];
	_itoa( nNumber, pBuffer, 10 );

	return CString( pBuffer );
}

CString CUtils::NumToBOOLStr( int nNumber )
{
	if( nNumber == 0 )
		return "FALSE";

	return "TRUE";
}

CString CUtils::NumToStr( int nNumber,int nLength )
{
	return NumToStr(( long ) nNumber , nLength);
}

CString CUtils::NumToStr( long lNumber,int nLength )
{
	char pBuffer[ 34 ];
	_ltoa( lNumber, pBuffer, 10 );
	CString RetStr=pBuffer;
	if(lNumber>=0)
		for(int i=RetStr.GetLength();i<nLength;i++)
			RetStr="0"+RetStr;
	else
		for(int i=RetStr.GetLength();i<nLength;i++)
			RetStr="-0"+RetStr.Mid(1);
	return RetStr;
}

CString CUtils::NumToStr( float fNumber, int nDecimalPlaces )
{
	return NumToStr(( double )fNumber, nDecimalPlaces );
}

CString CUtils::NumToStr( double dNumber, int nDecimalPlaces )
{
   int nDecimal = 0;
   int nSign = 0;

	char* pBuffer;
	pBuffer = _fcvt( dNumber, nDecimalPlaces, &nDecimal, &nSign );

	CString strNumber( pBuffer );
	
	if( nDecimal >= 0 )
		strNumber.Insert( nDecimal, "." );
	else
	{
		while( nDecimal < 0 )
		{
			strNumber.Insert( 0, "0" );
			nDecimal += 1;
		}

		strNumber.Insert( 0, "." );
	}

	if( strNumber == "" || strNumber == "0." )
		return "0.0";

	strNumber.TrimRight( "." );

	if( nSign )
		strNumber.Insert( 0, "-" );

	return strNumber;
}

long CUtils::StringToLong( CString strNumber )
{
	StripCommas( strNumber );

	if( strNumber == "" )
		return 0;

	return atol( strNumber );
}

double CUtils::StringToDouble( CString strNumber )
{
	StripCommas( strNumber );

	if( strNumber == "" )
		return 0;

	return atof( strNumber );
}

int CUtils::StringToInt( CString strNumber )
{
	return atoi( strNumber );
}

BOOL CUtils::StringToInt( CString strNumber, int& nNumber )
{
	strNumber.Trim();
	strNumber.Trim( "," );

	if( strNumber.GetLength() == 0 )
		return FALSE;

	nNumber = StringToInt( strNumber );

	return TRUE;
}

float CUtils::StringToFloat( CString strNumber )
{
	StripCommas( strNumber );

	if( strNumber == "" )
		return 0;

	return ( float )atof( strNumber );
}

bool CUtils::StringToBool(CString strBool){
	if(strBool=="0")
		return false;
	else if (strBool=="1")
		return true;
	else
	{
		SendAdminAlert( "Unhandled String", "CUtils::StrToBool()<-");
		return false;
	}
}

CString CUtils::BoolToStr(BOOL bBool){
	if(bBool)
		return "1";
	else
		return "0";
}


bool CUtils::VerifyPointer( void *pPointer, CString strCallingFunction )
{
	if( !pPointer )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Null pointer detected", "CUtils::VerifyPointer()<-" + strCallingFunction );
		return false;
	}

	return true;
}

void CUtils::HandleException( CException* pException, CString strCallingFunction, CString strExtraErrInfo )
{
	char szErrMsg[ 512 ];
	pException->GetErrorMessage( szErrMsg, 512 );
	pException->Delete();

	CString strErrMsg( szErrMsg );

	if( !( strExtraErrInfo.IsEmpty()))
		strErrMsg = strErrMsg + ", " + strExtraErrInfo;
	
	SendAdminAlert( "Exception: " + strErrMsg, strCallingFunction );
	TRACE0( "Exception handled: " + strErrMsg );
}

void CUtils::SendAdminAlert( CString strAdminMsg, CString strCallingFunction, CString strExtraInfo )
{
	ASSERT( FALSE );

	CString strErrorMsg = strCallingFunction + ": " + strAdminMsg;

	if( strExtraInfo.GetLength() > 0 )
		strErrorMsg += " ExtraInfo:" + strExtraInfo;

	LogError( strErrorMsg, m_strLogPath );
	TRACE0( "LOGGED ERROR: " + strErrorMsg + "\n" );
}

void CUtils::LogError( CString strError, CString strErrLogPath )
{
	if ( strErrLogPath == "UseDefault")
	{
		strErrLogPath = "C:\\logs\\" + ( COleDateTime::GetCurrentTime()).Format( "%Y%m%d" ) + "_Log.txt";
		if(_chdir("C:\\logs") == -1)
		{
			if(!CreateDirectory("C:\\logs",NULL))
				strErrLogPath = "C:\\" + ( COleDateTime::GetCurrentTime()).Format( "%Y%m%d" ) + "_Log.txt";
		}
	}

	CStdioFile File;
	try
	{
		if( !( File.Open( strErrLogPath, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::typeText )))
		{
			return;
		}

		CString strDateTime = COleDateTime::GetCurrentTime().Format( "%m/%d/%Y %H:%M:%S" );
		strDateTime += ": ";

		File.SeekToEnd();
		File.WriteString( strDateTime + strError + "\n" );
	}
	catch( CFileException* pFileException )
	{
		pFileException->Delete();
		ASSERT( FALSE );
		return;
	}
}

BOOL CUtils::DirectoryExists( CString strFilePath, CString& strErrMsg )
{
	return ( _chdir( strFilePath ) != -1 );
}

void CUtils::StripCommas( CString& strNumber )
{
	strNumber.Replace( ",", "" );
}

CString CUtils::GetLastErrorMsg()
{
	CString strLastErrorMsg = m_strLastErrorMsg;
	m_strLastErrorMsg = "";
	return strLastErrorMsg;
}

void CUtils::SetLogPath(CString strLogPath)
{
	m_strLogPath = strLogPath;
}