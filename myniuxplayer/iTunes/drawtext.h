#ifndef _KK_DRAWTEXT_H_20090921_
#define _KK_DRAWTEXT_H_20090921_

#if _MSC_VER > 1000
#pragma once
#endif //_MSC_VER

//Ver 1.0
//��;������ƽ�����ֺ�һЩ��Ч����

class CTextDrawer
{
public:
	static SIZE GetDrawSize(HDC hDc,LPCTSTR lpszText,UINT uFormat,bool& bHaveOffsety,short& yOffset);
	static int GetTextWidthPix(HDC hdc,CString str);
	static short CalcOffsety(HDC hDc); //���� j�ĸ߶�-b�ĸ߶ȵĸ߶Ȳ�,���ڶ����ַ����
	static bool IsYOffset(UINT uChar);	//���Ƶ�ʱ����Ҫ����ƫ�Ƶ��ַ�(jJgy,false)
	static bool IsDoubleByteChar(UINT uChar);

	static short m_cxSpacing; //�ַ����
	static short m_cwBlank; //�ո���Ŀ��(GetGlyphOutline��õĿ�ȸ߶���Ϊ1)
};


#endif	//_KK_DRAWTEXT_H_20090921_