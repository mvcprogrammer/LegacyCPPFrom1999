#include "StdAfx.h"
#include "Buffer.h"

CBuffer::CBuffer()
{
	m_pBuffer = new char[8000];
	ZeroMemory( m_pBuffer, 8000 );
	m_lBufferSize = 8000;
	m_lBufferUsed = 0;
}

CBuffer::CBuffer( long lBufferSize )
{
	m_pBuffer = new char[ lBufferSize ];
	ZeroMemory( m_pBuffer, lBufferSize );
	m_lBufferSize = lBufferSize;
	m_lBufferUsed = 0;
}

CBuffer::~CBuffer()
{
	delete m_pBuffer;
}

char* CBuffer::GetBuffer()
{
	return m_pBuffer;
}

void CBuffer::RefreshBuffer()
{
	ZeroMemory( m_pBuffer, m_lBufferSize );
	m_lBufferUsed = 0;
}

long CBuffer::GetBufferSize()
{
	return m_lBufferSize;
}

long CBuffer::GetBufferUsed()
{
	return m_lBufferUsed;
}

void CBuffer::IncreaseBufferUsed( long lBufferUsed )
{
	m_lBufferUsed += lBufferUsed;
}

void CBuffer::SetBufferSize( long lBufferSize )
{
	m_lBufferSize = lBufferSize;
	char* pBuffer = new char[ m_lBufferSize ];
	memcpy( pBuffer, m_pBuffer, m_lBufferSize );
	delete m_pBuffer;
	m_pBuffer = pBuffer;
}

void CBuffer::IncreaseBufferSize( long lBytesToIncrease )
{
	if( lBytesToIncrease <= 0 )
	{
		ASSERT( FALSE );
		return;
	}

	char* pBuffer = new char[ m_lBufferSize + lBytesToIncrease ];
	ZeroMemory( pBuffer,  m_lBufferSize + lBytesToIncrease  );
	memcpy( pBuffer, m_pBuffer, m_lBufferSize );
	m_lBufferSize += lBytesToIncrease;

	delete m_pBuffer;
	m_pBuffer = pBuffer;
}

BOOL CBuffer::AddDataToBuffer( char* pszData, long lDataSize )
{
	if(( m_lBufferUsed + lDataSize ) > m_lBufferSize )
		IncreaseBufferSize( m_lBufferSize + lDataSize );

	char* pCurBufPos = ( m_pBuffer + m_lBufferUsed );

	memcpy( pCurBufPos, pszData, lDataSize );

	m_lBufferUsed += lDataSize;

	return TRUE;
}