#pragma once
#include "windows.h"
#include "Shape.h"
#include "PtrArr.h"
#include "Line.h"
#include "Rectangle.h"
#include "Pencil.h"
#include "Ploy.h"
#include "Circle.h"
#define FILE_BEGIN_FLAG 0xBEBEBEBE
#define FILE_END_FLAG 0xEDEDEDED

class CShapeFile
{
public:
//	CShapeFile(TCHAR *pChar);
	void SaveToShapeFile(TCHAR * pChar,CPtrArr &ptr);
	BOOL  DeSerialize(TCHAR * pChar,CPtrArr &ptr);
private:
	HANDLE m_hFile;
};