#include "kpch.h"
#include "windowsx.h"
#include "kctrluibase.h"

namespace TinyDui 
{
KCtrlUIBase::KCtrlUIBase()
	: m_hCtrlWndHandle(NULL)
	, m_hOwnerWndHandle(NULL)
	, m_nCtrlID(INVALID_CTRL_ID)
	, m_bVisible(true)
	, m_bHit(true)
	, m_bDrag(false)
	, m_pHandlerCallBack(nullptr)
{
	::memset(&m_rcCtrl, 0, sizeof(m_rcCtrl));
	m_lfDpi = GetCurrentDpiValue();
}

KCtrlUIBase::~KCtrlUIBase()
{

}

void KCtrlUIBase::SetOwnerWndHandle(HWND hOwnerWndHandle)
{
	m_hOwnerWndHandle = hOwnerWndHandle;
}

HWND KCtrlUIBase::GetOwnerWndHandle() const
{
	return m_hOwnerWndHandle;
}

void KCtrlUIBase::SetCtrlWndHandle(HWND hCtrlWndHandle)
{
	m_hCtrlWndHandle = hCtrlWndHandle;
}

HWND KCtrlUIBase::GetCtrlWndHandle() const
{
	return m_hCtrlWndHandle;
}

void KCtrlUIBase::SetRect(const RECT& rc)
{
	m_rcCtrl = rc;
}

void KCtrlUIBase::SetRect(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcCtrl.left = nLeft;
	m_rcCtrl.top = nTop;
	m_rcCtrl.right = nRight;
	m_rcCtrl.bottom = nBottom;
}

RECT KCtrlUIBase::GetRect() const
{
	return m_rcCtrl;
}

bool KCtrlUIBase::IsCtrlIDValid() const
{
	return m_nCtrlID == INVALID_CTRL_ID;
}

void KCtrlUIBase::SetCtrlID(int nID)
{
	m_nCtrlID = nID;
}

int KCtrlUIBase::GetCtrlID() const
{
	return m_nCtrlID;
}

void KCtrlUIBase::SetUIHandler(KUIHandlerCallBack* pHandlerCallBack)
{
	if (pHandlerCallBack)
	{
		m_pHandlerCallBack = pHandlerCallBack;
	}
}

KUIHandlerCallBack* KCtrlUIBase::GetUIHandler() const
{
	return m_pHandlerCallBack;
}

bool KCtrlUIBase::IsVisible() const
{
	return m_bVisible;
}

void KCtrlUIBase::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
	ReDraw(nullptr, TRUE);
}

bool KCtrlUIBase::GetHit() const
{
	return m_bHit;
}

void KCtrlUIBase::SetHit(bool bHit)
{
	m_bHit = bHit;
}

bool KCtrlUIBase::IsDrag() const
{
	return m_bDrag;
}

void KCtrlUIBase::SetDrag(bool bDrag)
{
	m_bDrag = bDrag;
}

bool KCtrlUIBase::Hit(const POINT& pt) const
{
	if (!m_bVisible || (!m_bHit))
	{
		return false;
	}

	return ::PtInRect(&m_rcCtrl, pt) == TRUE;
}

bool KCtrlUIBase::IsSupportAutoAdjustRect() const
{
	return false;
}

RECT KCtrlUIBase::GetExpectantRect() const
{
	return m_rcCtrl;
}

void KCtrlUIBase::DelayDispatch(DWORD dwDelayMs)
{
	if (!(m_hOwnerWndHandle && ::IsWindow(m_hOwnerWndHandle)))
	{
		return;
	}

	DWORD dwTimestamp = ::GetTickCount();
	MSG msg = { 0 };
	while (::IsWindow(m_hOwnerWndHandle) && ::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

		if (::GetTickCount() - dwTimestamp > dwDelayMs)
		{
			break;
		}
	}
}

void KCtrlUIBase::DisableTopWindow()
{
	if (m_hOwnerWndHandle && ::IsWindow(m_hOwnerWndHandle))
	{
		DelayDispatch(200);
		HWND hWndParent = ::GetWindowOwner(m_hOwnerWndHandle);
		::EnableWindow(hWndParent, FALSE);
	}
}

void KCtrlUIBase::EnableTopWindow()
{
	if (m_hOwnerWndHandle && ::IsWindow(m_hOwnerWndHandle))
	{
		HWND hWndParent = ::GetWindowOwner(m_hOwnerWndHandle);
		::EnableWindow(hWndParent, TRUE);
	}
}

SIZE KCtrlUIBase::GetTextExtentPointSize(const std::wstring& strText) const
{	
	SIZE szExtent = {0};
	HWND hWnd =
		m_hCtrlWndHandle ? m_hCtrlWndHandle : m_hOwnerWndHandle;
	if (!hWnd)
	{
		hWnd = ::GetDesktopWindow();
	}

	if (!hWnd || !strText.length())
	{
		return szExtent;
	}

	HDC hDC = ::GetDC(hWnd);
	szExtent = GetTextExtentPointSize(strText, hDC);
	::ReleaseDC(hWnd, hDC);
	return szExtent;
}

SIZE KCtrlUIBase::GetTextExtentPointSize(const std::wstring& strText, HDC hDC)
{
	SIZE szExtent = {0};
	if (!hDC || !strText.length())
	{
		return szExtent;
	}

	::GetTextExtentExPoint(
		hDC,
		strText.c_str(),
		(int)strText.length(),
		0,
		NULL,
		NULL,
		&szExtent
	);
	return szExtent;
}

RECT KCtrlUIBase::GetDrawTextRect(const std::wstring& strText, UINT nFormat) const
{
	RECT rcText = {0};
	HWND hWnd =
		m_hCtrlWndHandle ? m_hCtrlWndHandle : m_hOwnerWndHandle;
	if (!hWnd)
	{
		hWnd = ::GetDesktopWindow();
	}

	if (!hWnd || !strText.length())
	{
		return rcText;
	}

	HDC hDC = ::GetDC(hWnd);
	rcText = GetDrawTextRect(strText, hDC, nFormat);
	::ReleaseDC(hWnd, hDC);
	return rcText;
}

RECT KCtrlUIBase::GetDrawTextRect(const std::wstring& strText, HDC hDC, UINT nFormat)
{
	RECT rcText = {0};
	if (!hDC || !strText.length())
	{
		return rcText;
	}

	RECT rc = {0};
	::DrawText(
		hDC,
		strText.c_str(),
		(int)strText.length(),
		&rc,
		DT_CALCRECT | nFormat
	);
	return rc;
}

void KCtrlUIBase::ReDrawSelf(BOOL bErase)
{
	ReDraw(&m_rcCtrl, bErase);
}

void KCtrlUIBase::ReDraw(const RECT* lpRcDraw, BOOL bErase)
{
	HWND hWnd =
		m_hCtrlWndHandle ? m_hCtrlWndHandle : m_hOwnerWndHandle;
	if (!hWnd)
	{
		return;
	}

	::InvalidateRect(hWnd, lpRcDraw, bErase);
}

void KCtrlUIBase::ReDrawCatcher()
{
	if (IsSupportAutoAdjustRect())
	{
		RECT rcUI = GetExpectantRect();
		if (::memcmp(&rcUI, &m_rcCtrl, sizeof(rcUI)) != 0)
		{
			ReDraw(nullptr, TRUE);
		}
		else
		{
			ReDrawSelf();
		}
	}
	else
	{
		ReDrawSelf();
	}
}

POINT KCtrlUIBase::GetCursorPt()
{
	POINT pt = {0};
	::GetCursorPos(&pt);
	HWND hWnd = 
		m_hCtrlWndHandle ? m_hCtrlWndHandle : m_hOwnerWndHandle;
	if (!hWnd)
	{
		return pt;
	}

	::ScreenToClient(hWnd, &pt);
	return pt;
}

RECT KCtrlUIBase::GetDPIRect(const RECT& rc) const
{
	return GetDPIRect(rc, m_lfDpi);
}

RECT KCtrlUIBase::GetDPIRect(const RECT& rc, double lfDPI)
{
	// 以左上角(0, 0)作为伸缩目标即可
	RECT rcNew = rc;
	rcNew.left = (LONG)(rcNew.left * lfDPI);
	rcNew.top = (LONG)(rcNew.top * lfDPI);
	rcNew.right = (LONG)(rcNew.right * lfDPI);
	rcNew.bottom = (LONG)(rcNew.bottom * lfDPI);
	return rcNew;
}

POINT KCtrlUIBase::GetDPIPoint(const POINT& pt) const
{
	return GetDPIPoint(pt, m_lfDpi);
}

POINT KCtrlUIBase::GetDPIPoint(const POINT& pt, double lfDPI)
{
	// 以左上角(0, 0)作为伸缩目标即可
	POINT ptNew = pt;
	ptNew.x = (LONG)(pt.x * lfDPI);
	ptNew.y = (LONG)(pt.y * lfDPI);
	return ptNew;
}

SIZE KCtrlUIBase::GetDPISize(const SIZE& sz) const
{
	return GetDPISize(sz, m_lfDpi);
}

SIZE KCtrlUIBase::GetDPISize(const SIZE& sz, double lfDPI)
{
	// 直接缩放
	SIZE szNew = sz;
	szNew.cx = (LONG)(sz.cx * lfDPI);
	szNew.cy = (LONG)(sz.cy * lfDPI);
	return szNew;
}

bool KCtrlUIBase::IsEnableDefault() const
{
	return false;
}

}