#ifndef _K_IMAGE_CHECKBOX_UI_H_
#define _K_IMAGE_CHECKBOX_UI_H_

#include <string>
#include "kbuttonui.h"


// 简易图片checkbox控件
namespace TinyDui 
{
class KImageCheckBoxUI : public KButtonUI
{
public:

	KImageCheckBoxUI();
	virtual ~KImageCheckBoxUI();

public:

	void SetChecked(bool bChecked);
	bool GetChecked() const;

	void SetUnCheckedImage(Gdiplus::Image *pUnCheckedImage);
	void SetCheckedImage(Gdiplus::Image *pCheckedImage);
	void SetUnCheckedHoverImage(Gdiplus::Image *pUnCheckedHoverImage);
	void SetCheckedHoverImage(Gdiplus::Image *pCheckedHoverImage);
	void SetTagImage(Gdiplus::Image *pTagImage);

	void Click(const RECT& rcCtrl);
	void DrawCtrl(HDC hMemDC, const RECT& rcDraw);

	void SetTagImageTextSpace(int nTagImageTextSpace);
	int GetTagImageTextSpace() const;

	void SetCheckImageTextSpace(int nCheckImageTextSpace);
	int GetCheckImageTextSpace() const;

public:

	virtual void Draw(HDC hMemDC);
	virtual void Click();
	virtual bool IsEnableDefault();

private:

	void DrawTitleText(HDC hMemDC, const RECT& rcText);
	void DrawImage(HDC hMemDC, const RECT& rcImage);
	bool IsHitCheck(const POINT& pt, const RECT& rcCtrl);
	void AutoCheck();

private:

	bool m_bChecked;
	Gdiplus::Image *m_pCheckedImage;
	Gdiplus::Image *m_pTagImage;
	Gdiplus::Image *m_pUnCheckedHoverImage;
	Gdiplus::Image *m_pCheckedHoverImage;
	int m_nTagImageTextSpace;
	int m_nCheckImageTextSpace;
};
}

#endif // _K_IMAGE_CHECKBOX_UI_H_
