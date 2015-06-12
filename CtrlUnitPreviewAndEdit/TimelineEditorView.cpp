// TimelineEditorView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditorView.h"

#include "OpenGLRect.h"
#include "TimelineDataOperator.h"
#include "ClipDataRect.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
#include "TrackDataManager.h"

#include "TimelineEditorDialog.h"

#include "TimecodeOperator.h"


#include <map>
#include <windows.h>
#include <mmsystem.h>
#include <random>
#include <thread>
#pragma comment(lib,"winmm.lib")

// TimelineEditorView

IMPLEMENT_DYNCREATE(TimelineEditorView, OpenGLView)

TimelineEditorView::TimelineEditorView()
{
	m_fInitialize = FALSE;
	m_pMainWnd = nullptr;
	m_pParentDialog = nullptr;
	m_pDebugInfoPanelRect = nullptr;
	m_pTimelineDataOperator = nullptr;
	m_pTimecodeOperator = nullptr;
}

TimelineEditorView::~TimelineEditorView()
{
}

BEGIN_MESSAGE_MAP(TimelineEditorView, OpenGLView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLIP_DELETE, &TimelineEditorView::OnClipDelete)
	ON_COMMAND(ID_TRANSITION_SET_IN, &TimelineEditorView::OnTransitionSetIn)
	ON_COMMAND(ID_TRANSITION_SET_OUT, &TimelineEditorView::OnTransitionSetOut)
	//ON_COMMAND(ID_PLAY, &TimelineEditorView::OnPlay)
	ON_COMMAND(ID_TRANSITION_RESET_IN_CENTER, &TimelineEditorView::OnTransitionResetInCenter)
	ON_COMMAND(ID_TRANSITION_RESET_IN_START, &TimelineEditorView::OnTransitionResetInStart)
	ON_COMMAND(ID_TRANSITION_RESET_IN_END, &TimelineEditorView::OnTransitionResetInEnd)
	ON_COMMAND(ID_TRANSITION_RESET_OUT_CENTER, &TimelineEditorView::OnTransitionResetOutCenter)
	ON_COMMAND(ID_TRANSITION_RESET_OUT_START, &TimelineEditorView::OnTransitionResetOutStart)
	ON_COMMAND(ID_TRANSITION_RESET_OUT_END, &TimelineEditorView::OnTransitionResetOutEnd)
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYUP()
	ON_WM_NCHITTEST()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// TimelineEditorView 描画

void TimelineEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。

	//DWORD dwTimeS = timeGetTime();
	//CString strTime;
	//strTime.Format(_T("TL%d\n"), dwTimeS);
	//OutputDebugString(strTime);

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// 背景塗りつぶし
	glClearColor(TIMELINEBASECOLOR_BRUSH_FLOAT);
	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	DrawTimelineEditorView(*pDC);

	glFlush();
	SwapBuffers(m_pDC->GetSafeHdc());

	wglMakeCurrent(NULL, NULL);

	//DWORD dwTimeE = timeGetTime();
	//strTime.Format(_T("TL%d\n"), (dwTimeE - dwTimeS));
	//OutputDebugString(strTime);


}



// TimelineEditorView 診断

#ifdef _DEBUG
void TimelineEditorView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void TimelineEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// ウィンドウスタイル初期値変更
BOOL TimelineEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	cs.dwExStyle |= WS_EX_ACCEPTFILES;

	return OpenGLView::PreCreateWindow(cs);
}

// 初期設定
int TimelineEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (OpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。
	// ドロップターゲットに登録
	if (m_DropTarget.Register(this))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}



// TimelineEditorView メッセージ ハンドラー

// Ｌボタンダウン
void TimelineEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	BOOL fRet = m_pTimelineDataOperator->OnLButtonDown(nFlags, point);
	m_pOperatingClipData = m_pTimelineDataOperator->GetOperatingClipData();
	if (fRet)
	{
		Invalidate();
		UpdateWindow();
	}
	if (m_pTimelineDataOperator->IsLButtonDown())
	{
		SetCapture();
	}
}

// Ｌボタンアップ
void TimelineEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	BOOL fStopPlay = FALSE;
	BOOL fRet = m_pTimelineDataOperator->OnLButtonUp(nFlags, point, fStopPlay);
	m_pOperatingClipData = m_pTimelineDataOperator->GetOperatingClipData();
	if (fRet)
	{
		if (fStopPlay && m_pTimelineDataOperator->IsPlaying())
		{
			m_pParentDialog->ChangePlay();
		}
		else
		{
			if (m_pTimelineDataOperator->IsJumpFrame())
			{
				DrawJumpFrame(m_pTimelineDataOperator->GetJumpFrameCount(), (point.x - m_iTimelineCursorPoint));
			}
			else
			{
				m_pParentDialog->SetTimelineCursorFrame(m_pTimelineDataOperator->GetOperatingTimelineCursorFramePosition());
				Invalidate();
				UpdateWindow();
			}
			//m_pMainWnd->PostMessageW(WM_COMMAND, MAKEWPARAM(ID_CHANGE_TIMELINECURSOR, 0), NULL);
		}
	}
	ReleaseCapture();
}

// Ｒボタンアップ
void TimelineEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_pTimelineDataOperator->OnRButtonUp(nFlags, point))
	{
		Invalidate();
		UpdateWindow();
	}
	else
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}
}

// マウス移動
void TimelineEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	BOOL fStartPlay = FALSE;
	if (m_pTimelineDataOperator->OnMouseMove(nFlags, point, fStartPlay))
	{
		if (fStartPlay)
		{
			if (!(m_pTimelineDataOperator->IsPlaying()))
			{
				m_pParentDialog->ChangePlay(m_pTimelineDataOperator->GetShuttleSpeedNumerator(), m_pTimelineDataOperator->GetShuttleSpeedDenominator());
			}
			else
			{
				m_pParentDialog->ChangePlaySpeed(m_pTimelineDataOperator->GetShuttleSpeedNumerator(), m_pTimelineDataOperator->GetShuttleSpeedDenominator());
			}
		}
		else if (!(m_pTimelineDataOperator->IsDragShuttle()))
		{
			Invalidate();
			UpdateWindow();
			m_pParentDialog->SetTimelineCursorFrame(m_pTimelineDataOperator->GetOperatingTimelineCursorFramePosition());
			//m_pMainWnd->PostMessageW(WM_COMMAND, MAKEWPARAM(ID_CHANGE_TIMELINECURSOR, 0), NULL);
		}
	}
	m_pOperatingClipData = m_pTimelineDataOperator->GetOperatingClipData();
}

// サイズ変更
void TimelineEditorView::OnSize(UINT nType, int cx, int cy)
{
	OpenGLView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (m_fInitialize)
	{
		SetPanelRect();
	}

}

// 初期設定
void TimelineEditorView::OnInitialUpdate()
{
	OpenGLView::OnInitialUpdate();

	m_fInitialize = TRUE;

	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	m_pTimelineDataOperator = new TimelineDataOperator();
	m_pTimelineDataOperator->InitializeTimelineDataOperator(m_uiTimelineDataOperatorId);

	m_pTimelineDataManager = m_pTimelineDataOperator->GetTimelineDataManager();
	m_uiTimelineDataManager = m_pTimelineDataOperator->GetTimelineDataManagerId();
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManager);
	ASSERT(m_pTrackDataVideoManager);
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManager);
	ASSERT(m_pTrackDataAudioManager);

	m_pTimecodeOperator = new TimecodeOperator();
	m_pTimecodeOperator->InitializeTimecodeOperator(m_strTimecodeOperator);

	InitAreaRect();
	m_pDebugInfoPanelRect->bottom = m_pDebugInfoPanelRect->top + kDebugInfoPanelDefaltHeight;

	//TODO: 動作確認用	
	InitTestObject();
}

// 破棄
void TimelineEditorView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	if (m_pTimelineDataOperator)
	{
		m_pTimelineDataOperator->DeleteTimelineDataOperator();
		delete m_pTimelineDataOperator;
	}
	if (m_pTimecodeOperator)
	{
		delete m_pTimecodeOperator;
	}
	if (m_pDebugInfoPanelRect)
	{
		delete m_pDebugInfoPanelRect;
	}
}

// ファイルドロップ
void TimelineEditorView::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CString strFileName;

	if (m_pTimelineDataOperator->OnDropFiles(hDropInfo, strFileName))
	{
		OutputDebugString(strFileName + "\n");
		GetParent()->SetForegroundWindow();
		Invalidate();
		UpdateWindow();
	}
	else
	{
		OutputDebugString(_T("DROP FALSE\n"));
	}
}

// ドラッグ開始
DROPEFFECT TimelineEditorView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	m_pTimelineDataOperator->OnDragEnter(pDataObject, dwKeyState, point);
	m_pParentDialog->SetForegroundWindow();
	if (m_pTimelineDataOperator->EnableDrawDragRect())
	{
		Invalidate();
		UpdateWindow();
		return DROPEFFECT_COPY;
	}
	return DROPEFFECT_NONE;
}

// ドラッグ終了
void TimelineEditorView::OnDragLeave()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	m_pTimelineDataOperator->OnDragLeave();
	Invalidate();
	UpdateWindow();
}

// ドラッグ移動
DROPEFFECT TimelineEditorView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	Invalidate();
	UpdateWindow();
	return m_pTimelineDataOperator->OnDragOver(pDataObject, dwKeyState, point);
}

// ドロップ
//BOOL TimelineEditorView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
//{
//	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
//	OutputDebugString(_T("Drop\n"));
//
//	return OpenGLView::OnDrop(pDataObject, dropEffect, point);
//}

// マウスホイール
BOOL TimelineEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	BOOL fCursorChange = FALSE;
	CPoint poPoint = pt;
	ScreenToClient(&poPoint);
	if (m_pTimelineDataOperator->OnMouseWheel(nFlags, zDelta, poPoint, fCursorChange))
	{
		if (fCursorChange)
		{
			m_pParentDialog->SetTimelineCursorFrame(m_pTimelineDataOperator->GetTimelineCursorFramePosition());
		}
		Invalidate();
		UpdateWindow();
	}

	return TRUE;
}

// キー操作
void TimelineEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	m_pParentDialog->OnKeyUp(nChar, nRepCnt, nFlags);

	//switch (nChar)
	//{

	//case VK_SPACE:
	//	if (!(m_pTimelineDataOperator->IsPlaying()))
	//	{
	//		m_pParentDialog->ChangePlay();
	//	}
	//	else
	//	{
	//		m_pParentDialog->PausePlay();
	//	}
	//	break;

	//case VK_RETURN:
	//	m_pParentDialog->ChangePlay();
	//	break;

	//case VK_ESCAPE:
	//	if (m_pTimelineDataOperator->IsPlaying())
	//	{
	//		m_pParentDialog->ChangePlay();
	//	}
	//	break;

	//case VK_PRIOR:
	//	if (m_pTimelineDataOperator->IsPlaying())
	//	{
	//		m_pParentDialog->PlaySpeedUp();
	//	}
	//	break;

	//case VK_NEXT:
	//	if (m_pTimelineDataOperator->IsPlaying())
	//	{
	//		m_pParentDialog->PlaySpeedDown();
	//	}
	//	break;

	//default:
	//	break;

	//}

	//OpenGLView::OnKeyUp(nChar, nRepCnt, nFlags);
}




/*

描画系

*/
//画面描画
void TimelineEditorView::DrawTimelineEditorView(const CDC& dcDC)
{
	CRect rcRect;
	GetClientRect(&rcRect);
	int iHeight = rcRect.Height();
	CString strPoint;

	// プレビューエリア枠線描画
	m_pDebugInfoPanelRect->DrawMyBorderRect(3.0f);
	
	// タイムラインヘッダーコントロールパネル描画
	DrawTimelineControlPanel();
	
	// シークバーエリア描画
	DrawSeekBar(dcDC, iHeight);
	
	// 静止クリップ描画
	DrawClip(dcDC, iHeight);
	
	// 操作イメージ描画
	if ((m_pTimelineDataOperator->IsSingleInTrim() || m_pTimelineDataOperator->IsSingleOutTrim() || m_pTimelineDataOperator->IsMove())
		&& (!(m_pOperatingClipData->GetOperatingRect()->IsRectEmpty())))
	{
		DrawOperatingClip(dcDC, iHeight);
	}

	if (m_pTimelineDataOperator->IsDragAndDrop())
	{
		DrawDragAndDropClip(dcDC, iHeight);
	}


	// トラックヘッダー描画
	DrawTrackHeader();

	// タイムラインデータエリア描画
	DrawTimelineDataRect();

	// トラック枠描画
	DrawTrack(dcDC, iHeight);

	// タイムラインカーソル／シャトル操作補助線描画
	DrawTimelineCursor(dcDC, iHeight);

#ifdef _DEBUG
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);

	strFrameNumber.Format(_T("TLCursor %d"), m_pTimelineDataOperator->GetTimelineCursorFramePosition());
	ChangeScreenPointToOpenGLPoint(5, 15, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Frame Left %d Right %d"), m_pTimelineDataOperator->GetLeftFrameNumber(), m_pTimelineDataOperator->GetRightFrameNumber());
	ChangeScreenPointToOpenGLPoint(5, 30, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating TLCursor %d"), m_pTimelineDataOperator->GetOperatingTimelineCursorFramePosition());
	ChangeScreenPointToOpenGLPoint(5, 45, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating Left %d Right %d"), m_pTimelineDataOperator->GetOperatingLeftFrameNumber(), m_pTimelineDataOperator->GetOperatingRightFrameNumber());
	ChangeScreenPointToOpenGLPoint(5, 60, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("CursorLine Point %d"), m_iTimelineCursorPoint);
	ChangeScreenPointToOpenGLPoint(5, 75, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("f/p %d p/f %d"), m_pTimelineDataOperator->GetFramePerPoint(), m_pTimelineDataOperator->GetPointPerFrame());
	ChangeScreenPointToOpenGLPoint(5, 90, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	DeleteObject(hfDrawFont);
#endif

}

// タイムラインコントロールパネル描画
void TimelineEditorView::DrawTimelineControlPanel(void)
{
	m_pTimelineControlPanelRect->DrawMyFillRect();
}

// シークバー描画
void TimelineEditorView::DrawSeekBar(const CDC& dcDC, const int& iHeight)
{

	// 背景塗りつぶし
	m_pSeekBarRect->DrawMyFillRect();

	int iDrawFrame = m_pTimelineDataOperator->GetOperatingLeftFrameNumber();
	if (iDrawFrame < 0)
	{
		iDrawFrame = 0;
	}
	else
	{
		int iSurplusFrame = iDrawFrame % m_pTimelineDataOperator->GetSmallScaleDrawInterval();
		if (iSurplusFrame != 0)
		{
			iDrawFrame += (m_pTimelineDataOperator->GetSmallScaleDrawInterval() - iSurplusFrame);
		}
	}

	POINT pScaleLine;
	pScaleLine.y = m_pSeekBarRect->top;
	pScaleLine.x = m_pTimelineDataOperator->ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);

	while (pScaleLine.x < m_pSeekBarRect->right)
	{
		if (pScaleLine.x > m_pSeekBarRect->left)
		{
			// 大目盛り
			if ((iDrawFrame % m_pTimelineDataOperator->GetBigScaleDrawInterval()) == 0)
			{
				DrawBigScale(dcDC, iDrawFrame, iHeight, pScaleLine);
			}
			// 中目盛り
			else if ((iDrawFrame % m_pTimelineDataOperator->GetMiddleScaleDrawInterval()) == 0)
			{
				DrawMiddleScale(dcDC, iDrawFrame, iHeight, pScaleLine);
			}
			// 小目盛り
			else
			{
				DrawSmallScale(dcDC, iDrawFrame, iHeight, pScaleLine);
			}
		}
		iDrawFrame += m_pTimelineDataOperator->GetSmallScaleDrawInterval();
		pScaleLine.x = m_pTimelineDataOperator->ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);
	}

}

// 大目盛り描画
void TimelineEditorView::DrawBigScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine)
{

	// TODO: 製品はタイムコードを表示
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	m_pTimecodeOperator->ChangeFrameToTimecode(iDrawFrame, strFrameNumber, DF_MODE);
	CreateDrawFont(SEEKBARTIMECODE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_pSeekBarRect->top + SEEKBARTIMECODE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	// 目盛り、ライン描画
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->top + SEEKBARBIGSCALE_TOPMARGIN, pScaleLine.x, m_pSeekBarRect->bottom,
		SEEKBARBIGSCALE_LINEWIDTH, SEEKBARBIGSCALECOLOR_BRUSH_FLOAT);
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->bottom + 1, pScaleLine.x, m_pTimelineDataRect->bottom,
		SEEKBARBIGSCALELINE_LINEWIDTH, SEEKBARBIGSCALELINECOLOR_BRUSH_FLOAT);

	DeleteObject(hfDrawFont);
	return;
}

// 中目盛り描画
void TimelineEditorView::DrawMiddleScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine)
{

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("%d"), iDrawFrame);
	CreateDrawFont(SEEKBARTIMECODE_MIDDLE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_pSeekBarRect->top + SEEKBARTIMECODE_MIDDLE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);
#endif

	// 目盛り、ライン描画
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->top + SEEKBARMIDDLESCALE_TOPMARGIN, pScaleLine.x, m_pSeekBarRect->bottom,
		SEEKBARMIDDLESCALE_LINEWIDTH, SEEKBARMIDDLESCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->bottom + 1, pScaleLine.x, m_pTimelineDataRect->bottom,
		SEEKBARMIDDLESCALELINE_LINEWIDTH, SEEKBARMIDDLESCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// 小目盛り描画
void TimelineEditorView::DrawSmallScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine)
{
	// 目盛り、ライン描画
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->top + SEEKBARSMALLSCALE_TOPMARGIN, pScaleLine.x, m_pSeekBarRect->bottom,
		SEEKBARSMALLSCALE_LINEWIDTH, SEEKBARSMALLSCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_SMALLSCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->bottom + 1, pScaleLine.x, m_pTimelineDataRect->bottom,
		SEEKBARSMALLSCALELINE_LINEWIDTH, SEEKBARSMALLSCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// トラックヘッダー描画
void TimelineEditorView::DrawTrackHeader(void)
{
	// TODO: とりあえず今は枠だけ
	m_pTrackHeaderRect->DrawMyBorderRect();
}

// トラック描画
void TimelineEditorView::DrawTrack(const CDC& dcDC, const int& iHeight)
{
	// TODO: とりあえず今は枠と名前だけ
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRect;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
	for (int i = 0; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		pTrackDataRect = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i));
		pTrackDataRect->DrawMyBottomLine();
		ChangeScreenPointToOpenGLPoint(pTrackDataRect->left + TRACK_NAME_MARGINLEFT, (pTrackDataRect->top + TRACK_NAME_MARGINTOP), iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(pTrackDataRect->GetTrackName()), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	}
	DeleteObject(hfDrawFont);
}

// タイムラインデータエリア描画
void TimelineEditorView::DrawTimelineDataRect(void)
{
	// TODO: とりあえず今は枠だけ
	m_pTimelineDataRect->DrawMyBorderRect();
}

// クリップの描画を行う
BOOL TimelineEditorView::DrawClip(const CDC& dcDC, const int& iHeight)
{
	int iClipCount = 0;
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	for (int i = 0; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		iClipCount = DrawClipInTrack(dcDC,  m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i)), iHeight, iClipCount);

	}
	return TRUE;
}

// トラック内の表示範囲内クリップをサーチして描画
int TimelineEditorView::DrawClipInTrack(const CDC& dcDC, TrackDataRect* pTrackDataRect, const int& iHeight, int iClipTotalCount)
{
	//TODO: 毎回全サーチするのではなくてvectorとかに表示範囲のオブジェクトを設定しておいて操作のたびにvectorを更新する
	int iStartFrame = m_pTimelineDataOperator->GetOperatingLeftFrameNumber();
	if (iStartFrame < 0)
	{
		iStartFrame = 0;
	}
	//ClipDataPositionMap mpClipDataMap;
	m_mpClipMap.clear();
	int iClipCount = pTrackDataRect->GetTrackDataInfo()->GetClipDataInRange(iStartFrame, m_pTimelineDataOperator->GetOperatingRightFrameNumber(), m_mpClipMap);
	ClipDataRect* pClipData;
	ClipDataRect* pClipDataLeft;
	pClipDataLeft = nullptr;
	if (iClipCount > 0)
	{
#ifdef _DEBUG
		CString strFrameNumber;
		double dPointX, dPointY;
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
#endif
		ClipDataPositionMap::iterator itr = m_mpClipMap.begin();
		while (itr != m_mpClipMap.end())
		{
			pClipData = (*itr).second;
			m_pTimelineDataOperator->CalcClipRectDisplayPoint(static_cast<CRect&>(*pClipData), pClipData, static_cast<CRect>(pTrackDataRect));
			pClipData->SetVert(iHeight);
			pClipData->DrawMyFillRect();
			if (pClipDataLeft != nullptr)
			{
				// トランジション部分塗り替え
				if (pClipData->left < pClipDataLeft->right)
				{
					m_pTransitionRect->CopyRect(pClipData);
					m_pTransitionRect->right = pClipDataLeft->right;
					m_pTransitionRect->SetVert(iHeight);
					m_pTransitionRect->DrawMyFillRect();
					m_pTransitionRect->DrawMyLeftLine();
					m_pTransitionRect->DrawMyRightLine();
					m_pTransitionRect->SetRectEmpty();

				}
				else if (pClipData->left == pClipDataLeft->right)
				{
					pClipDataLeft->DrawMyRightLine();
				}
			}
			pClipDataLeft = pClipData;
			++itr;

#ifdef _DEBUG
			strFrameNumber.Format(_T(" L:%d T:%d R:%d B:%d I:%d O:%d D:%d i:%d o:%d"), pClipData->left, pClipData->top, pClipData->right, pClipData->bottom, pClipData->GetTimelineInPoint(), pClipData->GetTimelineOutPoint(), pClipData->GetDuration(), pClipData->GetInPoint(), pClipData->GetOutPoint());
			ChangeScreenPointToOpenGLPoint(5, 105 + (iClipTotalCount * 15), iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(pTrackDataRect->GetTrackName() + strFrameNumber), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
#endif
			++iClipTotalCount;
		}
		m_mpClipMap.clear();
#ifdef _DEBUG
		DeleteObject(hfDrawFont);
#endif
	}
	return iClipTotalCount;
}

// 操作中クリップの描画を行う
BOOL TimelineEditorView::DrawOperatingClip(const CDC& dcDC, const int& iHeight)
{
	// TODO: 元のクリップの色を変える　元々各タイミングで実施するよう変更
	m_pOperatingClipData->DrawOperatingOldRect(iHeight);

	CRect* rcOperatingRect = m_pOperatingClipData->GetOperatingRect();

	if (m_pTimelineDataOperator->IsSingleInTrim() || m_pTimelineDataOperator->IsSingleOutTrim())
	{
		m_pOperatingClipData->DrawSingleTrimRect(iHeight, m_pTimelineDataOperator->IsSingleInTrim());

#ifdef _DEBUG
		//TODO: デバッグ
		CString strText;
		double dPointX, dPointY;
		int iPoint;
		int iDuration;
		if (m_pTimelineDataOperator->IsSingleInTrim())
		{
			iPoint = m_pOperatingClipData->GetTimelineInPoint() + m_pTimelineDataOperator->GetOperatingClipFrameCount();
		}
		else
		{
			iPoint = m_pOperatingClipData->GetTimelineInPoint();
		}
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
		strText.Format(_T("TrimingClipInPoint  %d"), iPoint);
		ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipLeftPoint  %d"), rcOperatingRect->left);
		ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		if (m_pTimelineDataOperator->IsSingleInTrim())
		{
			iPoint = m_pOperatingClipData->GetTimelineOutPoint();
			iDuration = m_pOperatingClipData->GetDuration() - m_pTimelineDataOperator->GetOperatingClipFrameCount();
		}
		else
		{
			iPoint = m_pOperatingClipData->GetTimelineOutPoint() + m_pTimelineDataOperator->GetOperatingClipFrameCount();
			iDuration = m_pOperatingClipData->GetDuration() + m_pTimelineDataOperator->GetOperatingClipFrameCount();
		}
		strText.Format(_T("TrimingClipOutPoint  %d"), iPoint);
		ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipRightPoint  %d"), rcOperatingRect->right);
		ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipDuration  %d"), iDuration);
		ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		DeleteObject(hfDrawFont);
#endif

	}
	else
	{
		// ドロップ位置用
		m_pOperatingClipData->DrawMovingRect(iHeight);

#ifdef _DEBUG
		//TODO: デバッグ
		CString strText;
		double dPointX, dPointY;
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
		strText.Format(_T("MovingClipInPoint  %d"), m_pOperatingClipData->GetTimelineInPoint() + m_pTimelineDataOperator->GetOperatingClipFrameCount());
		ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("MovingClipLeftPoint  %d"), rcOperatingRect->left);
		ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("MovingClipRightPoint  %d"), rcOperatingRect->right);
		ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
#endif

		// マウス位置追随用
		CRect rcMousePointRect = m_pTimelineDataOperator->GetMousePointRect();
		if (!(rcMousePointRect.EqualRect(rcOperatingRect)))
		{
			m_pOperatingClipData->SetOverlappingVert(static_cast<float>(rcMousePointRect.left), static_cast<float>(iHeight - rcMousePointRect.top),
				static_cast<float>(rcMousePointRect.right), static_cast<float>(iHeight - rcMousePointRect.bottom));
			m_pOperatingClipData->DrawOverlappingRect(iHeight);

#ifdef _DEBUG
			strText.Format(_T("MouseMoveClipLeftPoint  %d"), rcMousePointRect.left);
			ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			strText.Format(_T("MouseMoveClipRightPoint  %d"), rcMousePointRect.right);
			ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			DeleteObject(hfDrawFont);
#endif
		}
	}
	return TRUE;
}

// ドラッグ＆ドロップ中のイメージを描画する
BOOL TimelineEditorView::DrawDragAndDropClip(const CDC& dcDC, const int& iHeight)
{
	ClipDataRect* pClipRect = m_pTimelineDataOperator->GetDragAndDropClipDataRect();
	if (m_pTimelineDataOperator->EnableDrawDragRect())
	{
		pClipRect->DrawMovingRect(iHeight);
	}
	else if (m_pTimelineDataOperator->IsDropFileCrrect())
	{
		pClipRect->SetOverlappingVert(static_cast<float>(pClipRect->left), static_cast<float>(iHeight - pClipRect->top),
			static_cast<float>(pClipRect->right), static_cast<float>(iHeight - pClipRect->bottom));
		pClipRect->DrawOverlappingRect(iHeight);
	}

#ifdef _DEBUG
	//TODO: デバッグ
	CString strText;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
	strText.Format(_T("DropClipInPoint  %d"), pClipRect->GetTimelineInPoint());
	ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipLeftPoint  %d"), pClipRect->left);
	ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipOutPoint  %d"), pClipRect->GetTimelineOutPoint());
	ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipRightPoint  %d"), pClipRect->right);
	ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipDuration  %d"), pClipRect->GetDuration());
	ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);
#endif

	return TRUE;
}

// タイムラインカーソルの描画を行う
BOOL TimelineEditorView::DrawTimelineCursor(const CDC& dcDC, const int& iHeight)
{
	// ジャンプフレーム時の基準線を描画
	//if ((m_pTimelineDataOperator->IsScrub()) && (m_pTimelineDataOperator->GetJumpFrameCursorPosition() != 0))
	//{
	//	int iPosition = m_pTimelineDataOperator->ChangeTimelineFramePositionToDisplayPoint(m_pTimelineDataOperator->GetJumpFrameCursorPosition());
	//	DrawLine(iHeight, iPosition, m_pSeekBarRect->top, iPosition, m_pTimelineDataRect->bottom,
	//		JUMPFRAMETIMELINECURSOR_LINEWIDTH, JUMPFRAMETIMELINECURSORCOLOR_BRUSH_FLOAT);
	//	DrawLine(iHeight, m_iTimelineCursorPoint, m_pSeekBarRect->top, m_iTimelineCursorPoint, m_pTimelineDataRect->bottom,
	//		TIMELINECURSOR_LINEWIDTH, TIMELINECURSORCOLOR_BRUSH_FLOAT);
	//}
	//else
	//{
	//	if ((m_pTimelineDataOperator->IsJumpFrame()))		
	//	{
	//		int iPosition = m_pTimelineDataOperator->ChangeTimelineFramePositionToDisplayPoint(m_pTimelineDataOperator->GetJumpFrameCursorPosition());
	//		DrawLine(iHeight, iPosition, m_pSeekBarRect->top, iPosition, m_pTimelineDataRect->bottom,
	//			TIMELINECURSOR_LINEWIDTH, TIMELINECURSORCOLOR_BRUSH_FLOAT);
	//	}
	//	else
	//	{
			// ラインを描画
			DrawLine(iHeight, m_iTimelineCursorPoint, m_pSeekBarRect->top, m_iTimelineCursorPoint, m_pTimelineDataRect->bottom,
				TIMELINECURSOR_LINEWIDTH, TIMELINECURSORCOLOR_BRUSH_FLOAT);
	//	}
	//}


	//if (m_fDragShuttling)
	//{
	//	blAlphaBlend.SourceConstantAlpha = kTimelineCursorDragGuideLineAlpha;

	//	//TODO: 位置は検討が必要
	//	CString strText;
	//	int iOldBkMode = dcMemDc.SetBkMode(TRANSPARENT);
	//	COLORREF crOldTextColor = dcMemDc.SetTextColor(SEEKBARTIMECODETEXTCOLOR_BRUSH);
	//	strText.Format(_T("×  %d"), m_fSuttleSpeed);
	//	dcMemDc.TextOutW(90, m_pTimelineControlPanelRect.bottom - 20, strText);
	//	dcMemDc.SetBkMode(iOldBkMode);
	//	dcMemDc.SetTextColor(crOldTextColor);

	//	//補助線の描画
	//	CRect rcShuttleLineRect;
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragOneSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragTowSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragFourSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragEightSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragSixteenSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragThirtyTwoSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragSixtyFourSpeed);

	//}

	return TRUE;
}

// ジャンプフレーム時のスクロールを行う
void TimelineEditorView::DrawJumpFrame(int iMoveFrame, int iMoveDitance)
{
	int iMoveLength = 0, iMoveCount = 1;
	if (abs(iMoveDitance) > 500)
	{
		iMoveCount = 25;
	}
	else if (abs(iMoveDitance) > 100)
	{
		iMoveCount = abs(static_cast<int>(ceil(iMoveDitance / 35.0))) + 10;
	}
	else if (abs(iMoveDitance) > 10)
	{
		iMoveCount = abs(static_cast<int>(ceil(iMoveDitance / 30.0))) + 10;
	}
	else
	{
		iMoveCount = abs(iMoveDitance);
	}
	for (int i = iMoveCount; i > 0; --i)
	{
		iMoveLength = static_cast<int>(ceil(iMoveFrame / i));
		m_pTimelineDataOperator->MoveTimelineCursor(iMoveLength);
		m_pParentDialog->SetTimelineCursorFrame(m_pTimelineDataOperator->GetTimelineCursorFramePosition());
		Invalidate();
		UpdateWindow();
		Sleep(0);
		iMoveFrame -= iMoveLength;
	}
	m_pTimelineDataOperator->MoveTimelineCursor(iMoveFrame);
	Invalidate();
	UpdateWindow();
	m_pParentDialog->SetTimelineCursorFrame(m_pTimelineDataOperator->GetTimelineCursorFramePosition());
	m_pTimelineDataOperator->ResetJumpFrame();
	m_pTimelineDataOperator->ClearJumpFramecount();
}


//// シャトル操作時のガイドラインを表示する
//void TimelineEditorView::DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcLineRect, float fGuideAreaWidth)
//{
//	int iGuideAreaWidth = static_cast<int>(floor(m_pTimelineDataRect.Width() * fGuideAreaWidth));
//
//	rcLineRect.left = m_iTimelineCursorPoint + iGuideAreaWidth - kTimelineCursorDragGuideLineThickness;
//	rcLineRect.right = rcLineRect.left + kTimelineCursorDragGuideLineThickness;
//	rcLineRect.top = m_pTimelineDataRect.top;
//	rcLineRect.bottom = m_pTimelineDataRect.bottom;
//
//	// ラインを描画
//	CBrush brShuttleGuidLineBrush(TIMELINECURSORDRAGGUIDELINECOLOR_BRUSH);
//	dcMovingMemDc.FillRect(rcLineRect, &brShuttleGuidLineBrush);
//	dcMemDc.AlphaBlend(rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		&dcMovingMemDc, rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		blAlphaBlend);
//
//	rcLineRect.left = m_iTimelineCursorPoint - iGuideAreaWidth;
//	rcLineRect.right = rcLineRect.left + kTimelineCursorDragGuideLineThickness;
//
//	// ラインを描画
//	dcMovingMemDc.FillRect(rcLineRect, &brShuttleGuidLineBrush);
//	brShuttleGuidLineBrush.DeleteObject();
//	dcMemDc.AlphaBlend(rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		&dcMovingMemDc, rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		blAlphaBlend);
//}



/*

表示位置計算

*/
// Viewのサイズから各表示パネルの座標を計算して設定する。
void TimelineEditorView::SetPanelRect(void)
{
	CRect rcViewRect;
	GetClientRect(&rcViewRect);
	//rcViewRect.left += WINDOWBORDER_THICKNESS;
	//rcViewRect.right -= WINDOWBORDER_THICKNESS;
	//rcViewRect.top += WINDOWBORDER_THICKNESS;
	//rcViewRect.bottom -= WINDOWBORDER_THICKNESS;
	//double dLeft = 0.0, dRight = 0.0, dTop = 0.0, dBottom = 0.0;

	float fViewHeight = static_cast<float>(rcViewRect.Height());
	long lViewHeight = static_cast<long>(floor((fViewHeight - static_cast<float>(m_pSeekBarRect->Height())) / 2));

	// TODO: いずれはメンバ変数に！
	int iTimelineEditHeaderDefaltHeight = kTimelineEditHeaderDefaltHeight;
	int iTimelineControlPanelDefaultWidth = kTimelineControlPanelDefaultWidth;

	// デバッグ情報表示エリア配置
#ifdef _DEBUG
	m_pDebugInfoPanelRect->CopyRect(rcViewRect);
	m_pDebugInfoPanelRect->bottom = rcViewRect.top + lViewHeight;
	m_pDebugInfoPanelRect->SetVert(rcViewRect.Height());
#endif

	// タイムライン編集エリア配置
	m_pTimelineEditPanelRect->CopyRect(rcViewRect);
#ifdef _DEBUG
	m_pTimelineEditPanelRect->top = m_pDebugInfoPanelRect->bottom + kSplitterHeight;
#endif
	m_pTimelineEditPanelRect->SetVert(rcViewRect.Height());

	// タイムラインヘッダーエリアの配置
	m_pTimelineEditHeaderRect->CopyRect(m_pTimelineEditPanelRect);
	m_pTimelineEditHeaderRect->bottom = m_pTimelineEditHeaderRect->top + iTimelineEditHeaderDefaltHeight;
	m_pTimelineEditHeaderRect->SetVert(rcViewRect.Height());

	// タイムラインコントロールエリアの配置
	m_pTimelineControlPanelRect->CopyRect(m_pTimelineEditHeaderRect);
	m_pTimelineControlPanelRect->right = m_pTimelineControlPanelRect->left + iTimelineControlPanelDefaultWidth;
	m_pTimelineControlPanelRect->SetVert(rcViewRect.Height());

	// シークバーエリアの配置
	m_pSeekBarRect->CopyRect(m_pTimelineEditHeaderRect);
	m_pSeekBarRect->left = m_pTimelineControlPanelRect->right + kSplitterWidth;
	m_pSeekBarRect->SetVert(rcViewRect.Height());

	// トラックヘッダの配置
	m_pTrackHeaderRect->CopyRect(m_pTimelineEditPanelRect);
	m_pTrackHeaderRect->top = m_pTimelineEditHeaderRect->bottom + kSplitterHeight;
	m_pTrackHeaderRect->right = m_pTimelineControlPanelRect->right;
	m_pTrackHeaderRect->SetVert(rcViewRect.Height());

	// タイムラインデータエリアの配置
	m_pTimelineDataRect->CopyRect(m_pTimelineEditPanelRect);
	m_pTimelineDataRect->left = m_pSeekBarRect->left;
	m_pTimelineDataRect->top = m_pTrackHeaderRect->top;
	m_pTimelineDataRect->SetVert(rcViewRect.Height());

	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRectBefor = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(0));
	pTrackDataRectBefor->CopyRect(m_pTimelineEditPanelRect);
	pTrackDataRectBefor->top = m_pTrackHeaderRect->top;
	pTrackDataRectBefor->bottom = pTrackDataRectBefor->top + pTrackDataRectBefor->GetHeight();
	pTrackDataRectBefor->SetVert(rcViewRect.Height());
	TrackDataRect* pTrackDataRect = nullptr;
	for (int i = 1; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		pTrackDataRect = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i));
		pTrackDataRect->CopyRect(pTrackDataRectBefor);
		pTrackDataRect->top = pTrackDataRectBefor->bottom;
		pTrackDataRect->bottom = pTrackDataRect->top + pTrackDataRect->GetHeight();
		pTrackDataRect->SetVert(rcViewRect.Height());
		pTrackDataRectBefor = pTrackDataRect;
	}

	m_pTimelineDataOperator->CalcTimelineDisplayRange();
	m_iTimelineCursorPoint = m_pTimelineDataOperator->GetTimelineCursorPoint();

	// タイムラインカーソルヒット領域の配置
	m_pTimelineCursorHitArea->CopyRect(m_pTimelineEditPanelRect);
	m_pTimelineCursorHitArea->top = m_pSeekBarRect->top;
	m_pTimelineCursorHitArea->left = m_iTimelineCursorPoint - kTimelineCursorDragArea;
	m_pTimelineCursorHitArea->right = m_iTimelineCursorPoint + kTimelineCursorDragArea;
	m_pTimelineCursorHitArea->SetVert(rcViewRect.Height());

	return;
}

/*

初期設定系

*/
// 表示エリアの初期設定
void TimelineEditorView::InitAreaRect(void)
{
	// TODO: 色は後で定数を作る
	m_pDebugInfoPanelRect = new OpenGLRect();
	m_pDebugInfoPanelRect->SetRectEmpty();
	m_pDebugInfoPanelRect->SetBothColor(ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT);

	m_pTimelineEditPanelRect = m_pTimelineDataOperator->GetTimelineEditPanelRect();
	m_pTimelineEditPanelRect->SetRectEmpty();
	m_pTimelineEditPanelRect->SetBothColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_pTimelineEditHeaderRect = m_pTimelineDataOperator->GetTimelineEditHeaderRect();
	m_pTimelineEditHeaderRect->SetRectEmpty();

	m_pTimelineControlPanelRect = m_pTimelineDataOperator->GetTimelineControlPanelRect();
	m_pTimelineControlPanelRect->SetRectEmpty();
	m_pTimelineControlPanelRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, REDCOLOR_BRUSH_FLOAT, GREENCOLOR_BRUSH_FLOAT);

	m_pSeekBarRect = m_pTimelineDataOperator->GetSeekBarRect();
	m_pSeekBarRect->SetRectEmpty();
	m_pSeekBarRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT);

	m_pTrackHeaderRect = m_pTimelineDataOperator->GetTrackHeaderRect();
	m_pTrackHeaderRect->SetRectEmpty();
	m_pTrackHeaderRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_pTimelineDataRect = m_pTimelineDataOperator->GetTimelineDataRect();
	m_pTimelineDataRect->SetRectEmpty();
	m_pTimelineDataRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_pTimelineCursorHitArea = m_pTimelineDataOperator->GetTimelineCursorHitArea();
	m_pTimelineCursorHitArea->SetRectEmpty();

	m_pTransitionRect = m_pTimelineDataOperator->GetTransitionRect();
	m_pTransitionRect->SetRectEmpty();
	m_pTransitionRect->SetColor(ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT);
	m_pTransitionRect->SetBorderColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);
}

// 動作確認用オブジェクトの初期設定
void TimelineEditorView::InitTestObject(void)
{
	UUID uiTrackId, uiTrackRectId;

	m_pTrackDataVideoManager->CreateTrackData(0, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(1, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(2, uiTrackId, uiTrackRectId);

}

// 表示倍率関連のマップ作成
void TimelineEditorView::CreateZoomMap(void)
{
}











// コンテキストメニュー表示
void TimelineEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CPoint poClientPoint = point;
	ScreenToClient(&poClientPoint);
	CMenu mContextMenu;
	if (m_pTimelineDataOperator->OnContextMenu(poClientPoint, mContextMenu))
	{
		mContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
	}
}

// クリップ削除
void TimelineEditorView::OnClipDelete(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。

	if (m_pTimelineDataOperator->DeleteClip())
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション設定（In）
void TimelineEditorView::OnTransitionSetIn(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。

	CString strMessage;
	if (m_pTimelineDataOperator->OnTransitionSetIn(static_cast<PCTSTR>(strMessage)))
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション設定（Out）
void TimelineEditorView::OnTransitionSetOut(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。

	CString strMessage;
	if (m_pTimelineDataOperator->OnTransitionSetOut(static_cast<PCTSTR>(strMessage)))
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション解除（In・中間）
void TimelineEditorView::OnTransitionResetInCenter(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (m_pTimelineDataOperator->OnTransitionResetInCenter())
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション解除（In・開始点）
void TimelineEditorView::OnTransitionResetInStart(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (m_pTimelineDataOperator->OnTransitionResetInStart())
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション解除（In・終了点）
void TimelineEditorView::OnTransitionResetInEnd(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (m_pTimelineDataOperator->OnTransitionResetInEnd())
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション解除（Out・中間）
void TimelineEditorView::OnTransitionResetOutCenter(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (m_pTimelineDataOperator->OnTransitionResetOutCenter())
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション解除（Out・開始点）
void TimelineEditorView::OnTransitionResetOutStart(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (m_pTimelineDataOperator->OnTransitionResetOutStart())
	{
		Invalidate();
		UpdateWindow();
	}
}

// トランジション解除（Out・終了点）
void TimelineEditorView::OnTransitionResetOutEnd(void)
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (m_pTimelineDataOperator->OnTransitionResetOutEnd())
	{
		Invalidate();
		UpdateWindow();
	}
}

// 再生中の画面描画
void TimelineEditorView::OnPlay(int iSpeed)
{
	if (m_pTimelineDataOperator->OnPlay(iSpeed))
	{
		Invalidate();
		// 再生が間に合わなくなるのでUpdateWindowはしない
		//UpdateWindow();
	}
}

// 再生の停止
void TimelineEditorView::OnStop(void)
{
	m_pTimelineDataOperator->OnStop();
}

// タイムラインカーソル位置同期
int TimelineEditorView::GetTimelineCursorFrame(void)
{
	return m_pTimelineDataOperator->GetOperatingTimelineCursorFramePosition();
}





LRESULT TimelineEditorView::OnNcHitTest(CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CRect rcRect;
	GetClientRect(&rcRect);

	//if (point.x < rcRect.left + WINDOWBORDER_THICKNESS)
	//{
		//return HTLEFT;
	//}
	return OpenGLView::OnNcHitTest(point);
}
