// TimelineEditerDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditerDialog.h"
#include "TimelineEditerView.h"
#include "afxdialogex.h"


// TimelineEditerDialog ダイアログ

IMPLEMENT_DYNAMIC(TimelineEditerDialog, CDialogEx)

TimelineEditerDialog::TimelineEditerDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TimelineEditerDialog::IDD, pParent)
{

}

TimelineEditerDialog::~TimelineEditerDialog()
{
}

void TimelineEditerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TimelineEditerDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// TimelineEditerDialog メッセージ ハンドラー



void TimelineEditerDialog::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


void TimelineEditerDialog::SetPosition(CRect& rcParentRect)
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
	m_pEditerView->MoveWindow(rcMyRect);
}

BOOL TimelineEditerDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	// フレームウィンドウの表示領域
	CRect rcFrameWndRect;
	GetWindowRect(&rcFrameWndRect);
	ScreenToClient(&rcFrameWndRect);

	m_pEditerView = static_cast<TimelineEditerView*>((RUNTIME_CLASS(TimelineEditerView))->CreateObject());
	m_pEditerView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rcFrameWndRect, this, 0, NULL);
	m_pEditerView->SendMessage(WM_INITIALUPDATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void TimelineEditerDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CRect rcMyRect;
	GetClientRect(&rcMyRect);
	m_pEditerView->MoveWindow(rcMyRect);
}



BOOL TimelineEditerDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
