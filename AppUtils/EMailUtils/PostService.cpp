// PostService.cpp : implementation file
//

#include "stdafx.h"
#include "EMailUtils.h"
#include "PostService.h"


// CPostService

CPostService::CPostService( CString strServiceName, int nServiceSerialNum, double dServicePrice )
{
	CString strServiceId;
	m_WinUtils.GetUUID( strServiceId );
	m_strServiceId = strServiceId;
	m_strServiceName = strServiceName;
	m_nServiceSerialNum = nServiceSerialNum;	
	m_dServicePrice = dServicePrice;
}

CPostService::~CPostService()
{
}

void CPostService::SetServiceName( CString strServiceName )
{
	m_strServiceName = strServiceName;
}

CString CPostService::GetServiceName()
{
	return m_strServiceName;
}

void CPostService::SetServiceSerialNum( CString strServiceSerialNum )
{
	if( strServiceSerialNum.GetLength() == 0 )
		m_nServiceSerialNum = 0;
	else
		m_nServiceSerialNum = m_WinUtils.StringToInt( strServiceSerialNum );
}

int CPostService::GetServiceSerialNum()
{
	return m_nServiceSerialNum;
}

CString CPostService::GetServiceId()
{
	return m_strServiceId;
}

void CPostService::SetServicePrice( double dServicePrice )
{
	m_dServicePrice = dServicePrice;
}

CString CPostService::GetServicePrice()
{
	CString strCurrencyValue;
	
	if( m_WinUtils.IsCurrency( m_WinUtils.NumToStr( m_dServicePrice ), strCurrencyValue ))
		return strCurrencyValue;
	else
		return "$0.00";
}

double CPostService::GetServicePriceNumeric()
{
	return m_dServicePrice;
}

