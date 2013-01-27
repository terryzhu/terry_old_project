#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>


#ifdef _DEBUG
#define SVRLOG(text) CLog(text);
#else
#define SVRLOG(text)  
#endif // _DEBUG


#define LOG_FILE "C:\\service.log"

class CLog
{
private:
	class CLogHeader
	{
	public:
		CLogHeader()
		{
			std::ofstream ofile(LOG_FILE, std::ios::app | std::ios::out);
			SYSTEMTIME time;
			::GetSystemTime(&time);
			ofile << "[" << time.wMonth << "-" << time.wDay << " " << time.wHour << ":" << time.wMinute << "]" << std::endl;
			ofile.flush();
			ofile.close();
		}
	};
public:
	CLog(char* szText)
	{
		std::ofstream ofile(LOG_FILE, std::ios::app | std::ios::out);
		ofile << szText << std::endl;
		ofile.flush();
		ofile.close();
	}

private:
	static CLogHeader s_logHeader;
};
