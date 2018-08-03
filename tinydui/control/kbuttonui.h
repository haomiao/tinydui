#ifndef _K_BUTTON_UI_H_
#define _K_BUTTON_UI_H_

#include <string>
#include "kstaticui.h"
#include "base/kpopuptipwnd.h"

namespace TinyDui 
{
// 按钮控件
class KButtonUI : public KStaticUI
{
public:

	KButtonUI();
	virtual ~KButtonUI();

public:

	bool IsDefaultCtrl();

	void SetBorderColor(COLORREF clrBorder);
	void SetBackGroundColor(COLORREF clrBackGround);
	void SetHoverColor(COLORREF clrHover);
	void SetSelectHoverColor(COLORREF clrSelHover);
	void SetSelectColor(COLORREF clrSelBackGround);
	void SetBackGroundDownColor(COLORREF clrDownBackGround);

	void SetTextSelectColor(COLORREF clrTextSelect);

	void SetBorderWidth(int nBorderWidth);
	int GetBorderWidth() const;
	void SetToolTipText(const std::wstring& strToolTip);
	std::wstring GetToolTipText() const;
	void SetShowToolTip(bool bShowToolTip);
	bool GetShowToolTip() const;

public:

	virtual void Draw(HDC hMemDC);
	virtual void DrawBK(HDC hMemDC);
	virtual void DrawText(HDC hMemDC);

	virtual void Hover();
	virtual void Leave();
	virtual bool IsHover();

	virtual void Click();

	virtual void OnLButtonDown(const POINT &pt);
	virtual void OnMouseMove(const POINT &pt);
	virtual void OnLButtonUp(const POINT &pt);
	virtual void SetDefault(bool bDefalut);
	virtual bool IsEnableDefault();

private:

	void ShowToolTip();

protected:

	bool  m_bHover;
	bool  m_bDefaultCtrl;
	bool  m_bDown;
	bool  m_bMove;
	POINT m_ptDown;

	COLORREF m_clrBorder;
	COLORREF m_clrBackGround;
	COLORREF m_clrHover;
	COLORREF m_clrSelHover;
	COLORREF m_clrSelBackGround;
	COLORREF m_clrDownBackGround;

	COLORREF m_clrTextSelect;
	int m_nBorderWidth;
	std::wstring m_strToolTip;	// ToolTip信息
	bool m_bShowToolTip;	// 是否显示ToolTip
	KPopupTipWnd m_toolTipWnd;	// ToolTip窗口
	bool m_bAlreadyShowTip;
};

// 图片按钮,也即是(图片 + 文字)
// 若没有设置图片，则自动退化为KButtonUI
// 同样，若没有文字，则居中只绘制图片 
class KImageButtonUI : public KButtonUI
{
public:
	KImageButtonUI();
	virtual ~KImageButtonUI();

public:

	void SetNormalImage(Gdiplus::Image *pNormalImage);
	void SetHoverImage(Gdiplus::Image *pHoverImage);
	void SetImageTextSpace(int nImageTextSpace);
	int GetImageTextSpace() const;

public:

	virtual void Draw(HDC hMemDC);

private:
	
	void DrawImage(HDC hMemDC, RECT& rcDraw);
	void DrawText(HDC hMemDC, RECT& rcDraw);
	int GetImageWidth();

private:

	Gdiplus::Image *m_pHoverImage;
	int m_nImageTextSpace;
};
}

#endif // _K_BUTTON_UI_H_
