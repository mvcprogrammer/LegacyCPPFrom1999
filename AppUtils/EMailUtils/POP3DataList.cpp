#include "stdafx.h"
#include "POP3DataList.h"


CPOP3DataList::CPOP3DataList(void)
{
}


CPOP3DataList::~CPOP3DataList(void)
{
	POSITION posDataList = GetHeadPosition();

	while( posDataList != NULL )
	{
		CPOP3Data* pPOP3Data = ( CPOP3Data* )GetNext( posDataList );
		delete pPOP3Data;
		pPOP3Data = NULL;
	}
}

void CPOP3DataList::AddPostRequestData( CPOP3Data* pPostRequestData )
{
	AddTail(( void* )pPostRequestData );
}

BOOL CPOP3DataList::GetPostRequestData( CString strRequestId, CPOP3Data*& pRefPostRequestData )
{
	POSITION posDataList = GetHeadPosition();

	while( posDataList != NULL )
	{
		CPOP3Data* pPostRequestData = ( CPOP3Data* )GetNext( posDataList );

		if( pPostRequestData->GetRequestId() == strRequestId )
		{
			pRefPostRequestData = pPostRequestData;
			return TRUE;
		}
	}

	ASSERT( FALSE );
	return FALSE;
}

BOOL CPOP3DataList::RemovePostRequestItem( CString strMessageId )
{
	POSITION posDataList = GetHeadPosition();
	POSITION posPrevDataList = NULL;
	CString strErrMsg;

	while( posDataList != NULL )
	{
		posPrevDataList = posDataList;
		CPOP3Data* pPostRequestData = ( CPOP3Data* )GetNext( posDataList );
		
		if( pPostRequestData->GetRequestId() == strMessageId )
		{
			pPostRequestData->SetOrderStatus( ORDER_STATUS_PENDING_CLOSE );
			FileRequestItem( pPostRequestData, strErrMsg );
			RemoveAt( posPrevDataList );
			delete pPostRequestData;
			pPostRequestData = NULL;
			return TRUE;
		}
	}

	ASSERT( FALSE );
	return FALSE;
}

BOOL CPOP3DataList::DeletePostRequestItem( CString strMessageId, CString& strErrMsg )
{
	POSITION posDataList = GetHeadPosition();
	POSITION posPrevDataList = NULL;
	CString strFilePath;

	while( posDataList != NULL )
	{
		posPrevDataList = posDataList;
		CPOP3Data* pPostRequestData = ( CPOP3Data* )GetNext( posDataList );
		
		if( pPostRequestData->GetRequestId() == strMessageId )
		{
			if( !( GetFilePath( pPostRequestData, strFilePath, strErrMsg )))
				return FALSE;

			strFilePath += pPostRequestData->GetRequestId() + ".txt";

			if( !( RemoveFileFromCurrent( pPostRequestData, strFilePath, strErrMsg )))
				return FALSE;

			RemoveAt( posPrevDataList );
			delete pPostRequestData;
			pPostRequestData = NULL;
			return TRUE;
		}
	}

	ASSERT( FALSE );
	return FALSE;
}

BOOL CPOP3DataList::GetFilePath( CPOP3Data* pPostRequestData, CString& strFilePath, CString& strErrMsg )
{
	strFilePath = "C:\\Emails\\";

	switch( pPostRequestData->GetOrderStatus())
	{
	case ORDER_STATUS_RETRIEVED:
	case ORDER_STATUS_FILED:
	case ORDER_STATUS_PENDING_CLOSE:
		strFilePath += "Current\\";
		break;
			
	case ORDER_STATUS_CLOSED:
		strFilePath += "Completed\\";
		strFilePath += m_WinUtils.NumToStr( m_WinUtils.GetCurrentDate()) + "\\";
		break;

	default:
		strErrMsg = "Unhandled order status type detected.";
		return FALSE;
	}

	return TRUE;
}

BOOL CPOP3DataList::RemoveFileFromCurrent( CPOP3Data* pPostRequestData, CString strFilePath, CString& strErrMsg )
{
	if( m_TextFileUtils.FileExists( strFilePath ))
		m_TextFileUtils.KillFile( strFilePath );

	return TRUE;
}

BOOL CPOP3DataList::FileRequestItem( CPOP3Data* pPostRequestData, CString& strErrMsg )
{
	if( pPostRequestData->GetOrderStatus() == ORDER_STATUS_FILED )
		return TRUE;

	CStdioFile EmailFile;
	CString strFilePath;

	if( pPostRequestData->GetOrderStatus() == ORDER_STATUS_PENDING_CLOSE )
	{
		if( !( GetFilePath( pPostRequestData, strFilePath, strErrMsg )))
			return  FALSE;

		strFilePath += pPostRequestData->GetRequestId() + ".txt";

		if( !( RemoveFileFromCurrent( pPostRequestData, strFilePath, strErrMsg )))
			return FALSE;

		pPostRequestData->SetOrderStatus( ORDER_STATUS_CLOSED );
	}

	if( !( GetFilePath( pPostRequestData, strFilePath, strErrMsg )))
		return  FALSE;

	if( !( m_WinUtils.DirectoryExists( strFilePath, strErrMsg )))
	{
		if( !( CreateDirectory( strFilePath, NULL )))
		{
			strErrMsg = "Failed to create directory: " + strFilePath;
			return FALSE;
		}
	}

	CString strFileName = pPostRequestData->GetRequestId() + ".txt";
	strFilePath += strFileName;

	if( !( m_TextFileUtils.OpenFileForWriting( &EmailFile, strFilePath )))
	{
		strErrMsg = "Failed to write email file: " + strFilePath;
		return FALSE;
	}

	if( !( m_TextFileUtils.WriteLineToFile( &EmailFile, pPostRequestData->GetMessageRaw())))
	{
		strErrMsg = "Failed to write email line to file.";
		return FALSE;
	}
	
	EmailFile.Close();

	return TRUE;

}
