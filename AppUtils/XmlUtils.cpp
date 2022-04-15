#include "StdAfx.h"
#include "XmlUtils.h"
#include "XmlNodeWrapper.h"
#include "TextFileUtils.h"

CXmlUtils::CXmlUtils(void)
{
	::CoInitialize( NULL );
	m_pXmlDocumentWrapper = new CXmlDocumentWrapper();
}

CXmlUtils::~CXmlUtils(void)
{
	delete m_pXmlDocumentWrapper;
	m_pXmlDocumentWrapper = NULL;
	::CoUninitialize();
}

BOOL CXmlUtils::LoadXML( CString strXML )
{
	CString strCallingFunction = "CXmlUtils::LoadXML()";

	if( !( m_pXmlDocumentWrapper->LoadXML( strXML )))
	{
		SendAdminAlert( "Failed to load XML", strCallingFunction, strXML );
		return FALSE;
	}

	if( !( m_pXmlDocumentWrapper->IsValid()))
	{
		SendAdminAlert( "Invalid XML detected.", strCallingFunction, strXML );
		return FALSE;
	}

	return TRUE;
}

BOOL CXmlUtils::LoadXMLFromFile( CString strXMLFilePath )
{
	CString strCallingFunction = "CXmlUtils::LoadXMLFromFile()";

	CTextFileUtils TextFileUtils;
	CString strXml;

	CStdioFile XmlReadingFile;

	if( !( TextFileUtils.OpenFileForReading( &XmlReadingFile, strXMLFilePath )))
		return FALSE;

	CString strXmlLine;

	while( TextFileUtils.ReadNextLineFromFile( &XmlReadingFile, strXmlLine ))
		strXml += strXmlLine;

	if( !( m_pXmlDocumentWrapper->LoadXML( strXml )))
	{
		SendAdminAlert( "Failed to load XML from file.", strCallingFunction, strXMLFilePath );
		return FALSE;
	}

	if( !( m_pXmlDocumentWrapper->IsValid()))
	{
		SendAdminAlert( "Invalid XML detected.", strCallingFunction, strXMLFilePath );
		return FALSE;
	}

	return TRUE;
}

CXmlDocumentWrapper* CXmlUtils::GetXmlDocumentWrapper()
{
	return m_pXmlDocumentWrapper;
}