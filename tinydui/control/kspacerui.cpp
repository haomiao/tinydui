#include "kpch.h"
#include "kspacerui.h"

namespace TinyDui 
{
KSpacerUI::KSpacerUI()
	: m_pSpacerImage(nullptr)
	, m_spacerType(SPACER_VERTICAL)
	, m_nLineWidth(1)
	, m_crLine(RGB(255, 255, 255))
{

}

KSpacerUI::~KSpacerUI()
{
	SAFE_FREE(m_pSpacerImage);
}

void KSpacerUI::SetSpacerType(SPACER_TYPE eSpacerType) 
{
	m_spacerType = eSpacerType;
	ReDrawCatcher();
}

KSpacerUI::SPACER_TYPE KSpacerUI::GetSpacerType()
{
	return m_spacerType;
}

void KSpacerUI::SetSpacerLineWidth(int nLineWidth)
{
	m_nLineWidth = nLineWidth;
	ReDrawCatcher();
}

int KSpacerUI::GetSpacerLineWidth()
{
	return m_nLineWidth;
}

void KSpacerUI::SetSpacerLineColor(COLORREF crLine)
{
	m_crLine = crLine;
	ReDrawCatcher();
}

COLORREF KSpacerUI::GetSpacerLineColor()
{
	return m_crLine;
}

void KSpacerUI::SetSpacerImage(Gdiplus::Image *pImage)
{
	SAFE_FREE(m_pSpacerImage);
	m_pSpacerImage = pImage;
	ReDrawCatcher();
}

void KSpacerUI::Draw(HDC hMemDC)
{
	if (m_pSpacerImage)
	{
		int nImageWidth = (int)(m_pSpacerImage->GetWidth() * m_lfDpi);
		int nImageHeight = (int)(m_pSpacerImage->GetHeight() * m_lfDpi);

		int nTop = m_rcCtrl.top + 
			((m_rcCtrl.bottom - m_rcCtrl.top) - nImageHeight) / 2;
		if (nTop < 0)
		{
			nTop = 0;
		}

		int nLeft = m_rcCtrl.left + 
			((m_rcCtrl.right - m_rcCtrl.left) - nImageWidth) / 2;
		if (nLeft < 0)
		{
			nLeft = 0;
		}

		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			m_pSpacerImage,
			nLeft,
			nTop,
			nImageWidth,
			nImageHeight
			);
	}
	else
	{
		POINT start, end;
		if (m_spacerType == SPACER_HORIZONTAL)
		{
			start.x = m_rcCtrl.left;
			start.y = m_rcCtrl.top + (m_rcCtrl.bottom - m_rcCtrl.top) / 2;
			end.x = m_rcCtrl.right;
			end.y = start.y;
		} 
		else if(m_spacerType == SPACER_VERTICAL)
		{
			start.x = m_rcCtrl.left + (m_rcCtrl.right - m_rcCtrl.left) / 2;
			start.y = m_rcCtrl.top;
			end.x = start.x;
			end.y = m_rcCtrl.bottom;
		}

		DrawSpacerLine(hMemDC, start, end);
	}
}

void KSpacerUI::DrawSpacerLine(HDC hMemDC, const POINT& start, const POINT& end)
{
	HPEN hPen = (HPEN)::CreatePen(PS_SOLID, m_nLineWidth, m_crLine);
	HPEN oldPen = (HPEN)::SelectObject(hMemDC, hPen);
	::MoveToEx(hMemDC, start.x, start.y, 0);
	::LineTo(hMemDC, end.x, end.y);
	::SelectObject(hMemDC, oldPen);
	::DeleteObject(hPen);
}
}
