#include "StdAfx.h"
#include "ParameterList.h"


CParameterList::CParameterList(void)
{
	m_nNumOfParams = 0;
}

CParameterList::~CParameterList(void)
{
	POSITION posParameterList = GetHeadPosition();

	while( posParameterList )
	{
		CParameter* pParameter = ( CParameter* )GetNext( posParameterList );
		delete pParameter;
	}
}

void CParameterList::AddParameter( CParameter Parameter )
{
	AddTail( new CParameter( Parameter ));
	m_nNumOfParams++;
}

BOOL CParameterList::GetParameter( CString ParameterPropertyName, CParameter*& pParameter )
{
	POSITION posParameterList = GetHeadPosition();

	if( posParameterList == NULL )
		return FALSE;

	int nRetVal = FALSE;

	while( posParameterList )
	{
		CParameter* pParameterFromLookup = ( CParameter* )GetNext( posParameterList );

		if( pParameterFromLookup->GetProperty() == ParameterPropertyName )
		{
			pParameter = pParameterFromLookup;
			nRetVal = TRUE;
			break;
		}
	}

	return nRetVal;
}

int CParameterList::GetNumOfParams()
{
	return m_nNumOfParams;
}