#include "kpch.h"
#include "kspringcheckboxui.h"

namespace TinyDui 
{
KSpringCheckBoxUI::KSpringCheckBoxUI()
	: m_pExpandImage(nullptr)
	, m_pShrinkImage(nullptr)
	, m_bExpand(true)
	, m_nSpringSpace(5)
	, m_clrHoverSpringBackGround(RGB(205, 205, 205))
{
	::memset(&m_rcOrgin, 0, sizeof(RECT));
}

KSpringCheckBoxUI::~KSpringCheckBoxUI()
{
	SAFE_FREE(m_pExpandImage);
	SAFE_FREE(m_pShrinkImage);
}

void KSpringCheckBoxUI::SetExpandImage(Gdiplus::Image *pExpandImage)
{
	SAFE_FREE(m_pExpandImage);
	m_pExpandImage = pExpandImage;
	ReDrawCatcher();
}

void KSpringCheckBoxUI::SetShrinkImage(Gdiplus::Image *pShrinkImage)
{
	SAFE_FREE(m_pShrinkImage);
	m_pShrinkImage = pShrinkImage;
	ReDrawCatcher();
}

void KSpringCheckBoxUI::SetSpringHoverBackGroundColor(
	COLORREF clrHoverSpringBackGround
	)
{
	m_clrHoverSpringBackGround = clrHoverSpringBackGround;
}

COLORREF KSpringCheckBoxUI::GetSpringHoverBackGroundColor() const
{
	return m_clrHoverSpringBackGround;
}

void KSpringCheckBoxUI::SetSpringSpace(int nSpringSpace)
{
	m_nSpringSpace = nSpringSpace;
	ReDrawCatcher();
}

int KSpringCheckBoxUI::GetSpringSpace() const
{
	return m_nSpringSpace;
}

void KSpringCheckBoxUI::Draw(HDC hMemDC)
{
	RECT rcImage = m_rcCtrl;
	if (m_bExpand)
	{
		RECT rcCtrl = m_rcCtrl;
		int nWidth = GetCurImageWidth(m_bExpand);
		rcCtrl.right -= (LONG)(nWidth + m_nSpringSpace * m_lfDpi); // add space
		__super::DrawCtrl(hMemDC, rcCtrl);
		rcImage.left = rcImage.right - nWidth;
	}

	DrawImage(hMemDC, rcImage);
}

void KSpringCheckBoxUI::Click()
{
	RECT rcCtrl = {0};
	if (::memcmp(&m_rcOrgin, &rcCtrl, sizeof(RECT)) == 0
		&& m_bExpand)
	{
		m_rcOrgin = m_rcCtrl;
	}

	// 是否为点击弹簧区域
	POINT pt = GetCursorPt();
	if (IsHitSpring(pt))
	{
		m_bExpand = !m_bExpand;

		if (m_bExpand)
		{
			m_rcCtrl = m_rcOrgin;
		}
		else
		{
			m_rcCtrl.right = 
				m_rcCtrl.left + GetCurImageWidth(m_bExpand);
		}

		// 重绘所有区域
		if (::IsWindow(m_hOwnerWndHandle))
		{
			::InvalidateRect(m_hOwnerWndHandle, NULL, TRUE);
		}

		// 放缩事件
		if (m_pHandlerCallBack)
		{
			NotifyUIMsg uiMsg;
			uiMsg.dwTimestamp = ::GetTickCount();
			uiMsg.nMsgType = UI_MSG_CLICK;
			uiMsg.pSender = this;
			uiMsg.ptMouse = pt;
			uiMsg.wParam = 0;
			uiMsg.lParam = LPARAM(m_bExpand);
			m_pHandlerCallBack->UINotify(uiMsg);
		}
	}
	else
	{
		RECT rcCtrl = m_rcCtrl;
		int nWidth = GetCurImageWidth(m_bExpand);
		rcCtrl.right -= (LONG)(nWidth + m_nSpringSpace * m_lfDpi); // add space
		__super::Click(rcCtrl);
	}
}

bool KSpringCheckBoxUI::IsEnableDefault()
{
	return false;
}

int KSpringCheckBoxUI::GetCurImageWidth(bool bExpand)
{
	int nWidth = 0;
	Gdiplus::Image *pImage = 
		m_bExpand ? m_pShrinkImage : m_pExpandImage;
	if (pImage)
	{
		nWidth = (int)(pImage->GetWidth() * m_lfDpi);
	}
	return nWidth;
}

bool KSpringCheckBoxUI::IsHitSpring(const POINT& pt)
{
	int nImageWidth = 0;
	int nImageHeight = 0;
	Gdiplus::Image *pImage = 
		m_bExpand ? m_pShrinkImage : m_pExpandImage;
	if (pImage)
	{
		nImageWidth = (int)(pImage->GetWidth() * m_lfDpi);
		nImageHeight = (int)(pImage->GetHeight() * m_lfDpi);
	}

	RECT rcImage = m_rcCtrl;
	rcImage.left = m_rcCtrl.right - nImageWidth;
	return ::PtInRect(&rcImage, pt) == TRUE;
}

void KSpringCheckBoxUI::DrawImage(HDC hMemDC, const RECT& rcImage)
{
	Gdiplus::Image *pImage = 
		m_bExpand ? m_pShrinkImage : m_pExpandImage;
	if (pImage)
	{
		int nImageWidth = (int)(pImage->GetWidth() * m_lfDpi);
		int nImageHeight = (int)(pImage->GetHeight() * m_lfDpi);

		if (m_bHover && IsHitSpring(GetCursorPt()))
		{
			HBRUSH hBrush = ::CreateSolidBrush(m_clrHoverSpringBackGround);
			::FillRect(hMemDC, &rcImage, hBrush);
			::DeleteObject(hBrush);
		}

		// 居中绘制放缩图片
		int nTop = rcImage.top + 
			((rcImage.bottom - rcImage.top) - nImageHeight) / 2;
		if (nTop < 0)
		{
			nTop = 0;
		}

		int nLeft = rcImage.left + 
			((rcImage.right - rcImage.left) - nImageWidth) / 2;
		if (nLeft < 0)
		{
			nLeft = 0;
		}

		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			pImage, 
			rcImage.left,
			nTop,
			nImageWidth,
			nImageHeight
			);
	}
}
}

