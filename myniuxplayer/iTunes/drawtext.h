#ifndef _KK_DRAWTEXT_H_20090921_
#define _KK_DRAWTEXT_H_20090921_

#if _MSC_VER > 1000
#pragma once
#endif //_MSC_VER

//Ver 1.0
//用途：绘制平滑文字和一些特效文字

class CTextDrawer
{
public:
	static SIZE GetDrawSize(HDC hDc,LPCTSTR lpszText,UINT uFormat,bool& bHaveOffsety,short& yOffset);
	static int GetTextWidthPix(HDC hdc,CString str);
	static short CalcOffsety(HDC hDc); //计算 j的高度-b的高度的高度差,用于对齐字符输出
	static bool IsYOffset(UINT uChar);	//绘制的时候，需要向上偏移的字符(jJgy,false)
	static bool IsDoubleByteChar(UINT uChar);

	static short m_cxSpacing; //字符间隔
	static short m_cwBlank; //空格键的宽度(GetGlyphOutline获得的宽度高度总为1)
};


#endif	//_KK_DRAWTEXT_H_20090921_