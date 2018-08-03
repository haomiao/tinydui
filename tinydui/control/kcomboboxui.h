#ifndef _K_COMBOBOX_UI_H_
#define _K_COMBOBOX_UI_H_

#include <deque>
#include "kbuttonui.h"
#include "widget/kmenuwnd.h"


// 简易的ComboBox控件
// 采用组合菜单窗口 + 按钮窗口 方式实现组合框控件
// 对于复杂的带滚动条的则需要增加绘制滚动条或采用COMBOBOX 公共组件来创建包装
namespace TinyDui 
{
class KComboBoxUI : public KButtonUI
{
public:

	explicit KComboBoxUI(HWND hOwnerWndHandle);
	virtual ~KComboBoxUI();

public:

	/*++ SetDropDownImage
	* 设置dropdown背景图片
	--*/
	void SetDropDownImage(Gdiplus::Image *pDropDownImage);
	void SetDropDownBackGroundColor(COLORREF crDropDown);
	COLORREF GetDropDownBackGroundColor() const;

	/*++ InsertMenuItem
	* 指定位置处插入菜单项
	--*/
	bool InsertMenuItem(
		int nIndex,
		const KMenuWnd::MenuItemPair& menuItem
	);

	/*++ AppendMenuItem
	* 追加菜单项
	--*/
	bool AppendMenuItem(const KMenuWnd::MenuItemPair& menuItem);

	/*++ DeleteMenuItem
	* 移除菜单项
	--*/
	bool DeleteMenuItem(const KMenuWnd::MenuItemPair& menuItem);
	void ResetContent();

	/*++ SetCurSelItem/GetCurSelItem
	* 设置/获取选中项菜单项
	--*/
	bool SetCurSelItem(int nIndex);
	int GetCurSelItem() const;
	
	/*++ GetCount
	* 获取菜单项数
	--*/
	int GetCount() const;

private:

	virtual void Draw(HDC hMemDC);
	virtual void Click();

	virtual bool IsEnableDefault() { return false; }

private:

	void InitMenuWnd();
	void DrawText(HDC hMemDC, RECT& rcDraw);
	void DrawDropDownImage(HDC hMemDC, RECT& rcDraw);

private:

	KComboBoxUI();

private:
	
	KMenuWnd m_menuWnd;
	Gdiplus::Image *m_pDropDownImage;
	COLORREF m_clrDropDownBackGround;
	COLORREF m_clrHoverDropDownBackGround;
	int m_nCurSelItem;
};
}

#endif // _K_COMBOBOX_UI_H_
