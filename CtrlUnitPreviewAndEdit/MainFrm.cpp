
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullScreen)
END_MESSAGE_MAP()

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	// TODO: メンバー初期化コードをここに追加してください。
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	CRect rcDesktopRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktopRect, 0);

	cs.x = rcDesktopRect.left + 50;								// x 座標 表示位置
	cs.y = rcDesktopRect.top + 50;								// y 座標 表示位置
	cs.cx = (rcDesktopRect.right - rcDesktopRect.left) - 100;	// 幅
	cs.cy = (rcDesktopRect.bottom - rcDesktopRect.top) / 4;      // 高さ

	return TRUE;
}

// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame メッセージ ハンドラー





int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	m_fFullScreen = FALSE;

	return 0;
}


void CMainFrame::OnViewFullScreen()
{
	if (m_fFullScreen)
	{
		ASSERT(!m_rcWindowSize.IsRectEmpty());
		this->MoveWindow(m_rcWindowSize.left, m_rcWindowSize.top, m_rcWindowSize.Width(), m_rcWindowSize.Height());
		SetWindowLong(this->m_hWnd, GWL_STYLE, m_dwStyle);
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, m_dwExStyle);

		CMenu mMenu;
		mMenu.LoadMenu(IDR_CtrlUnitPreviewTYPE);
		SetMenu(&mMenu);
		DeleteObject(mMenu);

		SetWindowPos(&wndNoTopMost, m_rcWindowSize.left, m_rcWindowSize.top, m_rcWindowSize.Width(), m_rcWindowSize.Height(), SWP_FRAMECHANGED);
		
		m_rcWindowSize.SetRectEmpty();
		m_fFullScreen = FALSE;
	}
	else
	{
		HMONITOR hm;
		MONITORINFO mi;
		GetWindowRect(&m_rcWindowSize);
		RECT rect;
		rect = static_cast<RECT>(m_rcWindowSize);
		hm = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(hm, &mi);
		const RECT& rcMonitor = mi.rcMonitor;

		SetMenu(NULL);
		m_dwStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);
		SetWindowLong(this->m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

		m_dwExStyle = GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
		DWORD dwExStyle = m_dwExStyle;
		dwExStyle &= ~WS_EX_WINDOWEDGE;
		dwExStyle &= ~WS_EX_CLIENTEDGE;		
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, dwExStyle);

		SetWindowPos(&wndTopMost, rcMonitor.left, rcMonitor.top, rcMonitor.right - rcMonitor.left, rcMonitor.bottom - rcMonitor.top, SWP_FRAMECHANGED);

		//CMDIChildWnd* pWnd = MDIGetActive();
		//CView* pView = pWnd->GetActiveView();
		//CRect wrect;
		//pView->GetClientRect(&wrect);
		//CString str;
		//str.Format(_T("L %d T %d R %d B %d"), wrect.left, wrect.top, wrect.right, wrect.bottom);
		//OutputDebugString(str + "\n");

		m_fFullScreen = TRUE;
	}
}
