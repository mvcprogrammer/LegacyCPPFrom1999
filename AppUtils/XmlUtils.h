#pragma once
#include "utils.h"
#include "XmlNodeWrapper.h"

class AFX_EXT_CLASS	CXmlUtils : public CUtils
{
public:
	CXmlUtils(void);
	~CXmlUtils(void);

private:
	CXmlDocumentWrapper* m_pXmlDocumentWrapper;
	CUtils m_Utils;

public:
	BOOL LoadXML( CString strXML );
	BOOL LoadXMLFromFile( CString strXMLFilePath );
	CXmlDocumentWrapper* GetXmlDocumentWrapper( void );
};

