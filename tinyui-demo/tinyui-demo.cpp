// tinyui-demo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "tinyui-demo.h"

#include <GdiPlus.h>
#include "../tinydui/tinydui.h"
#include "kresourceutil.h"


extern HINSTANCE g_hResourceInstance = 0;

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND g_hWndHandle = NULL;

// UI事件回调处理器
class UIHandler : public TinyDui::KUIHandlerCallBack
{
public:

	UIHandler(){};
	virtual ~UIHandler(){};

public:

	// 容器内控件响应处理接口
	virtual void UINotify(TinyDui::NotifyUIMsg &notifyUIMsg)
	{
		if (notifyUIMsg.nMsgType == TinyDui::UI_MSG_CLICK)
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
				::PostMessage(
					notifyUIMsg.pSender->GetOwnerWndHandle(),
					WM_CLOSE,
					WPARAM(nID),
					0
					);
				return;
			}
		}
	}
};

TinyDui::KHorizontalContainerWnd *pHContainerWnd = nullptr;
TinyDui::KHorizontalContainerWnd *pHContainerWnd2 = nullptr;
TinyDui::KVerticalContainerWnd *pVContainerWnd = nullptr;

UIHandler *pgUIHandler = new UIHandler();

void InitHInternalTitleBar(HWND hWnd);
void InitHInternalTitleBar2(HWND hWnd);
void InitVInternalTitleBar(HWND hWnd);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_hResourceInstance = hInstance;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TINYUIDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	g_hWndHandle = InitInstance (hInstance, nCmdShow);
	if (!g_hWndHandle)
	{
		return 0;
	}

	InitHInternalTitleBar(g_hWndHandle);
	InitHInternalTitleBar2(g_hWndHandle);
	InitVInternalTitleBar(g_hWndHandle);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TINYUIDEMO));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return (int) msg.wParam;
}

void InitHInternalTitleBar(HWND hWnd)
{
	if (!pHContainerWnd)
	{
		double nDpiScale = TinyDui::GetCurrentDpiValue();
		pHContainerWnd = new TinyDui::KHorizontalContainerWnd();
		pHContainerWnd->SetWindowAppearance(
			RGB(242, 242, 242),
			RGB(0xFF, 0xF6, 0xDC),
			1
			);
		RECT rcWnd = {0};
		::GetClientRect(hWnd, &rcWnd);
		pHContainerWnd->CreateWnd(
			hWnd,
			L"ContainerWnd",
			L"",
			WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
			0,
			0,
			rcWnd.right - rcWnd.left,
			60 * nDpiScale,
			nDpiScale
			);

		// TODO: 其他容器窗口的初始化操作

		// 测试代码
		int index = 100;

		{
			Gdiplus::Image *pCheckImage = nullptr;
			Gdiplus::Image *pUnCheckImage = nullptr;
			Gdiplus::Image *pCheckHoverImage = nullptr;
			Gdiplus::Image *pUnCheckHoverImage = nullptr;
			Gdiplus::Image *pTagImage = nullptr;
			Gdiplus::Image *pExpandImage = nullptr;
			Gdiplus::Image *pShrinkImage = nullptr;
			TinyDui::GetResourceImage(IDB_PNG5, L"PNG", &pCheckImage);
			TinyDui::GetResourceImage(IDB_PNG4, L"PNG", &pUnCheckImage);
			TinyDui::GetResourceImage(IDB_PNG5, L"PNG", &pCheckHoverImage);
			TinyDui::GetResourceImage(IDB_PNG4, L"PNG", &pUnCheckHoverImage);
			TinyDui::GetResourceImage(IDB_PNG3, L"PNG", &pTagImage);
			TinyDui::GetResourceImage(IDB_PNG1, L"PNG", &pExpandImage);
			TinyDui::GetResourceImage(IDB_PNG2, L"PNG", &pShrinkImage);
			TinyDui::KSpringCheckBoxUI *pSpringCheckBox = new TinyDui::KSpringCheckBoxUI();
			RECT rcCtrl = {0, 0, 120, 20};
			rcCtrl = pSpringCheckBox->GetDPIRect(rcCtrl);
			pSpringCheckBox->SetRect(rcCtrl);
			pSpringCheckBox->SetUnCheckedImage(pUnCheckImage);
			pSpringCheckBox->SetCheckedImage(pCheckImage);
			pSpringCheckBox->SetTagImage(pTagImage);
			pSpringCheckBox->SetUnCheckedHoverImage(pUnCheckHoverImage);
			pSpringCheckBox->SetCheckedHoverImage(pCheckHoverImage);
			pSpringCheckBox->SetExpandImage(pExpandImage);
			pSpringCheckBox->SetShrinkImage(pShrinkImage);
			pSpringCheckBox->SetText(L"智能同步");
			pSpringCheckBox->SetCtrlID(index++);
			pSpringCheckBox->SetOwnerWndHandle(pHContainerWnd->GetWindowHandle());
			pHContainerWnd->AppendCtrlToHead(TinyDui::HORIZONTAL_LEFT, pSpringCheckBox);
		}
		
		{
			TinyDui::KSpacerUI *pSpacer = new TinyDui::KSpacerUI();
			RECT rcCtrl = {0, 0, 10, 20};
			rcCtrl = pSpacer->GetDPIRect(rcCtrl);
			pSpacer->SetRect(rcCtrl);
			pSpacer->SetSpacerType(TinyDui::KSpacerUI::SPACER_VERTICAL);
			pSpacer->SetSpacerLineWidth(2);
			pSpacer->SetSpacerLineColor(RGB(156, 156, 156));
			pSpacer->SetCtrlID(index++);
			pSpacer->SetOwnerWndHandle(pHContainerWnd->GetWindowHandle());
			pHContainerWnd->AppendCtrlToTail(TinyDui::HORIZONTAL_LEFT, pSpacer);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			RECT rcCtrl = {0, 0, 80, 25};
			rcCtrl = pBtn->GetDPIRect(rcCtrl);
			pBtn->SetRect(rcCtrl);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"最近使用");
			pBtn->SetOwnerWndHandle(pHContainerWnd->GetWindowHandle());
			pHContainerWnd->AppendCtrlToTail(TinyDui::HORIZONTAL_LEFT, pBtn);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			RECT rcCtrl = {0, 0, 80, 25};
			rcCtrl = pBtn->GetDPIRect(rcCtrl);
			pBtn->SetRect(rcCtrl);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"云端备份");
			pBtn->SetOwnerWndHandle(pHContainerWnd->GetWindowHandle());
			pHContainerWnd->AppendCtrlToTail(TinyDui::HORIZONTAL_RIGHT, pBtn);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			RECT rcCtrl = {0, 0, 80, 25};
			rcCtrl = pBtn->GetDPIRect(rcCtrl);
			pBtn->SetRect(rcCtrl);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"团队文档");
			pBtn->SetOwnerWndHandle(pHContainerWnd->GetWindowHandle());
			pHContainerWnd->AppendCtrlToTail(TinyDui::HORIZONTAL_RIGHT, pBtn);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			RECT rcCtrl = {0, 0, 80, 25};
			rcCtrl = pBtn->GetDPIRect(rcCtrl);
			pBtn->SetRect(rcCtrl);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"应用");
			pBtn->SetOwnerWndHandle(pHContainerWnd->GetWindowHandle());
			pHContainerWnd->AppendCtrlToTail(TinyDui::HORIZONTAL_RIGHT, pBtn);
		}

		{
			Gdiplus::Image *pDropDownImage = nullptr;
			TinyDui::GetResourceImage(IDB_PNG11, L"PNG", &pDropDownImage);

			TinyDui::KComboBoxUI *pComboBox = 
				new TinyDui::KComboBoxUI(pHContainerWnd->GetWindowHandle());
			RECT rcCtrl = {0, 0, 90, 25};
			rcCtrl = pComboBox->GetDPIRect(rcCtrl);
			pComboBox->SetRect(rcCtrl);
			pComboBox->SetCtrlID(index++);
			pComboBox->SetBackGroundColor(RGB(255, 255, 255));
			pComboBox->SetBackGroundDownColor(RGB(168, 212, 235));
			pComboBox->SetHoverColor(RGB(202,233, 249));
			pComboBox->SetBorderColor(RGB(156, 156, 156));
			pComboBox->SetTextColor(RGB(0, 0, 0));
			pComboBox->SetDropDownImage(pDropDownImage);

			TinyDui::KMenuWnd::MenuItemPair menuItem;
			menuItem.nMenuID = 500;
			menuItem.nIconID = -1;
			menuItem.nOffset = (UINT)(16 * nDpiScale);
			menuItem.strMenu = L"回收站";
			pComboBox->AppendMenuItem(menuItem);

			menuItem.nMenuID = 501;
			menuItem.nIconID = -1;
			menuItem.nOffset = (UINT)(16 * nDpiScale);
			menuItem.strMenu = L"与我共享";
			pComboBox->AppendMenuItem(menuItem);

			menuItem.nMenuID = 502;
			menuItem.nIconID = -1;
			menuItem.nOffset = (UINT)(16 * nDpiScale);
			menuItem.strMenu = L"其他";
			pComboBox->AppendMenuItem(menuItem);

			pComboBox->SetCurSelItem(0);

			pHContainerWnd->AppendCtrlToTail(TinyDui::HORIZONTAL_RIGHT, pComboBox);
		}

		pHContainerWnd->SetCtrlSpace(20, 10);
		pHContainerWnd->SetCtrlInternalSpace(12, 20);
		pHContainerWnd->SetupUIHandler(pgUIHandler);
	}

	if (pHContainerWnd)
	{
		pHContainerWnd->ShowWindow(TRUE);
	}
}

void InitHInternalTitleBar2(HWND hWnd)
{
	if (!pHContainerWnd2)
	{
		double nDpiScale = TinyDui::GetCurrentDpiValue();
		pHContainerWnd2 = new TinyDui::KHorizontalContainerWnd();
		pHContainerWnd2->SetWindowAppearance(
			RGB(242, 242, 242),
			RGB(0xFF, 0xF6, 0xDC),
			1
			);
		RECT rcWnd = {0};
		::GetClientRect(hWnd, &rcWnd);
		pHContainerWnd2->CreateWnd(
			hWnd,
			L"ContainerWnd",
			L"",
			WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
			0,
			90 * nDpiScale,
			rcWnd.right - rcWnd.left,
			50 * nDpiScale,
			nDpiScale
			);

		// TODO: 其他容器窗口的初始化操作

		// 测试代码
		int index = 200;

		{
			Gdiplus::Image *pNormalImage = nullptr;
			Gdiplus::Image *pHoverImage = nullptr;
			TinyDui::GetResourceImage(IDB_PNG6, L"PNG", &pNormalImage);
			TinyDui::GetResourceImage(IDB_PNG6, L"PNG", &pHoverImage);

			TinyDui::KImageButtonUI *pBtn = new TinyDui::KImageButtonUI;
			pBtn->SetRect(0, 0, 25, 25);
			pBtn->SetCtrlID(index++);
			pBtn->SetNormalImage(pNormalImage);
			pBtn->SetHoverImage(pHoverImage);
			pBtn->SetOwnerWndHandle(pHContainerWnd2->GetWindowHandle());
			pHContainerWnd2->AppendCtrlToTail(TinyDui::HORIZONTAL_RIGHT, pBtn);
		}

		{
			Gdiplus::Image *pNormalImage = nullptr;
			Gdiplus::Image *pHoverImage = nullptr;
			TinyDui::GetResourceImage(IDB_PNG8, L"PNG", &pNormalImage);
			TinyDui::GetResourceImage(IDB_PNG8, L"PNG", &pHoverImage);

			TinyDui::KImageButtonUI *pBtn = new TinyDui::KImageButtonUI;
			pBtn->SetRect(0, 0, 100, 25);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(254, 156, 17));
			pBtn->SetBackGroundDownColor(RGB(237, 137, 1));
			pBtn->SetHoverColor(RGB(234, 147, 1));
			pBtn->SetBorderColor(RGB(254, 147, 1));
			pBtn->SetTextColor(RGB(255, 255, 255));
			pBtn->SetText(L"升级会员");
			pBtn->SetNormalImage(pNormalImage);
			pBtn->SetHoverImage(pHoverImage);
			pBtn->SetOwnerWndHandle(pHContainerWnd2->GetWindowHandle());
			pHContainerWnd2->AppendCtrlToHead(TinyDui::HORIZONTAL_RIGHT, pBtn);
		}

		{
			Gdiplus::Image *pCheckImage = nullptr;
			Gdiplus::Image *pUnCheckImage = nullptr;
			Gdiplus::Image *pCheckHoverImage = nullptr;
			Gdiplus::Image *pUnCheckHoverImage = nullptr;
			TinyDui::GetResourceImage(IDB_PNG10, L"PNG", &pCheckImage);
			TinyDui::GetResourceImage(IDB_PNG9, L"PNG", &pUnCheckImage);
			TinyDui::GetResourceImage(IDB_PNG10, L"PNG", &pCheckHoverImage);
			TinyDui::GetResourceImage(IDB_PNG9, L"PNG", &pUnCheckHoverImage);
			TinyDui::KCheckBoxUI *pCheckBox = new TinyDui::KCheckBoxUI();
			pCheckBox->SetRect(
				0,
				0,
				pCheckImage->GetWidth() * nDpiScale + 120,
				pCheckImage->GetHeight() * nDpiScale + 20
				);
			pCheckBox->SetUnCheckedImage(pUnCheckImage);
			pCheckBox->SetCheckedImage(pCheckImage);
			pCheckBox->SetUnCheckedHoverImage(pUnCheckHoverImage);
			pCheckBox->SetCheckedHoverImage(pCheckHoverImage);
			pCheckBox->SetTextColor(RGB(0, 0, 0));
			pCheckBox->SetTextSelectColor(RGB(0, 255, 0));
			pCheckBox->SetText(L"此信息不再提示");
			pCheckBox->SetCtrlID(index++);
			pCheckBox->SetOwnerWndHandle(pHContainerWnd2->GetWindowHandle());
			pHContainerWnd2->AppendCtrlToHead(TinyDui::HORIZONTAL_LEFT, pCheckBox);
		}

		{
			TinyDui::KStaticUI *pStatic = new TinyDui::KStaticUI();
			pStatic->SetRect(
				0,
				0,
				200 * nDpiScale,
				30 * nDpiScale
				);
			pStatic->SetCtrlID(index++);
			pStatic->SetText(L"超出上传限制，请提升会员等级");
			pStatic->SetTextColor(RGB(0, 0, 0));
			pStatic->SetOwnerWndHandle(pHContainerWnd2->GetWindowHandle());
			pHContainerWnd2->AppendCtrlToTail(TinyDui::HORIZONTAL_CENTER, pStatic);
		}

		{
			TinyDui::KHyperTextUI *pHyperText = new TinyDui::KHyperTextUI();
			pHyperText->SetRect(
				0,
				0,
				160 * nDpiScale,
				30 * nDpiScale
				);
			pHyperText->SetText(L"2018秋游计划...等4个文件");
			pHyperText->SetHoverColor(RGB(192, 125, 44));
			pHyperText->SetOwnerWndHandle(pHContainerWnd2->GetWindowHandle());
			pHyperText->SetCtrlID(index++);
			pHyperText->SetTextColor(RGB(213, 146, 67));
			pHyperText->SetDrawUnderLine(false);
			pHyperText->SetClickedRecordTag(false);
			pHContainerWnd2->AppendCtrlToHead(TinyDui::HORIZONTAL_CENTER, pHyperText);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			RECT rcCtrl = {0, 0, 80, 25};
			pBtn->SetRect(rcCtrl);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"团队文档");
			pBtn->SetOwnerWndHandle(pHContainerWnd2->GetWindowHandle());
			pHContainerWnd2->AppendCtrlToHead(TinyDui::HORIZONTAL_RIGHT, pBtn);
		}

		pHContainerWnd2->SetCtrlSpace(10, 10);
		pHContainerWnd2->SetCtrlInternalSpace(12, 3, 12);
		pHContainerWnd2->SetupUIHandler(pgUIHandler);
	}

	if (pHContainerWnd2)
	{
		pHContainerWnd2->ShowWindow(TRUE);
	}
}

void InitVInternalTitleBar(HWND hWnd)
{
	if (!pVContainerWnd)
	{
		double nDpiScale = TinyDui::GetCurrentDpiValue();
		pVContainerWnd = new TinyDui::KVerticalContainerWnd();
		pVContainerWnd->SetWindowAppearance(
			RGB(242, 242, 242),
			RGB(0xFF, 0xF6, 0xDC),
			1
			);
		RECT rcWnd = {0};
		::GetClientRect(hWnd, &rcWnd);
		pVContainerWnd->CreateWnd(
			hWnd,
			L"ContainerWnd",
			L"",
			WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
			10,
			150 * nDpiScale,
			200 * nDpiScale,
			rcWnd.bottom - rcWnd.top - 150 * nDpiScale,
			nDpiScale
			);

		// TODO: 其他容器窗口的初始化操作

		// 测试代码
		int index = 300;

		{
			Gdiplus::Image *pCheckImage = nullptr;
			Gdiplus::Image *pUnCheckImage = nullptr;
			Gdiplus::Image *pCheckHoverImage = nullptr;
			Gdiplus::Image *pUnCheckHoverImage = nullptr;
			Gdiplus::Image *pTagImage = nullptr;
			Gdiplus::Image *pExpandImage = nullptr;
			Gdiplus::Image *pShrinkImage = nullptr;
			TinyDui::GetResourceImage(IDB_PNG5, L"PNG", &pCheckImage);
			TinyDui::GetResourceImage(IDB_PNG4, L"PNG", &pUnCheckImage);
			TinyDui::GetResourceImage(IDB_PNG5, L"PNG", &pCheckHoverImage);
			TinyDui::GetResourceImage(IDB_PNG4, L"PNG", &pUnCheckHoverImage);
			TinyDui::GetResourceImage(IDB_PNG3, L"PNG", &pTagImage);
			TinyDui::GetResourceImage(IDB_PNG1, L"PNG", &pExpandImage);
			TinyDui::GetResourceImage(IDB_PNG2, L"PNG", &pShrinkImage);
			TinyDui::KSpringCheckBoxUI *pSpringCheckBox = new TinyDui::KSpringCheckBoxUI();
			RECT rcCtrl = {0, 0, 150, 20};
			rcCtrl = pSpringCheckBox->GetDPIRect(rcCtrl);
			pSpringCheckBox->SetRect(rcCtrl);
			pSpringCheckBox->SetUnCheckedImage(pUnCheckImage);
			pSpringCheckBox->SetCheckedImage(pCheckImage);
			pSpringCheckBox->SetTagImage(pTagImage);
			pSpringCheckBox->SetUnCheckedHoverImage(pUnCheckHoverImage);
			pSpringCheckBox->SetCheckedHoverImage(pCheckHoverImage);
			pSpringCheckBox->SetExpandImage(pExpandImage);
			pSpringCheckBox->SetShrinkImage(pShrinkImage);
			pSpringCheckBox->SetText(L"智能同步");
			pSpringCheckBox->SetCtrlID(index++);
			pSpringCheckBox->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pVContainerWnd->AppendCtrlToHead(TinyDui::VERTICAL_TOP, pSpringCheckBox);
		}

		{
			TinyDui::KSpacerUI *pSpacer = new TinyDui::KSpacerUI();
			pSpacer->SetRect(
				0,
				0,
				40 * nDpiScale,
				20 * nDpiScale
				);
			pSpacer->SetSpacerType(TinyDui::KSpacerUI::SPACER_HORIZONTAL);
			pSpacer->SetSpacerLineWidth(2);
			pSpacer->SetSpacerLineColor(RGB(156, 156, 156));
			pSpacer->SetCtrlID(index++);
			pSpacer->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pVContainerWnd->AppendCtrlToTail(TinyDui::VERTICAL_TOP, pSpacer);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			pBtn->SetRect(0, 0, 80, 25);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"最近使用");
			pBtn->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pVContainerWnd->AppendCtrlToTail(TinyDui::VERTICAL_TOP, pBtn);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			pBtn->SetRect(0, 0, 80, 25);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"云端备份");
			pBtn->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pVContainerWnd->AppendCtrlToTail(TinyDui::VERTICAL_BOTTOM, pBtn);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			pBtn->SetRect(0, 0, 80, 25);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"团队文档");
			pBtn->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pVContainerWnd->AppendCtrlToTail(TinyDui::VERTICAL_BOTTOM, pBtn);
		}

		{
			TinyDui::KButtonUI *pBtn = new TinyDui::KButtonUI;
			pBtn->SetRect(0, 0, 80, 25);
			pBtn->SetCtrlID(index++);
			pBtn->SetBackGroundColor(RGB(255, 255, 255));
			pBtn->SetBackGroundDownColor(RGB(168, 212, 235));
			pBtn->SetHoverColor(RGB(202,233, 249));
			pBtn->SetBorderColor(RGB(156, 156, 156));
			pBtn->SetTextColor(RGB(0, 0, 0));
			pBtn->SetText(L"应用");
			pBtn->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pVContainerWnd->AppendCtrlToTail(TinyDui::VERTICAL_BOTTOM, pBtn);
		}

		{
			Gdiplus::Image *pDropDownImage = nullptr;
			TinyDui::GetResourceImage(IDB_PNG11, L"PNG", &pDropDownImage);

			TinyDui::KComboBoxUI *pComboBox = 
				new TinyDui::KComboBoxUI(pVContainerWnd->GetWindowHandle());
			pComboBox->SetRect(0, 0, 80, 25);
			pComboBox->SetCtrlID(index++);
			pComboBox->SetBackGroundColor(RGB(255, 255, 255));
			pComboBox->SetBackGroundDownColor(RGB(168, 212, 235));
			pComboBox->SetHoverColor(RGB(202,233, 249));
			pComboBox->SetBorderColor(RGB(156, 156, 156));
			pComboBox->SetTextColor(RGB(0, 0, 0));
			pComboBox->SetDropDownImage(pDropDownImage);

			TinyDui::KMenuWnd::MenuItemPair menuItem;
			menuItem.nMenuID = 600;
			menuItem.nIconID = -1;
			menuItem.nOffset = (UINT)(16 * nDpiScale);
			menuItem.strMenu = L"回收站";
			pComboBox->AppendMenuItem(menuItem);

			menuItem.nMenuID = 601;
			menuItem.nIconID = -1;
			menuItem.nOffset = (UINT)(16 * nDpiScale);
			menuItem.strMenu = L"与我共享";
			pComboBox->AppendMenuItem(menuItem);

			menuItem.nMenuID = 602;
			menuItem.nIconID = -1;
			menuItem.nOffset = (UINT)(16 * nDpiScale);
			menuItem.strMenu = L"其他";
			pComboBox->AppendMenuItem(menuItem);

			pComboBox->SetCurSelItem(0);

			pVContainerWnd->AppendCtrlToTail(TinyDui::VERTICAL_BOTTOM, pComboBox);
		}

		{
			TinyDui::KStaticUI *pStatic = new TinyDui::KStaticUI();
			pStatic->SetRect(
				0,
				0,
				90 * nDpiScale,
				30 * nDpiScale
				);
			pStatic->SetCtrlID(index++);
			pStatic->SetText(L"超出上传限制");
			pStatic->SetTextColor(RGB(0, 0, 0));
			pStatic->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pVContainerWnd->AppendCtrlToTail(TinyDui::VERTICAL_CENTER, pStatic);
		}

		{
			TinyDui::KHyperTextUI *pHyperText = new TinyDui::KHyperTextUI();
			pHyperText->SetRect(
				0,
				0,
				100 * nDpiScale,
				30 * nDpiScale
				);
			pHyperText->SetText(L"2018秋游计划...");
			pHyperText->SetHoverColor(RGB(192, 125, 44));
			pHyperText->SetOwnerWndHandle(pVContainerWnd->GetWindowHandle());
			pHyperText->SetCtrlID(index++);
			pHyperText->SetTextColor(RGB(213, 146, 67));
			pHyperText->SetDrawUnderLine(false);
			pHyperText->SetClickedRecordTag(false);
			pVContainerWnd->AppendCtrlToHead(TinyDui::VERTICAL_CENTER, pHyperText);
		}

		pVContainerWnd->SetCtrlSpace(10, 10);
		pVContainerWnd->SetCtrlInternalSpace(12, 20, 12);
		pVContainerWnd->SetupUIHandler(pgUIHandler);
	}

	if (pVContainerWnd)
	{
		pVContainerWnd->ShowWindow(TRUE);
	}
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TINYUIDEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TINYUIDEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return hWnd;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		{
			/*
			RECT rc = {0};
			::GetClientRect(pHContainerWnd->GetParentWindowHandle(), &rc);
			::SetWindowPos(
				pHContainerWnd->GetWindowHandle(),
				NULL, 0, 0, LOWORD(lParam),rc.right - rc.left, SWP_SHOWWINDOW);*/
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
