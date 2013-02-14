#include "stdafx.h"
#include "ShapeFile.h"
void CShapeFile::SaveToShapeFile(TCHAR * pChar,CPtrArr &ptrArr)
{
	m_hFile=CreateFile(pChar,GENERIC_WRITE | GENERIC_READ ,
		NULL,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwSetFlag=FILE_BEGIN_FLAG;
	DWORD dw;
	WriteFile(m_hFile,&dwSetFlag,4,&dw,NULL);
	for (int i=0;i<ptrArr.GetSize();i++)
	{
		if (ptrArr.GetAt(i)==0)
		{
			continue;
		}
		((CShape*)ptrArr.GetAt(i))->Serialize(m_hFile);
	}
	dwSetFlag=FILE_END_FLAG;
	WriteFile(m_hFile,&dwSetFlag,4,&dw,NULL);
	CloseHandle(m_hFile);
}

BOOL CShapeFile::DeSerialize(TCHAR * pChar,CPtrArr &ptr)
{
	m_hFile=CreateFile(pChar,GENERIC_READ ,
		NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwForTest;
	DWORD dw;
	ReadFile(m_hFile,&dwForTest,4,&dw,NULL);
	if (dwForTest!=FILE_BEGIN_FLAG)
	{
		CloseHandle(m_hFile);
		return FALSE;
	}
	int * temp;
	CShape *pSh;
	while (TRUE)
	{
		ReadFile(m_hFile,&dwForTest,4,&dw,NULL);
		if (dwForTest==FILE_END_FLAG)
		{
			CloseHandle(m_hFile);
			return TRUE;
		}
		if (dwForTest==HAS_OBJ_FLAG)
		{
			ReadFile(m_hFile,&dwForTest,4,&dw,NULL);
			switch (dwForTest)
			{
			case ISCLINE:
				{
					temp=new int[CLINE_VALUE_SPACE>>2];
					ReadFile(m_hFile,temp,28,&dw,NULL);
					POINT pt1;
					POINT pt2;
					pt1.x=temp[3];
					pt1.y=temp[4];
					pt2.x=temp[5];
					pt2.y=temp[6];
					pSh=new CLine(pt1,pt2,temp[1],temp[2],temp[0]);
					ptr.Add(pSh);
					delete [] temp;
				}
				break;
			case ISCCIRCLE:
				{
					temp=new int[CCIRCLE_VALUE_SPACE>>2];
					ReadFile(m_hFile,temp,28,&dw,NULL);
					RECT rect;
					rect.left=temp[3];
					rect.top=temp[4];
					rect.right=temp[5];
					rect.bottom=temp[6];
					pSh=new CCircle(rect,temp[1],temp[2],temp[0]);
					ptr.Add(pSh);
					delete [] temp;
				}
				break;
			case ISCRECTANGLE:
				{
					temp=new int[CRECTANGLE_VALUE_SPACE>>2];
					ReadFile(m_hFile,temp,32,&dw,NULL);
					POINT pt1;
					POINT pt2;
					pt1.x=temp[4];
					pt1.y=temp[5];
					pt2.x=temp[6];
					pt2.y=temp[7];
					pSh=new CRectangle(pt1,pt2,temp[1],temp[2],temp[0],temp[3]);
					ptr.Add(pSh);
					delete [] temp;
				}
				
				break;
			case ISCPLOY:
				{
					temp =new int[4];
					ReadFile(m_hFile,temp,16,&dw,NULL);
					int color=temp[0];
					int penstle=temp[1];
					int width=temp[2];
					int size=temp[3];
					delete [] temp;
					temp=new int[size<<1];
					ReadFile(m_hFile,temp,8*size,&dw,NULL);
					CPtrArr ptrTemp;
					POINT *pPt;
					for (int i=0;i<size;i++)
					{
						pPt=new POINT;
						pPt->x=temp[2*i];
						pPt->y=temp[2*i+1];
						ptrTemp.Add(pPt);
					}
					pSh=new CPloy(penstle,width,color,ptrTemp);
					ptr.Add(pSh);
					delete [] temp;
				}
				break;
			case ISCPENCIL:
				{
					temp =new int[4];
					ReadFile(m_hFile,temp,16,&dw,NULL);
					int color=temp[0];
					int penstle=temp[1];
					int width=temp[2];
					int size=temp[3];
					delete [] temp;
					temp=new int[size<<1];
					ReadFile(m_hFile,temp,8*size,&dw,NULL);
					CPtrArr ptrTemp;
					POINT *pPt;
					for (int i=0;i<size;i++)
					{
						pPt=new POINT;
						pPt->x=temp[2*i];
						pPt->y=temp[2*i+1];
						ptrTemp.Add(pPt);
					}
					pSh=new CPencil(penstle,width,color,ptrTemp);
					ptr.Add(pSh);
					delete [] temp;
				}
				break;
			}
		}
	}
	CloseHandle(m_hFile);



	return TRUE;
}