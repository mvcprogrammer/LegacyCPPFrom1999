#pragma once
#include "afxcoll.h"
#include "Parameter.h"

class AFX_EXT_CLASS CParameterList : public CPtrList
{
public:
	CParameterList();
	~CParameterList();

	void AddParameter( CParameter Parameter );
	BOOL GetParameter( CString ParameterPropertyName, CParameter*& pParameter );

	int GetNumOfParams();

private:
	int m_nNumOfParams;
};

