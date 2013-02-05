#include "StdAfx.h"
#include "TimerManager.h"

static const int m_gDownloadTimerIndex = 1;
static const int m_gCloseWindowsTimeeIndex = 2;

static const int m_gDownloadTimerEclapse = 30000;
static const int m_gCloseWindowTimerEclapse = 1000;

CTimerManager::CTimerManager(void)
{
}


CTimerManager::~CTimerManager(void)
{
}

int CTimerManager::getDownloadTimer()
{
	return m_gDownloadTimerIndex;
}

int CTimerManager::getCloseWindowTimer()
{
	return m_gCloseWindowsTimeeIndex;
}

int CTimerManager::DownloadTimerEclapse()
{
	return m_gDownloadTimerEclapse;
}

int CTimerManager::CloseWindowTimerEclapse()
{
	return m_gCloseWindowTimerEclapse;
}