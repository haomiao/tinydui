#ifndef _K_CTRL_UI_BASE_H_
#define _K_CTRL_UI_BASE_H_

#include <Windows.h>
#include <string>
#include "kuihandlercallback.h"
#include "utils/kuidefine.h"

namespace TinyDui 
{
// 控件基类
class KCtrlUIBase
{
public:

	KCtrlUIBase();
	virtual ~KCtrlUIBase();

public:

	void SetOwnerWndHandle(HWND hOwnerWndHandle);
	HWND GetOwnerWndHandle() const;

	// 设置窗口句柄，仅针对实际带句柄窗口有效
	void SetCtrlWndHandle(HWND hCtrlWndHandle);
	HWND GetCtrlWndHandle() const;

	// 注意：rect均需为经过DPI转化后的结果
	void SetRect(const RECT& rc);
	void SetRect(int nLeft, int nTop, int nRight, int nBottom);
	RECT GetRect() const;

	bool IsCtrlIDValid() const;
	void SetCtrlID(int nID);
	int GetCtrlID() const;

	void SetUIHandler(KUIHandlerCallBack* pHandlerCallBack);
	KUIHandlerCallBack* GetUIHandler() const;

	bool IsVisible() const;
	void SetVisible(bool bVisible);

	bool GetHit() const;
	void SetHit(bool bHit);

	bool IsDrag() const;
	void SetDrag(bool bDrag);

	void DelayDispatch(DWORD dwDelayMs);
	void DisableTopWindow();
	void EnableTopWindow();

	SIZE GetTextExtentPointSize(const std::wstring& strText) const;
	static SIZE GetTextExtentPointSize(const std::wstring& strText, HDC hDC);

	RECT GetDrawTextRect(const std::wstring& strText, UINT nFormat) const;
	static RECT GetDrawTextRect(const std::wstring& strText, HDC hDC, UINT nFormat);

	void ReDrawSelf(BOOL bErase = FALSE);
	void ReDraw(const RECT* lpRcDraw, BOOL bErase = FALSE);
	void ReDrawCatcher();

	POINT GetCursorPt();

	// 获取经过DPI转化后的结果
	RECT GetDPIRect(const RECT& rc) const;
	POINT GetDPIPoint(const POINT& pt) const;
	SIZE GetDPISize(const SIZE& sz) const;

	static RECT GetDPIRect(const RECT& rc, double lfDPI);
	static POINT GetDPIPoint(const POINT& pt, double lfDPI);
	static SIZE GetDPISize(const SIZE& sz, double lfDPI);

public:

	virtual bool IsEnableDefault() const;
	virtual bool Hit(const POINT& pt) const;

	virtual void Hover() = 0;
	virtual void Leave() = 0;
	virtual bool IsHover() = 0;

	virtual void OnSize() = 0;
	virtual void OnMouseMove(const POINT &pt) = 0;
	virtual void Click() = 0;
	virtual void OnLButtonDown(const POINT &pt) = 0;
	virtual void OnLButtonUp(const POINT &pt) = 0;

	virtual void Draw(HDC hMemDC) = 0;

	// 以下接口用于支持自动调整窗口大小和布局
	// 对于支持的控件实现此类接口即可
	virtual bool IsSupportAutoAdjustRect() const;
	virtual RECT GetExpectantRect() const;

protected:

	HWND m_hCtrlWndHandle;	// 控件的句柄(对于Dui控件则无窗口句柄(0))
	HWND m_hOwnerWndHandle;	// 控件所在拥有窗口句柄
	RECT m_rcCtrl;			// 控件区域
	int  m_nCtrlID;			// 控件ID

	bool m_bVisible;		// 控件是否可见
	bool m_bHit;			// 控件是否被击中测试
	bool m_bDrag;			// 控件是否支持拖拽
	double m_lfDpi;			// DPI分辨缩放值

	KUIHandlerCallBack *m_pHandlerCallBack;	// UI事件回调对象
};
}

#endif // _K_CTRL_UI_BASE_H_
