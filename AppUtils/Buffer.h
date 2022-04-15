#pragma once
#include "stdafx.h"

class AFX_EXT_CLASS CBuffer
{
public:
	void RefreshBuffer();
	void IncreaseBufferUsed( long lBufferUsed );
	BOOL AddDataToBuffer( char* pszData, long lDataSize );
	void IncreaseBufferSize( long nBytesToIncrease );
	long GetBufferUsed();
	long GetBufferSize();
	void SetBufferSize( long nBufferSize );
	char* GetBuffer();
	CBuffer();
	CBuffer( long lBufferSize );
	virtual ~CBuffer();

private:
	long m_lBufferUsed;
	long m_lBufferSize;
	char* m_pBuffer;
};

