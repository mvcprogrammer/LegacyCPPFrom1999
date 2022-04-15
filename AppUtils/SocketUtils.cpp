#include "StdAfx.h"
#include "SocketUtils.h"

#define BUFFER_SIZE 2048
#define CRLF "\r\n"

CSocketUtils::CSocketUtils()
{
}

CSocketUtils::CSocketUtils(CString strLogPath) : CUtils(strLogPath)
{
}

CSocketUtils::~CSocketUtils()
{
}

BOOL CSocketUtils::ValidateSocketPtr( CSocket* pSocket, CString strCallingFunction )
{
	if( pSocket == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	return TRUE;
}

BOOL CSocketUtils::CreateTalkSocket( CSocket* pTalkSocket, LPCTSTR lpszAddress, int nPort )
{
	CString strCallingFunction = "CSocketUtils::ConnectTalkSocket()";

	if( !VerifyPointer( pTalkSocket, strCallingFunction ))
		return FALSE;

	if(! pTalkSocket->Create())
	{
		ASSERT( FALSE );
		SendAdminAlert( "Socket returned following error: " + NumToStr( pTalkSocket->GetLastError()), strCallingFunction );
		return FALSE;
	}

	return ConnectTalkSocket( pTalkSocket, lpszAddress, nPort );
}

BOOL CSocketUtils::ConnectTalkSocket( CSocket* pTalkSocket, LPCTSTR lpszAddress, int nPort )
{
	CString strCallingFunction = "CSocketUtils::ConnectTalkSocket()";

	if( !pTalkSocket->Connect( lpszAddress, nPort ))
	{
		SendAdminAlert( "Socket returned following error: " + NumToStr( pTalkSocket->GetLastError()), strCallingFunction );
		return FALSE;
	}

	BOOL bValue = TRUE;
	if( !pTalkSocket->SetSockOpt( SO_KEEPALIVE, ( char* )&bValue, sizeof( BOOL )))
	{
		SendAdminAlert( "Warning: Failed to set keep alive socket option", strCallingFunction );
	}

	return TRUE;
}

BOOL CSocketUtils::CreateListenSocket( CSocket* pListenSocket, int nPort )
{
	CString strCallingFunction = "CSocketUtils::Connect ListenSocket()";

	if( !VerifyPointer( pListenSocket, strCallingFunction ))
		return FALSE;

	if( !pListenSocket->Create( nPort ))
	{
		ASSERT( FALSE );
		SendAdminAlert( "Failed to create socket", strCallingFunction );
		return FALSE;
	}
	
	if( !pListenSocket->Listen())
	{
		ASSERT( FALSE );
		SendAdminAlert( "Failed to begin listening", strCallingFunction );
		return FALSE;
	}

	return TRUE;
}

BOOL CSocketUtils::SocketSend( CSocket* pSendSocket, char* pBuffer, int nMsgLength, int nFlags  )
{
	if( !( ValidateSocketPtr( pSendSocket, "CSocketUtils::SocketSend()" )))
		return FALSE;

	ASSERT( pBuffer );

	int nBytesToSend = nMsgLength;
	int nBytesSent = 0;
	bool bRetVal = TRUE;

	while( nBytesToSend > nBytesSent )
	{
		int nBytesThisSend  = pSendSocket->Send( pBuffer, nBytesToSend, nFlags );

		if( nBytesThisSend <= 0 )
		{
			SendAdminAlert( "Failed to send socket data: " + NumToStr( pSendSocket->GetLastError()), "CSocketUtils::SendMessage" );
			bRetVal = FALSE;
			break;
		}

		nBytesSent += nBytesThisSend;
		pBuffer += nBytesThisSend;
	}

	return bRetVal;
}

BOOL CSocketUtils::SocketReceive( CSocket* pReceiveSocket, char* pBuffer, int nBufferSize, int& nBytesRecvd, int nFlags )
{
	CString strCallingFunction = "CSocketUtils::SocketReceive()";

	if( !VerifyPointer( pReceiveSocket, strCallingFunction ))
		return FALSE;

	if( !VerifyPointer( pBuffer, strCallingFunction ))
		return FALSE;

	if( nBufferSize <= 0 )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Missing buffer size", strCallingFunction );
		return FALSE;
	}

	nBytesRecvd = pReceiveSocket->Receive( pBuffer, nBufferSize, nFlags );

	if( nBytesRecvd <= 0 )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Failure receiving data", strCallingFunction );
		return FALSE;
	}

	return TRUE;
}

BOOL CSocketUtils::SocketReceive( CSocket* pReceiveSocket, CBuffer* pBuffer, int nFlags )
{
	CString strCallingFunction = "CSocketUtils::SocketReceive()";
	int nBytesReceived = 0;

	if( !VerifyPointer( pReceiveSocket, strCallingFunction ))
		return FALSE;

	if( !VerifyPointer( pBuffer, strCallingFunction ))
		return FALSE;

	if( pBuffer->GetBufferSize() <= 0 )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Invalid buffer size", strCallingFunction );
		return FALSE;
	}

	if( pBuffer->GetBufferUsed() > 0 )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Always use a new/empty buffer for receiving socket data", strCallingFunction );
		return FALSE;
	}

	nBytesReceived = pReceiveSocket->Receive( pBuffer->GetBuffer(), pBuffer->GetBufferSize() , nFlags );

	if( nBytesReceived <= 0 )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Failure receiving data. (Press 'Get Emails' again.)", strCallingFunction );
		return FALSE;
	}

	pBuffer->IncreaseBufferUsed( nBytesReceived );

	return TRUE;
}

BOOL CSocketUtils::SocketBroadcast( CSocket *pBroadcastSocket, char* pBuffer, int nBufferLength, UINT nHostPort, LPCTSTR lpszHostAddress )
{
	CString strCallingFunction = "CSocketUtils::SocketBroadcast()";

	if( !VerifyPointer( pBroadcastSocket, strCallingFunction ))
		return FALSE;

	if( !VerifyPointer( pBuffer, strCallingFunction ))
		return FALSE;

	int nBytesToSend = nBufferLength;
	int nBytesSent = 0;
	BOOL bRetVal = TRUE;

	while( nBytesToSend > nBytesSent )
	{
		int nBytesThisSend = pBroadcastSocket->SendTo( pBuffer, nBufferLength, nHostPort, lpszHostAddress );

		if( nBytesThisSend <= 0 )
		{
			SendAdminAlert( "Failed to broadcast data: " + GetSocketError( pBroadcastSocket ), strCallingFunction );
			bRetVal = FALSE;
			break;
		}

		nBytesSent += nBytesThisSend;
		pBuffer += nBytesThisSend;
	}

	return bRetVal;
}

BOOL CSocketUtils::SocketBroadcastReceive( CSocket* pBroadcastSocket, char* pBuffer, int nBufferSize, CString& strSenderIp, UINT nPort, int& nMsgSize )
{
	CString strCallingFunction = "CSocketUtils::SocketBroadcastReceive()";

	if( !VerifyPointer( pBroadcastSocket, strCallingFunction ))
		return FALSE;

	if( !VerifyPointer( pBuffer, strCallingFunction ))
		return FALSE;

	if( nBufferSize <= 0 )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Missing buffer size", strCallingFunction );
		return FALSE;
	}

	nMsgSize = pBroadcastSocket->ReceiveFrom( pBuffer, nBufferSize, strSenderIp, nPort );

	if( nMsgSize <= 0 )
	{
		ASSERT( FALSE );
		SendAdminAlert( "Failure receiving data: ", GetSocketError( pBroadcastSocket ));
		return FALSE;
	}

	return TRUE;
}

CString CSocketUtils::GetSocketError( CSocket* pSocket )
{
	CString strRetString = "";

	switch( pSocket->GetLastError())
	{
	case WSANOTINITIALISED:
		strRetString = "Socket was not initialized.";
		break;
		
	case WSAENETDOWN:
		strRetString = "The Windows Sockets implementation detected that the network subsystem failed.";
		break;

	case WSAEADDRINUSE:
		strRetString = "The specified address is already in use.";
		break;

	case WSAEINPROGRESS:
		strRetString = "A blocking Windows Sockets call is in progress.";
		break;

	case WSAEADDRNOTAVAIL:
		strRetString = "The specified address is not available from the local machine.";
		break;

	case WSAEAFNOSUPPORT:
		strRetString = "Addresses in the specified family cannot be used with this socket.";
		break;

	case WSAECONNREFUSED:
		strRetString = "The attempt to connect was rejected.";
		break;

	case WSAEDESTADDRREQ:
		strRetString = "A destination address is required.";
		break;

	case WSAEFAULT:
		strRetString = "The nSockAddrLen argument is incorrect.";
		break;

	case WSAEINVAL:
		strRetString = "Invalid host address.";
		break;

	case WSAEISCONN:
		strRetString = "The socket is already connected.";
		break;

	case WSAEMFILE:
		strRetString = "No more file descriptors are available.";
		break;

	case WSAENETUNREACH:
		strRetString = "The network cannot be reached from this host at this time.";
		break;

	case WSAENOBUFS:
		strRetString = "No buffer space is available. The socket cannot be connected.";
		break;

	case WSAENOTSOCK:
		strRetString = "The descriptor is not a socket.";
		break;

	case WSAETIMEDOUT:
		strRetString = "Attempt to connect timed out without establishing a connection.";
		break;

	case WSAEWOULDBLOCK:
		strRetString = "The socket is marked as nonblocking and the connection cannot be completed immediately.";
		break;

	case WSAENOPROTOOPT:
		strRetString = "The supplied socket option is unknown or unsupported.";
		break;

	default:
		strRetString = "Unknown socket error: " + NumToStr(( double )pSocket->GetLastError());
		break;
	}

	return strRetString;
}

BOOL CSocketUtils::GetSocketId( CSocket* pSocket, CString& strSocketId )
{
	if( !ValidateSocketPtr( pSocket, "CSocketUtils::GetSocketId()" ))
		return FALSE;

	CString strPeerAddress;
	UINT nPeerPort;
	
	if( !pSocket->GetPeerName( strPeerAddress, nPeerPort ))
		return FALSE;

	strSocketId = strPeerAddress + ":" + NumToStr( nPeerPort );
	return TRUE;
}