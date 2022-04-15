#include "StdAfx.h"
#include ".\buffer.h"

CBuffer::CBuffer()
{
	m_pBuffer = new char[1000];
	m_nBufferSize = 1000;
}

CBuffer::~CBuffer()
{
	delete m_pBuffer;
}


char* CBuffer::GetBuffer()
{
	return m_pBuffer;
}

int CBuffer::GetBufferSize()
{
	return m_nBufferSize;
}

void CBuffer::IncreaseBufferSize(int nBytesToIncrease)
{
	if( nBytesToIncrease <= 0 )
	{
		ASSERT( FALSE );
		return;
	}

	char* pBuffer = new char[ m_nBufferSize + nBytesToIncrease ];
	memcpy( pBuffer, m_pBuffer, m_nBufferSize );
	m_nBufferSize += nBytesToIncrease;

	delete m_pBuffer;
	m_pBuffer = pBuffer;
}

int CBuffer::GetBufferCurrentSize()
{
	return strlen(m_pBuffer);
}

void CBuffer::SetBuffer(const LPCTSTR pstrMessage)
{
	int iSize = strlen(pstrMessage);
	if(iSize > m_nBufferSize)
		IncreaseBufferSize(strlen(pstrMessage) - m_nBufferSize);
	memcpy( GetBuffer(), pstrMessage, strlen(pstrMessage));
}