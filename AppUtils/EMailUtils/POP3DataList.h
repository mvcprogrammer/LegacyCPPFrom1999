#pragma once
#include "afxcoll.h"
#include "POP3Data.h"
#include "..\..\AppUtils\TextFileUtils.h"
#include "..\..\AppUtils\WinUtils.h"

class AFX_EXT_CLASS CPOP3DataList : public CPtrList
{
public:
	CPOP3DataList(void);
	virtual ~CPOP3DataList(void);

public:
	void AddPostRequestData( CPOP3Data* pPOP3Data );
	BOOL GetPostRequestData( CString strMessageId, CPOP3Data*& pRefPostRequestData );
	BOOL RemovePostRequestItem( CString strMessageId );
	BOOL FileRequestItem( CPOP3Data* pPostRequestData, CString& strErrMsg );
	BOOL GetFilePath( CPOP3Data* pPostRequestData, CString& strFilePath, CString& strErrMsg );
	BOOL RemoveFileFromCurrent( CPOP3Data* pPostRequestData, CString strFileName, CString& strErrMsg );
	BOOL DeletePostRequestItem( CString strMessageId, CString& strErrMsg );


private:
	CWinUtils m_WinUtils;
	CTextFileUtils m_TextFileUtils;

private:
	void ShowPostDownEditDlg();
};

