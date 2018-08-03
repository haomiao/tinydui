#ifndef _K_SPRING_CHECKBOX_UI_H_
#define _K_SPRING_CHECKBOX_UI_H_

#include <string>
#include "kimagecheckboxui.h"

namespace TinyDui 
{
// 简易弹簧checkbox控件
class KSpringCheckBoxUI : public KImageCheckBoxUI
{
public:

	KSpringCheckBoxUI();
	virtual ~KSpringCheckBoxUI();

public:

	void SetExpandImage(Gdiplus::Image *pExpandImage);
	void SetShrinkImage(Gdiplus::Image *pShrinkImage);
	void SetSpringHoverBackGroundColor(COLORREF clrHoverSpringBackGround);
	COLORREF GetSpringHoverBackGroundColor() const;

	void SetSpringSpace(int nSpringSpace);
	int GetSpringSpace() const;

public:

	virtual void Draw(HDC hMemDC);
	virtual void Click();
	virtual bool IsEnableDefault();

private:

	void DrawImage(HDC hMemDC, const RECT& rcImage);
	int GetCurImageWidth(bool bExpand);
	bool IsHitSpring(const POINT& pt);

private:

	Gdiplus::Image *m_pExpandImage;
	Gdiplus::Image *m_pShrinkImage;
	bool m_bExpand;
	RECT m_rcOrgin;
	int m_nSpringSpace;
	COLORREF m_clrHoverSpringBackGround;
};
}

#endif // _K_SPRING_CHECKBOX_UI_H_
