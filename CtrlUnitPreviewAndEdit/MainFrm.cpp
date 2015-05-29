
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
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
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

