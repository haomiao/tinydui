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

	//������ʾ����Ϣ
	MONITORINFO monitorinfo;         
	monitorinfo.cbSize = sizeof(MONITORINFO);

	//�����ʾ����Ϣ������Ϣ���浽monitorinfo��
	GetMonitorInfo(hMonitor, &monitorinfo);

	//����⵽����
	if(monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
	{
		if(s_bFirstCheckMainMonitor)  //��һ�μ�⵽����
		{
			s_bFirstCheckMainMonitor = FALSE;
			list->push_back(monitorinfo.rcMonitor);
			return TRUE;  

		}
		else //�ڶ��μ�⵽����,˵�����еļ��������Ѿ������һ�飬�ʿ���ֹͣ�����
		{
			s_bFirstCheckMainMonitor = TRUE;    //��־��λ
			return FALSE;    //�������
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

	// ���������ʾ���ұ߽�ģ��ͽ������⴦��
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
	// ��dpiΪ100%ʱ��nFontDpiHeight��nFontOrgHeight��һ���ģ������Աȳ�ֵ��Ϊ75��ȡ�������ݱȽ�׼ȷ
	double lfFontDpiHeight = (double)MulDiv(75, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	double lfFontOrgHeight = 100.0;
	::ReleaseDC(NULL, hDC);
	double lfDpi = lfFontDpiHeight / lfFontOrgHeight;
	return lfDpi;
}
}
