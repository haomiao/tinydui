#include "kpch.h"
#include "kimagecheckboxui.h"


namespace TinyDui 
{
KImageCheckBoxUI::KImageCheckBoxUI()
	: m_bChecked(false)
	, m_pCheckedImage(nullptr)
	, m_pTagImage(nullptr)
	, m_pCheckedHoverImage(nullptr)
	, m_pUnCheckedHoverImage(nullptr)
	, m_nCheckImageTextSpace(10)
	, m_nTagImageTextSpace(5)
{
	m_dwTextAlign = DT_SINGLELINE | DT_RIGHT | DT_VCENTER;
}

KImageCheckBoxUI::~KImageCheckBoxUI()
{
	SAFE_FREE(m_pCheckedImage);
	SAFE_FREE(m_pTagImage);
	SAFE_FREE(m_pCheckedHoverImage);
	SAFE_FREE(m_pUnCheckedHoverImage);
}

void KImageCheckBoxUI::SetChecked(bool bChecked)
{
	m_bChecked = bChecked;
	ReDrawCatcher();
}

bool KImageCheckBoxUI::GetChecked() const
{
	return m_bChecked;
}

void KImageCheckBoxUI::AutoCheck()
{
	m_bChecked = !m_bChecked;
}

void KImageCheckBoxUI::SetUnCheckedImage(Gdiplus::Image *pUnCheckedImage)
{
	SetBackGroundImage(pUnCheckedImage);
}

void KImageCheckBoxUI::SetCheckedImage(Gdiplus::Image *pCheckedImage)
{
	SAFE_FREE(m_pCheckedImage);
	m_pCheckedImage = pCheckedImage;
	ReDrawCatcher();
}

void KImageCheckBoxUI::SetUnCheckedHoverImage(Gdiplus::Image *pUnCheckedHoverImage)
{
	SAFE_FREE(m_pUnCheckedHoverImage);
	m_pUnCheckedHoverImage = pUnCheckedHoverImage;
}

void KImageCheckBoxUI::SetCheckedHoverImage(Gdiplus::Image *pCheckedHoverImage)
{
	SAFE_FREE(m_pCheckedHoverImage);
	m_pCheckedHoverImage = pCheckedHoverImage;
}

void KImageCheckBoxUI::SetTagImage(Gdiplus::Image *pTagImage)
{
	SAFE_FREE(m_pTagImage);
	m_pTagImage = pTagImage;
	ReDrawCatcher();
}

void KImageCheckBoxUI::Draw(HDC hMemDC)
{
	DrawCtrl(hMemDC, m_rcCtrl);
}

void KImageCheckBoxUI::DrawCtrl(HDC hMemDC, const RECT& rcDraw)
{
	RECT rcText = rcDraw;

	Gdiplus::Image *pImage = nullptr;
	bool bHover = m_bHover;
	POINT pt = GetCursorPt();
	if (!IsHitCheck(pt, rcDraw))
	{
		bHover = false;
	}

	if (m_bChecked)
	{
		if (bHover)
		{
			pImage = m_pCheckedHoverImage;
		}
		else
		{
			pImage = m_pCheckedImage;
		}
	}
	else
	{
		if (bHover)
		{
			pImage = m_pUnCheckedHoverImage;
		}
		else
		{
			pImage = m_pBackGroundImage;
		}
	}

	if (pImage)
	{
		int nImageWidth = (int)(pImage->GetWidth() * m_lfDpi);
		int nImageHeight = (int)(pImage->GetHeight() * m_lfDpi);

		int nTop = rcDraw.top + 
			((rcDraw.bottom - rcDraw.top) - nImageHeight) / 2;
		if (nTop < 0)
		{
			nTop = 0;
		}

		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			pImage,
			rcDraw.right - nImageWidth,
			nTop, 
			nImageWidth,
			nImageHeight
			);

		rcText.right -= nImageWidth;
		rcText.right -= (LONG)(m_nCheckImageTextSpace * m_lfDpi);
	}

	DrawTitleText(hMemDC, rcText);
}

void KImageCheckBoxUI::SetTagImageTextSpace(int nTagImageTextSpace)
{
	m_nTagImageTextSpace = nTagImageTextSpace;
	ReDrawCatcher();
}

int KImageCheckBoxUI::GetTagImageTextSpace() const
{
	return m_nTagImageTextSpace;
}

void KImageCheckBoxUI::SetCheckImageTextSpace(int nCheckImageTextSpace)
{
	m_nCheckImageTextSpace = nCheckImageTextSpace;
	ReDrawCatcher();
}

int KImageCheckBoxUI::GetCheckImageTextSpace() const
{
	return m_nCheckImageTextSpace;
}

void KImageCheckBoxUI::Click()
{
	Click(m_rcCtrl);
}

void KImageCheckBoxUI::Click(const RECT& rcCtrl)
{
	// 是否为点击check区域
	POINT pt = GetCursorPt();
	if (IsHitCheck(pt, rcCtrl))
	{
		AutoCheck();
		ReDrawSelf(FALSE);

		if (m_pHandlerCallBack)
		{
			NotifyUIMsg uiMsg;
			uiMsg.dwTimestamp = ::GetTickCount();
			uiMsg.nMsgType = UI_MSG_CLICK;
			uiMsg.pSender = this;
			uiMsg.ptMouse = pt;
			uiMsg.wParam = WPARAM(m_bChecked);
			uiMsg.lParam = 0;
			m_pHandlerCallBack->UINotify(uiMsg);
		}
	}
}

bool KImageCheckBoxUI::IsEnableDefault()
{
	return false;
}

bool KImageCheckBoxUI::IsHitCheck(const POINT& pt, const RECT& rcCtrl)
{
	int nImageWidth = 0;
	int nImageHeight = 0;

	Gdiplus::Image *pImage = m_bChecked ? m_pCheckedImage : m_pBackGroundImage;
	if (pImage)
	{
		nImageWidth = (int)(pImage->GetWidth() * m_lfDpi);
		nImageHeight = (int)(pImage->GetHeight() * m_lfDpi);
	}

	RECT rcImage = rcCtrl;
	rcImage.left = rcCtrl.right - nImageWidth;
	return ::PtInRect(&rcImage, pt) == TRUE;
}

void KImageCheckBoxUI::DrawTitleText(HDC hMemDC, const RECT& rcText)
{
	COLORREF oldTxtClr = ::GetTextColor(hMemDC);
	if (m_bDefaultCtrl)
	{
		::SetTextColor(hMemDC, m_clrTextSelect);
	}
	else
	{
		::SetTextColor(hMemDC, m_txtClr);
	}

	::DrawText(
		hMemDC,
		m_strText.c_str(),
		(int)m_strText.length(),
		(LPRECT)&rcText,
		m_dwTextAlign
		);
	::SetTextColor(hMemDC, oldTxtClr);

	DrawImage(hMemDC, rcText);
}

void KImageCheckBoxUI::DrawImage(HDC hMemDC, const RECT& rcImage)
{
	if (m_pTagImage)
	{
		int nImageWidth = (int)(m_pTagImage->GetWidth() * m_lfDpi);
		int nImageHeight = (int)(m_pTagImage->GetHeight() * m_lfDpi);

		int nTop = m_rcCtrl.top + 
			((m_rcCtrl.bottom - m_rcCtrl.top) - nImageHeight) / 2;
		if (nTop < 0)
		{
			nTop = 0;
		}

		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			m_pTagImage,
			rcImage.left,
			nTop,
			nImageWidth,
			nImageHeight
			);
	}
}
}
