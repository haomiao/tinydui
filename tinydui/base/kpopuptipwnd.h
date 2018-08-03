﻿#ifndef _K_POPUP_TIP_WND_H_
#define _K_POPUP_TIP_WND_H_

#include <GdiPlus.h>
#include <string>
#include "kmessagemapwnd.h"

namespace TinyDui 
{
class KPopupTipWnd : public KMessageMapWnd
{
public:

    KPopupTipWnd();
    virtual ~KPopupTipWnd();

public:

	virtual void InitResources();
	virtual void InitControls();
	virtual void ReleaseResources();

	virtual void DrawBackground(HDC hDC);
	virtual void DrawOthers(HDC hDC);
	virtual void DrawControls(HDC hDC);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnSize(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnKeyUp(WPARAM wParam, LPARAM lParam);

private:

	virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnEraseBkgnd(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:

	 /*++ InitWindow
	  * 初始化窗体
    --*/
	virtual void InitializeWnd();
};
}

#endif  //_K_POPUP_TIP_WND_H_
