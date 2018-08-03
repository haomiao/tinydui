
#ifndef _K_HYPERTEXT_UI_H_
#define _K_HYPERTEXT_UI_H_

#include "kstaticui.h"
#include <string>

namespace TinyDui 
{
// 超链接控件
class KHyperTextUI : public KStaticUI
{
public:

	KHyperTextUI();
	virtual ~KHyperTextUI();

public:

	virtual void Draw(HDC hMemDC);

	virtual bool Hit(const POINT& pt) const;
	virtual void Hover();
	virtual void Leave();
	virtual bool IsHover();

	virtual void Click();

	virtual void OnLButtonDown(const POINT &pt);
	virtual void OnMouseMove(const POINT &pt);
	virtual void OnLButtonUp(const POINT &pt);

public:

	void SetHoverColor(COLORREF hClr);
	COLORREF GetHoverColor() const;
	void SetClickedColor(COLORREF hClr);
	COLORREF GetClickedColor() const;
	void SetClickedHoverColor(COLORREF hClr);
	COLORREF GetClickedHoverColor();
	void SetDrawUnderLine(bool bDrawLine);
	bool IsDrawUnderLine() const;
	void SetClickedRecordTag(bool bRecordTag);
	bool GetClickedRecordTag() const;

private:

	bool m_bHover;
	bool m_bDown;
	bool m_bMove;
	bool m_bClicked;
	bool m_bDrawLine;
	bool m_bRecordTag;
	POINT m_ptDown;

	COLORREF m_clrTextHover;
	COLORREF m_clrClicked;
	COLORREF m_clrClickedHover;
};
}

#endif // _K_HYPERTEXT_UI_H_
