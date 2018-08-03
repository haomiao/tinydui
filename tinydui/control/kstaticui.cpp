#include "kpch.h"
#include "kstaticui.h"

namespace TinyDui 
{
KStaticUI::KStaticUI()
	: m_pBackGroundImage(nullptr)
	, m_txtClr(0)
	, m_dwTextAlign(DT_SINGLELINE | DT_VCENTER | DT_CENTER)
{
}

KStaticUI::~KStaticUI()
{
	SAFE_FREE(m_pBackGroundImage);
}

void KStaticUI::SetText(const std::wstring &strText)
{
	m_strText = strText;
	ReDrawCatcher();
}

std::wstring KStaticUI::GetText() const
{
	return m_strText;
}

void KStaticUI::SetBackGroundImage(const std::wstring& strImgFile)
{
	SetBackGroundImage(Gdiplus::Image::FromFile(strImgFile.c_str()));
	ReDrawCatcher();
}

void KStaticUI::SetBackGroundImage(Gdiplus::Image *pBackGroundImage)
{
	SAFE_FREE(m_pBackGroundImage);
	m_pBackGroundImage = pBackGroundImage;
	ReDrawCatcher();
}

void KStaticUI::SetTextColor(COLORREF clrText)
{
	m_txtClr = clrText;
	ReDrawCatcher();
}

COLORREF KStaticUI::GetTextColor() const
{
	return m_txtClr;
}

void KStaticUI::SetTextAlign(DWORD dwTextAlign)
{
	m_dwTextAlign = dwTextAlign;
	ReDrawCatcher();
}

DWORD KStaticUI::GetTextAlign() const
{
	return m_dwTextAlign;
}

void KStaticUI::Draw(HDC hMemDC)
{
	if (m_pBackGroundImage)
	{
		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			m_pBackGroundImage,
			m_rcCtrl.left,
			m_rcCtrl.top,
			m_rcCtrl.right - m_rcCtrl.left,
			m_rcCtrl.bottom - m_rcCtrl.top
			);
	}
	else
	{
		int perLineNum = 0; // 每行字数
		int lineH = 0;
		RECT rc = {0, 0, 10000, 40};
		int showLineW = m_rcCtrl.right - m_rcCtrl.left;
		::DrawText(
			hMemDC, 
			m_strText.c_str(), 
			(int)m_strText.length(), 
			&rc, 
			DT_CALCRECT | DT_SINGLELINE | m_dwTextAlign
			);

		if (rc.right > showLineW)	// 显示多行
		{
			double df = double(rc.right) / double(showLineW);
			perLineNum = int(double(m_strText.length()) / df) - 1;
		}
		else
		{
			perLineNum = (int)m_strText.length(); // 处理单行显示
		}

		lineH = rc.bottom - rc.top + 4;
		if (perLineNum > 0)
		{
			int txt_size = int(m_strText.length());
			int plus = txt_size % perLineNum == 0 ? 0 : 1;
			int lineNum = (txt_size / perLineNum + plus);
			const wchar_t *pstr = m_strText.c_str();

			COLORREF oldTxtClr = ::GetTextColor(hMemDC);
			::SetTextColor(hMemDC, m_txtClr);
			
			if (lineNum == 1)
			{
				::DrawText(
					hMemDC,
					pstr, 
					(int)m_strText.length(), 
					&m_rcCtrl, 
					DT_SINGLELINE | DT_VCENTER
					);
			}
			else
			{
				for (int i = 0; i < lineNum; ++i)
				{
					rc = m_rcCtrl;
					rc.top = m_rcCtrl.top + lineH * i;

					if ((lineNum - 1) == i)
					{
						int surplus = txt_size - i * perLineNum;
						::DrawText(
							hMemDC,
							pstr + i * perLineNum, 
							(int)surplus, 
							&rc, 
							DT_SINGLELINE | DT_TOP | m_dwTextAlign
							);
					}
					else
					{
						::DrawText(
							hMemDC, 
							pstr + i * perLineNum, 
							(int)perLineNum, 
							&rc, 
							DT_SINGLELINE | DT_TOP | m_dwTextAlign
							);
					}
				}
			}
			
			::SetTextColor(hMemDC, oldTxtClr);
		}
	}
}

void KStaticUI::Hover()
{

}

void KStaticUI::Leave()
{

}

bool KStaticUI::IsHover()
{
	return false;
}

void KStaticUI::OnSize()
{

}

void KStaticUI::OnLButtonDown(const POINT &pt)
{

}

void KStaticUI::OnMouseMove(const POINT &pt)
{

}

void KStaticUI::OnLButtonUp(const POINT &pt)
{

}

void KStaticUI::Click()
{

}

KStaticBackGroundUI::KStaticBackGroundUI()
	: m_clrBackGround(RGB(0xFF, 0xFF, 0xFF))
	, m_clrUnderLine(RGB(0xFF, 0xFF, 0xFF))
	, m_bDown(false)
	, m_bMove(false)
	, m_bUnderLine(true)
	, m_nUnderLineWidth(1)
{
	::memset(&m_ptDown, 0, sizeof(m_ptDown));
}

KStaticBackGroundUI::~KStaticBackGroundUI()
{

}

void KStaticBackGroundUI::SetBackGroundColor(COLORREF clrBackGround)
{
	m_clrBackGround = clrBackGround;
	ReDrawCatcher();
}

COLORREF KStaticBackGroundUI::GetBackGroundColor() const
{
	return m_clrBackGround;
}

void KStaticBackGroundUI::SetUnderLine(bool bUnderLine)
{
	m_bUnderLine = bUnderLine;
	ReDrawCatcher();
}

bool KStaticBackGroundUI::IsUnderLine() const
{
	return m_bUnderLine;
}

void KStaticBackGroundUI::SetUnderLineWidth(int nUnderLineWidth)
{
	m_nUnderLineWidth = nUnderLineWidth;
	ReDrawCatcher();
}

int KStaticBackGroundUI::GetLineWidth() const
{
	return m_nUnderLineWidth;
}

void KStaticBackGroundUI::SetUnderLineColor(COLORREF clrUnderLine)
{
	m_clrUnderLine = clrUnderLine;
	ReDrawCatcher();
}

COLORREF KStaticBackGroundUI::GetUnderLineColor() const
{
	return m_clrUnderLine;
}

void KStaticBackGroundUI::Draw(HDC hMemDC)
{	
	if (m_bUnderLine)
	{
		DrawRect(hMemDC);
		DrawLine(hMemDC);
		__super::Draw(hMemDC);
	}
}

void KStaticBackGroundUI::DrawRect(HDC hMemDC)
{
	HPEN hPen = (HPEN)::GetStockObject(NULL_PEN);
	HPEN oldPen = (HPEN)::SelectObject(hMemDC, hPen);
	HBRUSH bsh = ::CreateSolidBrush(m_clrBackGround);
	::FillRect(hMemDC, &m_rcCtrl, bsh);
	::DeleteObject(bsh);
	::SelectObject(hMemDC, oldPen);
}

void KStaticBackGroundUI::DrawLine(HDC hMemDC)
{
	HPEN hPen = (HPEN)::CreatePen(PS_SOLID, m_nUnderLineWidth, m_clrUnderLine);
	HPEN oldPen = (HPEN)::SelectObject(hMemDC, hPen);
	::MoveToEx(hMemDC, m_rcCtrl.left, m_rcCtrl.bottom, 0);
	::LineTo(hMemDC, m_rcCtrl.right, m_rcCtrl.bottom);
	::SelectObject(hMemDC, oldPen);
	::DeleteObject(hPen);
}

void KStaticBackGroundUI::OnLButtonDown(const POINT &pt)
{
	m_bDown = true;
	m_ptDown = pt;
}

void KStaticBackGroundUI::OnMouseMove(const POINT &pt)
{
	if (m_bDown)
	{
		if (m_bMove)
		{
			// NC后将收不到LBUTTONUP消息, 所以这里要将变量m_bDown设置为false;
			m_bDown = false;
			if (m_hOwnerWndHandle)
			{
				if (::GetCapture() == m_hOwnerWndHandle)
				{
					::ReleaseCapture();
				}

				::PostMessage(
					m_hOwnerWndHandle, 
					WM_NCLBUTTONDOWN, 
					HTCAPTION, 
					MAKELPARAM(pt.x, pt.y)
					);
			}
		}
		else
		{
			if (abs(pt.x - m_ptDown.x) + abs(pt.y - m_ptDown.y) >= 2)
			{
				m_bMove = true;
			}
		}
	}
}

void KStaticBackGroundUI::OnLButtonUp(const POINT &pt)
{
	if (m_bDown)
	{
		if (m_bMove)
		{
			m_bMove = false;
		}
		m_bDown = false;
	}
}
}
