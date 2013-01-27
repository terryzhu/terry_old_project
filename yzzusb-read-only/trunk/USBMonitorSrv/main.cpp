#include "stdafx.h"
#include "SvrCtrl.h"
#include "UsbMonitorSvr.h"
#include "FileFinder.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 1)
	{
		SvrMain<CUsbMonitorSvr>();
		return 0;
	}
	else if (argc == 2)
	{
		CString strArg = argv[1];
		if (!strArg.CompareNoCase(_T("/i")))
		{
			CSvrCtrl svrCtrl(_T("MonitorSvr"));
			return !svrCtrl.Install(_T("MonitorSvr"),_T("MonitorSvr"));
		}
		else if (!strArg.CompareNoCase(_T("/u")))
		{
			CSvrCtrl svrCtrl(_T("MonitorSvr"));
			return !svrCtrl.Uninstall();
		}
	}

	return 0;
}