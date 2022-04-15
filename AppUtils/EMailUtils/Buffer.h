#pragma once

class CBuffer  
{
public:
	void IncreaseBufferSize( int nBytesToIncrease );
	int GetBufferSize();
	int GetBufferCurrentSize();
	char* GetBuffer();
	void SetBuffer(const LPCTSTR pstrMessage);
	CBuffer();
	virtual ~CBuffer();

private:
	int m_nBufferSize;
	char* m_pBuffer;
};