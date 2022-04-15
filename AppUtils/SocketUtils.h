#pragma once
#include "utils.h"
#include "Buffer.h"

class AFX_EXT_CLASS CSocketUtils : public CUtils
{
public:
	BOOL GetSocketId( CSocket* pSocket, CString& strSocketId );
	CString GetSocketError( CSocket* pSocket );
	BOOL ConnectTalkSocket( CSocket* pTalkSocket, LPCTSTR lpszAddress, int nPort );
	BOOL SocketBroadcastReceive( CSocket* pBroadcastSocket, char* pBuffer, int nBufferSize, CString& strSenderIp, UINT nPort, int& nMsgSize );
	BOOL SocketBroadcast( CSocket *pBroadcastSocket, char* pBuffer, int nBufferLength, UINT nHostPort, LPCTSTR lpszHostAddress );
	BOOL SocketReceive( CSocket* pReceiveSocket, CBuffer* pBuffer, int nFlags = 0 );
	BOOL SocketReceive( CSocket* pReceiveSocket, char* pBuffer, int nBufferSize, int& nBytesRecvd, int nFlags = 0 );
	BOOL SocketSend( CSocket* pSendSocket, char* pBuffer, int nMsgLength, int nFlags = 0 );
	BOOL CreateListenSocket( CSocket* pListenSocket, int nPort );
	BOOL CreateTalkSocket(CSocket* pTalkSocket, LPCTSTR lpszAddress, int nPort );
	CSocketUtils();
	CSocketUtils(CString strLogPath);
	virtual ~CSocketUtils();

private:
	BOOL ValidateSocketPtr( CSocket* pSocket, CString strCallingFunction );
};

