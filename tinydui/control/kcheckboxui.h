#ifndef _K_CHECKBOX_UI_H_
#define _K_CHECKBOX_UI_H_

#include "kbuttonui.h"

// 简易checkbox控件
namespace TinyDui 
{
class KCheckBoxUI : public KButtonUI
{
public:

	KCheckBoxUI();
	virtual ~KCheckBoxUI();

public:

	void SetChecked(bool bChecked);
	bool GetChecked() const;
	
	void SetCheckImageSpace(int nCheckImageSpace);
	int GetCheckImageSpace() const;

	void SetUnCheckedImage(Gdiplus::Image *pUnCheckedImage);
	void SetCheckedImage(Gdiplus::Image *pCheckedImage);
	void SetUnCheckedHoverImage(Gdiplus::Image *pUnCheckedHoverImage);
	void SetCheckedHoverImage(Gdiplus::Image *pCheckedHoverImage);

public:

	virtual void Draw(HDC hMemDC);
	virtual void DrawText(HDC hMemDC);
	virtual void Click();

	virtual bool IsEnableDefault();

private:

	void AutoCheck();

private:

	bool m_bChecked;
	int m_nCheckImageSpace;
	Gdiplus::Image *m_pCheckedImage;
	Gdiplus::Image *m_pCheckedHoverImage;
	Gdiplus::Image *m_pUnCheckedHoverImage;
	RECT m_rcText;
};
}

#endif // _K_CHECKBOX_UI_H_
