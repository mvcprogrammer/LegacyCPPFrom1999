#pragma once
class AFX_EXT_CLASS CUtils
{
public:
	BOOL DirectoryExists( CString strFilePath, CString& strErrMsg );
	void SetLogPath(CString strLogPath);
	CString GetLastErrorMsg();
	void StripCommas( CString& strNumber );
	void LogError( CString strError = CString( "" ), CString strErrLogPath = CString( "UseDefault" ));
	void HandleException( CException* pException, CString strCallingFunction, CString strExtraErrInfo = CString( "" ));
	void SendAdminAlert( CString strAdminMsg, CString strCallingFunction, CString strExtraInfo = CString( "" ));
	bool VerifyPointer( void* pPointer, CString strCallingFunction );
	double StringToDouble( CString strNumber );
	float StringToFloat( CString strNumber );
	long StringToLong( CString strNumber );
	int StringToInt( CString strNumber );
	BOOL StringToInt( CString strNumber, int& nNumber );
	CString NumToStr( UINT nNumber );
	CString NumToStr( long lNumber, int nLength= 0 );
	CString NumToStr( int nNumber, int nLength = 0 );
	CString NumToStr( float fNumber, int nDecimalPlaces = 4 );
	CString NumToStr( double dNumber, int nDecimalPlaces = 4 );
	CString NumToBOOLStr( int nNumber );
	bool StringToBool(CString strBool);
	CString BoolToStr(BOOL bBool);
	CUtils(CString strLogPath);
	CUtils();
	virtual ~CUtils();

private:
	CString m_strLastErrorMsg;
	CString m_strLogPath;
};

