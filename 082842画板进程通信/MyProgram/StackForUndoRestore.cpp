#include "stdafx.h"
#include "StackForUndoRestore.h"
void CStackForUndoRestore::Push(LPUNDO_RESTORE_INFO lp)
{
	if (m_nSpaceSize==UNDO_MAX_SIZE)//超出最大撤销范围
	{
		delete m_lpUndoRestoreInfo[0];//释放第一个撤销空间
		LPUNDO_RESTORE_INFO p[UNDO_MAX_SIZE];
		LPUNDO_RESTORE_INFO *temp=&m_lpUndoRestoreInfo[1];
		memcpy(p,temp,(UNDO_MAX_SIZE-1)*4);
		memcpy(m_lpUndoRestoreInfo,p,(UNDO_MAX_SIZE-1)*4);
		m_nContentSize=UNDO_MAX_SIZE;
		m_nSpaceSize=m_nContentSize;
		m_lpUndoRestoreInfo[UNDO_MAX_SIZE-1]=lp;
	}
	else
	{
		m_lpUndoRestoreInfo[m_nContentSize]=lp;
		m_nContentSize++;
		m_nSpaceSize=m_nContentSize;
	}

}
BOOL CStackForUndoRestore::Pop()
{
	if (m_nContentSize)
	{
		m_nContentSize--;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}
LPUNDO_RESTORE_INFO CStackForUndoRestore::Get()
{
	if (!m_nContentSize)
	{
		return NULL;
	}
	return m_lpUndoRestoreInfo[m_nContentSize-1];
}
LPUNDO_RESTORE_INFO CStackForUndoRestore::Restore()
{
	if (m_nContentSize==m_nSpaceSize)
	{
		return NULL;
	}
	return m_lpUndoRestoreInfo[m_nContentSize++];
}

void CStackForUndoRestore::Clear()
{
	for (int i=0;i<UNDO_MAX_SIZE;i++)
	{
		if (m_lpUndoRestoreInfo[i]!=NULL)
		{
			LPUNDO_RESTORE_INFO lp;
			lp=m_lpUndoRestoreInfo[i];
			delete lp;
			m_lpUndoRestoreInfo[i]=NULL;
		}
	}
	m_nContentSize=0;
	m_nSpaceSize=0;
}
CStackForUndoRestore ::CStackForUndoRestore()
{
	m_nContentSize=0;
	m_nSpaceSize=0;
	ZeroMemory(m_lpUndoRestoreInfo,UNDO_MAX_SIZE*sizeof(UNDO_RESTORE_INFO));
}