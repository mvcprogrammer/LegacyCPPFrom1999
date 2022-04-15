#include "StdAfx.h"
#include "TextFileUtils.h"


CTextFileUtils::CTextFileUtils()
{
}
CTextFileUtils::CTextFileUtils(CString strLogPath)
	: CUtils(strLogPath)
{
}

CTextFileUtils::~CTextFileUtils()
{

}

BOOL CTextFileUtils::OpenFileForReading( CStdioFile *pFile, CString strPathAndFileName )
{
	CString strCallingFunction = "CTextFileUtils::OpenFileForReading()";

	try
	{
		if( !( pFile->Open( strPathAndFileName, CStdioFile::modeRead | CStdioFile::typeText )))
		{
			SendAdminAlert( "Failed to open file", strCallingFunction, strPathAndFileName );
			return FALSE;
		}
	}
	catch( CFileException* pFileException )
	{
		HandleException( pFileException, strCallingFunction );
		return false;
	}

	return TRUE;
}

BOOL CTextFileUtils::OpenFileForWriting( CStdioFile *pFile, CString strFileAndPathName )
{
	CString strCallingFunction = "CTextFileUtils::OpenFileForWriting()";

	try
	{
		if( !( pFile->Open( strFileAndPathName, CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeText )))
		{
			SendAdminAlert( "Failed to open file", strCallingFunction, strFileAndPathName );
			return FALSE;
		}
	}
	catch( CFileException* pFileException )
	{
		HandleException( pFileException, strCallingFunction );
		return false;
	}

	return TRUE;	
}

BOOL CTextFileUtils::OpenFileForAppend( CStdioFile *pFile, CString strFileAndPathName )
{
	CString strCallingFunction = "CTextFileUtils::OpenFileForAppend()";

	try
	{
		if( !( pFile->Open( strFileAndPathName, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite | CStdioFile::typeText )))
		{
			SendAdminAlert( "Failed to open file", strCallingFunction, strFileAndPathName );
			return FALSE;
		}

		pFile->SeekToEnd();
	}
	catch( CFileException* pFileException )
	{
		HandleException( pFileException, strCallingFunction );
		return FALSE;
	}

	return TRUE;	
}

BOOL CTextFileUtils::ReadNextLineFromFile( CStdioFile *pFile, CString &rLine )
{
	CString strCallingFunction = "CTextFileUtils::ReadNextLineFromFile()";

	try
	{
		return pFile->ReadString( rLine );
	}
	catch( CFileException* pFileException )
	{
		HandleException( pFileException, strCallingFunction );
		return FALSE;
	}
}

BOOL CTextFileUtils::WriteLineToFile( CStdioFile *pFile, CString strLine )
{
	CString strCallingFunction = "CTextFileUtils::WriteLineToFile()";

	try
	{
		pFile->WriteString( strLine + "\n" );
	}
	catch( CFileException* pFileException )
	{
		HandleException( pFileException, strCallingFunction );
		return false;
	}

	return TRUE;
}

BOOL CTextFileUtils::KillDirectory(CString strDirectoryPath)
{
	return RemoveDirectory(strDirectoryPath);
}

BOOL CTextFileUtils::KillFile( CString strFileNameAndPath )
{
	ASSERT( strFileNameAndPath != "" );
	CString strCallingFunction = "CTextFileUtils::KillFile()";
	CFile file;

	try
	{
		file.Remove( strFileNameAndPath );
	}
	catch( CFileException* pFileException )
	{
		HandleException( pFileException, strCallingFunction );
		return false;
	}

	return TRUE;
}

BOOL CTextFileUtils::FileExists( CString strFileNameAndPath )
{
	CFileFind FileFinder;

	if( FileFinder.FindFile( strFileNameAndPath ))
		return TRUE;
	else
		return FALSE;
}