#ifndef _K_SPACER_UI_H_
#define _K_SPACER_UI_H_

#include <string>
#include "base/kctrluibase.h"
#include "kstaticui.h"

namespace TinyDui 
{
// 简易间隔控件,支持默认自绘间隔以及图片间隔
class KSpacerUI : public KStaticUI
{
public:

	enum SPACER_TYPE
	{
		SPACER_HORIZONTAL = 0,
		SPACER_VERTICAL,
	};

public:

	KSpacerUI();
	virtual ~KSpacerUI();

public:

	void SetSpacerType(SPACER_TYPE eSpacerType);
	SPACER_TYPE GetSpacerType();
	void SetSpacerLineWidth(int nLineWidth);
	int GetSpacerLineWidth();
	void SetSpacerLineColor(COLORREF crLine);
	COLORREF GetSpacerLineColor();
	void SetSpacerImage(Gdiplus::Image *pImage);

public:

	virtual void Draw(HDC hMemDC);

private:

	void DrawSpacerLine(HDC hMemDC, const POINT& start, const POINT& end);

private:
	
	Gdiplus::Image *m_pSpacerImage;
	SPACER_TYPE m_spacerType; 
	int m_nLineWidth;
	COLORREF m_crLine;
};
}

#endif // _K_SPACER_UI_H_
