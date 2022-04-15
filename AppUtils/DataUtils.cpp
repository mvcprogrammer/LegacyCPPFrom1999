#include "StdAfx.h"
#include "DataUtils.h"


CDataUtils::CDataUtils()
{
	m_strLastError = "";
}

CDataUtils::CDataUtils(CString strLogPath)
	: CUtils(strLogPath)
{
}

CDataUtils::~CDataUtils()
{

}

int CDataUtils::ConnectDatabase( CDatabase* pDatabase, CString strDSN, CString strUserId, CString strPassword, CString strCallingFunction )
{
	try
	{
		if( !( pDatabase->OpenEx( "DSN=" + strDSN + ";UID=" + strUserId + ";PWD=" + strPassword )))
		{
			SetLastError( "Failed to open database", "CDataUtils::ConnectDatabase()<-" + strCallingFunction );
			return FALSE;
		}
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, "CDataUtils::ConnectDatabase()()<-" + strCallingFunction, strDSN + "," + strUserId );
		return FALSE;
	}

	TRACE0( "Database Opened\n" );
	return TRUE;
}

BOOL CDataUtils::ConnectThreadSafeDatabase( CDatabase *pDatabase, CString strDSN, CString strUserId, CString strPassword, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::ConnectThreadSafeDatabase()";

	CString strConnectionString = "DSN=" + strDSN + ";UID=" + strUserId + ";PWD=" + strPassword + ";";

	try
	{
		if( !pDatabase->OpenEx( _T( strConnectionString ), CDatabase::openReadOnly | CDatabase::noOdbcDialog ))
		{
			SetLastError( "Failed to open thread-safe database", strCallingFunction );
			return FALSE;
		}
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction, strDSN + "," + strUserId );
		return FALSE;
	}

	TRACE0( "Thread-safe Database Opened\n" );
	return TRUE;
}

int CDataUtils::ExecuteSql( CDatabase* pDatabase, CString strSql, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::ExecuteSql()";
	if(strSql.GetLength() < 512)
	{
		TRACE0( "ExecuteSql: " + strSql + "\n" );
	}

	if( !( pDatabase ))
	{
		ASSERT( FALSE );
		SetLastError( "Null DB pointer", strCallingFunction );
		return FALSE;
	}

	if( !( pDatabase->IsOpen()))
	{
		ASSERT( FALSE );
		SetLastError( "Trying to execute sql on unopened database", strCallingFunction );
		return FALSE;
	}

	if( strSql.IsEmpty())
	{
		ASSERT( FALSE );
		SetLastError( "Tying to execute empty sql", strCallingFunction );
		return FALSE;
	}

	try
	{
		pDatabase->ExecuteSQL( strSql );
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction, strSql );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::CreateSnapshot( CRecordset* pRecordset, CString strSql, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::CreateSnapshot()";
	TRACE0( "CreateSnapshot" );
	return OpenRecordset( pRecordset, strSql, CRecordset::snapshot, strCallingFunction );
}


int CDataUtils::CreateDynaset( CRecordset* pRecordset, CString strSql, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::CreateDynaset()";
	TRACE0( "CreateDynaset" );
	return OpenRecordset( pRecordset, strSql, CRecordset::dynaset, strCallingFunction );
}

int CDataUtils::OpenRecordset( CRecordset* pRecordset, CString strSql, int nRecordsetType, CString strCallingFunction )
{
	//ASSERT( nRecordsetType == CRecordset::forwardOnly || nRecordsetType == CRecordset::dynaset );

	if( strCallingFunction.IsEmpty())
	{
		ASSERT( FALSE );
	}

	if( !( pRecordset ))
	{
		ASSERT( FALSE );
		SetLastError( "Null recordset pointer in create dynaset: ", strCallingFunction );
		return FALSE;
	}

	if( strSql.IsEmpty())
	{
		strSql = pRecordset->GetDefaultSQL();
		if( strSql.IsEmpty())
		{
			ASSERT( FALSE );
			SetLastError( "Empty recordset SQL string, with no default.", strCallingFunction );
			return FALSE;
		}
	}
	if(strSql.GetLength() < 509)
	{
		TRACE0( ": " + strSql + "\n" );
	}

	if( pRecordset->IsOpen())
		pRecordset->Close();

	try
	{
		if( nRecordsetType == CRecordset::forwardOnly )
			pRecordset->Open( nRecordsetType, strSql, CRecordset::readOnly );
		else
			pRecordset->Open( nRecordsetType, strSql );
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction, strSql );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction , strSql );
		return FALSE;
	}

	if( nRecordsetType != CRecordset::forwardOnly )
	{
		if( !( pRecordset->IsEOF()))
			if( !( rsMoveFirst( pRecordset, strCallingFunction )))
				return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsUpdate( CRecordset *pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsUpdate()";

	try
	{
		pRecordset->Update();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction  );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsAddNew( CRecordset *pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsAddNew()";

	try
	{
		pRecordset->AddNew();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction  );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsEdit( CRecordset* pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsEdit()";
	try
	{
		pRecordset->Edit();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction  );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsMoveNext( CRecordset* pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsMoveNext()";

	try
	{
		pRecordset->MoveNext();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction  );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsMovePrev( CRecordset* pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsMovePrev()";

	try
	{
		pRecordset->MovePrev();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction  );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsMoveFirst( CRecordset *pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsMoveFirst()";

	try
	{
		pRecordset->MoveFirst();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return false;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction  );
		return false;
	}

	return TRUE;
}

int CDataUtils::rsMoveLast( CRecordset *pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsMoveLast()";

	try
	{
		pRecordset->MoveLast();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction );
		return false;
	}

	return TRUE;
}

int CDataUtils::rsDelete( CRecordset* pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsDelete()";

	try
	{
		pRecordset->Delete();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction  );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsFindFirst( CRecordset* pRecordset, CString strFieldName, CString strValueToFind, CString strCallingFunction )
{
	ASSERT( FALSE ); // very inefficient routine, should be deleted if not used. 1/10/02
	strCallingFunction += "<-CDataUtils::rsFindFirst()";

	if( !( rsMoveFirst( pRecordset, strCallingFunction )))
		return FALSE;

	CString strRetrievedValue = "";

	while( !( pRecordset->IsEOF()))
	{
		if( !( rsGetFieldValue( pRecordset, strFieldName, strRetrievedValue, strCallingFunction )))
			return FALSE;

		if( strRetrievedValue == strValueToFind )
			return TRUE;

		if( !( rsMoveNext( pRecordset, strCallingFunction )))
			return FALSE;
	}

	if( !( pRecordset->IsEOF() && pRecordset->IsBOF()))
	{
		if( !( rsMoveFirst( pRecordset, strCallingFunction )))
			return FALSE;
	}

	return FALSE;
}

int CDataUtils::rsGetFieldValue( CRecordset* pRecordset, CString strFieldName, CString& strValue, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsGetFieldValue()";

	try
	{
		pRecordset->GetFieldValue( strFieldName, strValue );
		strValue.Trim();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction, "FieldName = " + strFieldName );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction );
		return FALSE;
	}

	return TRUE;
}

int CDataUtils::rsGetFieldValue( CRecordset* pRecordset, CString strFieldName, CDBVariant& DBVariant, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsGetFieldValue2()";

	try
	{
		pRecordset->GetFieldValue( strFieldName, DBVariant );
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction, "FieldName = " + strFieldName );
		return FALSE;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction );
		return FALSE;
	}

	return TRUE;
}
int CDataUtils::rsRequery( CRecordset* pRecordset, CString strCallingFunction )
{
	strCallingFunction += "<-CDataUtils::rsRequery()";
	int nRetVal = FALSE;

	try
	{
		nRetVal = pRecordset->Requery();
	}
	catch( CDBException* pDbException )
	{
		HandleException( pDbException, strCallingFunction );
		return nRetVal;
	}
	catch( CMemoryException* pMemoryException )
	{
		HandleException( pMemoryException, strCallingFunction );
		return FALSE;
	}

	return nRetVal;
}

void CDataUtils::HandleException( CException* pException, CString strCallingFunction, CString strExtraErrInfo )
{
	char szErrMsg[ 255 ];
	pException->GetErrorMessage( szErrMsg, 255 );
	pException->Delete();

	CString strErrMsg( szErrMsg );

	if( !( strExtraErrInfo.IsEmpty()))
		strErrMsg = strErrMsg + ", " + strExtraErrInfo;

	SetLastError( strErrMsg, strCallingFunction );
}

void CDataUtils::SetLastError( CString strLastError, CString strCallingFunction )
{
	SendAdminAlert( strLastError, strCallingFunction );
	m_strLastError = strLastError;
}

CString CDataUtils::GetLastError()
{
	ASSERT( !( m_strLastError.IsEmpty()));

	CString strRetVal = m_strLastError;
	m_strLastError = "";
	return strRetVal;
}