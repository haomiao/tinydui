#include "kpch.h"
#include "kwndhandleui.h"

namespace TinyDui 
{
KWndHandleUI::KWndHandleUI()
{

}

KWndHandleUI::~KWndHandleUI()
{

}

void KWndHandleUI::SetOwnerWndHandle(HWND hOwnerWndHandle)
{
	m_hOwnerWndHandle = hOwnerWndHandle;
}

HWND KWndHandleUI::GetOwnerWndHandle() const
{
	return m_hOwnerWndHandle;
}

void KWndHandleUI::SetCtrlWndHandle(HWND hCtrlWndHandle)
{
	m_hCtrlWndHandle = hCtrlWndHandle;
}

HWND KWndHandleUI::GetCtrlWndHandle() const
{
	return m_hCtrlWndHandle;
}

void KWndHandleUI::SetRect(const RECT& rc)
{
	m_rcCtrl = rc;
	ReDrawCatcher();
}

void KWndHandleUI::SetRect(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcCtrl.left = nLeft;
	m_rcCtrl.top = nTop;
	m_rcCtrl.right = nRight;
	m_rcCtrl.bottom = nBottom;
	ReDrawCatcher();
}

RECT KWndHandleUI::GetRect() const
{
	return m_rcCtrl;
}

bool KWndHandleUI::IsCtrlIDValid() const
{
	return m_nCtrlID == INVALID_CTRL_ID;
}

void KWndHandleUI::SetCtrlID(int nID)
{
	m_nCtrlID = nID;
}

int KWndHandleUI::GetCtrlID() const
{
	return m_nCtrlID;
}

void KWndHandleUI::SetUIHandler(KUIHandlerCallBack* pHandlerCallBack)
{
	if (pHandlerCallBack)
	{
		m_pHandlerCallBack = pHandlerCallBack;
	}
}

KUIHandlerCallBack* KWndHandleUI::GetUIHandler() const
{
	return m_pHandlerCallBack;
}

bool KWndHandleUI::IsVisible() const
{
	return m_bVisible;
}

void KWndHandleUI::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
	ReDrawCatcher();
}

bool KWndHandleUI::GetHit() const
{
	return m_bHit;
}

void KWndHandleUI::SetHit(bool bHit)
{
	m_bHit = bHit;
}

bool KWndHandleUI::IsDrag() const
{
	return m_bDrag;
}

void KWndHandleUI::SetDrag(bool bDrag)
{
	m_bDrag = bDrag;
}

bool KWndHandleUI::Hit(const POINT& pt) const
{
	if (!m_bVisible || (!m_bHit))
	{
		return false;
	}

	return ::PtInRect(&m_rcCtrl, pt) == TRUE;
}

void KWndHandleUI::DelayDispatch(DWORD dwDelayMs)
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

void KWndHandleUI::DisableTopWindow()
{
	if (m_hOwnerWndHandle && ::IsWindow(m_hOwnerWndHandle))
	{
		DelayDispatch(200);
		HWND hWndParent = ::GetWindowOwner(m_hOwnerWndHandle);
		::EnableWindow(hWndParent, FALSE);
	}
}

void KWndHandleUI::EnableTopWindow()
{
	if (m_hOwnerWndHandle && ::IsWindow(m_hOwnerWndHandle))
	{
		HWND hWndParent = ::GetWindowOwner(m_hOwnerWndHandle);
		::EnableWindow(hWndParent, TRUE);
	}
}

bool KWndHandleUI::IsEnableDefault() const
{
	return false;
}

}