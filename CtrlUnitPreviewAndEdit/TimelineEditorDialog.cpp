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
END_MESSAGE_MAP()


// TimelineEditorDialog メッセージ ハンドラー



void TimelineEditorDialog::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	ShowWindow(SW_HIDE);
	m_pParentWnd->PostMessageW(WM_COMMAND, MAKEWPARAM(ID_HIDE_EDITOR, 0), NULL);
	//CDialogEx::OnClose();
}


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

BOOL TimelineEditorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

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


void TimelineEditorDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CRect rcMyRect;
	GetClientRect(&rcMyRect);
	m_pEditorView->MoveWindow(rcMyRect);
}



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

// タイムラインカーソル位置同期
int TimelineEditorDialog::GetTimelineCursorFrame(void)
{
	return m_pEditorView->GetTimelineCursorFrame();
}

// タイムラインカーソル位置同期
void TimelineEditorDialog::SetTimelineCursorFrame(int iFrame)
{
	if (m_pMainView)
	{
		m_pMainView->SetTimelineCursorFrame(iFrame);
	}
}

// 再生状態の切り替え
void TimelineEditorDialog::ChangePlay(void)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlay();
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
