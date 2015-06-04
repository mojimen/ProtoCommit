// TimelineEditorDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditorDialog.h"
#include "TimelineEditorView.h"
#include "afxdialogex.h"


// TimelineEditorDialog ダイアログ

IMPLEMENT_DYNAMIC(TimelineEditorDialog, CDialogEx)

TimelineEditorDialog::TimelineEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TimelineEditorDialog::IDD, pParent)
{

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
END_MESSAGE_MAP()


// TimelineEditorDialog メッセージ ハンドラー



void TimelineEditorDialog::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	ShowWindow(SW_HIDE);
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
