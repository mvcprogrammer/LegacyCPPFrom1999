#pragma once
#import "MSXML3.dll" named_guids
using namespace MSXML2;

class AFX_EXT_CLASS CXmlNodeWrapper
{
public:
	CString NodeType();
	CString GetAttribVal(int index);
	CString GetAttribName(int index);
	int NumAttributes();
	void ReplaceNode(MSXML2::IXMLDOMNodePtr pOldNode,MSXML2::IXMLDOMNodePtr pNewNode);
	CString GetText();
	void SetText(LPCTSTR text);
	MSXML2::IXMLDOMNodePtr InsertAfter(MSXML2::IXMLDOMNodePtr refNode, MSXML2::IXMLDOMNodePtr pNode);
	CString Name();
	MSXML2::IXMLDOMNodeListPtr FindNodes(LPCTSTR searchStr);
	MSXML2::IXMLDOMNodePtr Parent();
	void RemoveNodes(LPCTSTR searchStr);
	MSXML2::IXMLDOMNodePtr InsertAfter(MSXML2::IXMLDOMNodePtr refNode, LPCTSTR nodeName);
	MSXML2::IXMLDOMNodePtr InsertBefore(MSXML2::IXMLDOMNodePtr refNode, LPCTSTR nodeName);
	MSXML2::IXMLDOMNode* Interface();
	MSXML2::IXMLDOMDocumentPtr  ParentDocument();
	CString GetXML();
	MSXML2::IXMLDOMNodePtr RemoveNode(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr InsertNode(int index,LPCTSTR nodeName);
	MSXML2::IXMLDOMNodePtr InsertNode(int index,MSXML2::IXMLDOMNodePtr pNode);
	long NumNodes();
	MSXML2::IXMLDOMNode* Detach();
	MSXML2::IXMLDOMNodePtr GetNode(LPCTSTR nodeName);
	MSXML2::IXMLDOMNodePtr GetNode(int nodeIndex);
	MSXML2::IXMLDOMNodePtr FindNode(LPCTSTR searchString);
	MSXML2::IXMLDOMNodePtr GetPrevSibling();
	MSXML2::IXMLDOMNodePtr GetNextSibling();
	void SetValue(LPCTSTR valueName,LPCTSTR value);
	void SetValue(LPCTSTR valueName,int value);
	void SetValue(LPCTSTR valueName,short value);
	void SetValue(LPCTSTR valueName,double value);
	void SetValue(LPCTSTR valueName,float value);
	void SetValue(LPCTSTR valueName,bool value);
	BOOL IsValid();
	CString GetValue(LPCTSTR valueName);
	CXmlNodeWrapper();
	CXmlNodeWrapper(MSXML2::IXMLDOMNodePtr pNode,BOOL bAutoRelease = TRUE);
	void operator=(MSXML2::IXMLDOMNodePtr pNode);
	virtual ~CXmlNodeWrapper();
private:
	BOOL m_bAutoRelease;
	MSXML2::IXMLDOMNodePtr m_xmlnode;
};

class AFX_EXT_CLASS CXmlDocumentWrapper
{
public:
	CString GetUrl();
	CString GetXML();
	BOOL IsValid();
	BOOL Load(LPCTSTR path);
	BOOL LoadXML(LPCTSTR xml);
	BOOL Save(LPCTSTR path = CString( "" ));
	MSXML2::IXMLDOMDocument* Detach();
	MSXML2::IXMLDOMDocumentPtr  Clone();
	CXmlDocumentWrapper();
	CXmlDocumentWrapper(MSXML2::IXMLDOMDocumentPtr pDoc);
	MSXML2::IXMLDOMNodePtr AsNode();
	virtual ~CXmlDocumentWrapper();
private:
	MSXML2::IXMLDOMDocumentPtr m_xmldoc;
};

class AFX_EXT_CLASS CXmlNodeListWrapper
{
public:
	MSXML2::IXMLDOMDocumentPtr  AsDocument();
	MSXML2::IXMLDOMNodePtr Node(int index);
	void Start();
	MSXML2::IXMLDOMNodePtr Next();
	BOOL IsValid();
	int Count();
	CXmlNodeListWrapper();
	CXmlNodeListWrapper(MSXML2::IXMLDOMNodeListPtr pList);
	void operator=(MSXML2::IXMLDOMNodeListPtr pList);
	virtual ~CXmlNodeListWrapper();
private:
	MSXML2::IXMLDOMNodeListPtr m_xmlnodelist;
};

