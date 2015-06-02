// TimelineEditerView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditerView.h"

#include "OpenGLRect.h"
#include "ClipDataRect.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
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
	m_prcPreviewPanelRect->DrawMyBorderRect(3.0f);


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
	DrawTrack();

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

	strFrameNumber.Format(_T("Cip1 Rect Point L %d T %d"), m_clClipData1->left, m_clClipData1->top);
	ChangeScreenPointToOpenGLPoint(5, 90, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("                R %d B %d"), m_clClipData1->right, m_clClipData1->bottom);
	ChangeScreenPointToOpenGLPoint(5, 105, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Cip2 Rect Point L %d T %d"), m_clClipData2->left, m_clClipData2->top);
	ChangeScreenPointToOpenGLPoint(5, 120, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("                R %d B %d"), m_clClipData2->right, m_clClipData2->bottom);
	ChangeScreenPointToOpenGLPoint(5, 135, iHeight, dPointX, dPointY);
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
			//--m_iSelectedDisplayScaleNumber;
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
		m_clOperateToTrackInfo->AddClip(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData);
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

	Invalidate(); // 再描画します。

	m_clMovingClipData = nullptr;
	m_clStaticClipData = nullptr;
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
		//++m_iSelectedDisplayScaleNumber;
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
BOOL TimelineEditerView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	InitAreaRect();

	// TODO: とりあえずはここにおいておくが、後で別のところに持っていく
	InitTestObject();

	m_fLButtonClicking = FALSE;
	m_fMoving = FALSE;
	m_fSingleInTriming = FALSE;
	m_fSingleOutTriming = FALSE;
	m_fScrubing = FALSE;
	m_fDragShuttling = FALSE;

	m_prcPreviewPanelRect->bottom = m_prcPreviewPanelRect->top + kPreviewPanelDefaltHeight;

	m_iLeftFrameNumber = 0;
	m_iRightFrameNumber = 0;
	m_iTimelineCursorFramePosition = 0;
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;

	//m_iSelectedDisplayScaleNumber = 4;
	m_iFramePerPoint = 0;
	m_iPointPerFrame = 1;
	ChangeDisplayScale();



	return OpenGLView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

// 破棄
void TimelineEditerView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	DeleteObject(m_prcPreviewPanelRect);
	DeleteObject(m_prcTimelineEditPanelRect);
	DeleteObject(m_prcTimelineEditHeaderRect);
	DeleteObject(m_prcTimelineControlPanelRect);
	DeleteObject(m_prcSeekBarRect);
	DeleteObject(m_prcTrackHeaderRect);
	DeleteObject(m_prcTimelineDataRect);
	DeleteObject(m_prcTimelineCursorHitArea);

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

	//m_prcSeekBarRect->GetWorldPoint(dLeft, dRight, dTop, dBottom);
	//DrawQuads(dLeft, dRight, dTop, dBottom, 1.0f, SEEKBARBACKGROUNDCOLOR_BRUSH_FLOAT, GL_QUADS);

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
	m_prcTrackHeaderRect->DrawMyBorderRect(1.0f);
}

// トラック描画
void TimelineEditerView::DrawTrack(void)
{
	// TODO: とりあえず今は枠だけ　Vectorとかで検索が必要
	m_clTrack1->GetBottomBorderVert(m_fLineVert);
	m_clTrack1->GetBottomBorderColor(m_fLineColor);
	DrawLine(1.0f);

	m_clTrack2->GetBottomBorderVert(m_fLineVert);
	m_clTrack2->GetBottomBorderColor(m_fLineColor);
	DrawLine(1.0f);
}

// タイムラインデータエリア描画
void TimelineEditerView::DrawTimelineDataRect(void)
{
	// TODO: とりあえず今は枠だけ
	m_prcTimelineDataRect->DrawMyBorderRect(1.0f);
}

// クリップの描画を行う
BOOL TimelineEditerView::DrawClip(const int iHeight)
{
	//TODO: vectorとかにトラックを押し込んで反復させる
	TrackDataRect* pTrackDataRect = m_clTrack1;
	DrawClipInTrack(pTrackDataRect, iHeight);
	pTrackDataRect = m_clTrack2;
	DrawClipInTrack(pTrackDataRect, iHeight);

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
	ClipDataInfoMap mpClipDataMap;
	int iClipCount = pTrackDataRect->GetTrackDataInfo()->GetClipDataInRange(iStartFrame, m_iRightFrameNumber + m_iOperatingFrameCount, mpClipDataMap);
	ClipDataRect* pClipData;
	ClipDataRect* pClipDataLeft;
	pClipDataLeft = nullptr;
	if (iClipCount > 0)
	{
		ClipDataInfoMap::iterator itr = mpClipDataMap.begin();
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
					m_prcTransisionRect->DrawMyLeftLine(1.0f);
					m_prcTransisionRect->DrawMyRightLine(1.0f);
					m_prcTransisionRect->SetRectEmpty();

				}
				else if (pClipData->left == pClipDataLeft->right)
				{
					pClipDataLeft->DrawMyRightLine(1.0f);
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
		if (!(m_rcMousePointRect.EqualRect(rcOperatingRect)))
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
	m_prcPreviewPanelRect->CopyRect(rcViewRect);
	m_prcPreviewPanelRect->bottom = rcViewRect.top + lViewHeight;
	m_prcPreviewPanelRect->SetVert(rcViewRect.Height());

	// タイムライン編集エリア配置
	m_prcTimelineEditPanelRect->CopyRect(rcViewRect);
	m_prcTimelineEditPanelRect->top = m_prcPreviewPanelRect->bottom + kSplitterHeight;
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

	// トラック領域の配置
	m_clTrack1->CopyRect(m_prcTimelineEditPanelRect);
	m_clTrack1->top = m_prcTrackHeaderRect->top;
	m_clTrack1->bottom = m_clTrack1->top + m_clTrack1->GetHeight();
	m_clTrack1->SetVert(rcViewRect.Height());

	m_clTrack2->CopyRect(m_clTrack1);
	m_clTrack2->top = m_clTrack1->bottom + 1;
	m_clTrack2->bottom = m_clTrack2->top + m_clTrack2->GetHeight();
	m_clTrack2->SetVert(rcViewRect.Height());

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
	//int iDisplayFrameCount = static_cast<int>(floor(m_prcSeekBarRect->Width() / m_fPointPerFrame));
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

	// ここから下はたぶんいらない



	//if (m_iSelectedDisplayScaleNumber < 0)
	//{
	//	m_iSelectedDisplayScaleNumber = 0;
	//	return FALSE;
	//}
	//else if (m_iSelectedDisplayScaleNumber > iArrayCount - 1)
	//{
	//	m_iSelectedDisplayScaleNumber = iArrayCount - 1;
	//	return FALSE;
	//}

	//m_iFramePerBase = kDisplayScaleArray[m_iSelectedDisplayScaleNumber];

	//m_fPointPerFrame = static_cast<float>(kSeekBarScaleBaseWidth) / static_cast<float>(m_iFramePerBase);

	//if (m_iFramePerBase < kSeekBarScaleMaxCountPerBase)
	//{
	//	m_iSeekBarScaleCountPerBase = m_iFramePerBase;
	//}
	//else
	//{
	//	m_iSeekBarScaleCountPerBase = kSeekBarScaleMaxCountPerBase;
	//}

	//m_iSmallScaleLength = kSeekBarScaleBaseWidth / m_iSeekBarScaleCountPerBase;

	//if (m_fPointPerFrame < 1)
	//{
	//	m_iPointPerOperation = 1;
	//}
	//else
	//{
	//	m_iPointPerOperation = static_cast<int>(floor(m_fPointPerFrame));
	//	if ((m_fPointPerFrame - m_iPointPerOperation) != 0)
	//	{
	//		return FALSE;
	//	}
	//}

	//m_fFramePerPoint = static_cast<float>(m_iFramePerBase) / static_cast<float>(kSeekBarScaleBaseWidth);

	//float fFramePerScale = static_cast<float>(m_iFramePerBase) / static_cast<float>(m_iSeekBarScaleCountPerBase);
	//if ((fFramePerScale - static_cast<int>(floor(fFramePerScale))) == 0)
	//{
	//	m_iFramePerScale = m_iFramePerBase / m_iSeekBarScaleCountPerBase;
	//}
	//else
	//{
	//	m_iFramePerScale = static_cast<int>(floor(fFramePerScale));
	//	return FALSE;
	//}




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
	rcClipRect.top = rcTrackRect.top + static_cast<int>(floor(kTrackDefaultHeight * (1 - CLIPHIGHT)));
	rcClipRect.bottom = rcClipRect.top + static_cast<int>(floor(kTrackDefaultHeight * CLIPHIGHT));
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
		//if (m_fPointPerFrame < 1)
	{
		// タイムラインカーソルからのフレーム数を求める
		iFrame = iHorizontalLengthFromTimelineCursor * m_iFramePerPoint + m_iTimelineCursorFramePosition;
		//iFrame = iHorizontalLengthFromTimelineCursor * m_fFramePerPoint + m_iTimelineCursorFramePosition;
	}
	else
	{
		// タイムラインカーソルからの相対座標を求める
		iFrame = static_cast<int>(floor(iHorizontalLengthFromTimelineCursor / m_iPointPerFrame)) + m_iTimelineCursorFramePosition;
		//iFrame = static_cast<int>(ceil(iHorizontalLengthFromTimelineCursor / m_fPointPerFrame)) + m_iTimelineCursorFramePosition;
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
		//if (m_fFramePerPoint < 1)
	{
		// 移動フレーム数は実際の移動長×１ポイントあたりのフレーム数（必要な幅を動かさないとフレームは動かない）
		iFrames = static_cast<int>(floor(szMoveSize.cx / m_iPointPerFrame));
		//iFrames = static_cast<int>(floor(szMoveSize.cx * m_fFramePerPoint));
	}
	else
	{
		// 表示に切りの良いフレーム位置でない場合は調整する
		int iSurPlus = iStartFrame % m_iFramePerPoint;
		//int iSurPlus = iStratFrame % static_cast<int>(m_fFramePerPoint);
		if (iSurPlus == 0)
		{
			// 移動フレーム数は実際の移動長×１ポイントあたりのフレーム数（１ポイントで複数フレーム動く）
			iFrames = szMoveSize.cx * m_iFramePerPoint;
			//iFrames = szMoveSize.cx * m_fFramePerPoint;
		}
		else
		{
			// （移動フレーム数は実際の移動長－１）×１ポイントあたりのフレーム数（最初の１ポイントは端数処理に使う）
			if (szMoveSize.cx < 0)
			{
				iFrames = ((szMoveSize.cx + 1) * m_iFramePerPoint) - iSurPlus;
				//iFrames = ((szMoveSize.cx + 1) * m_fFramePerPoint) - iSurPlus;
			}
			else
			{
				iFrames = ((szMoveSize.cx - 1) * m_iFramePerPoint) + m_iFramePerPoint - iSurPlus;
				//iFrames = ((szMoveSize.cx - 1) * m_fFramePerPoint) + (static_cast<int>(m_fFramePerPoint)-iSurPlus);
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
	// TODO: 最終的にはVectorとかでループをまわす

	if (m_clTrack1->PtInRect(point))
	{
		return m_clTrack1;
	}
	else if (m_clTrack2->PtInRect(point))
	{
		return m_clTrack2;
	}
	return nullptr;
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
		ClipDataInfoMap mpClipMap;
		int iSize = m_clSelectedTrackInfo->GetClipDataAtFrame(iFrame, mpClipMap);
		if (iSize == 0)
		{
			m_clMovingClipData = nullptr;
		}
		else if (iSize == 1)
		{
			ClipDataInfoMap::iterator itr = mpClipMap.begin();
			m_clMovingClipData = (*itr).second;
		}
		else
		{
			ClipDataInfoMap::iterator itr = mpClipMap.begin();
			++itr;
			m_clMovingClipData = (*itr).second;
		}

		if (m_clMovingClipData != nullptr)
		{
			if (IsPointInClipRect(point, static_cast<CRect&>(*m_clMovingClipData)))
			{
				//m_clMovingClipData->m_iTimelineInPoint = iInPoint;
				CalcClipRectDisplayPoint(static_cast<CRect>(m_clMovingClipData), m_clMovingClipData, static_cast<CRect>(m_clSelectedTrack));
			}
			if (m_clMovingClipData == m_clClipData1)
			{
				m_clStaticClipData = m_clClipData2;
			}
			else
			{
				m_clStaticClipData = m_clClipData1;
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
	// クリップ幅が規定値未満の場合はTrim決めうち
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
	// 範囲チェック
	if (m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount < 0)
	{
		m_iOperatingClipFrameCount = m_clMovingClipData->m_iTimelineInPoint * -1;
		return FALSE;
	}

	// 重なりチェック
	// TODO: 重なった先の再判定が必要（右移動と左移動で分けて考えないとだめそう（チェックＮＧだったら始点に戻っていく）
	int iMovingClipInFrame = m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount;
	int iMovingClipOutFrame = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + m_iOperatingClipFrameCount;
	ClipDataRect* pClipData;
	pClipData = m_clOperateToTrackInfo->CheckMove(m_clMovingClipData, iMovingClipInFrame, iMovingClipOutFrame);
	if ((pClipData != nullptr) && (pClipData != m_clMovingClipData))
	{
		int iStaticClipCenterFrame = pClipData->m_iTimelineInPoint + static_cast<int>(floor(pClipData->GetDuration() / 2));
		int iDropInPoint = 0;
		if (iMovingClipInFrame <= iStaticClipCenterFrame)
		{
			iDropInPoint = pClipData->m_iTimelineInPoint - m_clMovingClipData->GetDuration();
		}
		else
		{
			iDropInPoint = pClipData->m_iTimelineInPoint + pClipData->GetDuration();
		}
		m_iOperatingClipFrameCount = iDropInPoint - m_clMovingClipData->m_iTimelineInPoint;
		CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
		return FALSE;
	}

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
	m_prcPreviewPanelRect = new OpenGLRect();
	m_prcPreviewPanelRect->SetRectEmpty();
	m_prcPreviewPanelRect->SetBothColor(ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT);

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
	m_clTrack1 = new TrackDataRect();
	m_clTrack1->InitTrackData();
	m_clTrack1->SetHeight(kTrackDefaultHeight);
	m_clTrack1->SetTrackName(_T("Track1"));
	m_clTrackInfo1 = new TrackDataInfo();
	m_clTrack1->SetTrackDataInfo(m_clTrackInfo1);

	m_clTrack2 = new TrackDataRect();
	m_clTrack2->InitTrackData();
	m_clTrack2->SetHeight(kTrackDefaultHeight);
	m_clTrack2->SetTrackName(_T("Track2"));
	m_clTrackInfo2 = new TrackDataInfo();
	m_clTrack2->SetTrackDataInfo(m_clTrackInfo2);

	m_clClipData1 = new ClipDataRect();
	m_clClipData1->InitClipData();
	m_clClipData1->m_iTimelineInPoint = 101;
	m_clClipData1->SetDuration(10);
	m_clTrackInfo1->AddClip(m_clClipData1->m_iTimelineInPoint, m_clClipData1);

	m_clClipData2 = new ClipDataRect();
	m_clClipData2->InitClipData();
	m_clClipData2->m_iTimelineInPoint = 600;
	m_clClipData2->SetDuration(100);
	m_clTrackInfo1->AddClip(m_clClipData2->m_iTimelineInPoint, m_clClipData2);

}

// 表示倍率関連のマップ作成
void TimelineEditerView::CreateZoomMap(void)
{
}

