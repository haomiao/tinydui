#include "kpch.h"
#include "khypertextui.h"

namespace TinyDui 
{
KHyperTextUI::KHyperTextUI()
	: m_bHover(false)
	, m_bDown(false)
	, m_bMove(false)
	, m_bClicked(false)
	, m_bDrawLine(true)
	, m_bRecordTag(true)
	, m_clrTextHover(RGB(0, 0, 255))
	, m_clrClicked(RGB(225, 0, 0))
	, m_clrClickedHover(RGB(255, 0, 0))
{
	::memset(&m_ptDown, 0, sizeof(m_ptDown));
}

KHyperTextUI::~KHyperTextUI()
{

}

void KHyperTextUI::Draw(HDC hMemDC)
{
	HPEN hPen = NULL;
	COLORREF oldTxtClr = ::GetTextColor(hMemDC);

	COLORREF txtClr = m_txtClr;
	if (m_bClicked && m_bRecordTag)
	{
		txtClr = m_clrClicked;
		if (m_bHover)
		{
			txtClr = m_clrClickedHover;
		}
	}
	else
	{
		if (m_bHover)
		{
			txtClr = m_clrTextHover;
		}
	}
	
	::SetTextColor(hMemDC, txtClr);
	hPen = ::CreatePen(PS_SOLID, 1, txtClr);

	RECT rcDraw = m_rcCtrl;
	LONG nSpaceLine = (LONG)(3 * m_lfDpi);// add space for underline;
	if (m_bDrawLine)
	{
		rcDraw.bottom -= nSpaceLine;
	}

	::DrawText(
		hMemDC,
		m_strText.c_str(),
		(int)m_strText.length(),
		&m_rcCtrl,
		DT_SINGLELINE | DT_VCENTER
		);
	::SetTextColor(hMemDC, oldTxtClr);

	//画下划线
	if (m_bDrawLine)
	{
		HPEN oldPen = (HPEN)::SelectObject(hMemDC, hPen);
		SIZE szExtent = GetTextExtentPointSize(m_strText, hMemDC);
		RECT rc = {0, 0, szExtent.cx, szExtent.cy};
		// 采用DrawText的DT_CALCRECT参数来获取绘制区域并绘制下划线
		::DrawText(
			hMemDC,
			m_strText.c_str(),
			(int)m_strText.length(),
			&rc,
			DT_CALCRECT | DT_SINGLELINE | DT_VCENTER
			);
		::MoveToEx(
			hMemDC,
			m_rcCtrl.left,
			m_rcCtrl.top + (rc.bottom - rc.top) + nSpaceLine,
			0
			);
		::LineTo(
			hMemDC,
			m_rcCtrl.left + (rc.right - rc.left),
			m_rcCtrl.top + (rc.bottom - rc.top) + nSpaceLine
			);

		::SelectObject(hMemDC, oldPen);
	}
	
	::DeleteObject(hPen);
}

bool KHyperTextUI::Hit(const POINT& pt) const
{
	return ::PtInRect(&m_rcCtrl, pt) == TRUE;
}

void KHyperTextUI::Hover()
{
	m_bHover = true;
}

void KHyperTextUI::Leave()
{
	m_bHover = false;
}

bool KHyperTextUI::IsHover()
{
	return m_bHover;
}

void KHyperTextUI::Click()
{
	if (m_pHandlerCallBack)
	{
		NotifyUIMsg uiMsg;
		uiMsg.dwTimestamp = ::GetTickCount();
		uiMsg.nMsgType = UI_MSG_CLICK;
		uiMsg.pSender = this;
		uiMsg.ptMouse = GetCursorPt();
		uiMsg.wParam = 0;
		uiMsg.lParam = 0;
		m_pHandlerCallBack->UINotify(uiMsg);
	}
	m_bClicked = true;
}

void KHyperTextUI::OnLButtonDown(const POINT &pt)
{
	m_bDown = true;
	m_ptDown = pt;
}

void KHyperTextUI::OnMouseMove(const POINT &pt)
{
	if (abs(pt.x - m_ptDown.x) + abs(pt.y - m_ptDown.y) >= 2)
	{
		m_bMove = true;
	}
}

void KHyperTextUI::OnLButtonUp(const POINT &pt)
{
	if (m_bDown)
	{
		if (m_bMove)
		{
			m_bMove = false;
		}
		else
		{
			Click();
		}
	}
}

void KHyperTextUI::SetHoverColor(COLORREF hClr)
{
	m_clrTextHover = hClr;
}

COLORREF KHyperTextUI::GetHoverColor() const
{
   return m_clrTextHover;
}

void KHyperTextUI::SetClickedColor(COLORREF hClr)
{
	m_clrClicked = hClr;
	ReDrawCatcher();
}

COLORREF KHyperTextUI::GetClickedColor() const
{
	return m_clrClicked;
}

void KHyperTextUI::SetClickedHoverColor(COLORREF hClr)
{
	m_clrClickedHover = hClr;
}

COLORREF KHyperTextUI::GetClickedHoverColor()
{
	return m_clrClickedHover;
}

void KHyperTextUI::SetDrawUnderLine(bool bDrawLine)
{
	m_bDrawLine = bDrawLine;
	ReDrawCatcher();
}

bool KHyperTextUI::IsDrawUnderLine() const
{
	return m_bDrawLine;
}

void KHyperTextUI::SetClickedRecordTag(bool bRecordTag)
{
	m_bRecordTag = bRecordTag;
}

bool KHyperTextUI::GetClickedRecordTag() const
{
	return m_bRecordTag;
}

}
