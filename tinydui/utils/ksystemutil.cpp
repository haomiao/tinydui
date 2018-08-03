#include "kpch.h"
#include "ksystemutil.h"

#include <vector>

namespace TinyDui 
{
static BOOL s_bFirstCheckMainMonitor = TRUE;
static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
	HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	std::vector<RECT>* list = (std::vector<RECT>*)(dwData);

	//保存显示器信息
	MONITORINFO monitorinfo;         
	monitorinfo.cbSize = sizeof(MONITORINFO);

	//获得显示器信息，将信息保存到monitorinfo中
	GetMonitorInfo(hMonitor, &monitorinfo);

	//若检测到主屏
	if(monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
	{
		if(s_bFirstCheckMainMonitor)  //第一次检测到主屏
		{
			s_bFirstCheckMainMonitor = FALSE;
			list->push_back(monitorinfo.rcMonitor);
			return TRUE;  

		}
		else //第二次检测到主屏,说明所有的监视器都已经检测了一遍，故可以停止检测了
		{
			s_bFirstCheckMainMonitor = TRUE;    //标志复位
			return FALSE;    //结束检测
		}
	}

	list->push_back(monitorinfo.rcMonitor);
	return TRUE;
}


void AdjustWndPos(RECT &rcTargetWnd)
{
	std::vector<RECT> lstMonitor;
	s_bFirstCheckMainMonitor = TRUE;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&lstMonitor);

	// 如果窗口显示在右边界的，就进行特殊处理
	int nTargetWidth = rcTargetWnd.right - rcTargetWnd.left;
	int nSize = (int)lstMonitor.size();
	for(int i = 0; i < nSize; ++i)
	{
		if(rcTargetWnd.left <= lstMonitor[i].right && rcTargetWnd.right > lstMonitor[i].right)
		{
			rcTargetWnd.left = lstMonitor[i].right - nTargetWidth;
			rcTargetWnd.right = rcTargetWnd.left + nTargetWidth;
			break;
		}
		else if(rcTargetWnd.left < lstMonitor[i].left && rcTargetWnd.right >= lstMonitor[i].left)
		{
			rcTargetWnd.left = lstMonitor[i].left;
			rcTargetWnd.right = rcTargetWnd.left + nTargetWidth;
			break;
		}
	}
}

double GetCurrentDpiValue()
{
	HDC hDC = ::GetDC(NULL);
	// 在dpi为100%时，nFontDpiHeight和nFontOrgHeight是一样的，经过对比初值设为75获取到的数据比较准确
	double lfFontDpiHeight = (double)MulDiv(75, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	double lfFontOrgHeight = 100.0;
	::ReleaseDC(NULL, hDC);
	double lfDpi = lfFontDpiHeight / lfFontOrgHeight;
	return lfDpi;
}
}
