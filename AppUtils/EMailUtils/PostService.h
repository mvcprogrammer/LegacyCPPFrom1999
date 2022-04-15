#pragma once
#include "..\..\AppUtils\WinUtils.h"

// CPostService

class  AFX_EXT_CLASS CPostService
{
public:
	CPostService( CString strServiceName, int nServiceSerialNum, double dServicePrice );
	virtual ~CPostService();

private:
	CWinUtils m_WinUtils;

private:
	CString m_strServiceId;
	CString m_strServiceName;
	int m_nServiceSerialNum;
	double m_dServicePrice;

public:

	CString GetServiceId();

	void SetServiceName( CString strServiceName );
	CString GetServiceName();

	void SetServiceSerialNum( CString strServiceId );
	int GetServiceSerialNum();

	void SetServicePrice( double dServicePrice );
	CString GetServicePrice();
	double GetServicePriceNumeric();
};


