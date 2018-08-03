#ifndef _K_HORIZONTAL_CONTAINER_WND_H_
#define _K_HORIZONTAL_CONTAINER_WND_H_

#include <GdiPlus.h>
#include <string>
#include <deque>
#include "kpopuptipwnd.h"
#include "kctrluibase.h"

// 水平布局容器窗口，目前布局方式采用的是左、中、右窗口区域布局
// TODO: 后期可增加不同的布局方式,或者提供专门的外部不同的布局管理器
namespace TinyDui 
{
class KHorizontalContainerWnd : public KPopupTipWnd, public KUIHandlerCallBack
{
public:

    KHorizontalContainerWnd();
    ~KHorizontalContainerWnd();

public:

	 /*++ SetupUIHandler
	  * 安装外部UI事件回调处理器,若未安装则采用默认的处理器
    --*/
	void SetupUIHandler(
		__in KUIHandlerCallBack* pUIHandler
		);

	 /*++ SetupDefaultUIHandler
	  * 安装默认的处理器
    --*/
	void SetupDefaultUIHandler();

	 /*++ SetWindowAppearance
	  * 设置窗口外观
    --*/
	void SetWindowAppearance(
		__in COLORREF dwBackgroundColor = RGB(0xFF, 0xFF, 0xFF),
		__in COLORREF dwBorderColor = RGB(0xA5, 0xA5, 0xA5),
		__in int nBorderWidth = 1
		);

	/*++
	 * 自动调整窗口大小
	 * nLeftOffset:  窗口左偏移量
	 * nRightOffset: 窗口右偏移量
   --*/
	void AutoAdaptWindowRect(
		__in int nLeftOffset = 0,
		__in int nRightOffset = 0
		);
	void GetAdaptWindowRectOffset(
		__out int &nLeftOffset,
		__out int &nRightOffset
	);

   /*++
	* 设置控件间隙(px为单位)
	* nLeftCtrlSpace:  左窗口控件间隙
	* nCenterCtrlSpace:  中窗口控件间隙
	* nRightCtrlSpace: 右窗口控件间隙
	--*/
	void SetCtrlInternalSpace(
		__in int nLeftCtrlSpace = 12,
		__in int nCenterCtrlSpace = 12,
		__in int nRightCtrlSpace = 12
	);
	void GetCtrlInternalSpace(
		__out int &nLeftCtrlSpace,
		__out int &nCenterCtrlSpace,
		__out int &nRightCtrlSpace
	);

	/*++
	* 设置控件左右边界间隙(px为单位)
	* nLeftCtrlSpace:  左窗口控件间隙
	* nRightCtrlSpace: 右窗口控件间隙
	--*/
	void SetCtrlSpace(
		__in int nLeftSpace = 12,
		__in int nRightSpace = 12
	);
	void GetCtrlSpace(
		__out int& nLeftSpace,
		__out int& nRightSpace
	);

	/*++
	* 追加控件至某窗口的首部/尾部
	--*/
	bool AppendCtrlToHead(
		__in HORIZONTAL_POS eHorizontalPos,
		__in KCtrlUIBase* pCtrlBase
		);
	bool AppendCtrlToTail(
		__in HORIZONTAL_POS eHorizontalPos,
		__in KCtrlUIBase* pCtrlBase
		);

	/*++
	* 是否已存在对应的控件
	--*/
	bool IsExistCtrlByID(__in int nID) const;

	/*++
	* 设置对应控件是否可见
	--*/
	bool SetVisibleByID(__in int nID, __in bool bVisible);
	bool GetVisibleByID(__in int nID, __in bool &bVisible) const;

	/*++
	* 获取相应ID对应的控件
	--*/
	KCtrlUIBase* GetCtrlByID(__in int nID) const;
	
	/*++
	* 移除并返回相应ID对应的控件,返回的控件由外部维护生命周期
	--*/
	KCtrlUIBase* RemoveCtrlByID(__in int nID);

	/*++
	* 设置相应ID对应的控件为默认控件
	--*/
	bool SetDefalutCtrlByID(__in int nID);

	/*++
	* 设置控件绘制字体
	--*/
	bool SetCtrlFont(
		__in int    cHeight,
		__in int    cWidth = 0,
		__in int    cEscapement = 0,
		__in int    cOrientation = 0,
		__in int    cWeight = FW_NORMAL,
		__in DWORD  bItalic = FALSE,
		__in DWORD  bUnderline = FALSE,
		__in DWORD  bStrikeOut = FALSE,
		__in DWORD  iCharSet = ANSI_CHARSET,
		__in DWORD  iOutPrecision = OUT_DEFAULT_PRECIS,
		__in DWORD  iClipPrecision = CLIP_DEFAULT_PRECIS,
		__in DWORD  iQuality = DEFAULT_QUALITY,
		__in DWORD  iPitchAndFamily = DEFAULT_PITCH | FF_SWISS,
		__in LPCWSTR pszFaceName = L"微软雅黑"
	);
	HFONT GetCtrlFont() const;

public:

	LONG AddRef()
	{
		return ::InterlockedIncrement(&m_lRefCount);
	}

	LONG DecRef()
	{
		LONG lCnt = ::InterlockedDecrement(&m_lRefCount);
		if (lCnt == 0)
		{
			delete this;
		}
		return lCnt;
	}

private:

	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetCursor(WPARAM wParam, LPARAM lParam, BOOL &bHandle);

private:

	// 容器内控件响应处理接口
	virtual void UINotify(NotifyUIMsg &notifyUIMsg);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnSize(WPARAM wParam, LPARAM lParam);
	
	virtual void InitResources();
	virtual void InitControls();
	virtual void ReleaseResources();

	virtual void DrawBackground(HDC hDC);
	virtual void DrawOthers(HDC hDC);
	virtual void DrawControls(HDC hDC);

private:

	// 控件包装对象
	struct KCtrlWrapper
	{
		int m_nCtrlID;
		bool m_bEnable;
		KCtrlUIBase* m_pCtrlObj;

		KCtrlWrapper(
			int nCtrlID,
			bool bEnable,
			KCtrlUIBase* pCtrlObj
			) : m_nCtrlID(nCtrlID)
			, m_bEnable(bEnable)
			, m_pCtrlObj(pCtrlObj)
		{}

		KCtrlWrapper(const KCtrlWrapper& other)
		{
			memcpy(this, &other, sizeof(other));
		}

		KCtrlWrapper& operator= (const KCtrlWrapper& other)
		{
			if (&other != this)
			{
				memcpy(this, &other, sizeof(other));
			}
			return *this;
		}
	};

private:

	KCtrlUIBase* HitCtrl(POINT pt) const;

	void SmartAdjustCtrlsLayout(HDC hDC);
	void ReleaseCtrls();
	LONG GetCtrlsMaxHeight();
	LONG GetAllLeftCtrlsWidth();
	LONG GetAllCenterCtrlsWidth();
	LONG GetAllRightCtrlsWidth();
	bool IsExistCtrl(
		int nID, 
		const std::deque<KCtrlWrapper>& dequeWrapper
		) const;
	bool SetVisible(
		int nID, 
		bool bVisible,
		const std::deque<KCtrlWrapper>& dequeWrapper
		);
	bool GetVisible(
		int nID,
		bool &bVisible,
		const std::deque<KCtrlWrapper>& dequeWrapper
		) const;
	KCtrlUIBase* GetCtrl(
		int nID,
		const std::deque<KCtrlWrapper>& dequeWrapper
		) const;
	KCtrlUIBase* RemoveCtrl(
		int nID,
		std::deque<KCtrlWrapper>& dequeWrapper
		);
	bool SetDefalutCtrl(
		int nID,
		const std::deque<KCtrlWrapper>& dequeWrapper
		);
	KCtrlUIBase* HitCtrl(
		POINT pt,
		const std::deque<KCtrlWrapper>& dequeWrapper
		) const;
	void ReleaseCtrls(std::deque<KCtrlWrapper>& dequeWrapper);
	void SetUIHandler(
		KUIHandlerCallBack* pUIHandler,
		std::deque<KCtrlWrapper>& dequeWrapper
		);

private:

	KCtrlUIBase *m_pHoverCtrl;
	KCtrlUIBase *m_pPressDownCtrl;
	KCtrlUIBase *m_pDefaultCtrl;

private:

	COLORREF m_dwBackgroundColor;
	COLORREF m_dwBorderColor;
	int m_nBorderWidth;
	HFONT m_hCtrlFont;

	int m_nLeftOffset;
	int m_nRightOffset;

	int m_nLeftSpace;
	int m_nRightSpace;
	int m_nLeftCtrlSpace;
	int m_nCenterCtrlSpace;
	int m_nRightCtrlSpace;

	std::deque<KCtrlWrapper> m_dequeLeftCtrlWrapper;
	std::deque<KCtrlWrapper> m_dequeCenterCtrlWrapper;
	std::deque<KCtrlWrapper> m_dequeRightCtrlWrapper;

	LONG m_lRefCount;
	bool m_bTrackMouse;

	KUIHandlerCallBack* m_pUIHandler;
};
}

#endif  //_K_HORIZONTAL_CONTAINER_WND_H_
