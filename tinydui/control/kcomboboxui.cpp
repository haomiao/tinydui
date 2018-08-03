#include "kpch.h"
#include "kcomboboxui.h"

namespace TinyDui 
{
KComboBoxUI::KComboBoxUI(HWND hOwnerWndHandle)
	: m_pDropDownImage(nullptr)
	, m_nCurSelItem(-1)
	, m_clrDropDownBackGround(RGB(230, 230, 230))
	, m_clrHoverDropDownBackGround(RGB(205, 205, 205))
{
	m_hOwnerWndHandle = hOwnerWndHandle;
	m_dwTextAlign = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	InitMenuWnd();
}

KComboBoxUI::~KComboBoxUI()
{
	SAFE_FREE(m_pDropDownImage);
}

void KComboBoxUI::SetDropDownImage(Gdiplus::Image *pDropDownImage)
{
	SAFE_FREE(m_pDropDownImage);
	m_pDropDownImage = pDropDownImage;
	ReDrawCatcher();
}

void KComboBoxUI::SetDropDownBackGroundColor(COLORREF crDropDown)
{
	m_clrDropDownBackGround = crDropDown;
	ReDrawCatcher();
}

COLORREF KComboBoxUI::GetDropDownBackGroundColor() const
{
	return m_clrDropDownBackGround;
}

bool KComboBoxUI::InsertMenuItem(
	int nIndex,
	const KMenuWnd::MenuItemPair& menuItem
	)
{
	return m_menuWnd.InsertMenu(nIndex, menuItem);
}

bool KComboBoxUI::AppendMenuItem(const KMenuWnd::MenuItemPair& menuItem)
{
	m_menuWnd.AddMenu(menuItem);
	return true;
}

bool KComboBoxUI::DeleteMenuItem(const KMenuWnd::MenuItemPair& menuItem)
{
	return m_menuWnd.DeleteMenu(menuItem);
}

void KComboBoxUI::ResetContent()
{
	m_nCurSelItem = -1;
}

bool KComboBoxUI::SetCurSelItem(int nIndex)
{
	if (nIndex >= (int)(m_menuWnd.GetCount()) || nIndex < 0)
	{
		return false;
	}

	m_nCurSelItem = nIndex;
	KMenuWnd::MenuItemPair menuItem;
	if(m_menuWnd.GetMenu(m_nCurSelItem, menuItem))
	{
		m_strText = menuItem.strMenu;
		ReDrawCatcher();
		return true;
	}
	return false;
}

int KComboBoxUI::GetCurSelItem() const
{
	return m_nCurSelItem;
}

int KComboBoxUI::GetCount() const
{
	return m_menuWnd.GetCount();
}

void KComboBoxUI::Draw(HDC hMemDC)
{
	__super::DrawBK(hMemDC);

	// 先绘制dropdown图片，再绘制文本，若不存在dropdown图片，则只绘制文本即可
	RECT rcDraw = m_rcCtrl;
	DrawDropDownImage(hMemDC, rcDraw);
	DrawText(hMemDC, rcDraw);
}

void KComboBoxUI::Click()
{
	ReDrawSelf(FALSE);

	if (!(m_menuWnd.GetWindowHandle() || 
		::IsWindow(m_menuWnd.GetWindowHandle())))
	{
		return;
	}

	// 弹出菜单栏
	if (!::IsWindowVisible(m_menuWnd.GetWindowHandle()))
	{
		POINT pt = {0};
		pt.x = m_rcCtrl.left;
		pt.y = m_rcCtrl.bottom;
		::ClientToScreen(m_hOwnerWndHandle, &pt);
		UINT uID = m_menuWnd.ShowMenuWnd(pt);
		m_menuWnd.ShowWindow(FALSE);

		if (uID != (UINT)-1)
		{
			{
				KMenuWnd::MenuItemPair menuItem;
				if (m_menuWnd.GetItemByItemID(uID, menuItem))
				{
					m_nCurSelItem = m_menuWnd.GetItemIndexByItemID(uID);
					m_strText = menuItem.strMenu;
				}
			}

			if (m_pHandlerCallBack)
			{
				NotifyUIMsg uiMsg;
				uiMsg.dwTimestamp = ::GetTickCount();
				uiMsg.nMsgType = UI_MSG_CLICK;
				uiMsg.pSender = this;
				uiMsg.ptMouse = GetCursorPt();
				uiMsg.wParam = (WPARAM)uID;
				uiMsg.lParam = 0;
				m_pHandlerCallBack->UINotify(uiMsg);
			}
		}
	}
	else
	{
		m_menuWnd.ShowWindow(FALSE);
	}
}

void KComboBoxUI::InitMenuWnd()
{
	HWND hParentHandle = m_hCtrlWndHandle ? 
		m_hCtrlWndHandle : m_hOwnerWndHandle;
	if (!hParentHandle)
	{
		return;
	}

	m_menuWnd.CreateWnd(
		hParentHandle,
		L"KComboBoxMenuWnd",
		L"",
		WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_DBLCLKS,
		WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
		0,
		0,
		(int)(110 * m_lfDpi),
		(int)(90 * m_lfDpi),
		m_lfDpi
	);
}

void KComboBoxUI::DrawText(HDC hMemDC, RECT& rcDraw)
{
	if (!hMemDC)
	{
		return;
	}

	COLORREF oldTxtClr = ::GetTextColor(hMemDC);
	COLORREF clrTxt =  
		m_bDefaultCtrl ? m_clrTextSelect : m_txtClr;
	::SetTextColor(hMemDC, clrTxt);

	::DrawText(
		hMemDC,
		m_strText.c_str(),
		(int)m_strText.length(),
		&rcDraw,
		m_dwTextAlign
		);
	::SetTextColor(hMemDC, oldTxtClr);
}

void KComboBoxUI::DrawDropDownImage(HDC hMemDC, RECT& rcDraw)
{
	if (!hMemDC)
	{
		return;
	}

	int nImageWidth = 0;
	if (m_pDropDownImage)
	{
		nImageWidth = (int)(m_pDropDownImage->GetWidth() * m_lfDpi);
		int nImageHeight = (int)(m_pDropDownImage->GetHeight() * m_lfDpi);

		// 绘制绘制dropdown图片区域,始终为正方形
		LONG nBorderWidth = (LONG)(m_nBorderWidth * m_lfDpi);
		RECT rcImage = rcDraw;
		rcImage.left = rcImage.right - (m_rcCtrl.bottom - m_rcCtrl.top);
		rcImage.top += nBorderWidth;
		rcImage.bottom -= nBorderWidth;
		rcImage.right -= nBorderWidth;

		COLORREF clrBackGround = m_bHover ? m_clrHoverDropDownBackGround :
			m_clrDropDownBackGround;
		HBRUSH hBrush = ::CreateSolidBrush(clrBackGround);
		::FillRect(hMemDC, &rcImage, hBrush);
		::DeleteObject(hBrush);

		// 居中绘制图形至图片区域
		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(
			m_pDropDownImage,
			rcImage.left + abs((rcImage.right - rcImage.left) - nImageWidth) / 2,
			rcImage.top + abs((rcImage.bottom - rcImage.top) - nImageHeight) / 2,
			nImageWidth,
			nImageHeight
			);

		// 修改区域供Text绘制
		rcDraw.right = rcImage.left;
	}
}

}