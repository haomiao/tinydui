#ifndef _K_UI_DEFINE_H_
#define _K_UI_DEFINE_H_

namespace TinyDui
{
class KCtrlUIBase;
}

namespace TinyDui 
{
#define SAFE_DELETE_OBJECT(obj) {\
	if (obj) {\
	::DeleteObject(obj);\
	obj = NULL;\
	}\
}

#define SAFE_DELETE_DC(obj) {\
	if (obj) {\
	::DeleteDC(obj);\
	obj = NULL;\
	}\
}

#define SAFE_FREE(obj) {\
	if (obj) {\
	delete obj;\
	obj = nullptr;\
	}\
}

#define ID_MENUWND_CLOSEMENU 65432

#define INVALID_CTRL_ID   -1

// UI消息类型
enum UI_MSG_TYPE
{
	UI_MSG_CLICK = 0,
	UI_MSG_DBCLICK,
	UI_MSG_TIMER,
	UI_MSG_FOCUS,
	UI_MSG_KILLFOCUS,
	UI_MSG_MENU,
};

// 通告UI消息
struct NotifyUIMsg
{
	int nMsgType;
	KCtrlUIBase* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
};

// 布局类型
enum CONTAINER_LAYOUT_TYPE
{
	LAYOUT_HORIZONTAL = 0,	// 水平布局
	LAYOUT_VERTICAL,		// 垂直布局
	LAYOUT_GRIDS,			// 网格布局
};

// 水平布局窗口位置
enum HORIZONTAL_POS
{
	HORIZONTAL_LEFT = 0,
	HORIZONTAL_CENTER,
	HORIZONTAL_RIGHT,
};

// 垂直布局窗口位置
enum VERTICAL_POS
{
	VERTICAL_TOP = 0,
	VERTICAL_CENTER,
	VERTICAL_BOTTOM,
};

// 
}

#endif	// _KUI_DEFINE_H_
