#pragma once
#include "windows.h"
#include "Shape.h"
#define SHAPE_CREATE 0
#define SHAPE_MOVE 1
#define SHAPE_DELETE 2
#define SHAPE_CUT 3
#define SHAPE_PASTE 4
#define SHAPE_SCALING 5

typedef struct UNDO_RESTORE_INFO
{
	CShape *pSh;//那个图像的指针
	INT nWhichChange;//什么操作(删除或是剪切等)
	INT Index;//在PtrArr中的索引
	union
	{
		int nFirParam;
		float fFirParam;//仅当放缩时此参数才使用(为x的放缩倍数)
	};
	union
	{
		int nSecParam;
		float fSecParam;//仅当放缩时此参数才使用(为y的放缩倍数)
	};
	BasePoint Bp;//如果是放缩,那么这个是基准点
}*LPUNDO_RESTORE_INFO;
#define UNDO_MAX_SIZE 50
class CStackForUndoRestore
{
public:
	CStackForUndoRestore();
	void Push(LPUNDO_RESTORE_INFO lpUndoRestoreInfo);
	BOOL Pop();
	LPUNDO_RESTORE_INFO Get();
	LPUNDO_RESTORE_INFO Restore();
	void Clear();
private:
	LPUNDO_RESTORE_INFO m_lpUndoRestoreInfo[UNDO_MAX_SIZE];
	int m_nContentSize;//可以撤销的数量
	int m_nSpaceSize;//包括可还原的数量,该数量在Push后重置等于nContentSize
};