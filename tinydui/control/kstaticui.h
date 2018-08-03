#ifndef _K_STATIC_UI_H_
#define _K_STATIC_UI_H_

#include <string>
#include "base/kctrluibase.h"

namespace TinyDui 
{
// 静态文本控件
class KStaticUI : public KCtrlUIBase
{
public:

	KStaticUI();
	virtual ~KStaticUI();

public:

	void SetText(const std::wstring &strText);
	std::wstring GetText() const;

	void SetBackGroundImage(const std::wstring& strImgFile);
	void SetBackGroundImage(Gdiplus::Image *pBackGroundImage);

	void SetTextColor(COLORREF clrText);
	COLORREF GetTextColor() const;

	void SetTextAlign(DWORD dwAlign);
	DWORD GetTextAlign() const;

public:

	virtual void Draw(HDC hMemDC);

	virtual void Hover();
	virtual void Leave();
	virtual bool IsHover();

	virtual void OnSize();
	virtual void OnLButtonDown(const POINT &pt);
	virtual void OnMouseMove(const POINT &pt);
	virtual void OnLButtonUp(const POINT &pt);

	virtual void Click();

protected:

	std::wstring m_strText;
	Gdiplus::Image *m_pBackGroundImage;

	COLORREF m_txtClr;
	DWORD m_dwTextAlign;
};

// 静态文本带背景控件; 增加下划线和背景色设置
// 此外支持拖拽窗口
class KStaticBackGroundUI : public KStaticUI
{
public:

	KStaticBackGroundUI();
	virtual ~KStaticBackGroundUI();

public:

	void SetBackGroundColor(COLORREF clrBackGround);
	COLORREF GetBackGroundColor() const;

	void SetUnderLine(bool bUnderLine);
	bool IsUnderLine() const;

	void SetUnderLineWidth(int nWidth);
	int GetLineWidth() const;

	void SetUnderLineColor(COLORREF clr);
	COLORREF GetUnderLineColor() const;

public:

	virtual void Draw(HDC hMemDC);

	virtual void OnLButtonDown(const POINT &pt);
	virtual void OnMouseMove(const POINT &pt);
	virtual void OnLButtonUp(const POINT &pt);

public:

	virtual void DrawRect(HDC hMemDC);
	virtual void DrawLine(HDC hMemDC);

private:

	COLORREF m_clrBackGround;
	COLORREF m_clrUnderLine;

	bool m_bDown;
	bool m_bMove;
	bool m_bUnderLine;
	POINT m_ptDown;
	int m_nUnderLineWidth;
};
}

#endif // _K_STATIC_UI_H_
