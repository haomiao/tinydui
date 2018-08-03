#include "kpch.h"
#include "kcheckboxui.h"

namespace TinyDui 
{
KCheckBoxUI::KCheckBoxUI()
	: m_bChecked(false)
	, m_pCheckedImage(nullptr)
	, m_pCheckedHoverImage(nullptr)
	, m_pUnCheckedHoverImage(nullptr)
	, m_nCheckImageSpace(5)
{
	m_dwTextAlign = DT_SINGLELINE | DT_LEFT | DT_VCENTER;
}

KCheckBoxUI::~KCheckBoxUI()
{
	SAFE_FREE(m_pCheckedImage);
	SAFE_FREE(m_pCheckedHoverImage);
	SAFE_FREE(m_pUnCheckedHoverImage);
}

void KCheckBoxUI::SetChecked(bool bChecked)
{
	m_bChecked = bChecked;
	ReDrawCatcher();
}

bool KCheckBoxUI::GetChecked() const
{
	return m_bChecked;
}

void KCheckBoxUI::AutoCheck()
{
	m_bChecked = !m_bChecked;
}

void KCheckBoxUI::SetCheckImageSpace(int nCheckImageSpace)
{
	m_nCheckImageSpace = nCheckImageSpace;
	ReDrawCatcher();
}

int KCheckBoxUI::GetCheckImageSpace() const
{
	return m_nCheckImageSpace;
}

void KCheckBoxUI::SetUnCheckedImage(Gdiplus::Image *pUnCheckedImage)
{
	SetBackGroundImage(pUnCheckedImage);
}

void KCheckBoxUI::SetCheckedImage(Gdiplus::Image *pCheckedImage)
{
	SAFE_FREE(m_pCheckedImage);
	m_pCheckedImage = pCheckedImage;
	ReDrawCatcher();
}

void KCheckBoxUI::SetUnCheckedHoverImage(Gdiplus::Image *pUnCheckedHoverImage)
{
	SAFE_FREE(m_pUnCheckedHoverImage);
	m_pUnCheckedHoverImage = pUnCheckedHoverImage;
}

void KCheckBoxUI::SetCheckedHoverImage(Gdiplus::Image *pCheckedHoverImage)
{
	SAFE_FREE(m_pCheckedHoverImage);
	m_pCheckedHoverImage = pCheckedHoverImage;
}

void KCheckBoxUI::Draw(HDC hMemDC)
{
	m_rcText = m_rcCtrl;

	Gdiplus::Image *pImage = nullptr;
	if (m_bChecked)
	{
		if (m_bHover)
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
		if (m_bHover)
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

		int nTop = m_rcCtrl.top + 
			((m_rcCtrl.bottom - m_rcCtrl.top) - nImageHeight) / 2;
		if (nTop < 0)
		{
			nTop = 0;
		}

		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			pImage,
			m_rcCtrl.left,
			nTop,
			nImageWidth,
			nImageHeight
			);

		m_rcText.left += nImageWidth;
		m_rcText.left += (LONG)(m_nCheckImageSpace * m_lfDpi);
	}

	DrawText(hMemDC);
}

void KCheckBoxUI::Click()
{
	AutoCheck();
	ReDrawSelf(FALSE);

	if (m_pHandlerCallBack)
	{
		NotifyUIMsg uiMsg;
		uiMsg.dwTimestamp = ::GetTickCount();
		uiMsg.nMsgType = UI_MSG_CLICK;
		uiMsg.pSender = this;
		uiMsg.ptMouse = GetCursorPt();
		uiMsg.wParam = WPARAM(m_bChecked);
		uiMsg.lParam = 0;
		m_pHandlerCallBack->UINotify(uiMsg);
	}
}

bool KCheckBoxUI::IsEnableDefault()
{
	return false;
}

void KCheckBoxUI::DrawText(HDC hMemDC)
{
	COLORREF oldTxtClr = ::GetTextColor(hMemDC);
	COLORREF txtClr = m_bDefaultCtrl ? m_clrTextSelect : m_txtClr;
	::SetTextColor(hMemDC, txtClr);
	::DrawText(
		hMemDC,
		m_strText.c_str(),
		(int)m_strText.length(),
		&m_rcText,
		m_dwTextAlign
		);
	::SetTextColor(hMemDC, oldTxtClr);
}
}
