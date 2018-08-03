#ifndef _K_WND_HANDLE_UI_H_
#define _K_WND_HANDLE_UI_H_

#include <Windows.h>
#include "base/kctrluibase.h"

namespace TinyDui 
{
// 带窗口句柄控件
class KWndHandleUI : public KCtrlUIBase
{
public:

	KWndHandleUI();
	virtual ~KWndHandleUI();

public:

	void SetOwnerWndHandle(HWND hOwnerWndHandle);
	HWND GetOwnerWndHandle() const;

	// 设置窗口句柄，仅针对实际带句柄窗口有效
	void SetCtrlWndHandle(HWND hCtrlWndHandle);
	HWND GetCtrlWndHandle() const;

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
};
}

#endif // _K_WND_HANDLE_UI_H_
