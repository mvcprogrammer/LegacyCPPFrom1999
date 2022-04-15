#pragma once
#include "utils.h"

class AFX_EXT_CLASS CTextFileUtils : public CUtils
{
public:
	BOOL FileExists( CString strFileNameAndPath );
	BOOL KillDirectory(CString strDirectoryPath);
	BOOL KillFile( CString strFileNameAndPath );
	BOOL WriteLineToFile(		CStdioFile* pFile, CString strLine );
	BOOL ReadNextLineFromFile(	CStdioFile* pFile, CString& rLine );
	BOOL OpenFileForWriting(	CStdioFile* pFile, CString strFileAndPathName );
	BOOL OpenFileForAppend(		CStdioFile* pFile, CString strFileAndPathName );
	BOOL OpenFileForReading(	CStdioFile* pFile, CString strPathAndFileName );
	CTextFileUtils(CString strLogPath);
	CTextFileUtils();
	virtual ~CTextFileUtils();
};

