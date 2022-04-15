#pragma once
#include "utils.h"
#include <afxdb.h>

class AFX_EXT_CLASS CDataUtils : public CUtils
{
public:
	CString GetLastError();
	int rsRequery( CRecordset* pRecordset, CString strCallingFunction );
	int rsGetFieldValue( CRecordset* pRecordset, CString strFieldName, CDBVariant& DBVariant, CString strCallingFunction );
	int rsGetFieldValue( CRecordset* pRecordset, CString strFieldName, CString& strValueToFind, CString strCallingFunction );
	int rsFindFirst( CRecordset* pRecordset, CString strFieldName, CString strValue, CString strCallingFunction );
	int rsMovePrev( CRecordset* pRecordset, CString strCallingFunction );
	int rsDelete( CRecordset* pRecordset, CString strCallingFunction );
	int rsMoveLast(		CRecordset* pRecordset, CString strCallingFunction );
	int rsMoveFirst(	CRecordset* pRecordset, CString strCallingFunction );
	int rsMoveNext(		CRecordset* pRecordset, CString strCallingFunction );
	int rsEdit(			CRecordset* pRecordset, CString strCallingFunction );
	int rsAddNew(		CRecordset* pRecordset, CString strCallingFunction );
	int rsUpdate(		CRecordset* pRecordset, CString strCallingFunction );
	int CreateSnapshot(	CRecordset* pRecordset, CString strSql = CString ( "" ), CString strCallingFunction = CString( "" ));
	int CreateDynaset(	CRecordset* pRecordset, CString strSql = CString( "" ), CString strCallingFunction = CString( "" ));
	int ConnectDatabase( CDatabase* pDatabase, CString strDSN, CString strUserId, CString strPassword, CString strCallingFunction );
	BOOL ConnectThreadSafeDatabase( CDatabase *pDatabase, CString strDSN, CString strUserId, CString strPassword, CString strCallingFunction );
	int ExecuteSql( CDatabase* pDatabase, CString strSql, CString strCallingFunction );
	CDataUtils();
	CDataUtils(CString strLogPath);
	virtual ~CDataUtils();

private:
	int OpenRecordset( CRecordset* pRecordset, CString strSql, int nRecordsetType, CString strCallingFunction );
	void HandleException( CException* pException, CString strCallingFunction = CString( "" ), CString strExtraErrInfo = CString( "" ));
	void SetLastError( CString strLastError, CString strCallingFunction );
	CString m_strLastError;
};

