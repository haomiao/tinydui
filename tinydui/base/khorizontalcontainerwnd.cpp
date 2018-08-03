#include "kpch.h"
#include <commctrl.h>
#include "khorizontalcontainerwnd.h"

namespace
{
#define DEFAULT_TEXT_HEIGHT 14
}

namespace TinyDui 
{
KHorizontalContainerWnd::KHorizontalContainerWnd()
	: m_dwBackgroundColor(RGB(255, 255, 255))
	, m_dwBorderColor(RGB(230, 230, 230))
	, m_nBorderWidth(2)
	, m_nLeftOffset(0)
	, m_nRightOffset(0)
	, m_nLeftSpace(12)
	, m_nRightSpace(12)
	, m_nLeftCtrlSpace(12)
	, m_nCenterCtrlSpace(12)
	, m_nRightCtrlSpace(12)
	, m_pHoverCtrl(nullptr)
	, m_pPressDownCtrl(nullptr)
	, m_pDefaultCtrl(nullptr)
	, m_hCtrlFont(NULL)
	, m_lRefCount(0)
	, m_bTrackMouse(false)
	, m_pUIHandler(nullptr)
{
	SetCtrlFont(DEFAULT_TEXT_HEIGHT);
}

KHorizontalContainerWnd::~KHorizontalContainerWnd()
{
	ReleaseCtrls();
	ReleaseResources();
}

void KHorizontalContainerWnd::SetupUIHandler(__in KUIHandlerCallBack* pUIHandler)
{
	m_pUIHandler = pUIHandler;
	SetUIHandler(m_pUIHandler, m_dequeLeftCtrlWrapper);
	SetUIHandler(m_pUIHandler, m_dequeCenterCtrlWrapper);
	SetUIHandler(m_pUIHandler, m_dequeRightCtrlWrapper);
}

void KHorizontalContainerWnd::SetupDefaultUIHandler()
{
	m_pUIHandler = nullptr;
	SetUIHandler(this, m_dequeLeftCtrlWrapper);
	SetUIHandler(this, m_dequeCenterCtrlWrapper);
	SetUIHandler(this, m_dequeRightCtrlWrapper);
}

void KHorizontalContainerWnd::SetWindowAppearance(
	__in COLORREF dwBackgroundColor,
	__in COLORREF dwBorderColor,
	__in int nBorderWidth
	)
{
	m_dwBackgroundColor = dwBackgroundColor;
	m_dwBorderColor = dwBorderColor;
	m_nBorderWidth = nBorderWidth;
	RePaint();
}

void KHorizontalContainerWnd::AutoAdaptWindowRect(
	__in int nLeftOffset,
	__in int nRightOffset
	)
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return;
	}

	RECT rcParent = {};
	::GetWindowRect(m_hParent, &rcParent);
	if (::IsRectEmpty(&rcParent))
	{
		return;
	}

	CRect rcWnd;
	::GetClientRect(m_hWnd, &rcWnd);
	if (::IsRectEmpty(&rcWnd))
	{
		return;
	}

	m_nLeftOffset = nLeftOffset;
	m_nRightOffset = nRightOffset;

	::MoveWindow(
		m_hWnd, 		
		rcParent.left + m_nLeftOffset,
		rcParent.top,
		rcParent.right - rcParent.left - m_nRightOffset,
		rcWnd.Height(),
		TRUE
		);																																																																											
}

void KHorizontalContainerWnd::GetAdaptWindowRectOffset(
	__out int &nLeftOffset, 
	__out int &nRightOffset
	)
{
	nLeftOffset = m_nLeftOffset;
	nRightOffset = m_nRightOffset;
}

void KHorizontalContainerWnd::SetCtrlInternalSpace(
	__in int nLeftCtrlSpace,
	__in int nCenterCtrlSpace,
	__in int nRightCtrlSpace
	)
{
	m_nLeftCtrlSpace = nLeftCtrlSpace;
	m_nCenterCtrlSpace = nCenterCtrlSpace;
	m_nRightCtrlSpace = nRightCtrlSpace;
}

void KHorizontalContainerWnd::GetCtrlInternalSpace(
	__out int &nLeftCtrlSpace,
	__out int &nCenterCtrlSpace,
	__out int &nRightCtrlSpace
	)
{
	nLeftCtrlSpace = m_nLeftCtrlSpace;
	nCenterCtrlSpace = m_nCenterCtrlSpace;
	nRightCtrlSpace = m_nRightCtrlSpace;
}

void KHorizontalContainerWnd::SetCtrlSpace(
	__in int nLeftSpace,
	__in int nRightSpace
	)
{
	m_nLeftSpace = nLeftSpace;
	m_nRightSpace = nRightSpace;
}

void KHorizontalContainerWnd::GetCtrlSpace(
	__out int& nLeftSpace,
	__out int& nRightSpace
	)
{
	nLeftSpace = m_nLeftSpace;
	nRightSpace = m_nRightSpace;
}

bool KHorizontalContainerWnd::AppendCtrlToHead(
	__in HORIZONTAL_POS eHorizontalPos,
	__in KCtrlUIBase* pCtrlBase
	)
{
	if (!pCtrlBase || IsExistCtrlByID(pCtrlBase->GetCtrlID()))
	{
		return false;
	}

	if (m_pUIHandler)
	{
		pCtrlBase->SetUIHandler(m_pUIHandler);
	}
	else
	{
		pCtrlBase->SetUIHandler(this);
	}
	
	KCtrlWrapper ctrlWrapper = 
		KCtrlWrapper(
		pCtrlBase->GetCtrlID(),
		true, 
		pCtrlBase
		); 
	switch (eHorizontalPos)
	{
	case HORIZONTAL_LEFT:
		{
			m_dequeLeftCtrlWrapper.emplace_front(ctrlWrapper);
		}
		break;
	case HORIZONTAL_CENTER:
		{
			m_dequeCenterCtrlWrapper.emplace_front(ctrlWrapper);
		}
		break;
	case HORIZONTAL_RIGHT:
		{
			m_dequeRightCtrlWrapper.emplace_front(ctrlWrapper);
		}
		break;
	}
	return true;
}

bool KHorizontalContainerWnd::AppendCtrlToTail(
	__in HORIZONTAL_POS eHorizontalPos,
	__in KCtrlUIBase* pCtrlBase
	)
{
	if (!pCtrlBase || IsExistCtrlByID(pCtrlBase->GetCtrlID()))
	{
		return false;
	}

	if (m_pUIHandler)
	{
		pCtrlBase->SetUIHandler(m_pUIHandler);
	}
	else
	{
		pCtrlBase->SetUIHandler(this);
	}

	KCtrlWrapper ctrlWrapper = 
		KCtrlWrapper(
		pCtrlBase->GetCtrlID(),
		true, 
		pCtrlBase
		);
	switch (eHorizontalPos)
	{
	case HORIZONTAL_LEFT:
		{
			m_dequeLeftCtrlWrapper.emplace_back(ctrlWrapper);
		}
		break;
	case HORIZONTAL_CENTER:
		{
			m_dequeCenterCtrlWrapper.emplace_back(ctrlWrapper);
		}
		break;
	case HORIZONTAL_RIGHT:
		{
			m_dequeRightCtrlWrapper.emplace_back(ctrlWrapper);
		}
		break;
	}
	return true;
}

bool KHorizontalContainerWnd::IsExistCtrlByID(__in int nID) const
{
	bool bExist = IsExistCtrl(nID, m_dequeLeftCtrlWrapper);
	if (!bExist)
	{
		bExist = IsExistCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!bExist)
	{
		bExist = IsExistCtrl(nID, m_dequeRightCtrlWrapper);
	}
	return bExist;
}

bool KHorizontalContainerWnd::SetVisibleByID(__in int nID, __in bool bVisible)
{
	bool bIsVisible = SetVisible(nID, bVisible, m_dequeLeftCtrlWrapper);
	if (!bIsVisible)
	{
		bIsVisible = SetVisible(nID, bVisible, m_dequeCenterCtrlWrapper);
	}

	if (!bIsVisible)
	{
		bIsVisible = SetVisible(nID, bVisible, m_dequeRightCtrlWrapper);
	}
	return bIsVisible;
}

bool KHorizontalContainerWnd::GetVisibleByID(__in int nID, __in bool &bVisible) const
{
	bool bGetID = GetVisible(nID, bVisible, m_dequeLeftCtrlWrapper);
	if (!bGetID)
	{
		bGetID =  GetVisible(nID, bVisible, m_dequeCenterCtrlWrapper);
	}

	if (!bGetID)
	{
		bGetID =  GetVisible(nID, bVisible, m_dequeRightCtrlWrapper);
	}
	return bGetID;
}

KCtrlUIBase* KHorizontalContainerWnd::GetCtrlByID(__in int nID) const
{
	KCtrlUIBase* pCtrlUI = GetCtrl(nID, m_dequeLeftCtrlWrapper);
	if (!pCtrlUI)
	{
		pCtrlUI = GetCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!pCtrlUI)
	{
		pCtrlUI = GetCtrl(nID, m_dequeRightCtrlWrapper);
	}
	return pCtrlUI;
}

KCtrlUIBase* KHorizontalContainerWnd::RemoveCtrlByID(__in int nID)
{
	KCtrlUIBase *pCtrlUI = RemoveCtrl(nID, m_dequeLeftCtrlWrapper);
	if (!pCtrlUI)
	{
		pCtrlUI = RemoveCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!pCtrlUI)
	{
		pCtrlUI = RemoveCtrl(nID, m_dequeRightCtrlWrapper);
	}
	return pCtrlUI;
}

KCtrlUIBase* KHorizontalContainerWnd::RemoveCtrl(
	int nID,
	std::deque<KCtrlWrapper>& dequeWrapper
	)
{
	KCtrlUIBase * pCtrl = nullptr;
	for (auto iter = dequeWrapper.begin();
		iter != dequeWrapper.end();
		++iter)
	{
		if (iter->m_nCtrlID == nID)
		{
			pCtrl = iter->m_pCtrlObj;
			dequeWrapper.erase(iter);
			break;
		}
	}
	return pCtrl;
}

bool KHorizontalContainerWnd::SetDefalutCtrl(
	int nID,
	const std::deque<KCtrlWrapper>& dequeWrapper
	)
{
	for (auto iter = dequeWrapper.begin();
		iter != dequeWrapper.end();
		++iter)
	{
		if (iter->m_nCtrlID == nID)
		{
			m_pDefaultCtrl = iter->m_pCtrlObj;
			return true;
		}
	}
	return false;
}

bool KHorizontalContainerWnd::SetDefalutCtrlByID(__in int nID)
{
	bool bSetDefault = SetDefalutCtrl(nID, m_dequeLeftCtrlWrapper);
	if (!bSetDefault)
	{
		bSetDefault = SetDefalutCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!bSetDefault)
	{
		bSetDefault = SetDefalutCtrl(nID, m_dequeRightCtrlWrapper);
	}
	return bSetDefault;
}

bool KHorizontalContainerWnd::SetCtrlFont(
	__in int    cHeight,
	__in int    cWidth,
	__in int    cEscapement,
	__in int    cOrientation,
	__in int    cWeight,
	__in DWORD  bItalic,
	__in DWORD  bUnderline,
	__in DWORD  bStrikeOut,
	__in DWORD  iCharSet,
	__in DWORD  iOutPrecision,
	__in DWORD  iClipPrecision,
	__in DWORD  iQuality,
	__in DWORD  iPitchAndFamily,
	__in LPCWSTR pszFaceName
	)
{
	SAFE_DELETE_OBJECT(m_hCtrlFont);

	HDC hDC = ::GetDC(m_hWnd);
	int nCaps = ::GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(m_hWnd, hDC);
	int nFontHeight = MulDiv(cHeight, nCaps, 72);

	m_hCtrlFont = ::CreateFontW(
		nFontHeight,
		cWidth,
		cEscapement,
		cOrientation,
		cWeight,
		bItalic,
		bUnderline,
		bStrikeOut,
		iCharSet,
		iOutPrecision,
		iClipPrecision,
		iQuality,
		iPitchAndFamily,
		pszFaceName
	);

	return m_hCtrlFont != NULL;
}

HFONT KHorizontalContainerWnd::GetCtrlFont() const
{
	return m_hCtrlFont;
}

LRESULT KHorizontalContainerWnd::OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bTrackMouse)
	{
		TRACKMOUSEEVENT MouseEvent = {sizeof(TRACKMOUSEEVENT)};
		MouseEvent.dwFlags = TME_LEAVE/* | TME_HOVER*/;
		MouseEvent.hwndTrack = m_hWnd;
		MouseEvent.dwHoverTime = 100;
		if(::_TrackMouseEvent(&MouseEvent))
		{
			m_bTrackMouse = true;
		}
	}

	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	if (!m_pPressDownCtrl)
	{
		POINT pt = { 0 };
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		KCtrlUIBase *pCtrl = HitCtrl(pt);

		if (pCtrl && (pCtrl != m_pHoverCtrl))
		{
			pCtrl->Hover();
			RECT rc = pCtrl->GetRect();

			if (m_pHoverCtrl)
			{
				m_pHoverCtrl->Leave();
				RECT trc = m_pHoverCtrl->GetRect();
				::UnionRect(&rc, &rc, &trc);
			}

			m_pHoverCtrl = pCtrl;
			if (!pCtrl->IsDrag()) {
				::InvalidateRect(m_hWnd, &rc, TRUE);
			}
		}
		else if (!pCtrl)
		{
			if (m_pHoverCtrl)
			{
				m_pHoverCtrl->Leave();
				RECT rc = m_pHoverCtrl->GetRect();
				::InvalidateRect(m_hWnd, &rc, TRUE);
				m_pHoverCtrl = nullptr;
			}
		}
		else
		{
			pCtrl->Hover();
			RECT rc = pCtrl->GetRect();
			::InvalidateRect(m_hWnd, &rc, TRUE);
		}
	}
	else
	{
		m_pPressDownCtrl->OnMouseMove(pt);
	}

	bHandled = FALSE;
	return S_OK;
}

LRESULT KHorizontalContainerWnd::OnMouseHover(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	m_pHoverCtrl = HitCtrl(pt);
	if (m_pHoverCtrl)
	{
		m_pHoverCtrl->Hover();
		RECT rc = m_pHoverCtrl->GetRect();
		::InvalidateRect(m_hWnd, &rc, FALSE);
	}

	bHandled = FALSE;
	return TRUE;
}

LRESULT KHorizontalContainerWnd::OnMouseLeave(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bTrackMouse = false;

	if (m_pHoverCtrl)
	{
		m_pHoverCtrl->Leave();
		RECT rc = m_pHoverCtrl->GetRect();
		::InvalidateRect(m_hWnd, &rc, FALSE);
		m_pHoverCtrl = nullptr;
	}

	bHandled = FALSE;
	return TRUE;
}

LRESULT KHorizontalContainerWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	m_pPressDownCtrl = HitCtrl(pt);
	if (m_pPressDownCtrl)
	{
		m_pPressDownCtrl->OnLButtonDown(pt);
		RECT rc = m_pPressDownCtrl->GetRect();
		::InvalidateRect(m_hWnd, &rc, FALSE);
	}

	bHandled = FALSE;
	return S_OK;
}

LRESULT KHorizontalContainerWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	if (m_pPressDownCtrl)
	{
		if (m_pDefaultCtrl && (m_pDefaultCtrl != m_pPressDownCtrl))
		{
			KButtonUI *pPressButton = dynamic_cast<KButtonUI*>(m_pPressDownCtrl);
			if (pPressButton && pPressButton->IsEnableDefault())
			{
				KButtonUI *pDefaultButton = dynamic_cast<KButtonUI*>(m_pDefaultCtrl);
				if (pPressButton)
				{
					pPressButton->SetDefault(false);
					RECT rc = pPressButton->GetRect();
					InvalidateRect(m_hWnd, &rc, TRUE);
				}
				m_pDefaultCtrl = pPressButton;
				pPressButton->SetDefault(true);
			}
		}

		m_pPressDownCtrl->OnLButtonUp(pt);
		RECT rc = m_pPressDownCtrl->GetRect();
		::InvalidateRect(m_hWnd, &rc, FALSE);

		m_pPressDownCtrl = nullptr;
	}

	bHandled = FALSE;
	return TRUE;
}

LRESULT KHorizontalContainerWnd::OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == VK_ESCAPE)
	{
		PostMessage(WM_CLOSE, WPARAM(IDCANCEL), 0);
	}

	if (wParam == VK_RETURN)
	{
		int nID = INVALID_CTRL_ID;
		if (m_pDefaultCtrl)
		{
			nID = m_pDefaultCtrl->GetCtrlID();
		}
		PostMessage(WM_CLOSE, WPARAM(nID), 0);
	}
	bHandled = FALSE;
	return TRUE;
}

LRESULT KHorizontalContainerWnd::OnSetCursor(WPARAM wParam, LPARAM lParam, BOOL &bHandle)
{
	return TRUE;
}

void KHorizontalContainerWnd::UINotify(NotifyUIMsg &notifyUIMsg)
{
	if (notifyUIMsg.nMsgType == UI_MSG_CLICK)
	{
		int nID = -1;
		if (notifyUIMsg.pSender)
		{
			nID = notifyUIMsg.pSender->GetCtrlID();
		}

		if (nID == -1)
		{
			return;
		}

		// 对话框常用ID默认为关闭窗口
		if ((nID == IDOK) || (nID == IDCANCEL) ||
			(nID == IDYES) || (nID == IDCLOSE) ||
			(nID == IDCLOSE))
		{
			PostMessage(WM_CLOSE, WPARAM(nID), 0);
			return;
		}
	}
}

BOOL KHorizontalContainerWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(HIWORD(wParam))
	{
	case BN_CLICKED:
		{
		/*
			switch(LOWORD(wParam))
			{
			case IDC_BTN_CLOSE:
				{
					
				}
				break;
			}
		*/
		}
		break;
	}
	return TRUE;
}

BOOL KHorizontalContainerWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return FALSE;
	}

	//AutoAdaptWindowRect(m_nLeftOffset, m_nRightOffset);
	RePaint();

	return TRUE;
}

void KHorizontalContainerWnd::DrawBackground(HDC hDC)
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return;
	}

	RECT rcWnd = {};
	::GetClientRect(m_hWnd, &rcWnd);
	int nWidth = rcWnd.right - rcWnd.left;
	int nHeight = rcWnd.bottom - rcWnd.top;

	// 绘制窗口边框和背景色
	HBRUSH hFillBrush = ::CreateSolidBrush(m_dwBackgroundColor);
	HPEN hBorderPen = ::CreatePen(PS_SOLID, m_nBorderWidth, m_dwBorderColor);
	HBRUSH hOldFillBrush = (HBRUSH)::SelectObject(hDC, hFillBrush);
	HBRUSH hOldBorderPen = (HBRUSH)::SelectObject(hDC, hBorderPen);
	::Rectangle(hDC, 0, 0, nWidth, nHeight);
	::SelectObject(hDC, hOldFillBrush);
	::SelectObject(hDC, hOldBorderPen);
	SAFE_DELETE_OBJECT(hFillBrush);
	SAFE_DELETE_OBJECT(hBorderPen);
}

void KHorizontalContainerWnd::DrawOthers(HDC hDC)
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return;
	}
}

void KHorizontalContainerWnd::DrawControls(HDC hDC)
{
	if (m_hCtrlFont)
	{
		HFONT oldFont = (HFONT)::SelectObject(hDC, m_hCtrlFont);
		SmartAdjustCtrlsLayout(hDC);
		::SelectObject(hDC, oldFont);
	}
	else
	{
		SmartAdjustCtrlsLayout(hDC);
	}
}

KCtrlUIBase* KHorizontalContainerWnd::HitCtrl(POINT pt) const
{
	KCtrlUIBase *pCtrlUI = HitCtrl(pt, m_dequeLeftCtrlWrapper);
	if (!pCtrlUI)
	{
		pCtrlUI = HitCtrl(pt, m_dequeCenterCtrlWrapper);
	}

	if (!pCtrlUI)
	{
		pCtrlUI = HitCtrl(pt, m_dequeRightCtrlWrapper);
	}
	return pCtrlUI;
}

KCtrlUIBase* KHorizontalContainerWnd::HitCtrl(
	POINT pt,
	const std::deque<KCtrlWrapper>& dequeWrapper
	) const
{
	KCtrlUIBase *pCtrlUI = nullptr;
	for (auto iter = dequeWrapper.begin();
		iter != dequeWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->Hit(pt))
		{
			pCtrlUI = iter->m_pCtrlObj;
			break;
		}
	}
	return pCtrlUI;
}

void KHorizontalContainerWnd::SmartAdjustCtrlsLayout(HDC hDC)
{
	// 所有控件均居中绘制
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return;
	}

	//RECT rcUpdate, rcDest;
	//::GetUpdateRect(m_hWnd, &rcUpdate, FALSE);

	CRect rcWnd;
	::GetClientRect(m_hWnd, &rcWnd);
	int nWndHeight = rcWnd.Height();
	int nLeftPos = (int)(m_nLeftSpace * m_lfScale);
	int nTopPos = rcWnd.top;
	int nRightPos = rcWnd.Width() - (int)(m_nRightSpace * m_lfScale);
	if (nRightPos <= 0)
	{
		return;
	}

	int nOrginWidth = 0;
	int nOrginHeight = 0;

	// 从右至左绘制各控件,当中间空余空间不足时(也即是左/中/右空间窗口交叉时)，
	// 将中、左边各控件左移即可。
	int nTempRightPos = nRightPos;
	for (auto iter = m_dequeRightCtrlWrapper.rbegin();
		iter != m_dequeRightCtrlWrapper.rend();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rc = iter->m_pCtrlObj->GetRect();
			nOrginWidth = (int)abs(rc.right - rc.left);
			nOrginHeight = (int)abs(rc.bottom - rc.top);
			nTopPos = (nWndHeight - nOrginHeight) / 2 + rcWnd.top;
			rc.right = nTempRightPos;
			rc.left = rc.right - nOrginWidth;
			rc.top = nTopPos;
			rc.bottom = rc.top + nOrginHeight;

			/*
			if (::IntersectRect(&rcDest, &rc, &rcUpdate))
			{
				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
			}*/

			iter->m_pCtrlObj->SetRect(rc);
			iter->m_pCtrlObj->Draw(hDC);
			nTempRightPos -= nOrginWidth;
			nTempRightPos -= (int)(m_nRightCtrlSpace * m_lfScale);
		}
	}

	// 若空间有冗余，则左边窗口控件从左至右绘制, 中间窗口从中间绘制，
	// 否则中间和左边窗口均从右至左绘制
	int nCenterWidth = GetAllCenterCtrlsWidth();
	int nLeftAndCenterWidth = GetAllLeftCtrlsWidth() + nCenterWidth;
	int nRedundancySpace = nTempRightPos - (nLeftAndCenterWidth + nLeftPos);
	
	// 若存在中间窗口，则预留5px空间
	if (nCenterWidth > 0)
	{
		nRedundancySpace -= (int)(5 * GetCurrentDpiValue());
	}
	if (nRedundancySpace > 0)
	{
		// 居中绘制中间窗口
		nTempRightPos -= (int)(nRedundancySpace / 2.0);
		for (auto iter = m_dequeCenterCtrlWrapper.rbegin();
			iter != m_dequeCenterCtrlWrapper.rend();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nTopPos = (nWndHeight - nOrginHeight) / 2 + rcWnd.top;
				rc.right = nTempRightPos;
				rc.left = rc.right - nOrginWidth;
				rc.top = nTopPos;
				rc.bottom = rc.top + nOrginHeight;

				/*
				if (::IntersectRect(&rcDest, &rc, &rcUpdate))
				{
					iter->m_pCtrlObj->SetRect(rc);
					iter->m_pCtrlObj->Draw(hDC);
				}*/

				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
				nTempRightPos -= nOrginWidth;
				nTempRightPos -= (int)(m_nCenterCtrlSpace * m_lfScale);
			}
		}

		// 绘制左窗口
		for (auto iter = m_dequeLeftCtrlWrapper.begin();
			iter != m_dequeLeftCtrlWrapper.end();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nTopPos = (nWndHeight - nOrginHeight) / 2 + rcWnd.top;
				rc.left = nLeftPos;
				rc.right = rc.left + nOrginWidth;
				rc.top = nTopPos;
				rc.bottom = rc.top + nOrginHeight;

				/*
				if (::IntersectRect(&rcDest, &rc, &rcUpdate))
				{
					iter->m_pCtrlObj->SetRect(rc);
					iter->m_pCtrlObj->Draw(hDC);
				}*/

				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
				nLeftPos += nOrginWidth;
				nLeftPos += (int)(m_nLeftCtrlSpace * m_lfScale);
			}
		}
	}
	else
	{
		 // 依次从右向左绘制中窗口、左边窗口
		for (auto iter = m_dequeCenterCtrlWrapper.rbegin();
			iter != m_dequeCenterCtrlWrapper.rend();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nTopPos = (nWndHeight - nOrginHeight) / 2 + rcWnd.top;
				rc.right = nTempRightPos;
				rc.left = rc.right - nOrginWidth;
				rc.top = nTopPos;
				rc.bottom = rc.top + nOrginHeight;

				/*
				if (::IntersectRect(&rcDest, &rc, &rcUpdate))
				{
					iter->m_pCtrlObj->SetRect(rc);
					iter->m_pCtrlObj->Draw(hDC);
				}*/

				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
				nTempRightPos -= nOrginWidth;
				nTempRightPos -= (int)(m_nCenterCtrlSpace * m_lfScale);
			}
		}

		for (auto iter = m_dequeLeftCtrlWrapper.rbegin();
			iter != m_dequeLeftCtrlWrapper.rend();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nTopPos = (nWndHeight - nOrginHeight) / 2 + rcWnd.top;
				rc.right = nTempRightPos;
				rc.left = rc.right - nOrginWidth;
				rc.top = nTopPos;
				rc.bottom = rc.top + nOrginHeight;

				/*
				if (::IntersectRect(&rcDest, &rc, &rcUpdate))
				{
					iter->m_pCtrlObj->SetRect(rc);
					iter->m_pCtrlObj->Draw(hDC);
				}*/

				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
				nTempRightPos -= nOrginWidth;
				nTempRightPos -= (int)(m_nLeftCtrlSpace * m_lfScale);
			}
		}
	}
}

void KHorizontalContainerWnd::ReleaseCtrls()
{
	ReleaseCtrls(m_dequeLeftCtrlWrapper);
	ReleaseCtrls(m_dequeCenterCtrlWrapper);
	ReleaseCtrls(m_dequeRightCtrlWrapper);
}

void KHorizontalContainerWnd::ReleaseCtrls(
	std::deque<KCtrlWrapper>& dequeWrapper
	)
{
	while (!dequeWrapper.empty())
	{
		auto iter = dequeWrapper.begin();
		if (iter->m_pCtrlObj)
		{
			delete iter->m_pCtrlObj;
		}
		dequeWrapper.erase(iter);
	}
}

void KHorizontalContainerWnd::SetUIHandler(
	KUIHandlerCallBack* pUIHandler,
	std::deque<KCtrlWrapper>& dequeWrapper
	)
{
	for (auto iter = dequeWrapper.begin();
		iter != dequeWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj)
		{
			iter->m_pCtrlObj->SetUIHandler(pUIHandler);
		}
	}
}

LONG KHorizontalContainerWnd::GetCtrlsMaxHeight()
{
	LONG nCtrlMaxHeight = -1;
	LONG nTempHeight = -1;
	for (auto iter = m_dequeLeftCtrlWrapper.begin();
		iter != m_dequeLeftCtrlWrapper.end();
		++iter)
	{
		RECT rcWnd = iter->m_pCtrlObj->GetRect();
		nTempHeight = abs(rcWnd.bottom - rcWnd.top);
		if (nTempHeight > nCtrlMaxHeight)
		{
			nCtrlMaxHeight = nTempHeight;
		}
	}

	for (auto iter = m_dequeCenterCtrlWrapper.begin();
		iter != m_dequeCenterCtrlWrapper.end();
		++iter)
	{
		RECT rcWnd = iter->m_pCtrlObj->GetRect();
		nTempHeight = abs(rcWnd.bottom - rcWnd.top);
		if (nTempHeight > nCtrlMaxHeight)
		{
			nCtrlMaxHeight = nTempHeight;
		}
	}

	for (auto iter = m_dequeRightCtrlWrapper.begin();
		iter != m_dequeRightCtrlWrapper.end();
		++iter)
	{
		RECT rcWnd = iter->m_pCtrlObj->GetRect();
		nTempHeight = abs(rcWnd.bottom - rcWnd.top);
		if (nTempHeight > nCtrlMaxHeight)
		{
			nCtrlMaxHeight = nTempHeight;
		}
	}
	return nCtrlMaxHeight;
}

LONG KHorizontalContainerWnd::GetAllLeftCtrlsWidth()
{
	LONG nAllCtrlsWidth = 0;
	int nValidCtrs = 0;
	for (auto iter = m_dequeLeftCtrlWrapper.begin();
		iter != m_dequeLeftCtrlWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rcWnd = iter->m_pCtrlObj->GetRect();
			nAllCtrlsWidth += abs(rcWnd.right - rcWnd.left);
			++nValidCtrs;
		}
	}
	return nAllCtrlsWidth + 
		(nValidCtrs > 1 ? (nValidCtrs - 1) * m_nLeftCtrlSpace : 0);
}

LONG KHorizontalContainerWnd::GetAllCenterCtrlsWidth()
{
	LONG nAllCtrlsWidth = 0;
	int nValidCtrs = 0;
	for (auto iter = m_dequeCenterCtrlWrapper.begin();
		iter != m_dequeCenterCtrlWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rcWnd = iter->m_pCtrlObj->GetRect();
			nAllCtrlsWidth += abs(rcWnd.right - rcWnd.left);
			++nValidCtrs;
		}
	}
	return nAllCtrlsWidth + 
		(nValidCtrs > 1 ? (nValidCtrs - 1) * m_nCenterCtrlSpace : 0);
}

LONG KHorizontalContainerWnd::GetAllRightCtrlsWidth()
{
	LONG nAllCtrlsWidth = 0;
	int nValidCtrs = 0;
	for (auto iter = m_dequeRightCtrlWrapper.begin();
		iter != m_dequeRightCtrlWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rcWnd = iter->m_pCtrlObj->GetRect();
			nAllCtrlsWidth += abs(rcWnd.right - rcWnd.left);
			++nValidCtrs;
		}
	}
	return nAllCtrlsWidth +
		(nValidCtrs > 1 ? (nValidCtrs - 1) * m_nRightCtrlSpace : 0);
}

bool KHorizontalContainerWnd::IsExistCtrl(
	int nID,
	const std::deque<KCtrlWrapper>& dequeWrapper
	) const
{
	for (auto iter = dequeWrapper.begin(); 
		iter != dequeWrapper.end(); 
		++iter)
	{
		if (iter->m_nCtrlID == nID)
		{
			return true;
		}
	}
	return false;
}

bool KHorizontalContainerWnd::SetVisible(
	int nID,
	bool bVisible,
	const std::deque<KCtrlWrapper>& dequeWrapper
	)
{
	for (auto iter = dequeWrapper.begin();
		iter != dequeWrapper.end();
		++iter)
	{
		if (iter->m_nCtrlID == nID)
		{
			iter->m_pCtrlObj->SetVisible(bVisible);
			return true;
		}
	}
	return false;
}

bool KHorizontalContainerWnd::GetVisible(
	int nID,
	bool &bVisible, 
	const std::deque<KCtrlWrapper>& dequeWrapper
	) const
{
	for (auto iter = dequeWrapper.begin();
		iter != dequeWrapper.end();
		++iter)
	{
		if (iter->m_nCtrlID == nID)
		{
			bVisible = iter->m_pCtrlObj->IsVisible();
			return true;
		}
	}
	return false;
}

KCtrlUIBase* KHorizontalContainerWnd::GetCtrl(
	int nID,
	const std::deque<KCtrlWrapper>& dequeWrapper
	) const
{
	for (auto iter = dequeWrapper.begin();
		iter != dequeWrapper.end();
		++iter)
	{
		if (iter->m_nCtrlID == nID)
		{
			return iter->m_pCtrlObj;
		}
	}
	return nullptr;
}

void KHorizontalContainerWnd::InitResources()
{
	//SetCtrlFont(DEFAULT_TEXT_HEIGHT);
}

void KHorizontalContainerWnd::InitControls()
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return;
	}
}

void KHorizontalContainerWnd::ReleaseResources()
{
	SAFE_DELETE_OBJECT(m_hCtrlFont);
}
}

