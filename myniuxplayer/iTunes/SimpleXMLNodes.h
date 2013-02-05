#pragma once
#include "SimpleXML.h"
class CSimpleXMLNodes
{
	friend class CSimpleXML;
public:
	CSimpleXMLNodes(void);
	~CSimpleXMLNodes(void);
	CSimpleXMLNodes(IXMLDOMNodeListPtr pNodeList);
	INT GetCount();
	BOOL GetNumberedItem(LONG nIndex,CString & strOut);
private:
	IXMLDOMNodeListPtr m_pNodeList;
};
