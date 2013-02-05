#include "StdAfx.h"
#include "SimpleXML.h"

CSimpleXML::CSimpleXML(void)
{
	m_pDoc.CreateInstance( __uuidof(MSXML2::DOMDocument30) );
	m_pNodeForAppendChild=NULL;
}

CSimpleXML::~CSimpleXML(void)
{
	SAFE_RELEASE(m_pDoc)
	SAFE_RELEASE(m_xmlRoot)
	SAFE_RELEASE(m_xmlNode)
	SAFE_RELEASE(m_pNode)
	/*m_pDoc.Release();
	m_xmlRoot.Release();
	m_xmlNode.Release();*/
}
BOOL CSimpleXML::Open(CString strXmlFilePath)
{
	
	_variant_t t(strXmlFilePath);
	VARIANT_BOOL b;
	HRESULT hr=m_pDoc->load(t,&b);
	return hr==S_OK;
}
BOOL CSimpleXML::Save(CString strSavePath)
{
	_variant_t t(strSavePath);
	HRESULT hr=m_pDoc->save(t);
	return SUCCEEDED(hr);
}

//BOOL CSimpleXML::InsertItem(CString strName,CString strValue)
//{
//	
//	CComBSTR bt1(strName);
//	CComBSTR bt2(strValue);
//	IXMLDOMElementPtr p=NULL;
//	m_pDoc->createElement(bt1,&p);
//	p->put_text(bt2);
//	m_pNode->appendChild(p,&m_xmlNode);
//	return TRUE;
//}
BOOL CSimpleXML::InsertMainItem()
{
	IXMLDOMProcessingInstructionPtr pi = NULL;
	m_pDoc->createProcessingInstruction(_T("xml"),_T("version=\"1.0\" encoding=\"UTF-8\""),&pi);
	m_pDoc->appendChild(pi,&m_xmlNode);
	m_pDoc->createElement((_bstr_t)"NiuxPlayer",&m_xmlRoot);
	m_pDoc->appendChild(m_xmlRoot,&m_xmlNode);
	SAFE_RELEASE(pi)
//	IXMLDOMElementPtr pNode;

	return TRUE;
}
BOOL CSimpleXML::SelectNodes(CString strPath,CSimpleXMLNodes & pNodes)
{
	IXMLDOMNodeListPtr pNodeList = NULL;
	IXMLDOMNodePtr pNode=NULL;
	CComBSTR bstr(strPath);
	m_pDoc->selectSingleNode(bstr,&pNode);
	pNode->get_childNodes(&pNodeList);
	 //m_pDoc->selectNodes( _bstr_t(strPath) ,&);
	pNodes.m_pNodeList=pNodeList;

	return TRUE;
}
BOOL CSimpleXML::InsertOneNewMusic(CString str)
{
//	IXMLDOMElementPtr pNode;
	CComBSTR bstr(str);
	m_pDoc->createElement((_bstr_t)"music",&m_pNode);
	m_pNode->put_text(bstr);
	m_xmlRoot->appendChild(m_pNode,&m_xmlNode);
	return TRUE;
}
//
//BOOL CSimpleXML::BeginToAppendChild()
//{
//
//	CComBSTR bstr(L"NiuxPlayer");
//	m_pDoc->selectSingleNode(bstr,&m_pNodeForAppendChild);
//	return TRUE;
//}
//BOOL CSimpleXML::AppendOneNewMusic()
//{
//	m_pDoc->createElement((_bstr_t)"music",&m_pNode);
//	m_pNodeForAppendChild->appendChild(m_pNode,&m_xmlNode);
//	return TRUE;
//}
//BOOL CSimpleXML::AppendChild(CString strName,CString strValue)
//{
//	CComBSTR bt1(strName);
//	CComBSTR bt2(strValue);
//	IXMLDOMElementPtr p=NULL;
//	m_pDoc->createElement(bt1,&p);
//	p->put_text(bt2);
//	m_pNode->appendChild(p,&m_xmlNode);
//	return TRUE;
//}