// TimelineEditerView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditerView.h"

#include "OpenGLRect.h"
#include "ClipDataRect.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
#include "TrackDataManager.h"
//#include "math.h"

#include <map>


// TimelineEditerView

IMPLEMENT_DYNCREATE(TimelineEditerView, OpenGLView)

TimelineEditerView::TimelineEditerView()
{

}

TimelineEditerView::~TimelineEditerView()
{
}

BEGIN_MESSAGE_MAP(TimelineEditerView, OpenGLView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// TimelineEditerView 描画

void TimelineEditerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// TimelineEditerView 診断

#ifdef _DEBUG
void TimelineEditerView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void TimelineEditerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// TimelineEditerView メッセージ ハンドラー
// 画面描画
void TimelineEditerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで OpenGLView::OnPaint() を呼び出さないでください。

	CRect rcRect;
	GetClientRect(&rcRect);
	int iHeight = rcRect.Height();
	CString strPoint;

	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// 背景塗りつぶし
	::glClearColor(TIMELINEBASECOLOR_BRUSH_FLOAT);
	::glClear(GL_COLOR_BUFFER_BIT);
	//::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// プレビューエリア枠線描画
	m_prcDebugInfoPanelRect->DrawMyBorderRect(3.0f);


	// タイムラインヘッダーコントロールパネル描画
	DrawTimelineControlPanel();

	// シークバーエリア描画
	DrawSeekBar(dc, iHeight);

	//// 静止クリップ描画
	DrawClip(iHeight);

	// 操作イメージ描画
	if ((m_fSingleInTriming || m_fSingleOutTriming || m_fMoving) && (!(m_clMovingClipData->GetOperatingRect()->IsRectEmpty())))
	{
		DrawOperatingClip(dc, iHeight);
	}

	// トラックヘッダー描画
	DrawTrackHeader();

	// タイムラインデータエリア枠線描画
	DrawTimelineDataRect();

	// トラック枠描画
	DrawTrack(iHeight, dc);

	// タイムラインカーソル／シャトル操作補助線描画
	DrawTimelineCursor(dc, iHeight);



#ifdef PROTOTYPEMODE
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);

	strFrameNumber.Format(_T("TLCursor %d"), m_iTimelineCursorFramePosition);
	ChangeScreenPointToOpenGLPoint(5, 15, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Frame Left %d Right %d"), m_iLeftFrameNumber, m_iRightFrameNumber);
	ChangeScreenPointToOpenGLPoint(5, 30, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating TLCursor %d"), m_iTimelineCursorFramePosition + m_iOperatingFrameCount);
	ChangeScreenPointToOpenGLPoint(5, 45, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating Left %d Right %d"), m_iLeftFrameNumber + m_iOperatingFrameCount, m_iRightFrameNumber + m_iOperatingFrameCount);
	ChangeScreenPointToOpenGLPoint(5, 60, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("CursorLine Point %d"), m_iTimelineCursorPoint);
	ChangeScreenPointToOpenGLPoint(5, 75, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("f/p %d p/f %d"), m_iFramePerPoint, m_iPointPerFrame);
	ChangeScreenPointToOpenGLPoint(5, 150, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	DeleteObject(hfDrawFont);
#endif



	::glFlush();
	::SwapBuffers(m_pDC->GetSafeHdc());

	::wglMakeCurrent(NULL, NULL);
}

// Ｌボタンダウン
void TimelineEditerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// タイムラインデータエリア内判定
	if (m_prcTimelineDataRect->PtInRect(point))
	{
		//BOOL fRet;
		//fRet = m_pTimelineDataOperator->OnLButtonDown(nFlags, point);
		// トラック判定
		m_clSelectedTrack = IsPointInAnyTrack(point);
		m_clSelectedTrackInfo = m_clSelectedTrack->GetTrackDataInfo();
		if (m_clSelectedTrack != nullptr)
		{
			m_clOperateToTrack = m_clSelectedTrack;
			m_clOperateToTrackInfo = m_clSelectedTrackInfo;

			if (IsPointInAnyClipRect(point))
			{
				m_fLButtonClicking = TRUE;
				SetCapture(); // マウスをキャプチャー( OnLButtonUp()で解放)
				m_poMousePointerLocation = point;	// 移動量計算のため、初期座標を保存
				m_clMovingClipData->SetOperatingRect(static_cast<CRect>(m_clMovingClipData));
				m_rcMousePointRect.CopyRect(static_cast<CRect>(m_clMovingClipData));
				m_iOperatingClipFrameCount = 0;
				m_iEnableMovingFrameCount = 0;
				m_pEnableMovingTrack = nullptr;
				Invalidate();
			}
		}
		if (!m_fLButtonClicking)
		{
			//// タイムラインカーソル判定
			//else if (m_prcTimelineCursorHitArea.PtInRect(point))
			//{
			//	m_fLButtonClicking = TRUE;
			//	m_fDragShuttling = TRUE;
			//	SetCapture(); // マウスをキャプチャー( OnLButtonUp()で解放)
			//	m_poMousePointerLocation = point;	// 移動量計算のため、初期座標を保存
			//	m_iOperatingFrameCount = 0;
			//	m_fSuttleSpeed = 0;
			//	Invalidate();
			//}
		}
	}
	else
	{
		// タイムラインカーソル判定
		if (m_prcTimelineCursorHitArea->PtInRect(point))
		{
			m_fLButtonClicking = TRUE;
			m_fDragShuttling = TRUE;
			SetCapture(); // マウスをキャプチャー( OnLButtonUp()で解放)
			m_poMousePointerLocation = point;	// 移動量計算のため、初期座標を保存
			m_iOperatingFrameCount = 0;
			m_fSuttleSpeed = 0;
			Invalidate();
		}
		// シークバー内判定
		else if (IsPointInSeekBar(point))
		{
			m_fLButtonClicking = TRUE;
			m_fScrubing = TRUE;
			SetCapture(); // マウスをキャプチャー( OnLButtonUp()で解放)
			m_poMousePointerLocation = point;	// 移動量計算のため、初期座標を保存
			m_iOperatingFrameCount = 0;

			//TODO: デバッグ用なので後で削除
#ifdef PROTOTYPEMODE
			double dPointX, dPointY;
			CPaintDC dc(this);
			int iDummy;
			int iFrame = ChangeDisplayPointToTimelineFramePosition(point, iDummy);
			CRect rcViewRect;
			GetClientRect(&rcViewRect);
			HFONT hfDrawFont;
			CreateDrawFont(10, 0, DEFAULT_FONTFACE, hfDrawFont);
			CString strFrameNumber;
			strFrameNumber.Format(_T("SEEKBAR CLICK FRAME %d"), iFrame);
			ChangeScreenPointToOpenGLPoint(350, 15, rcViewRect.Height(), dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			strFrameNumber.Format(_T("SEEKBAR CLICK POINT %d"), point.x);
			ChangeScreenPointToOpenGLPoint(350, 30, rcViewRect.Height(), dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			DeleteObject(hfDrawFont);
			DeleteObject(dc);
#endif
		}
		// タイムラインコントロールパネル内判定
		else if (IsPointInTimelineControlPanel(point))
		{
			if (m_iFramePerPoint < 1)
			{
				--m_iPointPerFrame;
				if (m_iPointPerFrame < 1)
				{
					m_iFramePerPoint = 2;
				}
			}
			else
			{
				if (m_iFramePerPoint <= 216000)
				{
					// TODO: もっと効率よく！
					if (m_iFramePerPoint <10)
					{
						++m_iFramePerPoint;
					}
					else if (m_iFramePerPoint <60)
					{
						m_iFramePerPoint += 5;
					}
					else if (m_iFramePerPoint <600)
					{
						m_iFramePerPoint += 60;
					}
					else if (m_iFramePerPoint <3600)
					{
						m_iFramePerPoint += 300;
					}
					else if (m_iFramePerPoint <36000)
					{
						m_iFramePerPoint += 18000;
					}
					else
					{
						m_iFramePerPoint += 36000;
						if (m_iFramePerPoint > 216000)
						{
							m_iFramePerPoint = 216000;
						}
					}
				}
				else
				{
					m_iFramePerPoint = 216000;
				}
			}
			if (ChangeDisplayScale())
			{
				Invalidate();
			}
		}
	}


	OpenGLView::OnLButtonDown(nFlags, point);
}

// Ｌボタンアップ
void TimelineEditerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	m_fLButtonClicking = FALSE; // フラグを下げる
	ReleaseCapture(); // マウスを解放します。

	if (m_fMoving)
	{
		m_clSelectedTrackInfo->DeleteClip(m_clMovingClipData->m_iTimelineInPoint);
		m_clMovingClipData->m_iTimelineInPoint += m_iOperatingClipFrameCount;
		(m_pEnableMovingTrack->GetTrackDataInfo())->AddClip(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData);
		//m_clOperateToTrackInfo->AddClip(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData);
		m_clMovingClipData->CopyRect(m_clMovingClipData->GetOperatingRect());
		//TODO: いずれは復活させないといけない－＞移動中はいいけど静止状態の時にトラックからIn点を持ってくるのが難しい！
		//m_clMovingClipData->m_iTimelineInPoint = 0;
	}
	else if (m_fSingleInTriming)
	{
		m_clSelectedTrackInfo->ChangeClip(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount, m_clMovingClipData);
		m_clMovingClipData->m_iTimelineInPoint += m_iOperatingClipFrameCount;
		int iClipDuration = m_clMovingClipData->GetDuration();
		// In側に伸びる（マイナス方向への移動）は長さを加算
		m_clMovingClipData->SetDuration(iClipDuration - m_iOperatingClipFrameCount);
		m_clMovingClipData->CopyRect(m_clMovingClipData->GetOperatingRect());
	}
	else if (m_fSingleOutTriming)
	{
		int iClipDuration = m_clMovingClipData->GetDuration();
		m_clMovingClipData->SetDuration(iClipDuration + m_iOperatingClipFrameCount);
		m_clMovingClipData->CopyRect(m_clMovingClipData->GetOperatingRect());
	}
	else if (m_fScrubing)
	{
		if ((point.x == m_poMousePointerLocation.x) && (point.y == m_poMousePointerLocation.y))
		{
			int iDummy;
			int iFrame = ChangeDisplayPointToTimelineFramePosition(point, iDummy);
			// TODO: できれば実現
			//DrawAnimation(iFrame - m_iTimelineCursorFramePosition);
			m_iOperatingFrameCount = iFrame - m_iTimelineCursorFramePosition;
			m_iTimelineCursorFramePosition = iFrame;
			m_iLeftFrameNumber += m_iOperatingFrameCount;
			m_iRightFrameNumber += m_iOperatingFrameCount;
		}
		else
		{
			m_iTimelineCursorFramePosition += m_iOperatingFrameCount;
			m_iLeftFrameNumber += m_iOperatingFrameCount;
			m_iRightFrameNumber += m_iOperatingFrameCount;
		}
	}

	m_rcMousePointRect.SetRectEmpty();
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;
	m_iEnableMovingFrameCount = 0;
	m_pEnableMovingTrack = nullptr;

	Invalidate();

	m_clMovingClipData = nullptr;
	m_fMoving = FALSE;
	m_fSingleInTriming = FALSE;
	m_fSingleOutTriming = FALSE;
	m_fScrubing = FALSE;
	m_fDragShuttling = FALSE;

	OpenGLView::OnLButtonUp(nFlags, point);
}

// Ｒボタンアップ
void TimelineEditerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if (IsPointInTimelineControlPanel(point))
	{
		if (m_iFramePerPoint < 1)
		{
			if (m_iPointPerFrame < 120)
			{
				++m_iPointPerFrame;
			}
		}
		else
		{
			// TODO: もっと効率よく！
			if (m_iFramePerPoint <= 10)
			{
				--m_iFramePerPoint;
			}
			else if (m_iFramePerPoint <= 60)
			{
				m_iFramePerPoint -= 5;
			}
			else if (m_iFramePerPoint <= 600)
			{
				m_iFramePerPoint -= 60;
			}
			else if (m_iFramePerPoint <= 3600)
			{
				m_iFramePerPoint -= 300;
			}
			else if (m_iFramePerPoint <= 36000)
			{
				m_iFramePerPoint -= 18000;
			}
			else
			{
				m_iFramePerPoint -= 36000;
			}
			if (m_iFramePerPoint < 1)
			{
				m_iPointPerFrame = 2;
			}
		}
		if (ChangeDisplayScale())
		{
			Invalidate();
		}
	}
	else
	{
		ClientToScreen(&point);
		//OnContextMenu(this, point);
	}

	//OpenGLView::OnRButtonUp(nFlags, point);
}

// マウス移動
void TimelineEditerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if (m_fLButtonClicking)
	{
		CSize szMoveSize(point - m_poMousePointerLocation);
		if (m_fMoving)
		{
			m_clOperateToTrack = IsPointInAnyTrack(point);
			if (m_clOperateToTrack == nullptr)
			{
				m_clOperateToTrack = m_clSelectedTrack;
			}
			m_clOperateToTrackInfo = m_clOperateToTrack->GetTrackDataInfo();
			m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_clMovingClipData->m_iTimelineInPoint);
			CalcClipRectDisplayPoint(m_rcMousePointRect, m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), m_iOperatingClipFrameCount);
			CheckMove(point);
			CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), m_iOperatingClipFrameCount);
			Invalidate();
		}
		else if (m_fSingleInTriming || m_fSingleOutTriming)
		{
			m_clMovingClipData->SetOperatingRect(static_cast<CRect>(m_clMovingClipData)); // 伸縮分のイメージ座標
			if (m_fSingleInTriming)
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_clMovingClipData->m_iTimelineInPoint);
				CheckInTrim();
				CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), 0, m_iOperatingClipFrameCount);
			}
			else
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize,
					(m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration()));
				CheckOutTrim();
				CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), 0, 0, m_iOperatingClipFrameCount);
			}
			Invalidate();
		}
		else if (m_fScrubing)
		{
			szMoveSize.cx *= -1;
			m_iOperatingFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_iTimelineCursorFramePosition);
			// 最小範囲チェック
			// TODO: 最大範囲チェックは？
			if ((m_iTimelineCursorFramePosition + m_iOperatingFrameCount) < 0)
			{
				m_iOperatingFrameCount = -1 * m_iTimelineCursorFramePosition;
			}
			Invalidate();
		}
		else if (m_fDragShuttling)
		{
			//// マウス位置から倍速速度を取得
			//m_fSuttleSpeed = SetShuttleSpeed(point, szMoveSize);
			//Invalidate();
		}
	}

	OpenGLView::OnMouseMove(nFlags, point);
}

// サイズ変更
void TimelineEditerView::OnSize(UINT nType, int cx, int cy)
{
	OpenGLView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	SetPanelRect();

}

// 初期設定
void TimelineEditerView::OnInitialUpdate()
{
	OpenGLView::OnInitialUpdate();

	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	InitAreaRect();

	m_pTimelineDataManager = new TimelineDataManager();
	m_pTimelineDataManager->InitializeTimelineDataManager(m_uiTimelineDataManager);
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManager);
	assert(m_pTrackDataVideoManager);
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManager);
	assert(m_pTrackDataAudioManager);
	m_pClipDataManager = m_pTimelineDataManager->GetClipDataManager(m_uiClipDataManager);
	assert(m_pClipDataManager);
	InitTestObject();

	m_fLButtonClicking = FALSE;
	m_fMoving = FALSE;
	m_fSingleInTriming = FALSE;
	m_fSingleOutTriming = FALSE;
	m_fScrubing = FALSE;
	m_fDragShuttling = FALSE;

	m_prcDebugInfoPanelRect->bottom = m_prcDebugInfoPanelRect->top + kPreviewPanelDefaltHeight;

	m_iLeftFrameNumber = 0;
	m_iRightFrameNumber = 0;
	m_iTimelineCursorFramePosition = 0;
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;

	m_iFramePerPoint = 0;
	m_iPointPerFrame = 1;
	ChangeDisplayScale();


}

// 破棄
void TimelineEditerView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	m_pTimelineDataManager->DeleteTimelineDataManager();
	delete m_pTimelineDataManager;


	delete m_prcDebugInfoPanelRect;
	delete m_prcTimelineEditPanelRect;
	delete m_prcTimelineEditHeaderRect;
	delete m_prcTimelineControlPanelRect;
	delete m_prcSeekBarRect;
	delete m_prcTrackHeaderRect;
	delete m_prcTimelineDataRect;
	delete m_prcTimelineCursorHitArea;

}




/*

描画系

*/
// タイムラインコントロールパネル描画
void TimelineEditerView::DrawTimelineControlPanel(void)
{
	m_prcTimelineControlPanelRect->DrawMyFillRect();
}

// シークバー描画
void TimelineEditerView::DrawSeekBar(const CDC& dcPaintDC, const int iHeight)
{

	// 背景塗りつぶし
	m_prcSeekBarRect->DrawMyFillRect();

	int iDrawFrame = m_iLeftFrameNumber + m_iOperatingFrameCount;
	if (iDrawFrame < 0)
	{
		iDrawFrame = 0;
	}
	else
	{
		int iSurplusFrame = iDrawFrame % m_iSmallScaleDrawInterval;
		if (iSurplusFrame != 0)
		{
			iDrawFrame += (m_iSmallScaleDrawInterval - iSurplusFrame);
		}
	}

	POINT pScaleLine;
	pScaleLine.y = m_prcSeekBarRect->top;
	pScaleLine.x = ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);

	while (pScaleLine.x < m_prcSeekBarRect->right)
	{
		if (pScaleLine.x > m_prcSeekBarRect->left)
		{
			// 大目盛り
			if ((iDrawFrame % m_iBigScaleDrawInterval) == 0)
			{
				DrawBigScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
			// 中目盛り
			else if ((iDrawFrame % m_iMiddleScaleDrawInterval) == 0)
			{
				DrawMiddleScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
			// 小目盛り
			else
			{
				DrawSmallScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
		}
		iDrawFrame += m_iSmallScaleDrawInterval;
		pScaleLine.x = ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);
	}

}

// 大目盛り描画
void TimelineEditerView::DrawBigScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{

	// TODO: 製品はタイムコードを表示
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("%d"), iDrawFrame);
	CreateDrawFont(SEEKBARTIMECODE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_prcSeekBarRect->top + SEEKBARTIMECODE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	// 目盛り、ライン描画
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->top + SEEKBARBIGSCALE_TOPMARGIN, pScaleLine.x, m_prcSeekBarRect->bottom,
		SEEKBARBIGSCALE_LINEWIDTH, SEEKBARBIGSCALECOLOR_BRUSH_FLOAT);
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->bottom + 1, pScaleLine.x, m_prcTimelineDataRect->bottom,
		SEEKBARBIGSCALELINE_LINEWIDTH, SEEKBARBIGSCALELINECOLOR_BRUSH_FLOAT);

	DeleteObject(hfDrawFont);
	return;
}

// 中目盛り描画
void TimelineEditerView::DrawMiddleScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("%d"), iDrawFrame);
	CreateDrawFont(SEEKBARTIMECODE_MIDDLE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_prcSeekBarRect->top + SEEKBARTIMECODE_MIDDLE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);
#endif

	// 目盛り、ライン描画
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->top + SEEKBARMIDDLESCALE_TOPMARGIN, pScaleLine.x, m_prcSeekBarRect->bottom,
		SEEKBARMIDDLESCALE_LINEWIDTH, SEEKBARMIDDLESCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->bottom + 1, pScaleLine.x, m_prcTimelineDataRect->bottom,
		SEEKBARMIDDLESCALELINE_LINEWIDTH, SEEKBARMIDDLESCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// 小目盛り描画
void TimelineEditerView::DrawSmallScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{
	// 目盛り、ライン描画
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->top + SEEKBARSMALLSCALE_TOPMARGIN, pScaleLine.x, m_prcSeekBarRect->bottom,
		SEEKBARSMALLSCALE_LINEWIDTH, SEEKBARSMALLSCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_SMALLSCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->bottom + 1, pScaleLine.x, m_prcTimelineDataRect->bottom,
		SEEKBARSMALLSCALELINE_LINEWIDTH, SEEKBARSMALLSCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// トラックヘッダー描画
void TimelineEditerView::DrawTrackHeader(void)
{
	// TODO: とりあえず今は枠だけ
	m_prcTrackHeaderRect->DrawMyBorderRect();
}

// トラック描画
void TimelineEditerView::DrawTrack(const int iHeight, const CPaintDC& dcPaintDC)
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
		DrawTextOnGL(static_cast<PCTSTR>(pTrackDataRect->GetTrackName()), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	}
	DeleteObject(hfDrawFont);
}

// タイムラインデータエリア描画
void TimelineEditerView::DrawTimelineDataRect(void)
{
	// TODO: とりあえず今は枠だけ
	m_prcTimelineDataRect->DrawMyBorderRect();
}

// クリップの描画を行う
BOOL TimelineEditerView::DrawClip(const int iHeight)
{
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	for (int i = 0; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		DrawClipInTrack(m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i)), iHeight);
	}

	return TRUE;
}

// トラック内の表示範囲内クリップをサーチして描画
void TimelineEditerView::DrawClipInTrack(TrackDataRect* pTrackDataRect, const int iHeight)
{
	int iStartFrame = m_iLeftFrameNumber + m_iOperatingFrameCount;
	if (iStartFrame < 0)
	{
		iStartFrame = 0;
	}
	ClipDataPositionMap mpClipDataMap;
	int iClipCount = pTrackDataRect->GetTrackDataInfo()->GetClipDataInRange(iStartFrame, m_iRightFrameNumber + m_iOperatingFrameCount, mpClipDataMap);
	ClipDataRect* pClipData;
	ClipDataRect* pClipDataLeft;
	pClipDataLeft = nullptr;
	if (iClipCount > 0)
	{
		ClipDataPositionMap::iterator itr = mpClipDataMap.begin();
		while (itr != mpClipDataMap.end())
		{
			pClipData = (*itr).second;
			CalcClipRectDisplayPoint(static_cast<CRect&>(*pClipData), pClipData, static_cast<CRect>(pTrackDataRect));
			pClipData->SetVert(iHeight);
			pClipData->DrawMyFillRect();
			if (pClipDataLeft != nullptr)
			{
				// トランジション部分塗り替え
				if (pClipData->left < pClipDataLeft->right)
				{
					m_prcTransisionRect->CopyRect(pClipData);
					m_prcTransisionRect->right = pClipDataLeft->right;
					m_prcTransisionRect->SetVert(iHeight);
					m_prcTransisionRect->DrawMyFillRect();
					m_prcTransisionRect->DrawMyLeftLine();
					m_prcTransisionRect->DrawMyRightLine();
					m_prcTransisionRect->SetRectEmpty();

				}
				else if (pClipData->left == pClipDataLeft->right)
				{
					pClipDataLeft->DrawMyRightLine();
				}
			}
			pClipDataLeft = pClipData;
			++itr;
		}
	}

}

// 操作中クリップの描画を行う
BOOL TimelineEditerView::DrawOperatingClip(const CDC& dcPaintDC, const int iHeight)
{
	// TODO: 元のクリップの色を変える　元々各タイミングで実施するよう変更
	m_clMovingClipData->DrawOperatingOldRect(iHeight);

	CRect* rcOperatingRect = m_clMovingClipData->GetOperatingRect();

	if (m_fSingleInTriming || m_fSingleOutTriming)
	{
		m_clMovingClipData->DrawSingleTrimRect(iHeight, m_fSingleInTriming);

#ifdef PROTOTYPEMODE
		//TODO: デバッグ
		CString strText;
		double dPointX, dPointY;
		int iPoint;
		int iDuration;
		if (m_fSingleInTriming)
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount;
		}
		else
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint;
		}
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
		strText.Format(_T("TrimingClipInPoint  %d"), iPoint);
		ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipLeftPoint  %d"), rcOperatingRect->left);
		ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		if (m_fSingleInTriming)
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1;
			iDuration = m_clMovingClipData->GetDuration() - m_iOperatingClipFrameCount;
		}
		else
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + m_iOperatingClipFrameCount;
			iDuration = m_clMovingClipData->GetDuration() + m_iOperatingClipFrameCount;
		}
		strText.Format(_T("TrimingClipOutPoint  %d"), iPoint);
		ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipRightPoint  %d"), rcOperatingRect->right);
		ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipDuration  %d"), iDuration);
		ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		DeleteObject(hfDrawFont);
#endif

	}
	else
	{
		// ドロップ位置用
		m_clMovingClipData->DrawMovingRect(iHeight);

#ifdef PROTOTYPEMODE
		//TODO: デバッグ
		CString strText;
		double dPointX, dPointY;
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
		strText.Format(_T("MovingClipInPoint  %d"), m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount);
		ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("MovingClipLeftPoint  %d"), rcOperatingRect->left);
		ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("MovingClipRightPoint  %d"), rcOperatingRect->right);
		ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("BorderClipLeftPoint  %d"), m_rcMousePointRect.left);
		ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("BorderClipRightPoint  %d"), m_rcMousePointRect.right);
		ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
#endif

		// マウス位置追随用
		if ((!(m_rcMousePointRect.EqualRect(rcOperatingRect))) /*&& ((m_rcMousePointRect->m_iTimelineInPoint + m_iOperatingClipFrameCount) > 0)*/)
		{
			double dLeft, dRight, dTop, dBottom;
			ChangeScreenRectToOpenGLPoint(m_rcMousePointRect, iHeight, dLeft, dRight, dTop, dBottom);
			m_clMovingClipData->SetOverlappingVert(static_cast<float>(m_rcMousePointRect.left), static_cast<float>(iHeight - m_rcMousePointRect.top),
				static_cast<float>(m_rcMousePointRect.right), static_cast<float>(iHeight - m_rcMousePointRect.bottom));
			m_clMovingClipData->DrawOverlappingRect(iHeight);

#ifdef PROTOTYPEMODE
			strText.Format(_T("ClipBorderLeftPoint  %d"), m_rcMousePointRect.left);
			ChangeScreenPointToOpenGLPoint(700, 90, iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			DeleteObject(hfDrawFont);
#endif
		}
	}

	return TRUE;
}

// タイムラインカーソルの描画を行う
BOOL TimelineEditerView::DrawTimelineCursor(const CDC& dcPaintDC, const int iHeight)
{
	// ラインを描画
	DrawLine(iHeight, m_iTimelineCursorPoint, m_prcSeekBarRect->top, m_iTimelineCursorPoint, m_prcTimelineDataRect->bottom,
		TIMELINECURSOR_LINEWIDTH, TIMELINECURSORCOLOR_BRUSH_FLOAT);

	//if (m_fDragShuttling)
	//{
	//	blAlphaBlend.SourceConstantAlpha = kTimelineCursorDragGuideLineAlpha;

	//	//TODO: 位置は検討が必要
	//	CString strText;
	//	int iOldBkMode = dcMemDc.SetBkMode(TRANSPARENT);
	//	COLORREF crOldTextColor = dcMemDc.SetTextColor(SEEKBARTIMECODETEXTCOLOR_BRUSH);
	//	strText.Format(_T("×  %d"), m_fSuttleSpeed);
	//	dcMemDc.TextOutW(90, m_prcTimelineControlPanelRect.bottom - 20, strText);
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

//// シャトル操作時のガイドラインを表示する
//void TimelineEditerView::DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcLineRect, float fGuideAreaWidth)
//{
//	int iGuideAreaWidth = static_cast<int>(floor(m_prcTimelineDataRect.Width() * fGuideAreaWidth));
//
//	rcLineRect.left = m_iTimelineCursorPoint + iGuideAreaWidth - kTimelineCursorDragGuideLineThickness;
//	rcLineRect.right = rcLineRect.left + kTimelineCursorDragGuideLineThickness;
//	rcLineRect.top = m_prcTimelineDataRect.top;
//	rcLineRect.bottom = m_prcTimelineDataRect.bottom;
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

表示位置計算系

*/
// Viewのサイズから各表示パネルの座標を計算して設定する。
void TimelineEditerView::SetPanelRect(void)
{
	CRect rcViewRect;
	GetClientRect(&rcViewRect);
	double dLeft = 0.0, dRight = 0.0, dTop = 0.0, dBottom = 0.0;

	float fViewHeight = static_cast<float>(rcViewRect.Height());
	long lViewHeight = static_cast<long>(floor((fViewHeight - static_cast<float>(m_prcSeekBarRect->Height())) / 2));

	// TODO: いずれはメンバ変数に！
	int iTimelineEditHeaderDefaltHeight = kTimelineEditHeaderDefaltHeight;
	int iTimelineControlPanelDefaultWidth = kTimelineControlPanelDefaultWidth;

	// プレビューエリア配置
	m_prcDebugInfoPanelRect->CopyRect(rcViewRect);
	m_prcDebugInfoPanelRect->bottom = rcViewRect.top + lViewHeight;
	m_prcDebugInfoPanelRect->SetVert(rcViewRect.Height());

	// タイムライン編集エリア配置
	m_prcTimelineEditPanelRect->CopyRect(rcViewRect);
	m_prcTimelineEditPanelRect->top = m_prcDebugInfoPanelRect->bottom + kSplitterHeight;
	m_prcTimelineEditPanelRect->SetVert(rcViewRect.Height());

	// タイムラインヘッダーエリアの配置
	m_prcTimelineEditHeaderRect->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTimelineEditHeaderRect->bottom = m_prcTimelineEditHeaderRect->top + iTimelineEditHeaderDefaltHeight;
	m_prcTimelineEditHeaderRect->SetVert(rcViewRect.Height());

	// タイムラインコントロールエリアの配置
	m_prcTimelineControlPanelRect->CopyRect(m_prcTimelineEditHeaderRect);
	m_prcTimelineControlPanelRect->right = m_prcTimelineControlPanelRect->left + iTimelineControlPanelDefaultWidth;
	m_prcTimelineControlPanelRect->SetVert(rcViewRect.Height());

	// シークバーエリアの配置
	m_prcSeekBarRect->CopyRect(m_prcTimelineEditHeaderRect);
	m_prcSeekBarRect->left = m_prcTimelineControlPanelRect->right + kSplitterWidth;
	m_prcSeekBarRect->SetVert(rcViewRect.Height());

	// トラックヘッダの配置
	m_prcTrackHeaderRect->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTrackHeaderRect->top = m_prcTimelineEditHeaderRect->bottom + kSplitterHeight;
	m_prcTrackHeaderRect->right = m_prcTimelineControlPanelRect->right;
	m_prcTrackHeaderRect->SetVert(rcViewRect.Height());

	// タイムラインデータエリアの配置
	m_prcTimelineDataRect->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTimelineDataRect->left = m_prcSeekBarRect->left;
	m_prcTimelineDataRect->top = m_prcTrackHeaderRect->top;
	m_prcTimelineDataRect->SetVert(rcViewRect.Height());

	// タイムラインカーソルヒット領域の配置
	m_prcTimelineCursorHitArea->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTimelineCursorHitArea->top = m_prcSeekBarRect->top;
	m_prcTimelineCursorHitArea->left = m_iTimelineCursorPoint - kTimelineCursorDragArea;
	m_prcTimelineCursorHitArea->right = m_iTimelineCursorPoint + kTimelineCursorDragArea;
	m_prcTimelineCursorHitArea->SetVert(rcViewRect.Height());

	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRectBefor = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(0));
	pTrackDataRectBefor->CopyRect(m_prcTimelineEditPanelRect);
	pTrackDataRectBefor->top = m_prcTrackHeaderRect->top;
	pTrackDataRectBefor->bottom = pTrackDataRectBefor->top + pTrackDataRectBefor->GetHeight();
	pTrackDataRectBefor->SetVert(rcViewRect.Height());
	TrackDataRect* pTrackDataRect = nullptr;
	for (int i = 1; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		pTrackDataRect = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i));
		pTrackDataRect->CopyRect(pTrackDataRectBefor);
		pTrackDataRect->top = pTrackDataRectBefor->bottom + 1;
		pTrackDataRect->bottom = pTrackDataRect->top + pTrackDataRect->GetHeight();
		pTrackDataRect->SetVert(rcViewRect.Height());
		pTrackDataRectBefor = pTrackDataRect;
	}

	CalcTimelineDisplayRange();

	return;
}

// 表示可能なタイムラインの範囲（左端、右端）を計算する
void TimelineEditerView::CalcTimelineDisplayRange()
{
	// 表示可能フレーム範囲の計算
	int iDisplayFrameCount = 0;
	if (m_iPointPerFrame < 1)
	{
		iDisplayFrameCount = m_prcSeekBarRect->Width() * m_iFramePerPoint;
	}
	else
	{
		iDisplayFrameCount = static_cast<int>(ceil(m_prcSeekBarRect->Width() / m_iPointPerFrame));
	}
	m_iTimelineCursorPoint = static_cast<int>(floor(m_prcSeekBarRect->Width() / 2.0f)) + m_prcSeekBarRect->left;
	m_iLeftFrameNumber = m_iTimelineCursorFramePosition - static_cast<int>(floor((iDisplayFrameCount / 2.0f)));
	m_iRightFrameNumber = m_iTimelineCursorFramePosition + static_cast<int>(ceil((iDisplayFrameCount / 2.0f))) + 1;
}

// タイムラインデータ表示倍率の変更
BOOL TimelineEditerView::ChangeDisplayScale(void)
{
	//int kDisplayScaleArray[13] = { 1, 2, 3, 6, 10, 30, 60, 300, 600, 1800, 3600, 18000, 36000 };
	//int iArrayCount = sizeof kDisplayScaleArray / sizeof kDisplayScaleArray[0];

	if ((m_iFramePerPoint == 1) || (m_iPointPerFrame == 1))
	{
		m_iSmallScaleDrawInterval = 20;
		m_iMiddleScaleDrawInterval = 60;
		m_iBigScaleDrawInterval = 300;
	}
	else if (m_iFramePerPoint < 1)
	{
		if (m_iPointPerFrame >= 96)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 1;
			m_iBigScaleDrawInterval = 10;
		}
		else if (m_iPointPerFrame >= 64)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 2;
			m_iBigScaleDrawInterval = 10;
		}
		else if (m_iPointPerFrame >= 56)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 3;
			m_iBigScaleDrawInterval = 30;
		}
		else if (m_iPointPerFrame >= 48)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 4;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 40)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 6;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 32)
		{
			m_iSmallScaleDrawInterval = 2;
			m_iMiddleScaleDrawInterval = 10;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 16)
		{
			m_iSmallScaleDrawInterval = 2;
			m_iMiddleScaleDrawInterval = 12;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 8)
		{
			m_iSmallScaleDrawInterval = 4;
			m_iMiddleScaleDrawInterval = 20;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 4)
		{
			m_iSmallScaleDrawInterval = 5;
			m_iMiddleScaleDrawInterval = 30;
			m_iBigScaleDrawInterval = 60;
		}
		else
		{
			m_iSmallScaleDrawInterval = 10;
			m_iMiddleScaleDrawInterval = 60;
			m_iBigScaleDrawInterval = 120;
		}
	}
	else
	{
		// TODO: もっと効率よく！
		if (m_iFramePerPoint <= 2)
		{
			m_iSmallScaleDrawInterval = 30;
			m_iMiddleScaleDrawInterval = 120;
			m_iBigScaleDrawInterval = 600;
		}
		else if (m_iFramePerPoint <= 4)
		{
			m_iSmallScaleDrawInterval = 60;
			m_iMiddleScaleDrawInterval = 300;
			m_iBigScaleDrawInterval = 1200;
		}
		else if (m_iFramePerPoint <= 6)
		{
			m_iSmallScaleDrawInterval = 120;
			m_iMiddleScaleDrawInterval = 600;
			m_iBigScaleDrawInterval = 1800;
		}
		else if (m_iFramePerPoint <= 8)
		{
			m_iSmallScaleDrawInterval = 180;
			m_iMiddleScaleDrawInterval = 900;
			m_iBigScaleDrawInterval = 1800;
		}
		else if (m_iFramePerPoint <= 10)
		{
			m_iSmallScaleDrawInterval = 300;
			m_iMiddleScaleDrawInterval = 1200;
			m_iBigScaleDrawInterval = 3600;
		}
		else if (m_iFramePerPoint <= 20)
		{
			m_iSmallScaleDrawInterval = 600;
			m_iMiddleScaleDrawInterval = 1800;
			m_iBigScaleDrawInterval = 3600;
		}
		else if (m_iFramePerPoint <= 40)
		{
			m_iSmallScaleDrawInterval = 600;
			m_iMiddleScaleDrawInterval = 1800;
			m_iBigScaleDrawInterval = 7200;
		}
		else if (m_iFramePerPoint <= 60)
		{
			m_iSmallScaleDrawInterval = 1200;
			m_iMiddleScaleDrawInterval = 3600;
			m_iBigScaleDrawInterval = 7200;
		}
		else if (m_iFramePerPoint <= 90)
		{
			m_iSmallScaleDrawInterval = 1200;
			m_iMiddleScaleDrawInterval = 3600;
			m_iBigScaleDrawInterval = 10800;
		}
		else if (m_iFramePerPoint <= 120)
		{
			m_iSmallScaleDrawInterval = 1200;
			m_iMiddleScaleDrawInterval = 3600;
			m_iBigScaleDrawInterval = 14400;
		}
		else if (m_iFramePerPoint <= 180)
		{
			m_iSmallScaleDrawInterval = 3600;
			m_iMiddleScaleDrawInterval = 18000;
			m_iBigScaleDrawInterval = 108000;
		}
		else if (m_iFramePerPoint <= 300)
		{
			m_iSmallScaleDrawInterval = 108000;
			m_iMiddleScaleDrawInterval = 432000;
			m_iBigScaleDrawInterval = 1080000;
		}
		else
		{
			m_iSmallScaleDrawInterval = 1080000;
			m_iMiddleScaleDrawInterval = 4320000;
			m_iBigScaleDrawInterval = 10800000;
		}
	}

	CalcTimelineDisplayRange();

	return TRUE;
}



/*

座標、位置計算系

*/
// クリップ位置計算
BOOL TimelineEditerView::CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{

	if (CalcClipRect(rcClipRect, const_cast<ClipDataRect*>(clClipData)->m_iTimelineInPoint, const_cast<ClipDataRect*>(clClipData)->GetDuration(),
		rcTrackRect, iMoveFrames, iIntrimFrames, iOuttrimFrames))
	{
		if (rcClipRect.left < m_prcTimelineDataRect->left)
		{
			rcClipRect.left = m_prcTimelineDataRect->left;
		}
		if (rcClipRect.right > m_prcTimelineDataRect->right)
		{
			rcClipRect.right = m_prcTimelineDataRect->right;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

// クリップ位置計算（はみ出し補正なし）
BOOL TimelineEditerView::CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{
	rcClipRect.top = rcTrackRect.top + static_cast<int>(rcTrackRect.Height() * (1 - CLIPHIGHT));
	rcClipRect.bottom = rcClipRect.top + static_cast<int>(rcTrackRect.Height() * CLIPHIGHT);
	int iLeftScrubingFrameCount = m_iLeftFrameNumber + m_iOperatingFrameCount;
	int iRightScrubingFrameCount = m_iRightFrameNumber + m_iOperatingFrameCount;

	if (iInPoint + iMoveFrames + iIntrimFrames > iRightScrubingFrameCount)
	{
		rcClipRect.SetRectEmpty();
		return FALSE;
	}
	if ((iInPoint + iDuration + iMoveFrames + iOuttrimFrames) < iLeftScrubingFrameCount)
	{
		rcClipRect.SetRectEmpty();
		return FALSE;
	}
	int iDisplayInPoint = iInPoint + iMoveFrames + iIntrimFrames;
	int iDisplayOutPoint = iInPoint + iDuration + iMoveFrames + iOuttrimFrames;

	rcClipRect.left = ChangeTimelineFramePositionToDisplayPoint(iDisplayInPoint);
	rcClipRect.right = ChangeTimelineFramePositionToDisplayPoint(iDisplayOutPoint);

	return TRUE;
}

// フレーム位置を画面上の座標に変換する
int TimelineEditerView::ChangeTimelineFramePositionToDisplayPoint(const int iFrame)
{
	int iXPoint;
	int iFrameCountFromTimelineCursor = iFrame - m_iTimelineCursorFramePosition - m_iOperatingFrameCount;

	// １ポイントあたりのフレーム数が１未満の場合（１フレームが複数ポイントに跨る）
	if (m_iFramePerPoint < 1)
	{
		// タイムラインカーソルからの相対座標を求める
		iXPoint = iFrameCountFromTimelineCursor * m_iPointPerFrame;
	}
	else
	{
		// タイムラインカーソルからの相対座標を求める
		iXPoint = static_cast<int>(floor(iFrameCountFromTimelineCursor / m_iFramePerPoint));
	}
	return (iXPoint + m_iTimelineCursorPoint);
}

// クリック位置をフレーム位置に変換する
int TimelineEditerView::ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame)
{
	int iFrame;
	int iHorizontalLengthFromTimelineCursor = point.x - m_iTimelineCursorPoint;

	// １フレームあたりのポイント数が１未満の場合
	if (m_iPointPerFrame < 1)
	{
		// タイムラインカーソルからのフレーム数を求める
		iFrame = iHorizontalLengthFromTimelineCursor * m_iFramePerPoint + m_iTimelineCursorFramePosition;
	}
	else
	{
		// タイムラインカーソルからの相対座標を求める
		iFrame = static_cast<int>(floor(iHorizontalLengthFromTimelineCursor / m_iPointPerFrame)) + m_iTimelineCursorFramePosition;
	}
	iActualFrame = iFrame;
	if (iFrame < 0)
	{
		return 0;
	}
	else
	{
		return iFrame;
	}
}

// 操作量を操作フレーム数に変換する
int TimelineEditerView::ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame /* = 0 */)
{

	int iFrames = 0;
	if (szMoveSize.cx == 0)
	{
		return iFrames;
	}

	// １ポイントあたりのフレーム数が１未満の場合（１フレームが複数ポイントに跨る）
	if (m_iFramePerPoint < 1)
	{
		// 移動フレーム数は実際の移動長×１ポイントあたりのフレーム数（必要な幅を動かさないとフレームは動かない）
		iFrames = static_cast<int>(floor(szMoveSize.cx / m_iPointPerFrame));
	}
	else
	{
		// 表示に切りの良いフレーム位置でない場合は調整する
		int iSurPlus = iStartFrame % m_iFramePerPoint;
		if (iSurPlus == 0)
		{
			// 移動フレーム数は実際の移動長×１ポイントあたりのフレーム数（１ポイントで複数フレーム動く）
			iFrames = szMoveSize.cx * m_iFramePerPoint;
		}
		else
		{
			// （移動フレーム数は実際の移動長－１）×１ポイントあたりのフレーム数（最初の１ポイントは端数処理に使う）
			if (szMoveSize.cx < 0)
			{
				iFrames = ((szMoveSize.cx + 1) * m_iFramePerPoint) - iSurPlus;
			}
			else
			{
				iFrames = ((szMoveSize.cx - 1) * m_iFramePerPoint) + m_iFramePerPoint - iSurPlus;
			}
		}
	}
	return iFrames;
}



/*

ヒットテスト系

*/
// クリックポイントがトラック領域にあるかの判定
TrackDataRect* TimelineEditerView::IsPointInAnyTrack(const CPoint& point)
{
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRect = nullptr;
	int i = 0;
	while ((i < m_pTrackDataVideoManager->GetTrackCount()) && (pTrackDataRect == nullptr))
	{
		pTrackDataRect = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i));
		if (!(pTrackDataRect->PtInRect(point)))
		{
			pTrackDataRect = nullptr;
			++i;
		}
	}
	return pTrackDataRect;
}

// クリック位置がシークバー内かを判定する
BOOL TimelineEditerView::IsPointInSeekBar(const CPoint& point)
{
	if (!(m_prcSeekBarRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// クリック位置がタイムラインコントロールパネル内かを判定する
BOOL TimelineEditerView::IsPointInTimelineControlPanel(const CPoint& point)
{
	if (!(m_prcTimelineControlPanelRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// クリック位置がクリップ内かを判定する
BOOL TimelineEditerView::IsPointInAnyClipRect(const CPoint& point)
{
	m_fMoving = FALSE;

	// TODO: マイナスを返すパターンも必要
	int iFrame;
	ChangeDisplayPointToTimelineFramePosition(point, iFrame);
	if ((m_clSelectedTrack != nullptr) && (m_clSelectedTrackInfo != nullptr) && (iFrame >= 0))
	{
		int iInPoint = 0;
		ClipDataPositionMap mpClipMap;
		int iSize = m_clSelectedTrackInfo->GetClipDataAtFrame(iFrame, mpClipMap);
		if (iSize == 0)
		{
			m_clMovingClipData = nullptr;
		}
		else if (iSize == 1)
		{
			ClipDataPositionMap::iterator itr = mpClipMap.begin();
			m_clMovingClipData = (*itr).second;
		}
		else
		{
			ClipDataPositionMap::iterator itr = mpClipMap.begin();
			++itr;
			m_clMovingClipData = (*itr).second;
		}

		if (m_clMovingClipData != nullptr)
		{
			if (IsPointInClipRect(point, static_cast<CRect&>(*m_clMovingClipData)))
			{
				CalcClipRectDisplayPoint(static_cast<CRect>(m_clMovingClipData), m_clMovingClipData, static_cast<CRect>(m_clSelectedTrack));
			}
		}
	}

	return (m_fMoving || m_fSingleInTriming || m_fSingleOutTriming);
}

// Move/Trim振り分け
BOOL TimelineEditerView::IsPointInClipRect(const CPoint& point, const CRect& rcClipRect)
{
	CRect rcHitTestRect;
	rcHitTestRect.CopyRect(rcClipRect);
	// クリップ長が規定値より短い場合は規定値幅で判定する
	if (rcClipRect.Width() < kClipHitCheckMinWidth)
	{
		int iAdditionalLength = static_cast<int>(floor((kClipHitCheckMinWidth - rcHitTestRect.Width()) / 2));
		rcHitTestRect.left -= iAdditionalLength;
		rcHitTestRect.right += iAdditionalLength;
	}
	// 入力のClipRect内にポインタが存在するか？
	if (rcHitTestRect.PtInRect(point))
	{
		// Trim判定で漏れたらMove
		m_fMoving = !(IsPointInTrimRange(point, &rcClipRect));
	}
	return (m_fMoving || m_fSingleInTriming || m_fSingleOutTriming);
}

// クリック箇所がクリップ内のトリム操作エリア内かを判定
BOOL TimelineEditerView::IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect)
{

	m_fSingleInTriming = FALSE;
	m_fSingleOutTriming = FALSE;

	CRect rcTrimRect;

	// In側判定
	rcTrimRect.CopyRect(rcClipRect);
	// クリップ幅が規定値未満の場合はInTrim決めうち
	if (rcTrimRect.Width() < kClipHitCheckMinWidth)
	{
		//TODO: In側映像がない場合（ClipIn点=0）OutTrimにふる
		m_fSingleInTriming = TRUE;
	}
	else
	{
		rcTrimRect.right = rcTrimRect.left + static_cast<long>(floor(rcTrimRect.Width() * kTrimAreaRate));
		if (rcTrimRect.Width() < kTrimHitCheckMinWidth)
		{
			rcTrimRect.right = rcTrimRect.left + kTrimHitCheckMinWidth;
		}
		else if (rcTrimRect.Width() > kTrimHitCheckMaxWidth)
		{
			rcTrimRect.right = rcTrimRect.left + kTrimHitCheckMaxWidth;
		}
		m_fSingleInTriming = rcTrimRect.PtInRect(point);
	}

	if (!(m_fSingleInTriming || m_fSingleOutTriming))
	{
		// Out側判定
		rcTrimRect.CopyRect(rcClipRect);
		rcTrimRect.left = rcTrimRect.right - static_cast<long>(floor(rcTrimRect.Width() * kTrimAreaRate));
		if (rcTrimRect.Width() < kTrimHitCheckMinWidth)
		{
			rcTrimRect.left = rcTrimRect.right - kTrimHitCheckMinWidth;
		}
		else if (rcTrimRect.Width() > kTrimHitCheckMaxWidth)
		{
			rcTrimRect.left = rcTrimRect.right - kTrimHitCheckMaxWidth;
		}
		m_fSingleOutTriming = rcTrimRect.PtInRect(point);
	}

	return (m_fSingleInTriming || m_fSingleOutTriming);
}



/*

操作チェック系（コントローラーへ移動）

*/
// 操作がIn側トリム可能な範囲内かを判定して位置を調整する
BOOL TimelineEditerView::CheckInTrim(void)
{
	int iDuration = m_clMovingClipData->GetDuration();
	// 範囲チェック
	if (iDuration - m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = iDuration - 1;
		return FALSE;
	}

	// 重なりチェック
	// TODO: In点の場所にクリップがあるかをサーチする。
	m_iOperatingClipFrameCount = m_clOperateToTrackInfo->CheckClipInSingleInTrimRange(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount)
		- m_clMovingClipData->m_iTimelineInPoint;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	// 範囲チェック（重なりチェックより後にやらないと0を超えてドラッグされた場合に0に設定されてしまう）
	if (m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount < 0)
	{
		m_iOperatingClipFrameCount = m_clMovingClipData->m_iTimelineInPoint * -1;
		return FALSE;
	}

	return TRUE;
}

// 操作がOut側トリム可能な範囲内かを判定して位置を調整する
BOOL TimelineEditerView::CheckOutTrim(void)
{
	// 範囲チェック
	int iDuration = m_clMovingClipData->GetDuration();
	// 範囲チェック
	if (iDuration + m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = (iDuration * -1) + 1;
		return FALSE;
	}

	// 重なりチェック
	int iStartFrame = m_clMovingClipData->m_iTimelineInPoint + iDuration - 1;
	m_iOperatingClipFrameCount = m_clOperateToTrackInfo->CheckClipInSingleOutTrimRange(iStartFrame, iStartFrame + m_iOperatingClipFrameCount) - iStartFrame;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	return TRUE;
}

// 操作がMove可能な範囲内かを判定して位置を調整する
BOOL TimelineEditerView::CheckMove(CPoint& point)
{
	int iMovingClipInFrame, iMovingClipOutFrame, iWorkOperatingClipFrameCount, iDropInPoint = 0;
	CRect rcWorkRect;
	ClipDataRect* pClipData;
	// 範囲チェック
	if (m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount < 0)
	{
		iWorkOperatingClipFrameCount = m_clMovingClipData->m_iTimelineInPoint * -1;
		// ワークに操作中の矩形領域をコピー
		rcWorkRect.CopyRect(m_clMovingClipData->GetOperatingRect());
		// 仮置きした位置に合わせてワークの矩形を移動する
		CalcClipRect(rcWorkRect, iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
		// 仮置きした場所でクリップと衝突しないかを再判定
		iMovingClipInFrame = m_clMovingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount;
		iMovingClipOutFrame = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + iWorkOperatingClipFrameCount;
		pClipData = m_clOperateToTrackInfo->CheckMove(m_clMovingClipData, iMovingClipInFrame, iMovingClipOutFrame);
		if ((pClipData != nullptr) && (pClipData != m_clMovingClipData))
		{
			// 重なりがあった場合
			m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
			m_clOperateToTrack = m_pEnableMovingTrack;
			CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			return FALSE;
		}
		else
		{
			// 重なりがない場合、その場所に配置する
			m_iOperatingClipFrameCount = m_clMovingClipData->m_iTimelineInPoint * -1;
			m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
			m_pEnableMovingTrack = m_clOperateToTrack;
			CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			return TRUE;
		}
	}

	// 重なりチェック
	// TODO: 重なった先の再判定が必要（右移動と左移動で分けて考えないとだめそう（チェックＮＧだったら始点に戻っていく）
	iMovingClipInFrame = m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount;
	iMovingClipOutFrame = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + m_iOperatingClipFrameCount;
	pClipData = m_clOperateToTrackInfo->CheckMove(m_clMovingClipData, iMovingClipInFrame, iMovingClipOutFrame);
	if ((pClipData != nullptr) && (pClipData != m_clMovingClipData))
	{
		// 重なりがあった場合
		int iStaticClipCenterFrame = pClipData->m_iTimelineInPoint + static_cast<int>(floor(pClipData->GetDuration() / 2));
		int iDropInPoint = 0;
		// 重なっているクリップの左右に仮配置
		if (iMovingClipInFrame <= iStaticClipCenterFrame)
		{
			// 中心より左の場合、重なっているクリップのIn点-自身のDurationを配置点に仮置き
			iDropInPoint = pClipData->m_iTimelineInPoint - m_clMovingClipData->GetDuration();
		}
		else
		{
			// 中心より右の場合、重なっているクリップのOut点を配置点に仮置き
			iDropInPoint = pClipData->m_iTimelineInPoint + pClipData->GetDuration();
		}
		// 仮置きしたポイントへの移動量を算出
		iWorkOperatingClipFrameCount = iDropInPoint - m_clMovingClipData->m_iTimelineInPoint;
		// 移動の結果In点が0未満になる場合は移動しない
		if (m_clMovingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount > 0)
		{
			// ワークに操作中の矩形領域をコピー
			rcWorkRect.CopyRect(m_clMovingClipData->GetOperatingRect());
			// 仮置きした位置に合わせてワークの矩形を移動する
			CalcClipRect(rcWorkRect, iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			// 仮置きした場所でクリップと衝突しないかを再判定
			iMovingClipInFrame = m_clMovingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount;
			iMovingClipOutFrame = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + iWorkOperatingClipFrameCount;
			pClipData = m_clOperateToTrackInfo->CheckMove(m_clMovingClipData, iMovingClipInFrame, iMovingClipOutFrame);
			if ((pClipData != nullptr) && (pClipData != m_clMovingClipData))
			{
				// 重なりがあった場合
			}
			else
			{
				// 重なりがない場合、その場所に配置する
				m_iOperatingClipFrameCount = iDropInPoint - m_clMovingClipData->m_iTimelineInPoint;
				m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
				m_pEnableMovingTrack = m_clOperateToTrack;
				CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			}
		}
		// 移動先が配置できない場合、前回移動可能だった場所に戻す。
		m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
		m_clOperateToTrack = m_pEnableMovingTrack;
		CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
		return FALSE;
	}


	m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
	m_pEnableMovingTrack = m_clOperateToTrack;
	CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, m_clOperateToTrack, m_iOperatingClipFrameCount);
	return TRUE;
}



/*

初期設定系

*/
// 表示エリアの初期設定
void TimelineEditerView::InitAreaRect(void)
{
	// TODO: 色は後で定数を作る
	m_prcDebugInfoPanelRect = new OpenGLRect();
	m_prcDebugInfoPanelRect->SetRectEmpty();
	m_prcDebugInfoPanelRect->SetBothColor(ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT);

	m_prcTimelineEditPanelRect = new OpenGLRect();
	m_prcTimelineEditPanelRect->SetRectEmpty();
	m_prcTimelineEditPanelRect->SetBothColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_prcTimelineEditHeaderRect = new OpenGLRect();
	m_prcTimelineEditHeaderRect->SetRectEmpty();

	m_prcTimelineControlPanelRect = new OpenGLRect();
	m_prcTimelineControlPanelRect->SetRectEmpty();
	m_prcTimelineControlPanelRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, REDCOLOR_BRUSH_FLOAT, GREENCOLOR_BRUSH_FLOAT);

	m_prcSeekBarRect = new OpenGLRect();
	m_prcSeekBarRect->SetRectEmpty();
	m_prcSeekBarRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT);

	m_prcTrackHeaderRect = new OpenGLRect();
	m_prcTrackHeaderRect->SetRectEmpty();
	m_prcTrackHeaderRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_prcTimelineDataRect = new OpenGLRect();
	m_prcTimelineDataRect->SetRectEmpty();
	m_prcTimelineDataRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_prcTimelineCursorHitArea = new OpenGLRect();
	m_prcTimelineCursorHitArea->SetRectEmpty();

	m_prcTransisionRect = new OpenGLRect();
	m_prcTransisionRect->SetRectEmpty();
	m_prcTransisionRect->SetColor(ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT);
	m_prcTransisionRect->SetBorderColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);
}

// 動作確認用オブジェクトの初期設定
void TimelineEditerView::InitTestObject(void)
{
	// TODO: 適切な作り方を提供する
	UUID uiTrackId, uiTrackRectId;

	m_pTrackDataVideoManager->CreateTrackData(0, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(1, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(2, uiTrackId, uiTrackRectId);

	UUID uiClipId, uiClipRectId;

	ClipDataRect* pClipDataRect = nullptr;
	m_pClipDataManager->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pClipDataManager->GetClipDataRect(uiClipRectId);
	pClipDataRect->m_iTimelineInPoint = 101;
	pClipDataRect->SetDuration(10);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->m_iTimelineInPoint, pClipDataRect);

	m_pClipDataManager->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pClipDataManager->GetClipDataRect(uiClipRectId);
	pClipDataRect->m_iTimelineInPoint = 600;
	pClipDataRect->SetDuration(100);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->m_iTimelineInPoint, pClipDataRect);

	m_pClipDataManager->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pClipDataManager->GetClipDataRect(uiClipRectId);
	pClipDataRect->m_iTimelineInPoint = 300;
	pClipDataRect->SetDuration(50);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->m_iTimelineInPoint, pClipDataRect);



}

// 表示倍率関連のマップ作成
void TimelineEditerView::CreateZoomMap(void)
{
}



