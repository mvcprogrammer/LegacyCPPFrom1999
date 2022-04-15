#include "StdAfx.h"
#include "Parameter.h"

CParameter::CParameter(void)
{
}

CParameter::~CParameter(void)
{
}

CString CParameter::GetProperty(void)
{
	return m_strProperty;
}

void CParameter::SetProperty( CString strProperty )
{
	m_strProperty = strProperty;
}

CString CParameter::GetValue(void)
{
	return m_strValue;
}

void CParameter::SetValue( CString strValue )
{
	m_strValue = strValue;
}