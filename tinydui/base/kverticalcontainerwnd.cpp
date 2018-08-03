#include "kpch.h"
#include <commctrl.h>
#include "kverticalcontainerwnd.h"

namespace
{
#define DEFAULT_TEXT_HEIGHT 14
}

namespace TinyDui 
{
KVerticalContainerWnd::KVerticalContainerWnd()
	: m_dwBackgroundColor(RGB(255, 255, 255))
	, m_dwBorderColor(RGB(230, 230, 230))
	, m_nBorderWidth(2)
	, m_nTopOffset(0)
	, m_nBottomOffset(0)
	, m_nTopSpace(12)
	, m_nBottomSpace(12)
	, m_nTopCtrlSpace(12)
	, m_nCenterCtrlSpace(12)
	, m_nBottomCtrlSpace(12)
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

KVerticalContainerWnd::~KVerticalContainerWnd()
{
	ReleaseCtrls();
	ReleaseResources();
}

void KVerticalContainerWnd::SetupUIHandler(__in KUIHandlerCallBack* pUIHandler)
{
	m_pUIHandler = pUIHandler;
	SetUIHandler(m_pUIHandler, m_dequeTopCtrlWrapper);
	SetUIHandler(m_pUIHandler, m_dequeCenterCtrlWrapper);
	SetUIHandler(m_pUIHandler, m_dequeBottomCtrlWrapper);
}

void KVerticalContainerWnd::SetupDefaultUIHandler()
{
	m_pUIHandler = nullptr;
	SetUIHandler(this, m_dequeTopCtrlWrapper);
	SetUIHandler(this, m_dequeCenterCtrlWrapper);
	SetUIHandler(this, m_dequeBottomCtrlWrapper);
}

void KVerticalContainerWnd::SetWindowAppearance(
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

void KVerticalContainerWnd::AutoAdaptWindowRect(
	__in int nTopOffset,
	__in int nBottomOffset
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

	m_nTopOffset = nTopOffset;
	m_nBottomOffset = nBottomOffset;

	::MoveWindow(
		m_hWnd, 		
		rcParent.left,
		rcParent.top + m_nTopOffset,
		rcParent.right - rcParent.left,
		rcWnd.Height() - m_nBottomOffset,
		TRUE
		);
}

void KVerticalContainerWnd::GetAdaptWindowRectOffset(
	__out int &nTopOffset,
	__out int &nBottomOffset
)
{
	nTopOffset = m_nTopOffset;
	nBottomOffset = m_nBottomOffset;
}

void KVerticalContainerWnd::SetCtrlInternalSpace(
	__in int nTopCtrlSpace,
	__in int nCenterCtrlSpace,
	__in int nBottomCtrlSpace
	)
{
	m_nTopCtrlSpace = nTopCtrlSpace;
	m_nCenterCtrlSpace = nCenterCtrlSpace;
	m_nBottomCtrlSpace = nBottomCtrlSpace;
}

void KVerticalContainerWnd::GetCtrlInternalSpace(
	__out int &nTopCtrlSpace,
	__out int &nCenterCtrlSpace,
	__out int &nBottomCtrlSpace
	)
{
	nTopCtrlSpace = m_nTopCtrlSpace;
	nCenterCtrlSpace = m_nCenterCtrlSpace;
	nBottomCtrlSpace = m_nBottomCtrlSpace;
}

void KVerticalContainerWnd::SetCtrlSpace(
	__in int nTopSpace,
	__in int nBottomSpace
	)
{
	m_nTopSpace = nTopSpace;
	m_nBottomSpace = nBottomSpace;
}

void KVerticalContainerWnd::GetCtrlSpace(
	__out int &nTopSpace,
	__out int &nBottomSpace
	)
{
	nTopSpace = m_nTopSpace;
	nBottomSpace = m_nBottomSpace;
}

bool KVerticalContainerWnd::AppendCtrlToHead(
	__in VERTICAL_POS eVerticalPos,
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
	switch (eVerticalPos)
	{
	case VERTICAL_TOP:
		{
			m_dequeTopCtrlWrapper.emplace_front(ctrlWrapper);
		}
		break;
	case VERTICAL_CENTER:
		{
			m_dequeCenterCtrlWrapper.emplace_front(ctrlWrapper);
		}
		break;
	case VERTICAL_BOTTOM:
		{
			m_dequeBottomCtrlWrapper.emplace_front(ctrlWrapper);
		}
		break;
	}
	return true;
}

bool KVerticalContainerWnd::AppendCtrlToTail(
	__in VERTICAL_POS eVerticalPos,
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
	switch (eVerticalPos)
	{
	case VERTICAL_TOP:
		{
			m_dequeTopCtrlWrapper.emplace_back(ctrlWrapper);
		}
		break;
	case VERTICAL_CENTER:
		{
			m_dequeCenterCtrlWrapper.emplace_back(ctrlWrapper);
		}
		break;
	case VERTICAL_BOTTOM:
		{
			m_dequeBottomCtrlWrapper.emplace_back(ctrlWrapper);
		}
		break;
	}
	return true;
}

bool KVerticalContainerWnd::IsExistCtrlByID(__in int nID) const
{
	bool bExist = IsExistCtrl(nID, m_dequeTopCtrlWrapper);
	if (!bExist)
	{
		bExist = IsExistCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!bExist)
	{
		bExist = IsExistCtrl(nID, m_dequeBottomCtrlWrapper);
	}
	return bExist;
}

bool KVerticalContainerWnd::SetVisibleByID(__in int nID, __in bool bVisible)
{
	bool bIsVisible = SetVisible(nID, bVisible, m_dequeTopCtrlWrapper);
	if (!bIsVisible)
	{
		bIsVisible = SetVisible(nID, bVisible, m_dequeCenterCtrlWrapper);
	}

	if (!bIsVisible)
	{
		bIsVisible = SetVisible(nID, bVisible, m_dequeBottomCtrlWrapper);
	}
	return bIsVisible;
}

bool KVerticalContainerWnd::GetVisibleByID(__in int nID, __in bool &bVisible) const
{
	bool bGetID = GetVisible(nID, bVisible, m_dequeTopCtrlWrapper);
	if (!bGetID)
	{
		bGetID =  GetVisible(nID, bVisible, m_dequeCenterCtrlWrapper);
	}

	if (!bGetID)
	{
		bGetID =  GetVisible(nID, bVisible, m_dequeBottomCtrlWrapper);
	}
	return bGetID;
}

KCtrlUIBase* KVerticalContainerWnd::GetCtrlByID(__in int nID) const
{
	KCtrlUIBase* pCtrlUI = GetCtrl(nID, m_dequeTopCtrlWrapper);
	if (!pCtrlUI)
	{
		pCtrlUI = GetCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!pCtrlUI)
	{
		pCtrlUI = GetCtrl(nID, m_dequeBottomCtrlWrapper);
	}
	return pCtrlUI;
}

KCtrlUIBase* KVerticalContainerWnd::RemoveCtrlByID(__in int nID)
{
	KCtrlUIBase *pCtrlUI = RemoveCtrl(nID, m_dequeTopCtrlWrapper);
	if (!pCtrlUI)
	{
		pCtrlUI = RemoveCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!pCtrlUI)
	{
		pCtrlUI = RemoveCtrl(nID, m_dequeBottomCtrlWrapper);
	}
	return pCtrlUI;
}

KCtrlUIBase* KVerticalContainerWnd::RemoveCtrl(
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

bool KVerticalContainerWnd::SetDefalutCtrl(
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

bool KVerticalContainerWnd::SetDefalutCtrlByID(__in int nID)
{
	bool bSetDefault = SetDefalutCtrl(nID, m_dequeTopCtrlWrapper);
	if (!bSetDefault)
	{
		bSetDefault = SetDefalutCtrl(nID, m_dequeCenterCtrlWrapper);
	}

	if (!bSetDefault)
	{
		bSetDefault = SetDefalutCtrl(nID, m_dequeBottomCtrlWrapper);
	}
	return bSetDefault;
}

bool KVerticalContainerWnd::SetCtrlFont(
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

	m_hCtrlFont = ::CreateFont(
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

HFONT KVerticalContainerWnd::GetCtrlFont() const
{
	return m_hCtrlFont;
}

LRESULT KVerticalContainerWnd::OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT KVerticalContainerWnd::OnMouseHover(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT KVerticalContainerWnd::OnMouseLeave(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT KVerticalContainerWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT KVerticalContainerWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT KVerticalContainerWnd::OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT KVerticalContainerWnd::OnSetCursor(WPARAM wParam, LPARAM lParam, BOOL &bHandle)
{
	return TRUE;
}

void KVerticalContainerWnd::UINotify(NotifyUIMsg &notifyUIMsg)
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

BOOL KVerticalContainerWnd::OnCommand(WPARAM wParam, LPARAM lParam)
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

BOOL KVerticalContainerWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return FALSE;
	}

	//AutoAdaptWindowRect(m_nTopOffset, m_nBottomOffset);
	RePaint();

	return TRUE;
}

void KVerticalContainerWnd::DrawBackground(HDC hDC)
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

void KVerticalContainerWnd::DrawOthers(HDC hDC)
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return;
	}
}

void KVerticalContainerWnd::DrawControls(HDC hDC)
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

KCtrlUIBase* KVerticalContainerWnd::HitCtrl(POINT pt) const
{
	KCtrlUIBase *pCtrlUI = HitCtrl(pt, m_dequeTopCtrlWrapper);
	if (!pCtrlUI)
	{
		pCtrlUI = HitCtrl(pt, m_dequeCenterCtrlWrapper);
	}

	if (!pCtrlUI)
	{
		pCtrlUI = HitCtrl(pt, m_dequeBottomCtrlWrapper);
	}
	return pCtrlUI;
}

KCtrlUIBase* KVerticalContainerWnd::HitCtrl(
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

void KVerticalContainerWnd::SmartAdjustCtrlsLayout(HDC hDC)
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
	int nWndWidth = rcWnd.Width();
	int nTopPos = (int)(m_nTopSpace * m_lfScale);
	int nLeftPos = rcWnd.left;
	int nBottomPos = rcWnd.Height() - (int)(m_nBottomSpace * m_lfScale);
	if (nBottomPos <= 0)
	{
		return;
	}

	int nOrginWidth = 0;
	int nOrginHeight = 0;

	// 从上至下绘制各控件,当中间空余空间不足时(也即是上/中/下空间窗口交叉时)，
	// 将中、下边各控件下移即可。
	int nTempTopPos = nTopPos;
	for (auto iter = m_dequeTopCtrlWrapper.begin();
		iter != m_dequeTopCtrlWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rc = iter->m_pCtrlObj->GetRect();
			nOrginWidth = (int)abs(rc.right - rc.left);
			nOrginHeight = (int)abs(rc.bottom - rc.top);
			nLeftPos = (nWndWidth - nOrginWidth) / 2 + rcWnd.left;
			rc.left = nLeftPos;
			rc.right = rc.left + nOrginWidth;
			rc.top = nTempTopPos;
			rc.bottom = rc.top + nOrginHeight;

			/*
			if (::IntersectRect(&rcDest, &rc, &rcUpdate))
			{
				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
			}*/

			iter->m_pCtrlObj->SetRect(rc);
			iter->m_pCtrlObj->Draw(hDC);
			nTempTopPos += nOrginHeight;
			nTempTopPos += (int)(m_nTopCtrlSpace * m_lfScale);
		}
	}

	// 若空间有冗余，则下边窗口控件从下至上绘制, 中间窗口从中间绘制，
	// 否则中间和下边窗口均从上至下绘制
	int nCenterHeight = GetAllCenterCtrlsHeight();
	int nCenterAndBottomHeight = GetAllBottomCtrlsHeight() + nCenterHeight;
	int nRedundancySpace = nBottomPos - (nCenterAndBottomHeight + nTopPos);
	
	// 若存在中间窗口，则预留5px空间
	if (nCenterHeight > 0)
	{
		nRedundancySpace -= (int)(5 * GetCurrentDpiValue());
	}
	if (nRedundancySpace > 0)
	{
		// 居中绘制中间窗口
		nTempTopPos += (int)(nRedundancySpace / 2.0);
		for (auto iter = m_dequeCenterCtrlWrapper.begin();
			iter != m_dequeCenterCtrlWrapper.end();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nLeftPos = (nWndWidth - nOrginWidth) / 2 + rcWnd.left;
				rc.left = nLeftPos;
				rc.right = rc.left + nOrginWidth;
				rc.top = nTempTopPos;
				rc.bottom = rc.top + nOrginHeight;

				/*
				if (::IntersectRect(&rcDest, &rc, &rcUpdate))
				{
					iter->m_pCtrlObj->SetRect(rc);
					iter->m_pCtrlObj->Draw(hDC);
				}*/

				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
				nTempTopPos += nOrginHeight;
				nTempTopPos += (int)(m_nCenterCtrlSpace * m_lfScale);
			}
		}

		// 绘制下窗口
		for (auto iter = m_dequeBottomCtrlWrapper.rbegin();
			iter != m_dequeBottomCtrlWrapper.rend();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nLeftPos = (nWndWidth - nOrginWidth) / 2 + rcWnd.left;
				rc.left = nLeftPos;
				rc.right = rc.left + nOrginWidth;
				rc.bottom = nBottomPos;
				rc.top = rc.bottom - nOrginHeight;

				/*
				if (::IntersectRect(&rcDest, &rc, &rcUpdate))
				{
					iter->m_pCtrlObj->SetRect(rc);
					iter->m_pCtrlObj->Draw(hDC);
				}*/

				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
				nBottomPos -= nOrginHeight;
				nBottomPos -= (int)(m_nBottomCtrlSpace * m_lfScale);
			}
		}
	}
	else
	{
		 // 依次从上向下绘制中窗口、下边窗口
		for (auto iter = m_dequeCenterCtrlWrapper.rbegin();
			iter != m_dequeCenterCtrlWrapper.rend();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nLeftPos = (nWndWidth - nOrginWidth) / 2 + rcWnd.left;
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
				nTempTopPos -= nOrginHeight;
				nTempTopPos -= (int)(m_nCenterCtrlSpace * m_lfScale);
			}
		}

		for (auto iter = m_dequeBottomCtrlWrapper.rbegin();
			iter != m_dequeBottomCtrlWrapper.rend();
			++iter)
		{
			if (iter->m_pCtrlObj->IsVisible())
			{
				RECT rc = iter->m_pCtrlObj->GetRect();
				nOrginWidth = (int)abs(rc.right - rc.left);
				nOrginHeight = (int)abs(rc.bottom - rc.top);
				nLeftPos = (nWndWidth - nOrginWidth) / 2 + rcWnd.left;
				rc.left = nLeftPos;
				rc.right = rc.left + nOrginWidth;
				rc.top = nTempTopPos;
				rc.bottom = rc.top + nOrginHeight;

				/*
				if (::IntersectRect(&rcDest, &rc, &rcUpdate))
				{
					iter->m_pCtrlObj->SetRect(rc);
					iter->m_pCtrlObj->Draw(hDC);
				}*/

				iter->m_pCtrlObj->SetRect(rc);
				iter->m_pCtrlObj->Draw(hDC);
				nTempTopPos -= nOrginHeight;
				nTempTopPos -= (int)(m_nBottomCtrlSpace * m_lfScale);
			}
		}
	}
}

void KVerticalContainerWnd::ReleaseCtrls()
{
	ReleaseCtrls(m_dequeTopCtrlWrapper);
	ReleaseCtrls(m_dequeCenterCtrlWrapper);
	ReleaseCtrls(m_dequeBottomCtrlWrapper);
}

void KVerticalContainerWnd::ReleaseCtrls(
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

void KVerticalContainerWnd::SetUIHandler(
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

LONG KVerticalContainerWnd::GetCtrlsMaxWidth()
{
	LONG nCtrlMaxWidth = -1;
	LONG nTempWidth = -1;
	for (auto iter = m_dequeTopCtrlWrapper.begin();
		iter != m_dequeTopCtrlWrapper.end();
		++iter)
	{
		RECT rcWnd = iter->m_pCtrlObj->GetRect();
		nTempWidth = abs(rcWnd.right - rcWnd.left);
		if (nTempWidth > nCtrlMaxWidth)
		{
			nCtrlMaxWidth = nTempWidth;
		}
	}

	for (auto iter = m_dequeCenterCtrlWrapper.begin();
		iter != m_dequeCenterCtrlWrapper.end();
		++iter)
	{
		RECT rcWnd = iter->m_pCtrlObj->GetRect();
		nTempWidth = abs(rcWnd.right - rcWnd.left);
		if (nTempWidth > nCtrlMaxWidth)
		{
			nCtrlMaxWidth = nTempWidth;
		}
	}

	for (auto iter = m_dequeBottomCtrlWrapper.begin();
		iter != m_dequeBottomCtrlWrapper.end();
		++iter)
	{
		RECT rcWnd = iter->m_pCtrlObj->GetRect();
		nTempWidth = abs(rcWnd.right - rcWnd.left);
		if (nTempWidth > nCtrlMaxWidth)
		{
			nCtrlMaxWidth = nTempWidth;
		}
	}
	return nCtrlMaxWidth;
}

LONG KVerticalContainerWnd::GetAllTopCtrlsHeight()
{
	LONG nAllCtrlsHeight = 0;
	int nValidCtrs = 0;
	for (auto iter = m_dequeTopCtrlWrapper.begin();
		iter != m_dequeTopCtrlWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rcWnd = iter->m_pCtrlObj->GetRect();
			nAllCtrlsHeight += abs(rcWnd.bottom - rcWnd.top);
			++nValidCtrs;
		}
	}
	return nAllCtrlsHeight + 
		(nValidCtrs > 1 ? (nValidCtrs - 1) * m_nTopCtrlSpace : 0);
}

LONG KVerticalContainerWnd::GetAllCenterCtrlsHeight()
{
	LONG nAllCtrlsHeight = 0;
	int nValidCtrs = 0;
	for (auto iter = m_dequeCenterCtrlWrapper.begin();
		iter != m_dequeCenterCtrlWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rcWnd = iter->m_pCtrlObj->GetRect();
			nAllCtrlsHeight += abs(rcWnd.bottom - rcWnd.top);
			++nValidCtrs;
		}
	}
	return nAllCtrlsHeight + 
		(nValidCtrs > 1 ? (nValidCtrs - 1) * m_nCenterCtrlSpace : 0);
}

LONG KVerticalContainerWnd::GetAllBottomCtrlsHeight()
{
	LONG nAllCtrlsHeight = 0;
	int nValidCtrs = 0;
	for (auto iter = m_dequeBottomCtrlWrapper.begin();
		iter != m_dequeBottomCtrlWrapper.end();
		++iter)
	{
		if (iter->m_pCtrlObj->IsVisible())
		{
			RECT rcWnd = iter->m_pCtrlObj->GetRect();
			nAllCtrlsHeight += abs(rcWnd.bottom - rcWnd.top);
			++nValidCtrs;
		}
	}
	return nAllCtrlsHeight +
		(nValidCtrs > 1 ? (nValidCtrs - 1) * m_nBottomCtrlSpace : 0);
}

bool KVerticalContainerWnd::IsExistCtrl(
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

bool KVerticalContainerWnd::SetVisible(
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

bool KVerticalContainerWnd::GetVisible(
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

KCtrlUIBase* KVerticalContainerWnd::GetCtrl(
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

void KVerticalContainerWnd::InitResources()
{
	//SetCtrlFont(DEFAULT_TEXT_HEIGHT);
}

void KVerticalContainerWnd::InitControls()
{
	if (!(m_hWnd && ::IsWindow(m_hWnd)))
	{
		return;
	}
}

void KVerticalContainerWnd::ReleaseResources()
{
	SAFE_DELETE_OBJECT(m_hCtrlFont);
}
}

