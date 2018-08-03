#include "kpch.h"
#include "kbuttonui.h"
#include <CommCtrl.h>

namespace TinyDui 
{
KButtonUI::KButtonUI()
	: m_bHover(false)
	, m_bDefaultCtrl(false)
	, m_bDown(false)
	, m_bMove(false)
	, m_clrBorder(RGB(0xFF, 0xFF, 0xFF))
	, m_clrBackGround(RGB(0xFF, 0xFF, 0xFF))
	, m_clrHover(RGB(0xFF, 0xFF, 0xFF))
	, m_clrSelHover(RGB(0xFF, 0xFF, 0xFF))
	, m_clrSelBackGround(RGB(0xFF, 0xFF, 0xFF))
	, m_clrDownBackGround(RGB(0xFF, 0xFF, 0xFF))
	, m_clrTextSelect(RGB(0, 0, 0))
	, m_nBorderWidth(1)
	, m_bShowToolTip(false)
	, m_bAlreadyShowTip(false)
{
	::memset(&m_ptDown, 0, sizeof(m_ptDown));
	m_strToolTip = _T("");
}

KButtonUI::~KButtonUI()
{

}

bool KButtonUI::IsDefaultCtrl()
{
	return m_bDefaultCtrl;
}

void KButtonUI::SetBorderColor(COLORREF clrBorder)
{
	m_clrBorder = clrBorder;
	ReDrawCatcher();
}

void KButtonUI::SetBackGroundColor(COLORREF clrBackGround)
{
	m_clrBackGround = clrBackGround;
	ReDrawCatcher();
}

void KButtonUI::SetHoverColor(COLORREF clrHover)
{
	m_clrHover = clrHover;
}

void KButtonUI::SetSelectHoverColor(COLORREF clrSelHover)
{
	m_clrSelHover = clrSelHover;
}

void KButtonUI::SetSelectColor(COLORREF clrSelBackGround)
{
	m_clrSelBackGround = clrSelBackGround;
	ReDrawCatcher();
}

void KButtonUI::SetBackGroundDownColor(COLORREF clrDownBackGround)
{
	m_clrDownBackGround = clrDownBackGround;
	ReDrawCatcher();
}

void KButtonUI::SetTextSelectColor(COLORREF clrTextSelect)
{
	m_clrTextSelect = clrTextSelect;
	ReDrawCatcher();
}

void KButtonUI::SetBorderWidth(int nBorderWidth)
{
	m_nBorderWidth = nBorderWidth;
	ReDrawCatcher();
}

int KButtonUI::GetBorderWidth() const
{
	 return m_nBorderWidth;
}

void KButtonUI::Draw(HDC hMemDC)
{
	DrawBK(hMemDC);
	DrawText(hMemDC);
}

void KButtonUI::DrawBK(HDC hMemDC)
{
	HPEN hPen = ::CreatePen(PS_SOLID, m_nBorderWidth, m_clrBorder);
	HPEN oldPen = (HPEN)::SelectObject(hMemDC, hPen);

	COLORREF bkClr = m_clrBackGround;
	if (m_bHover)
	{
		if (m_bDown)
		{
			bkClr = m_clrDownBackGround;
		}
		else
		{
			bkClr = m_bDefaultCtrl ? m_clrSelHover : m_clrHover;
		}
	}
	else
	{
		if (m_bDefaultCtrl)
		{
			bkClr = m_clrSelBackGround;
		}
	}

	HBRUSH bsh = ::CreateSolidBrush(bkClr);
	HBRUSH oldBrush = (HBRUSH)::SelectObject(hMemDC, bsh);

	::Rectangle(
		hMemDC,
		m_rcCtrl.left,
		m_rcCtrl.top,
		m_rcCtrl.right,
		m_rcCtrl.bottom
		);

	::SelectObject(hMemDC, oldBrush);
	::DeleteObject(bsh);


	::SelectObject(hMemDC, oldPen);
	::DeleteObject(hPen);
}

void KButtonUI::DrawText(HDC hMemDC)
{
	COLORREF oldTxtClr = ::GetTextColor(hMemDC);
	COLORREF clrTxt =  
		m_bDefaultCtrl ? m_clrTextSelect : m_txtClr;
	::SetTextColor(hMemDC, clrTxt);
	::DrawText(
		hMemDC,
		m_strText.c_str(),
		(int)m_strText.length(),
		&m_rcCtrl,
		m_dwTextAlign
		);
	::SetTextColor(hMemDC, oldTxtClr);
}

void KButtonUI::Hover()
{
	m_bHover = true;
	if (m_bShowToolTip && !m_bAlreadyShowTip)
	{
		ShowToolTip();
		m_bAlreadyShowTip = true;
	}

	/*
	if (m_bShowToolTip)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
	}
	*/
}

void KButtonUI::Leave()
{
	m_bHover = false;
	m_bAlreadyShowTip = false;
}

bool KButtonUI::IsHover()
{
	return m_bHover;
}

void KButtonUI::Click()
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
}

void KButtonUI::OnLButtonDown(const POINT &pt)
{
	m_bDown = true;
	m_bMove = false;
	m_ptDown = pt;
}

void KButtonUI::OnMouseMove(const POINT &pt)
{
	if (abs(pt.x - m_ptDown.x) + abs(pt.y - m_ptDown.y) >= 2)
	{
		m_bMove = true;
	}
}

void KButtonUI::OnLButtonUp(const POINT &pt)
{
	if (m_bDown)
	{
		m_bDown = false;
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

void KButtonUI::SetDefault(bool bDefalut)
{
	m_bDefaultCtrl = bDefalut;
}

bool KButtonUI::IsEnableDefault()
{
	return true;
}

void KButtonUI::ShowToolTip()
{
	HWND hWndHandle = m_toolTipWnd.GetWindowHandle();
	if (!hWndHandle)
	{
		return;
	}

	::SendMessage(hWndHandle,
		TTM_ACTIVATE,
		FALSE,
		NULL
	);

	::SendMessageW(hWndHandle,
		TTM_ACTIVATE,
		TRUE,
		NULL
	);
}

void KButtonUI::SetToolTipText(const std::wstring& strToolTip)
{
	if (!strToolTip.c_str())
	{
		return;
	}

	m_strToolTip = strToolTip;
	if (!m_toolTipWnd.GetWindowHandle())
	{
		HWND hWnd = m_hCtrlWndHandle ? m_hCtrlWndHandle : m_hOwnerWndHandle;
		if (hWnd)
		{
			return;
		}

		BOOL bResult = m_toolTipWnd.CreateWnd(
			hWnd, 
			TOOLTIPS_CLASSW,// L"tooltips_class32"
			L"",
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | 
			WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			WS_EX_TOPMOST,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_lfDpi
		);
		if (!bResult)
		{
			return;
		}

		TOOLINFOW stToolInfo = { 0 };
		stToolInfo.cbSize = sizeof(TOOLINFOW);
		stToolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		stToolInfo.hinst = GetModuleHandle(NULL);
		stToolInfo.lpszText = (LPWSTR)m_strToolTip.c_str();
		stToolInfo.uId = (UINT_PTR)hWnd;
		stToolInfo.hwnd = hWnd;

		::SendMessage(
			m_toolTipWnd.GetWindowHandle(),
			TTM_ADDTOOLW,
			FALSE,
			(LPARAM)(LPTOOLINFOW)&stToolInfo
		);

		::SendMessage(
			m_toolTipWnd.GetWindowHandle(),
			TTM_ACTIVATE,
			FALSE,
			NULL
		);
	}
}

std::wstring KButtonUI::GetToolTipText() const
{
	return m_strToolTip;
}

void KButtonUI::SetShowToolTip(bool bShowToolTip)
{
	m_bShowToolTip = bShowToolTip;
}

bool KButtonUI::GetShowToolTip() const
{
	return m_bShowToolTip;
}

KImageButtonUI::KImageButtonUI()
	: m_pHoverImage(nullptr)
	, m_nImageTextSpace(4)
{

}

KImageButtonUI::~KImageButtonUI()
{
	 SAFE_FREE(m_pHoverImage);
}

void KImageButtonUI::SetNormalImage(Gdiplus::Image *pNormalImage)
{
	 SetBackGroundImage(pNormalImage);
}

void KImageButtonUI::SetHoverImage(Gdiplus::Image *pHoverImage)
{
	 SAFE_FREE(m_pHoverImage);
	 m_pHoverImage = pHoverImage;
}

void KImageButtonUI::SetImageTextSpace(int nImageTextSpace)
{
	m_nImageTextSpace = nImageTextSpace;
	ReDrawCatcher();
}

int KImageButtonUI::GetImageTextSpace() const
{
	return m_nImageTextSpace;
}

void KImageButtonUI::Draw(HDC hMemDC)
{
	// 居中绘制图片 + 文字
	__super::DrawBK(hMemDC);
	RECT rcDraw = m_rcCtrl;
	DrawText(hMemDC, rcDraw);
	DrawImage(hMemDC, rcDraw);
}

void KImageButtonUI::DrawImage(HDC hMemDC, RECT& rcDraw)
{
	if (!hMemDC)
	{
		return;
	}

	// 若只有图片，则铺满绘制区域，否则居中绘制Image图片
	Gdiplus::Image *pImage = 
		m_bHover ? m_pHoverImage : m_pBackGroundImage;
	if (m_bHover && !pImage)
	{
		pImage = m_pBackGroundImage;
	}

	if (pImage)
	{
		int nTop = rcDraw.top;
		int nBottom = rcDraw.bottom;
		int nImageHeight = nBottom - nTop;
		int nImageWidth = rcDraw.right - rcDraw.left;

		// 若有文本，则只绘制Image合适大小区域即可,否则铺满区域
		SIZE szText = GetTextExtentPointSize(m_strText, hMemDC);
		if (szText.cx > 0)
		{
			nImageHeight = (int)(pImage->GetHeight() * m_lfDpi);
			nImageWidth = (int)(pImage->GetWidth() * m_lfDpi);
			nTop = rcDraw.top + 
				(abs(nBottom - nTop) - nImageHeight) / 2;
		}
		
		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			pImage,
			rcDraw.left,
			nTop,
			nImageWidth,
			nImageHeight
			);
	}
}

void KImageButtonUI::DrawText(HDC hMemDC, RECT& rcDraw)
{
	if (!hMemDC)
	{
		return;
	}

	SIZE szText = GetTextExtentPointSize(m_strText, hMemDC);
	if (!szText.cx)
	{
		return;
	}

	int nImageWidth = GetImageWidth();
	int nRedundancySpace = (rcDraw.right - rcDraw.left) -
		(szText.cx + nImageWidth);
	if (nImageWidth)
	{
	   // 增加图片+文字间的间隔
	   nRedundancySpace -= (int)(m_nImageTextSpace * m_lfDpi);
	   nRedundancySpace = (int)(nRedundancySpace / 2.0);

	   RECT rcText = rcDraw;
	   rcText.right -= nRedundancySpace;
	   rcText.left = rcText.right - szText.cx;

	   COLORREF oldTxtClr = ::GetTextColor(hMemDC);
	   COLORREF clrTxt =  
		   m_bDefaultCtrl ? m_clrTextSelect : m_txtClr;
	   ::SetTextColor(hMemDC, clrTxt);
	   ::DrawText(
		   hMemDC,
		   m_strText.c_str(),
		   (int)m_strText.length(),
		   &rcText,
		   DT_SINGLELINE | DT_VCENTER | DT_CENTER
		   );
	   ::SetTextColor(hMemDC, oldTxtClr);

	   // 修改区域供Image绘制
	   rcDraw.left += nRedundancySpace;
	   rcDraw.right = rcDraw.left + nImageWidth;
	}
}

int KImageButtonUI::GetImageWidth()
{
	int nImageWidth = 0;
	Gdiplus::Image *pImage = 
		m_bHover ? m_pHoverImage : m_pBackGroundImage;
	if (pImage)
	{
		nImageWidth = (int)(pImage->GetWidth() * m_lfDpi);
	}
	return nImageWidth;
}

}