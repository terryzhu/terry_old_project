#include "StdAfx.h"
#include "SimpleXMLNodes.h"

CSimpleXMLNodes::CSimpleXMLNodes(void)
{
}
CSimpleXMLNodes::CSimpleXMLNodes(IXMLDOMNodeListPtr pNodeList)
{
	m_pNodeList=pNodeList;
}
CSimpleXMLNodes::~CSimpleXMLNodes(void)
{
}
INT CSimpleXMLNodes::GetCount()
{
	LONG count=0;
	m_pNodeList->get_length(&count);
	return count;
}
BOOL CSimpleXMLNodes::GetNumberedItem(LONG nIndex,CString & strOut)
{

	CComBSTR bstr;
	IXMLDOMNodePtr pNode=NULL;
	m_pNodeList->get_item(nIndex,&pNode);
	pNode->get_text(&bstr);
	strOut=bstr;
/*
	IXMLDOMNodeListPtr pNodes=NULL;
	pNode->get_childNodes(&pNodes);
	LONG len=0;
	pNodes->get_length(&len);
	for (int i=0;i<len;i++)
	{
		pNodes->get_item(i,&pNode);
		CComBSTR bstr;
		
		pNode->get_nodeName(&bstr);
		CString cstr(bstr);
		if (cstr==strSearch)
		{
			CComBSTR str;	
			pNode->get_text(&str);
			strOut=str;
			break;
			
		}
	}*/
	return TRUE;
}