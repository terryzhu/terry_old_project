#pragma once
//-------------------------------------------------------------------------
//  [2/24/2010 ZJ]
//ʹ�÷���:
//1.��ȡXML:
//xml.Open(L"ttt.xml");
//CSimpleXMLNodes xmlNodes;
//xml.SelectNodes(L"NiuxPlayer",xmlNodes);
//for (int i=0;i<xmlNodes.GetCount();i++)
//{
//	CString str;
//	xmlNodes.GetNumberedItem(i,str);
//	MessageBox(str);
//}
//2.д��XML:
//CSimpleXML xml;
//xml.InsertMainItem();
//xml.InsertOneNewMusic(L"mmm");
//xml.InsertOneNewMusic(L"nnn");
//xml.InsertOneNewMusic(L"jjj");
//xml.Save(L"ttt.xml");
//  [2/24/2010 ZJ]
//-------------------------------------------------------------------------


#include   "msxml.h"
//using namespace MSXML2;
#include "SimpleXMLNodes.h"
#define SAFE_RELEASE(p)      if(p){p.Release();p=NULL;}
class CSimpleXML
{
	friend class CSimpleXMLNodes;
public:
	CSimpleXML(void);
	~CSimpleXML(void);
	BOOL Open(CString str);
	BOOL Save(CString str);
//	BOOL InsertItem(CString strName,CString strValue);
	BOOL InsertMainItem();
	 BOOL SelectNodes(CString strPath,CSimpleXMLNodes&pNodes);
	 BOOL InsertOneNewMusic(CString str);
//	 BOOL BeginToAppendChild();
//	 BOOL AppendOneNewMusic();
//	 BOOL AppendChild(CString strName,CString strValue);
private:
	IXMLDOMDocumentPtr m_pDoc;
	IXMLDOMElementPtr m_xmlRoot;
	IXMLDOMNodePtr m_xmlNode;
	IXMLDOMElementPtr m_pNode;
	IXMLDOMNodePtr m_pNodeForAppendChild;
	CString	m_strFilePath;
	
};
//�������Ϲ�ʱ
//  [2/11/2010 ZJ]
//-------------------------------------------------------------------------
//ʹ�÷���:
//1.��ȡXML:
//CSimpleXML xml;
//xml.Open(L"sample.xml");
//CSimpleXMLNodes sxmlnodes;
//xml.SelectNodes(L"NiuxPlayer",sxmlnodes);
//CString str;
//sxmlnodes.GetItem(i,L"name",str);
//sxmlnodes.GetItem(i,L"person",str);
//sxmlnodes.GetItem(i,L"time",str);
//--------------------------------------------------------------------------
//2.д��XML:
//CSimpleXML xml;
//xml.InsertMainItem();//����Ҫ
//xml.InsertOneNewMusic();//�����µ�һ�׸�������Ϣ
//xml.InsertItem(L"name",L"�ܽ���");
//xml.InsertItem(L"song",L"ҹ��");
//xml.InsertItem(L"time",L"100");
//xml.InsertOneNewMusic();//�����µ�һ�׸�������Ϣ
//xml.InsertItem(L"name",L"�ܽ���");
//xml.InsertItem(L"song",L"ҹ��");
//xml.InsertItem(L"time",L"100");
//xml.Save(L"example2.xml");
//-------------------------------------------------------------------------
//  [2/22/2010 ZJ]
//3.β��׷��
//CSimpleXML xml;
//xml.Open(L"sample.xml");
//xml.BeginToAppendChild();
//xml.AppendOneNewMusic();
//xml.AppendChild(L"person",L"aaa");
//xml.AppendChild(L"name",L"123");
//xml.AppendChild(L"time",L"321");
//xml.AppendOneNewMusic();
//xml.AppendChild(L"person",L"bbb");
//xml.AppendChild(L"name",L"123");
//xml.AppendChild(L"time",L"321");
//xml.Save(L"sample.xml");
//-------------------------------------------------------------------------