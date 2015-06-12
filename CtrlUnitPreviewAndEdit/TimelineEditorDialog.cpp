// TimelineEditorDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditorDialog.h"
#include "TimelineEditorView.h"
#include "afxdialogex.h"

#include "CtrlUnitPreviewView.h"


// TimelineEditorDialog ダイアログ

IMPLEMENT_DYNAMIC(TimelineEditorDialog, CDialogEx)

TimelineEditorDialog::TimelineEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TimelineEditorDialog::IDD, pParent)
{
	m_pParentWnd = pParent;
}

TimelineEditorDialog::~TimelineEditorDialog()
{
}

void TimelineEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TimelineEditorDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// TimelineEditorDialog メッセージ ハンドラー


// 非表示
void TimelineEditorDialog::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	ShowWindow(SW_HIDE);
	m_pParentWnd->PostMessageW(WM_COMMAND, MAKEWPARAM(ID_HIDE_EDITOR, 0), NULL);
	//CDialogEx::OnClose();
}

// 初期表示位置決定
void TimelineEditorDialog::SetPosition(CRect& rcParentRect)
{
	CRect rcDesktopRect, rcMyRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktopRect, 0);
	int iHeight;
	if ((rcDesktopRect.bottom - rcParentRect.bottom - 50) < 400)
	{
		iHeight = 400;
	}
	else
	{
		iHeight = rcDesktopRect.bottom - rcParentRect.bottom - 50;
	}
	SetWindowPos(&wndTop, rcParentRect.left, rcParentRect.bottom, (rcParentRect.right - rcParentRect.left), iHeight, SWP_NOACTIVATE);
	GetClientRect(&rcMyRect);
	m_pEditorView->MoveWindow(rcMyRect);
}

// 初期設定
BOOL TimelineEditorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	//ModifyStyle(WS_CAPTION, 0, 0);
	ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0, 0);
	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//pSysMenu->AppendMenu(SC_SIZE);

	// フレームウィンドウの表示領域
	CRect rcFrameWndRect;
	GetWindowRect(&rcFrameWndRect);
	ScreenToClient(&rcFrameWndRect);

	m_pEditorView = static_cast<TimelineEditorView*>((RUNTIME_CLASS(TimelineEditorView))->CreateObject());
	m_pEditorView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rcFrameWndRect, this, 0, NULL);
	m_pEditorView->SendMessage(WM_INITIALUPDATE);

	m_pEditorView->SetMainWnd(m_pParentWnd);
	m_pEditorView->SetParentDialog(this);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// サイズ変更
void TimelineEditorDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CRect rcMyRect;
	GetClientRect(rcMyRect);
	//GetWindowRect(&rcMyRect);
	//ScreenToClient(&rcMyRect);
	//rcMyRect.left += 50;
	//rcMyRect.top += 50;
	//rcMyRect.right -= 50;
	//rcMyRect.bottom -= 50;


	m_pEditorView->MoveWindow(rcMyRect);
}

// 背景クリアを無効化
BOOL TimelineEditorDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

// 再生開始
void TimelineEditorDialog::OnPlay(int iSpeed /* = 1*/)
{
	m_pEditorView->OnPlay(iSpeed);
	//PostMessage(WM_COMMAND, MAKEWPARAM(ID_PLAY, 0), NULL);
}

// 再生停止
void TimelineEditorDialog::OnStop(void)
{
	m_pEditorView->OnStop();
}

// タイムラインカーソル位置同期（プレビューから取得）
int TimelineEditorDialog::GetTimelineCursorFrame(void)
{
	return m_pEditorView->GetTimelineCursorFrame();
}

// タイムラインカーソル位置同期（ダイアログから設定）
void TimelineEditorDialog::SetTimelineCursorFrame(int iFrame)
{
	if (m_pMainView)
	{
		m_pMainView->SetTimelineCursorFrame(iFrame);
	}
}

// 再生状態の切り替え
void TimelineEditorDialog::ChangePlay(const int iNumerator /*= 1*/, const int iDenominator /*= 1*/)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlay(iNumerator, iDenominator);
	}
}

// 再生速度変更
void TimelineEditorDialog::ChangePlaySpeed(const int& iNumerator, const int& iDenominator)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlaySpeed(iNumerator, iDenominator);
	}
}

// 再生速度アップ
void TimelineEditorDialog::PlaySpeedUp(const int iSpeed /*= 1*/)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlaySpeed(iSpeed);
	}
}

// 再生速度ダウン
void TimelineEditorDialog::PlaySpeedDown(const int iSpeed /*= 1*/)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlaySpeed(iSpeed * -1);
	}
}

// 再生ポーズ
void TimelineEditorDialog::PausePlay(void)
{
	if (m_pMainView)
	{
		m_pMainView->PausePlay();
	}
}

// ESCで閉じるを無効化
void TimelineEditorDialog::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnCancel();
}

// Enterで閉じるを無効化
void TimelineEditorDialog::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}

// キー入力を引き渡す
void TimelineEditorDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pMainView)
	{
		m_pMainView->KeyUp(nChar, nRepCnt, nFlags);
	}
}


void TimelineEditorDialog::OnNcPaint()
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CDialogEx::OnNcPaint() を呼び出さないでください。

	int icxFrame = GetSystemMetrics(SM_CXFRAME);//　フレームの幅
	int icyFrame = GetSystemMetrics(SM_CYFRAME);//　フレームの高さ
	int iCaptionHeight = GetSystemMetrics(SM_CYCAPTION);

	CDC* pWDC = GetWindowDC();

	CRect rcRect;
	GetWindowRect(&rcRect);
	CString str;
	str.Format(_T("L%d T%d R%d B%d C%d X%d Y%d\n"), rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, iCaptionHeight, icxFrame, icyFrame);
	ScreenToClient(&rcRect);
	int cxFrame = 0 - rcRect.left;
	int cyFrame = 0 - rcRect.top - iCaptionHeight;
	str.Format(_T("L%d T%d R%d B%d C%d X%d Y%d\n"), rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, iCaptionHeight, cxFrame, cyFrame);
	OutputDebugString(str);

	////rcRect.left += 2;
	////rcRect.right -= 2;
	////rcRect.top += 2;
	////rcRect.bottom -= 2;

	//HBRUSH hBrs = CreateSolidBrush(GRAY_BRUSH);

	pWDC->FillSolidRect(0, 0, rcRect.Width(), cyFrame, RGB(100, 100, 230));

	pWDC->FillSolidRect(0, cyFrame, rcRect.Width(), iCaptionHeight, RGB(100, 100, 230));

	pWDC->FillSolidRect(0, rcRect.bottom + iCaptionHeight + cyFrame - WINDOWBORDER_THICKNESS, rcRect.Width(), WINDOWBORDER_THICKNESS, RGB(100, 100, 230));
	pWDC->FillSolidRect(WINDOWBORDER_THICKNESS, rcRect.bottom + iCaptionHeight, rcRect.Width() - WINDOWBORDER_THICKNESS, cyFrame - WINDOWBORDER_THICKNESS, RGB(255, 255, 255));

	pWDC->FillSolidRect(0, 0, WINDOWBORDER_THICKNESS, rcRect.Height(), RGB(100, 100, 230));
	pWDC->FillSolidRect(WINDOWBORDER_THICKNESS, cyFrame + iCaptionHeight, cxFrame - WINDOWBORDER_THICKNESS, rcRect.Height() - WINDOWBORDER_THICKNESS - cyFrame - iCaptionHeight, RGB(255, 255, 255));

	pWDC->FillSolidRect(rcRect.right + cxFrame - WINDOWBORDER_THICKNESS, 0, WINDOWBORDER_THICKNESS, rcRect.Height(), RGB(100, 100, 230));
	pWDC->FillSolidRect(rcRect.right, cyFrame + iCaptionHeight, cxFrame - WINDOWBORDER_THICKNESS, rcRect.Height() - WINDOWBORDER_THICKNESS - cyFrame - iCaptionHeight, RGB(255, 255, 255));

	ReleaseDC(pWDC);

	//m_pEditorView->OnDraw(GetDC());

}


//LRESULT TimelineEditorDialog::OnNcHitTest(CPoint point)
//{
//	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
//
//	CRect rcRect;
//	GetWindowRect(&rcRect);
//
//	//if (point.x < rcRect.left + WINDOWBORDER_THICKNESS)
//	//{
//		//return HTLEFT;
//	//}
//
//	return CDialogEx::OnNcHitTest(point);
//}
