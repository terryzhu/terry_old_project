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
	CShape *pSh;//�Ǹ�ͼ���ָ��
	INT nWhichChange;//ʲô����(ɾ�����Ǽ��е�)
	INT Index;//��PtrArr�е�����
	union
	{
		int nFirParam;
		float fFirParam;//��������ʱ�˲�����ʹ��(Ϊx�ķ�������)
	};
	union
	{
		int nSecParam;
		float fSecParam;//��������ʱ�˲�����ʹ��(Ϊy�ķ�������)
	};
	BasePoint Bp;//����Ƿ���,��ô����ǻ�׼��
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
	int m_nContentSize;//���Գ���������
	int m_nSpaceSize;//�����ɻ�ԭ������,��������Push�����õ���nContentSize
};