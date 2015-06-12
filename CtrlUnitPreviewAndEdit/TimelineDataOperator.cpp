// TimelineDataOperator.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineDataOperator.h"

#include "OpenGLRect.h"
#include "ClipDataRect.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
#include "TrackDataManager.h"
#include "DragAndDropOperator.h"
#include "TransitionManager.h"
#include "TransitionData.h"



// TimelineDataOperator

TimelineDataOperator::TimelineDataOperator()
{
	m_fLButtonDown = FALSE;
	m_fMove = FALSE;
	m_fSingleInTrim = FALSE;
	m_fSingleOutTrim = FALSE;
	m_fScrub = FALSE;
	m_fDragShuttle = FALSE;
	m_fDragAndDrop = FALSE;
	m_fAllowFile = FALSE;
	m_fAllowDrop = FALSE;
	m_fPreviewWork = FALSE;
	m_fPlay = FALSE;
	m_fJumpFrame = FALSE;

	m_iLeftFrameNumber = 0;
	m_iRightFrameNumber = 0;
	m_iTimelineCursorFramePosition = 0;
	m_iOperatingTimelineCursorFramePosition = m_iTimelineCursorFramePosition;
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;

	m_iFramePerPoint = 0;
	m_iPointPerFrame = 1;
}

TimelineDataOperator::~TimelineDataOperator()
{
}

BOOL TimelineDataOperator::InitializeTimelineDataOperator(UUID& uiTimelineDataOperatorId)
{
	m_pTimelineEditPanelRect = new OpenGLRect();
	m_pTimelineEditHeaderRect = new OpenGLRect();
	m_pTimelineControlPanelRect = new OpenGLRect();
	m_pSeekBarRect = new OpenGLRect();
	m_pTrackHeaderRect = new OpenGLRect();
	m_pTimelineDataRect = new OpenGLRect();
	m_pTimelineCursorHitArea = new OpenGLRect();
	m_pTransitionRect = new OpenGLRect();

	m_pTimelineDataManager = new TimelineDataManager();
	if (!(m_pTimelineDataManager->InitializeTimelineDataManager(m_uiTimelineDataManagerId)))
	{
		return FALSE;
	}
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManagerId);
	ASSERT(m_pTrackDataVideoManager);
	if (m_pTrackDataVideoManager == nullptr)
	{
		return FALSE;
	}
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManagerId);
	ASSERT(m_pTrackDataAudioManager);
	if (m_pTrackDataAudioManager == nullptr)
	{
		return FALSE;
	}
	m_pClipDataManager = m_pTimelineDataManager->GetClipDataManager(m_uiClipDataManagerUUID);
	ASSERT(m_pClipDataManager);
	if (m_pClipDataManager == nullptr)
	{
		return FALSE;
	}

	m_pDropAndDragOperator = new DragAndDropOperator(this);
	ASSERT(m_pDropAndDragOperator);
	if (m_pDropAndDragOperator == nullptr)
	{
		return FALSE;
	}
	m_pDropAndDragOperator->Initialize(m_uiDropAndDragOperatorId);

	PCTSTR pszClipInfoUUID = nullptr;
	m_pClipDataManager->CreateClipData(pszClipInfoUUID, m_pszDnDClipDataRectUUID);
	m_pDnDClipDataRect = m_pClipDataManager->GetClipDataRect(m_pszDnDClipDataRectUUID);
	ASSERT(m_pDnDClipDataRect);
	if (m_pDnDClipDataRect == nullptr)
	{
		return FALSE;
	}

	m_pTransitionManager = new TransitionManager();
	if (!(m_pTransitionManager->InitializeTransitionManager(m_strTransitionManagerUUID)))
	{
		return FALSE;
	}



	ChangeDisplayScale();

	if (RPC_S_OK == UuidCreate(&m_uiTimelineDataOperatorId))
	{
		uiTimelineDataOperatorId = m_uiTimelineDataOperatorId;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void TimelineDataOperator::DeleteTimelineDataOperator(void)
{

	if (m_pTimelineDataManager)
	{
		m_pTimelineDataManager->DeleteTimelineDataManager();
		delete m_pTimelineDataManager;
	}
	if (m_pDropAndDragOperator)
	{
		delete m_pDropAndDragOperator;
	}
	if (m_pTransitionManager)
	{
		m_pTransitionManager->DeleteTransitionManager();
		delete m_pTransitionManager;
	}

	if (m_pTimelineEditPanelRect)
	{
		delete m_pTimelineEditPanelRect;
	}
	if (m_pTimelineEditHeaderRect)
	{
		delete m_pTimelineEditHeaderRect;
	}
	if (m_pTimelineControlPanelRect)
	{
		delete m_pTimelineControlPanelRect;
	}
	if (m_pSeekBarRect)
	{
		delete m_pSeekBarRect;
	}
	if (m_pTrackHeaderRect)
	{
		delete m_pTrackHeaderRect;
	}
	if (m_pTimelineDataRect)
	{
		delete m_pTimelineDataRect;
	}
	if (m_pTimelineCursorHitArea)
	{
		delete m_pTimelineCursorHitArea;
	}
	if (m_pTransitionRect)
	{
		delete m_pTransitionRect;
	}
}


// TimelineDataOperator メンバー関数

// Ｌボタンダウン
BOOL TimelineDataOperator::OnLButtonDown(UINT nFlags, CPoint point)
{
	// クリップ内判定
	if (IsPointInClip(point))
	{
		// 再生中
		if (m_fPlay)
		{
			return FALSE;
		}
		if (!(m_pOperatingClipData->GetInTransition()) && !(m_pOperatingClipData->GetOutTransition()))
		{
			// Move/Trimの振り分け
			if (IsPointInClipRect(point, static_cast<CRect&>(*m_pOperatingClipData)))
			{
				CalcClipRectDisplayPoint(static_cast<CRect>(m_pOperatingClipData), m_pOperatingClipData, static_cast<CRect>(m_pSelectedTrack));
				m_pOperateToTrack = m_pSelectedTrack;
				m_pOperateToTrackInfo = m_pSelectedTrackInfo;
				m_fLButtonDown = TRUE;
				m_poMousePointerLocation = point;	// 移動量計算のため、初期座標を保存
				m_pOperatingClipData->SetOperatingRect(static_cast<CRect>(m_pOperatingClipData));
				m_rcMousePointRect.CopyRect(static_cast<CRect>(m_pOperatingClipData));
				m_iOperatingClipFrameCount = 0;
				m_iEnableMovingFrameCount = 0;
				m_pEnableMovingTrack = m_pOperateToTrack;
				return TRUE;
			}
		}
	}
	else
	{
		// シークバー内、または タイムラインエディットエリア内のトラック上以外かタイムラインカーソル以外
		if ((IsPointInSeekBar(point)) || 
			(m_pTimelineDataRect->PtInRect(point) && (IsPointInAnyTrack(point) == nullptr) && (!m_pTimelineCursorHitArea->PtInRect(point))))
		{
			// 再生中
			if (m_fPlay)
			{
				return FALSE;
			}
			int iDummy;
			m_iJumpFrameCursorPosition = ChangeDisplayPointToTimelineFramePosition(point, iDummy);
			m_fLButtonDown = TRUE;
			m_fScrub = TRUE;
			m_poMousePointerLocation = point;	// 移動量計算のため、初期座標を保存
			m_iOperatingFrameCount = 0;
			return TRUE;
		}
		// タイムラインカーソル判定
		else if (m_pTimelineCursorHitArea->PtInRect(point))
		{
			// 再生中
			if (m_fPlay)
			{
				return FALSE;
			}
			m_fLButtonDown = TRUE;
			m_fDragShuttle = TRUE;
			m_poMousePointerLocation = point;	// 移動量計算のため、初期座標を保存
			m_iOperatingFrameCount = 0;
			m_fSuttleSpeed = 0;
			return TRUE;
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
				if (m_iFramePerPoint <= FRAMEPERPOINT_MAX)
				{
				//	// TODO: もっと効率よく！
				//	if (m_iFramePerPoint < 10)
				//	{
						++m_iFramePerPoint;
				//	}
				//	else if (m_iFramePerPoint < 60)
				//	{
				//		m_iFramePerPoint += 5;
				//	}
				//	else if (m_iFramePerPoint < 600)
				//	{
				//		m_iFramePerPoint += 60;
				//	}
				//	else if (m_iFramePerPoint < 3600)
				//	{
				//		m_iFramePerPoint += 300;
				//	}
				//	else if (m_iFramePerPoint < 36000)
				//	{
				//		m_iFramePerPoint += 18000;
				//	}
				//	else
				//	{
				//		m_iFramePerPoint += 36000;
				//		if (m_iFramePerPoint > 216000)
				//		{
				//			m_iFramePerPoint = 216000;
				//		}
				//	}
				}
				else
				{
					m_iFramePerPoint = FRAMEPERPOINT_MAX;
				}
			}
			if (ChangeDisplayScale())
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

// Ｌボタンアップ
BOOL TimelineDataOperator::OnLButtonUp(UINT nFlags, CPoint point, BOOL& fStopPlay)
{
	m_fJumpFrame = FALSE;
	m_fLButtonDown = FALSE;
	if (m_fDragShuttle)
	{
		m_iShuttleSpeedNumerator = 0;
		m_iShuttleSpeedDenominator = 0;
		fStopPlay = TRUE;
	}
	else
	{
		// 再生中
		if (m_fPlay)
		{
			return FALSE;
		}
		if (m_fMove)
		{
			int iIn = m_pOperatingClipData->GetTimelineInPoint();
			m_pSelectedTrackInfo->DeleteClip(iIn);
			iIn += m_iOperatingClipFrameCount;
			m_pOperatingClipData->SetTimelineInPoint(iIn);
			m_pOperatingClipData->SetTimelineOutPoint();
			(m_pEnableMovingTrack->GetTrackDataInfo())->AddClip(iIn, m_pOperatingClipData);
			m_pOperatingClipData->CopyRect(m_pOperatingClipData->GetOperatingRect());
		}
		else if (m_fSingleInTrim)
		{
			int iIn = m_pOperatingClipData->GetTimelineInPoint();
			m_pSelectedTrackInfo->ChangeClip(iIn, iIn + m_iOperatingClipFrameCount, m_pOperatingClipData);
			m_pOperatingClipData->AddTimelineInPoint(m_iOperatingClipFrameCount);
			m_pOperatingClipData->AddInPoint(m_iOperatingClipFrameCount);
			// In側に伸びる（マイナス方向への移動）は長さを加算
			m_pOperatingClipData->AddDuration((-1 * m_iOperatingClipFrameCount));
			m_pOperatingClipData->CopyRect(m_pOperatingClipData->GetOperatingRect());
		}
		else if (m_fSingleOutTrim)
		{
			m_pOperatingClipData->AddTimelineOutPoint(m_iOperatingClipFrameCount);
			m_pOperatingClipData->AddOutPoint(m_iOperatingClipFrameCount);
			m_pOperatingClipData->AddDuration(m_iOperatingClipFrameCount);
			m_pOperatingClipData->CopyRect(m_pOperatingClipData->GetOperatingRect());
		}
		else if (m_fScrub)
		{
			if ((point.x == m_poMousePointerLocation.x) && (point.y == m_poMousePointerLocation.y))
			{
				int iDummy;
				m_iJumpFrameCursorPosition = ChangeDisplayPointToTimelineFramePosition(point, iDummy);
				m_fJumpFrame = TRUE;
				m_iJumpFrameCount = 0;
				m_iJumpFrameCount = m_iJumpFrameCursorPosition - m_iTimelineCursorFramePosition;
			}
			else
			{
				MoveTimelineCursor(m_iOperatingFrameCount);
			}
		}
	}

	m_rcMousePointRect.SetRectEmpty();
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;
	m_iEnableMovingFrameCount = 0;
	m_pEnableMovingTrack = nullptr;

	m_pOperatingClipData = nullptr;
	m_fMove = FALSE;
	m_fSingleInTrim = FALSE;
	m_fSingleOutTrim = FALSE;
	m_fScrub = FALSE;
	m_fDragShuttle = FALSE;

	return TRUE;
}

// Ｒボタンアップ
BOOL TimelineDataOperator::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (IsPointInTimelineControlPanel(point))
	{
		if (m_iFramePerPoint < 1)
		{
			if (m_iPointPerFrame < POINTPERFRAME_MAX)
			{
				++m_iPointPerFrame;
			}
		}
		else
		{
			//// TODO: もっと効率よく！
			//if (m_iFramePerPoint <= 10)
			//{
				--m_iFramePerPoint;
			//}
			//else if (m_iFramePerPoint <= 60)
			//{
			//	m_iFramePerPoint -= 5;
			//}
			//else if (m_iFramePerPoint <= 600)
			//{
			//	m_iFramePerPoint -= 60;
			//}
			//else if (m_iFramePerPoint <= 3600)
			//{
			//	m_iFramePerPoint -= 300;
			//}
			//else if (m_iFramePerPoint <= 36000)
			//{
			//	m_iFramePerPoint -= 18000;
			//}
			//else
			//{
			//	m_iFramePerPoint -= 36000;
			//}
			if (m_iFramePerPoint < 1)
			{
				m_iPointPerFrame = 2;
			}
		}
		if (ChangeDisplayScale())
		{
			return TRUE;
		}
	}
	return FALSE;
}

// マウス移動
BOOL TimelineDataOperator::OnMouseMove(UINT nFlags, CPoint point, BOOL& fStartPlay)
{
	if (m_fLButtonDown)
	{
		CSize szMoveSize(point - m_poMousePointerLocation);
		if (m_fDragShuttle)
		{
			if (szMoveSize.cx == 0)
			{
				return FALSE;
			}
			// マウス位置から倍速速度を取得
			int iNumerator = m_iShuttleSpeedNumerator;
			int iDenominator = m_iShuttleSpeedDenominator;
			CalcShuttleSpeed(point, szMoveSize, iNumerator, iDenominator);
			if ((iNumerator == m_iShuttleSpeedNumerator) && (iDenominator == m_iShuttleSpeedDenominator))
			{
				return FALSE;
			}
			// TODO: 分数を使うようになったら再考
			m_iShuttleSpeedNumerator = iNumerator;
			m_iShuttleSpeedDenominator = iDenominator;
			if ((iNumerator < 0) && (m_iTimelineCursorFramePosition < abs(iNumerator)))
			{
				return FALSE;
			}
			fStartPlay = TRUE;
			return TRUE;
		}
		// 再生中
		if (m_fPlay)
		{
			return FALSE;
		}
		if (m_fMove)
		{
			m_pOperateToTrack = IsPointInAnyTrack(point);
			if (m_pOperateToTrack == nullptr)
			{
				m_pOperateToTrack = m_pSelectedTrack;
			}
			m_pOperateToTrackInfo = m_pOperateToTrack->GetTrackDataInfo();
			m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_pOperatingClipData->GetTimelineInPoint());
			CalcClipRectDisplayPoint(m_rcMousePointRect, m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), m_iOperatingClipFrameCount);
			CheckMove(point);
			CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), m_iOperatingClipFrameCount);
			return TRUE;
		}
		else if (m_fSingleInTrim || m_fSingleOutTrim)
		{
			m_pOperatingClipData->SetOperatingRect(static_cast<CRect>(m_pOperatingClipData)); // 伸縮分のイメージ座標
			if (m_fSingleInTrim)
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_pOperatingClipData->GetTimelineInPoint());
				CheckInTrim();
				CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), 0, m_iOperatingClipFrameCount);
			}
			else
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_pOperatingClipData->GetTimelineOutPoint());
				CheckOutTrim();
				CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), 0, 0, m_iOperatingClipFrameCount);
			}
			return TRUE;
		}
		else if (m_fScrub)
		{
			if ((szMoveSize.cx != 0) && (szMoveSize.cx != 0))
			{
				m_iJumpFrameCursorPosition = 0;
			}
			szMoveSize.cx *= -1;
			m_iOperatingFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_iTimelineCursorFramePosition);
			// 最小範囲チェック
			// TODO: 最大範囲チェックは？
			if ((m_iTimelineCursorFramePosition + m_iOperatingFrameCount) < 0)
			{
				m_iOperatingFrameCount = -1 * m_iTimelineCursorFramePosition;
			}
			m_iOperatingLeftFrameNumber = m_iLeftFrameNumber + m_iOperatingFrameCount;
			m_iOperatingRightFrameNumber = m_iRightFrameNumber + m_iOperatingFrameCount;
			m_iOperatingTimelineCursorFramePosition = m_iTimelineCursorFramePosition + m_iOperatingFrameCount;
			return TRUE;
		}
	}
	return FALSE;
}

// マウスホイール
BOOL TimelineDataOperator::OnMouseWheel(UINT nFlags, short zDelta, CPoint poPoint, BOOL& fCursorChange)
{
	int iLength = static_cast<int>(ceil(zDelta / 120.0)) * -1;

	// クリップ内判定
	// TODO: ホイール中にクリップをドラッグするとまずいので抑止が必要だけどどうやる？
	if (IsPointInClip(poPoint))
	{
		// 再生中
		if (m_fPlay)
		{
			return FALSE;
		}
		if (m_fLButtonDown)
		{
			return FALSE;
		}
		if (!(m_pOperatingClipData->GetInTransition()) && !(m_pOperatingClipData->GetOutTransition()))
		{
			int iIn = m_pOperatingClipData->GetTimelineInPoint();
			if (iIn + iLength < 0)
			{
				return FALSE;
			}
			CalcClipRectDisplayPoint(static_cast<CRect>(m_pOperatingClipData), m_pOperatingClipData, static_cast<CRect>(m_pSelectedTrack));
			m_pOperateToTrack = m_pSelectedTrack;
			m_pOperateToTrackInfo = m_pSelectedTrackInfo;
			m_iOperatingClipFrameCount = iLength;
			if (!CheckMove(poPoint))
			{
				m_iOperatingClipFrameCount = 0;
				return FALSE;
			}
			m_pSelectedTrackInfo->DeleteClip(iIn);
			iIn += iLength;
			m_pOperatingClipData->SetTimelineInPoint(iIn);
			m_pOperatingClipData->SetTimelineOutPoint();
			m_pSelectedTrackInfo->AddClip(iIn, m_pOperatingClipData);
			m_pOperatingClipData->CopyRect(m_pOperatingClipData->GetOperatingRect());
			m_iOperatingClipFrameCount = 0;
		}
		return TRUE;
	}
	else if (IsPointInTimelineControlPanel(poPoint))
	{
		if (m_iFramePerPoint < 1)
		{
			m_iPointPerFrame += iLength;
			if (m_iPointPerFrame < 1)
			{
				m_iPointPerFrame = 0;
				m_iFramePerPoint = 2;
			}
			else if (m_iPointPerFrame > POINTPERFRAME_MAX)
			{
				m_iPointPerFrame = POINTPERFRAME_MAX;
			}
		}
		else
		{
			m_iFramePerPoint -= iLength;
			if (m_iFramePerPoint < 1)
			{
				m_iFramePerPoint = 0;
				m_iPointPerFrame = 2;
			}
			else if (m_iPointPerFrame > FRAMEPERPOINT_MAX)
			{
				m_iPointPerFrame = FRAMEPERPOINT_MAX;
			}
		}
		if (ChangeDisplayScale())
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		// 再生中
		if (m_fPlay)
		{
			return FALSE;
		}
		int iPosition = m_iTimelineCursorFramePosition;
		m_iTimelineCursorFramePosition += iLength;
		if (m_iTimelineCursorFramePosition < 0)
		{
			m_iTimelineCursorFramePosition = 0;
		}
		if (iPosition == m_iTimelineCursorFramePosition)
		{
			return FALSE;
		}
		fCursorChange = TRUE;
		CalcTimelineDisplayRange();
		if (m_iPointPerFrame > 0)
		{
			return TRUE;
		}
		if (m_iTimelineCursorFramePosition % m_iFramePerPoint == 0)
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


// ドロップによるファイルのクリップ化
BOOL TimelineDataOperator::OnDropFiles(const HDROP& hDropInfo, CString& strFileName)
{
	m_fDragAndDrop = FALSE;
	if (!m_fAllowFile)
	{
		return FALSE;
	}
	m_fAllowFile = FALSE;
	if (!m_fAllowDrop)
	{
		return FALSE;
	}
	m_fAllowDrop = FALSE;

	// 再生中
	if (m_fPlay)
	{
		return FALSE;
	}
	////ドロップされたファイルの個数をチェック
	//UINT uCount = DragQueryFile(hDropInfo, ~0lu, NULL, 0);
	//if (uCount != 1)
	//{
	//	return FALSE;
	//}

	// ファイル名を取得 無駄なんでメンバ変数に持とう
	UINT uLen = DragQueryFile(hDropInfo, 0, NULL, 0);
	DragQueryFile(hDropInfo, 0, strFileName.GetBuffer(uLen + 1), uLen + 1);
	strFileName.ReleaseBuffer();

	//// ファイル拡張子チェック
	//if (!(m_pDropAndDragOperator->CheckFileNameExtension(strFileName)))
	//{
	//	return FALSE;
	//}

	// TODO: ファイル形式チェック 無駄なんでメンバ変数に持とう
	CString strFilePath;
	UINT uIn = 0, uOut = 0;
	int iFileDuration = 0;
	if (!(m_pDropAndDragOperator->CheckDropFile(strFileName, strFilePath, uIn, uOut, iFileDuration)))
	{
		return FALSE;
	}

	// ドロップ先のトラックを特定
	POINT poDorpPoint;
	DragQueryPoint(hDropInfo, &poDorpPoint);
	m_pSelectedTrack = m_pDropAndDragOperator->GetDropTrack(poDorpPoint);
	if (m_pSelectedTrack == nullptr)
	{
		return FALSE;
	}

	// クリップデータを作成する
	int iFrame = 0, iDropFrame = 0;
	m_pSelectedTrackInfo = m_pSelectedTrack->GetTrackDataInfo();
	iDropFrame = ChangeDisplayPointToTimelineFramePosition(poDorpPoint, iFrame);
	if (m_pDropAndDragOperator->CreateClipDataFromDropFile(*m_pSelectedTrackInfo, iDropFrame, static_cast<PCTSTR>(strFilePath), uIn, uOut, iFileDuration))
	{
		return TRUE;
	}
	return FALSE;
}

// ドラッグ＆ドロップ開始
BOOL TimelineDataOperator::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point)
{
	// 再生中
	if (m_fPlay)
	{
		return FALSE;
	}
	m_fDragAndDrop = TRUE;
	m_fAllowFile = FALSE;
	m_fAllowDrop = FALSE;

	// ファイルか？
	if (!(pDataObject->IsDataAvailable(CF_HDROP)))
	{
		return FALSE;
	}
	STGMEDIUM st;
	if (!(pDataObject->GetData(CF_HDROP, &st)))
	{
		return FALSE;
	}

	HDROP hDropInfo = static_cast<HDROP>(st.hGlobal);
	const UINT uCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	//ドロップされたファイルの個数をチェック
	if (uCount != 1)
	{
		return FALSE;
	}

	// ファイル名を取得
	CString strFileName;
	UINT uLen = DragQueryFile(hDropInfo, 0, NULL, 0);
	DragQueryFile(hDropInfo, 0, strFileName.GetBuffer(uLen + 1), uLen + 1);
	strFileName.ReleaseBuffer();

	// ファイル拡張子チェック
	if (!(m_pDropAndDragOperator->CheckFileNameExtension(strFileName)))
	{
		return FALSE;
	}

	// ファイル形式チェック
	CString strFilePath;
	UINT uIn = 0, uOut = 0;
	int iFileDuration;
	if (!(m_pDropAndDragOperator->CheckDropFile(strFileName, strFilePath, uIn, uOut, iFileDuration)))
	{
		return FALSE;
	}

	m_fAllowFile = TRUE;

	// 暫定クリップ作成
	int iDuration = m_pDropAndDragOperator->CreateClipData(*m_pDnDClipDataRect, static_cast<PCTSTR>(strFilePath), uIn, uOut, iFileDuration);

	// ドロップ先イメージ作成
	OnDragOver(pDataObject, dwKeyState, point);

	return TRUE;
}

// ドラッグ＆ドロップ終了
void TimelineDataOperator::OnDragLeave(void)
{
	m_fDragAndDrop = FALSE;
	m_fAllowFile = FALSE;
	m_fAllowDrop = FALSE;

	// TODO: クリップデータのクリアをしたい！！
	m_pDnDClipDataRect->SetRectEmpty();
}

// ドラッグ＆ドロップ移動
DROPEFFECT TimelineDataOperator::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point)
{
	// 再生中
	if (m_fPlay)
	{
		return FALSE;
	}
	DROPEFFECT dDropEff = DROPEFFECT_COPY;
	if (!m_fAllowFile)
	{
		return DROPEFFECT_NONE;
	}
	m_fAllowDrop = FALSE;

	int iFrame = 0, iDropFrame = 0;
	iDropFrame = ChangeDisplayPointToTimelineFramePosition(point, iFrame);
	m_pDropAndDragOperator->SetClipDataInOutPoint(*m_pDnDClipDataRect, static_cast<UINT>(iDropFrame),
		static_cast<UINT>(m_pDnDClipDataRect->GetDuration()));

	// ドロップ先のトラックを特定
	m_pSelectedTrack = m_pDropAndDragOperator->GetDropTrack(point);
	if (m_pSelectedTrack != nullptr)
	{
		// 表示座標を確定する
		CalcClipRectDisplayPoint(static_cast<CRect&>(*m_pDnDClipDataRect), m_pDnDClipDataRect, m_pSelectedTrack);
		m_pDnDClipDataRect->SetOperatingRect(static_cast<CRect>(m_pDnDClipDataRect));
		// ドロップ可否判定
		if (m_pSelectedTrack->GetTrackDataInfo()->CheckPlaceInRange(nullptr, m_pDnDClipDataRect->GetTimelineInPoint(), m_pDnDClipDataRect->GetTimelineOutPoint()) != nullptr)
		{
			dDropEff = DROPEFFECT_NONE;
		}
		else
		{
			m_fAllowDrop = TRUE;
		}
	}
	else
	{
		// ドロップ不可の場合、マウスを中心に配置する。
		SetRectAroundPoint(*m_pDnDClipDataRect, point, TRACK_HIEGHT_DEFAULT);
		m_pDnDClipDataRect->SetOperatingRect(static_cast<CRect>(m_pDnDClipDataRect));
		dDropEff = DROPEFFECT_NONE;
	}
	return dDropEff;
}

// 右クリック時の表示メニューを確定する
BOOL TimelineDataOperator::OnContextMenu(const CPoint& poClientPoint, CMenu& mContextMenu)
{
	// 再生中
	if (m_fPlay)
	{
		return FALSE;
	}
	if (!(IsPointInClip(poClientPoint)))
	{
		return FALSE;
	}
	ASSERT(m_pSelectedTrackInfo);
	ASSERT(m_pOperatingClipData);
	if (!m_pSelectedTrackInfo)
	{
		return FALSE;
	}
	if (!m_pOperatingClipData)
	{
		return FALSE;
	}

	CString strTransSet = _T("トランジション設定(&S)");
	CString strTransReset = _T("トランジション解除(&R)");
	CString strSetIn = _T("In側設定(&I)");
	CString strSetOut = _T("Out側設定(&O)");
	CString strResetIn = _T("In側解除(&I)");
	CString strResetOut = _T("Out側解除(&O)");
	BOOL fDelTransSet = FALSE, fDelTransReset = FALSE, fDelSetIn = FALSE, fDelSetOut = FALSE, fDelResetIn = FALSE, fDelResetOut = FALSE;

	mContextMenu.LoadMenuW(IDR_CONTEXT_CLIP);
	CMenu* pSubMenu = mContextMenu.GetSubMenu(0);
	int iPos = 0;
	CMenu* pDel = nullptr, *pSubDel = nullptr;

	// In側メニューの調整
	if (m_pOperatingClipData->GetInTransition())
	{
		// 既にトランジション設定ありの場合、In設定メニューを削除
		pSubMenu->RemoveMenu(ID_TRANSITION_SET_IN, MF_BYCOMMAND);
		fDelSetIn = TRUE;
	}
	else
	{
		// 既にトランジション設定なしの場合、In解除メニューを削除
		pDel = FindSubMenuFromString(pSubMenu, static_cast<LPCTSTR>(strTransReset), iPos);
		if (iPos >= 0)
		{
			pSubDel = FindSubMenuFromString(pDel, static_cast<LPCTSTR>(strResetIn), iPos);
			if (iPos >= 0)
			{
				pDel->RemoveMenu(iPos, MF_BYPOSITION);
				fDelResetIn = TRUE;
			}
			pSubDel = nullptr;
		}
		pDel = nullptr;
		if ((m_pOperatingClipData->GetInPoint() == 0) || (m_pOperatingClipData->GetDuration() <= 1))
		{
			// 自身のIn側ギャップがないまたは長さが足りない場合、In設定メニューを削除
			pSubMenu->RemoveMenu(ID_TRANSITION_SET_IN, MF_BYCOMMAND);
			fDelSetIn = TRUE;
		}
		else
		{
			ClipDataRect* pClipRect = IsClipAtIn(*m_pSelectedTrackInfo, m_pOperatingClipData->GetTimelineInPoint());
			if (pClipRect == nullptr)
			{
				// In側隣接クリップなしの場合、In設定メニューを削除
				pSubMenu->RemoveMenu(ID_TRANSITION_SET_IN, MF_BYCOMMAND);
				fDelSetIn = TRUE;
			}
			else
			{
				if ((pClipRect->GetOutPoint() == pClipRect->GetFileDuration()) || (pClipRect->GetDuration() <= 1))
				{
					// In側隣接クリップありだが、隣接クリップのOut側ギャップがないまたは長さが足りない場合、Out設定メニューを削除
					pSubMenu->RemoveMenu(ID_TRANSITION_SET_IN, MF_BYCOMMAND);
					fDelSetIn = TRUE;
				}
			}
		}
	}

	// Out側メニューの調整
	if (m_pOperatingClipData->GetOutTransition())
	{
		// 既にトランジション設定ありの場合、Out設定メニューを削除
		pSubMenu->RemoveMenu(ID_TRANSITION_SET_OUT, MF_BYCOMMAND);
		fDelSetOut = TRUE;
	}
	else
	{
		// 既にトランジション設定なしの場合、Out解除メニューを削除
		pDel = FindSubMenuFromString(pSubMenu, static_cast<LPCTSTR>(strTransReset), iPos);
		if (iPos >= 0)
		{
			pSubDel = FindSubMenuFromString(pDel, static_cast<LPCTSTR>(strResetOut), iPos);
			if (iPos >= 0)
			{
				pDel->RemoveMenu(iPos, MF_BYPOSITION);
				fDelResetOut = TRUE;
			}
			pSubDel = nullptr;
		}
		pDel = nullptr;
		if ((m_pOperatingClipData->GetOutPoint() == m_pOperatingClipData->GetFileDuration()) || (m_pOperatingClipData->GetDuration() <= 1))
		{
			// 自身のOut側ギャップがないまたは長さが足りない場合、Out設定メニューを削除
			pSubMenu->RemoveMenu(ID_TRANSITION_SET_OUT, MF_BYCOMMAND);
			fDelSetOut = TRUE;
		}
		else 
		{
			ClipDataRect* pClipRect = IsClipAtOut(*m_pSelectedTrackInfo, m_pOperatingClipData->GetTimelineOutPoint());
			if (pClipRect == nullptr)
			{
				// Out側隣接クリップなしの場合、Out設定メニューを削除
				pSubMenu->RemoveMenu(ID_TRANSITION_SET_OUT, MF_BYCOMMAND);
				fDelSetOut = TRUE;
			}
			else
			{
				if ((pClipRect->GetInPoint() == 0) || (pClipRect->GetDuration() <= 1))
				{
					// Out側隣接クリップありだが、隣接クリップのIn側ギャップがないまたは長さが足りない場合、Out設定メニューを削除
					pSubMenu->RemoveMenu(ID_TRANSITION_SET_OUT, MF_BYCOMMAND);
					fDelSetOut = TRUE;
				}
			}
		}
	}

	// 1段目メニューの調整
	if (fDelSetIn && fDelSetOut)
	{
		pDel = FindSubMenuFromString(pSubMenu, static_cast<LPCTSTR>(strTransSet), iPos);
		if (iPos >= 0)
		{
			pSubMenu->RemoveMenu(iPos, MF_BYPOSITION);
			fDelTransSet = TRUE;
		}
		pDel = nullptr;
	}
	if (fDelResetIn && fDelResetOut)
	{
		pDel = FindSubMenuFromString(pSubMenu, static_cast<LPCTSTR>(strTransReset), iPos);
		if (iPos >= 0)
		{
			pSubMenu->RemoveMenu(iPos, MF_BYPOSITION);
			fDelTransReset = TRUE;
		}
		pDel = nullptr;
	}
	if (fDelTransSet && fDelTransReset)
	{
		pSubMenu->RemoveMenu(0, MF_BYPOSITION);
	}
	return TRUE;
}

// トランジション設定（In側）
BOOL TimelineDataOperator::OnTransitionSetIn(PCTSTR pszMessage)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	ClipDataRect* pClipRect = IsClipAtIn(*m_pSelectedTrackInfo, m_pOperatingClipData->GetTimelineInPoint());
	if (pClipRect == nullptr)
	{
		return FALSE;
	}

	int iLength = TRANSITION_DEFAULT_LENGTH;
	if (m_pOperatingClipData->GetInPoint() < iLength)
	{
		iLength = m_pOperatingClipData->GetInPoint();
	}
	if ((pClipRect->GetFileDuration() - pClipRect->GetOutPoint()) < iLength)
	{
		iLength = pClipRect->GetFileDuration() - pClipRect->GetOutPoint();
	}
	if ((m_pOperatingClipData->GetDuration() - 1) < iLength)
	{
		iLength = m_pOperatingClipData->GetDuration() - 1;
	}
	if ((pClipRect->GetDuration() - 1) < iLength)
	{
		iLength = pClipRect->GetDuration() - 1;
	}
	if (iLength == 0)
	{
		pszMessage = TRANSITION_GAP_NOT_EXIST;
		return FALSE;
	}
	CString strInTransitionUUID, strOutTransitionUUID;
	if (!(m_pTransitionManager->CreateTransitionData(strInTransitionUUID, iLength)))
	{
		return FALSE;
	}
	if (!(m_pTransitionManager->CreateTransitionData(strOutTransitionUUID, iLength, FALSE)))
	{
		m_pTransitionManager->DeleteTransitionData(strInTransitionUUID);
		return FALSE;
	}
	m_pOperatingClipData->SetInTransitionUUID(static_cast<PCTSTR>(strInTransitionUUID));
	m_pSelectedTrackInfo->ChangeClip(m_pOperatingClipData->GetTimelineInPoint(), m_pOperatingClipData->GetTimelineInPoint() + (iLength * -1), m_pOperatingClipData);
	m_pOperatingClipData->AddInPoint(iLength * -1);
	m_pOperatingClipData->AddTimelineInPoint(iLength * -1);
	m_pOperatingClipData->SetDuration();
	m_pOperatingClipData->SetInTransition(TRUE);
	m_pOperatingClipData->SetInTransitionClipInfo(pClipRect->GetClipDataInfo());

	pClipRect->SetOutTransitionUUID(static_cast<PCTSTR>(strOutTransitionUUID));
	pClipRect->AddOutPoint(iLength);
	pClipRect->AddTimelineOutPoint(iLength);
	pClipRect->SetDuration();
	pClipRect->SetOutTransition(TRUE);
	pClipRect->SetOutTransitionClipInfo(m_pOperatingClipData->GetClipDataInfo());

	return TRUE;
}

// トランジション設定（Out側）
BOOL TimelineDataOperator::OnTransitionSetOut(PCTSTR pszMessage)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	ClipDataRect* pClipRect = IsClipAtOut(*m_pSelectedTrackInfo, m_pOperatingClipData->GetTimelineOutPoint());
	if (pClipRect == nullptr)
	{
		return FALSE;
	}

	int iLength = TRANSITION_DEFAULT_LENGTH;
	if ((m_pOperatingClipData->GetFileDuration() - m_pOperatingClipData->GetOutPoint()) < iLength)
	{
		iLength = m_pOperatingClipData->GetFileDuration() - m_pOperatingClipData->GetOutPoint();
	}
	if (pClipRect->GetInPoint() < iLength)
	{
		iLength = pClipRect->GetInPoint();
	}
	if ((m_pOperatingClipData->GetDuration() - 1) < iLength)
	{
		iLength = m_pOperatingClipData->GetDuration() - 1;
	}
	if ((pClipRect->GetDuration() - 1) < iLength)
	{
		iLength = pClipRect->GetDuration() - 1;
	}
	if (iLength == 0)
	{
		pszMessage = TRANSITION_GAP_NOT_EXIST;
		return FALSE;
	}
	CString strInTransitionUUID, strOutTransitionUUID;
	if (!(m_pTransitionManager->CreateTransitionData(strInTransitionUUID, iLength)))
	{
		return FALSE;
	}
	if (!(m_pTransitionManager->CreateTransitionData(strOutTransitionUUID, iLength, FALSE)))
	{
		m_pTransitionManager->DeleteTransitionData(strInTransitionUUID);
		return FALSE;
	}
	m_pOperatingClipData->SetOutTransitionUUID(static_cast<PCTSTR>(strOutTransitionUUID));
	m_pOperatingClipData->AddOutPoint(iLength);
	m_pOperatingClipData->AddTimelineOutPoint(iLength);
	m_pOperatingClipData->SetDuration();
	m_pOperatingClipData->SetOutTransition(TRUE);
	m_pOperatingClipData->SetOutTransitionClipInfo(pClipRect->GetClipDataInfo());

	pClipRect->SetInTransitionUUID(static_cast<PCTSTR>(strInTransitionUUID));
	m_pSelectedTrackInfo->ChangeClip(pClipRect->GetTimelineInPoint(), pClipRect->GetTimelineInPoint() + (iLength * -1), pClipRect);
	pClipRect->AddInPoint(iLength * -1);
	pClipRect->AddTimelineInPoint(iLength * -1);
	pClipRect->SetDuration();
	pClipRect->SetInTransition(TRUE);
	pClipRect->SetInTransitionClipInfo(m_pOperatingClipData->GetClipDataInfo());

	return TRUE;
}

// トランジション解除（In・中間）
BOOL TimelineDataOperator::OnTransitionResetInCenter(void)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	if (!(m_pOperatingClipData->GetInTransition()))
	{
		return FALSE;
	}

	// トランジションのペアを解除
	ClipDataInfo* pClipInfo = m_pOperatingClipData->GetInTransitionClipInfo();
	ASSERT(pClipInfo);
	if (!pClipInfo)
	{
		return FALSE;
	}
	TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
	int iLength = pTransitionData->GetTransitionDuration();
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
	pClipInfo->ResetTransition(FALSE, iLength);
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetInTransitionUUID()));
	int iOldInPoint = m_pOperatingClipData->GetTimelineInPoint();
	m_pOperatingClipData->ResetTransition(TRUE, iLength);
	m_pSelectedTrackInfo->ChangeClip(iOldInPoint, m_pOperatingClipData->GetTimelineInPoint(), m_pOperatingClipData);

	return TRUE;
}

// トランジション解除（In・開始点）
BOOL TimelineDataOperator::OnTransitionResetInStart(void)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	if (!(m_pOperatingClipData->GetInTransition()))
	{
		return FALSE;
	}

	// トランジションのペアを解除
	ClipDataInfo* pClipInfo = m_pOperatingClipData->GetInTransitionClipInfo();
	ASSERT(pClipInfo);
	if (!pClipInfo)
	{
		return FALSE;
	}
	TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
	int iLength = pTransitionData->GetTransitionDuration() * 2;
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
	pClipInfo->ResetTransition(FALSE, iLength);
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetInTransitionUUID()));
	m_pOperatingClipData->ResetTransition(TRUE, 0);

	return TRUE;
}

// トランジション解除（In・終了点）
BOOL TimelineDataOperator::OnTransitionResetInEnd(void)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	if (!(m_pOperatingClipData->GetInTransition()))
	{
		return FALSE;
	}

	// トランジションのペアを解除
	ClipDataInfo* pClipInfo = m_pOperatingClipData->GetInTransitionClipInfo();
	ASSERT(pClipInfo);
	if (!pClipInfo)
	{
		return FALSE;
	}
	TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
	int iLength = pTransitionData->GetTransitionDuration() * 2;
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
	pClipInfo->ResetTransition(FALSE, 0);
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetInTransitionUUID()));
	int iOldInPoint = m_pOperatingClipData->GetTimelineInPoint();
	m_pOperatingClipData->ResetTransition(TRUE, iLength);
	m_pSelectedTrackInfo->ChangeClip(iOldInPoint, m_pOperatingClipData->GetTimelineInPoint(), m_pOperatingClipData);

	return TRUE;
}

// トランジション解除（Out・中間）
BOOL TimelineDataOperator::OnTransitionResetOutCenter(void)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	if (!(m_pOperatingClipData->GetOutTransition()))
	{
		return FALSE;
	}

	// トランジションのペアを解除
	ClipDataInfo* pClipInfo = m_pOperatingClipData->GetOutTransitionClipInfo();
	ASSERT(pClipInfo);
	if (!pClipInfo)
	{
		return FALSE;
	}
	TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
	int iLength = pTransitionData->GetTransitionDuration();
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
	int iOldInPoint = pClipInfo->GetTimelineInPoint();
	pClipInfo->ResetTransition(TRUE, iLength);
	m_pSelectedTrackInfo->ChangeClip(iOldInPoint, pClipInfo->GetTimelineInPoint(), pClipInfo->GetClipDataRect());
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetOutTransitionUUID()));
	m_pOperatingClipData->ResetTransition(FALSE, iLength);

	return TRUE;
}

// トランジション解除（Out・開始点）
BOOL TimelineDataOperator::OnTransitionResetOutStart(void)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	if (!(m_pOperatingClipData->GetOutTransition()))
	{
		return FALSE;
	}

	// トランジションのペアを解除
	ClipDataInfo* pClipInfo = m_pOperatingClipData->GetOutTransitionClipInfo();
	ASSERT(pClipInfo);
	if (!pClipInfo)
	{
		return FALSE;
	}
	TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
	int iLength = pTransitionData->GetTransitionDuration() * 2;
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
	pClipInfo->ResetTransition(TRUE, 0);
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetOutTransitionUUID()));
	m_pOperatingClipData->ResetTransition(FALSE, iLength);

	return TRUE;
}

// トランジション解除（Out・終了点）
BOOL TimelineDataOperator::OnTransitionResetOutEnd(void)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((!m_pOperatingClipData) || !(m_pSelectedTrackInfo))
	{
		return FALSE;
	}
	if (!(m_pOperatingClipData->GetOutTransition()))
	{
		return FALSE;
	}

	// トランジションのペアを解除
	ClipDataInfo* pClipInfo = m_pOperatingClipData->GetOutTransitionClipInfo();
	ASSERT(pClipInfo);
	if (!pClipInfo)
	{
		return FALSE;
	}
	TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
	int iLength = pTransitionData->GetTransitionDuration() * 2;
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
	int iOldInPoint = pClipInfo->GetTimelineInPoint();
	pClipInfo->ResetTransition(TRUE, iLength);
	m_pSelectedTrackInfo->ChangeClip(iOldInPoint, pClipInfo->GetTimelineInPoint(), pClipInfo->GetClipDataRect());
	m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetOutTransitionUUID()));
	m_pOperatingClipData->ResetTransition(FALSE, 0);

	return TRUE;
}


// 再生処理
BOOL TimelineDataOperator::OnPlay(int iSpeed)
{
	if (!m_fPlay)
	{
		m_fPreviewWork = TRUE;
		m_fPlay = TRUE;
		m_iPlayFrameCount = 0;
	}
	MoveTimelineCursor(iSpeed);
	if (m_iPointPerFrame > 0)
	{
		return TRUE;
	}
	m_iPlayFrameCount += abs(iSpeed);
	//if (m_iTimelineCursorFramePosition % m_iFramePerPoint == 0)
	//{
	if (m_iPlayFrameCount > m_iFramePerPoint)
	{
		m_iPlayFrameCount -= m_iFramePerPoint;
		return TRUE;
	}
	//}
	return FALSE;
}

// 再生停止
void TimelineDataOperator::OnStop(void)
{
	m_fPreviewWork = FALSE;
	m_fPlay = FALSE;
	m_iPlayFrameCount = 0;
}



// タイムラインデータ表示倍率の変更
BOOL TimelineDataOperator::ChangeDisplayScale(void)
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

// 表示可能なタイムラインの範囲（左端、右端）を計算する
void TimelineDataOperator::CalcTimelineDisplayRange(void)
{
	// 表示可能フレーム範囲の計算
	int iDisplayFrameCount = 0;
	if (m_iPointPerFrame < 1)
	{
		iDisplayFrameCount = m_pSeekBarRect->Width() * m_iFramePerPoint;
	}
	else
	{
		iDisplayFrameCount = static_cast<int>(ceil(m_pSeekBarRect->Width() / m_iPointPerFrame));
	}
	m_iTimelineCursorPoint = static_cast<int>(floor(m_pSeekBarRect->Width() / 2.0f)) + m_pSeekBarRect->left;
	m_iLeftFrameNumber = m_iTimelineCursorFramePosition - static_cast<int>(floor((iDisplayFrameCount / 2.0f)));
	m_iOperatingLeftFrameNumber = m_iLeftFrameNumber;
	m_iRightFrameNumber = m_iTimelineCursorFramePosition + static_cast<int>(ceil((iDisplayFrameCount / 2.0f))) + 1;
	m_iOperatingRightFrameNumber = m_iRightFrameNumber;
	m_iOperatingTimelineCursorFramePosition = m_iTimelineCursorFramePosition;
}


/*

ヒットテスト系

*/
// クリックポイントがトラック領域にあるかの判定
TrackDataRect* TimelineDataOperator::IsPointInAnyTrack(const CPoint& point)
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
BOOL TimelineDataOperator::IsPointInSeekBar(const CPoint& point)
{
	if (!(m_pSeekBarRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// クリック位置がタイムラインコントロールパネル内かを判定する
BOOL TimelineDataOperator::IsPointInTimelineControlPanel(const CPoint& point)
{
	if (!(m_pTimelineControlPanelRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// クリック位置がクリップ内かを判定する
BOOL TimelineDataOperator::IsPointInClip(const CPoint& poPoint)
{
	// タイムラインデータエリア内判定
	if (m_pTimelineDataRect->PtInRect(poPoint))
	{
		// トラック判定
		m_pSelectedTrack = IsPointInAnyTrack(poPoint);
		if (m_pSelectedTrack != nullptr)
		{
			m_pSelectedTrackInfo = m_pSelectedTrack->GetTrackDataInfo();

			if (IsPointInAnyClipRect(poPoint, *m_pSelectedTrackInfo))
			{
				return TRUE;
			}
			else
			{
				m_pSelectedTrack = nullptr;
				m_pSelectedTrackInfo = nullptr;
			}
		}
	}
	return FALSE;
}

// クリック位置がクリップ内かを判定する（トラック確定後）
BOOL TimelineDataOperator::IsPointInAnyClipRect(const CPoint& point, TrackDataInfo& cTrackInfo)
{
	int iFrame;
	ChangeDisplayPointToTimelineFramePosition(point, iFrame);
	if (iFrame < 0)
	{
		return FALSE;
	}

	int iInPoint = 0;
	ClipDataPositionMap mpClipMap;
	int iSize = cTrackInfo.GetClipDataAtFrame(iFrame, mpClipMap);
	if (iSize == 0)
	{
		m_pOperatingClipData = nullptr;
		mpClipMap.clear();
		return FALSE;
	}
	else if (iSize == 1)
	{
		ClipDataPositionMap::iterator itr = mpClipMap.begin();
		m_pOperatingClipData = (*itr).second;
	}
	else
	{
		// TODO: 複数選択にしないといけない！
		ClipDataPositionMap::iterator itr = mpClipMap.begin();
		++itr;
		m_pOperatingClipData = (*itr).second;
	}
	mpClipMap.clear();
	return TRUE;
}

// Move/Trim振り分け
BOOL TimelineDataOperator::IsPointInClipRect(const CPoint& point, const CRect& rcClipRect)
{
	m_fMove = FALSE;
	m_fSingleInTrim = FALSE;
	m_fSingleOutTrim = FALSE;

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
		m_fMove = !(IsPointInTrimRange(point, &rcClipRect));
	}
	return (m_fMove || m_fSingleInTrim || m_fSingleOutTrim);
}

// クリック箇所がクリップ内のトリム操作エリア内かを判定
BOOL TimelineDataOperator::IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect)
{
	CRect rcTrimRect;

	// In側判定
	rcTrimRect.CopyRect(rcClipRect);
	// クリップ幅が規定値未満の場合はInTrim決めうちでInTrim可否をチェック
	if (rcTrimRect.Width() < kClipHitCheckMinWidth)
	{
		ASSERT(m_pOperatingClipData);
		ASSERT(m_pSelectedTrackInfo);
		if ((m_pOperatingClipData->GetInPoint() != 0) && (m_pOperatingClipData->GetTimelineInPoint() != 0) &&
			(IsClipAtIn(*m_pSelectedTrackInfo, m_pOperatingClipData->GetTimelineInPoint()) == nullptr))
		{
			m_fSingleInTrim = TRUE;
		}
		else
		{
			m_fSingleOutTrim = TRUE;
		}
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
		m_fSingleInTrim = rcTrimRect.PtInRect(point);
	}

	if (!(m_fSingleInTrim || m_fSingleOutTrim))
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
		m_fSingleOutTrim = rcTrimRect.PtInRect(point);
	}

	return (m_fSingleInTrim || m_fSingleOutTrim);
}

// 入力されたクリップのIn側に隣接クリップがあるかを判定する
ClipDataRect* TimelineDataOperator::IsClipAtIn(TrackDataInfo& cTrackInfo, const int& iIn)
{
	if (iIn < 1)
	{
		return nullptr;
	}
	ClipDataPositionMap mpClipMap;
	int iCount = cTrackInfo.GetClipDataAtFrame(iIn - 1, mpClipMap);
	if (iCount == 1)
	{
		ClipDataPositionMap::iterator itr = mpClipMap.begin();
		ClipDataRect* pClipData = (*itr).second;
		if (pClipData->GetTimelineOutPoint() == iIn)
		{
			mpClipMap.clear();
			return pClipData;
		}
	}
	mpClipMap.clear();
	return nullptr;
}

// 入力されたクリップのOut側に隣接クリップがあるかを判定する
ClipDataRect* TimelineDataOperator::IsClipAtOut(TrackDataInfo& cTrackInfo, const int& iOut)
{
	if (iOut < 1)
	{
		return nullptr;
	}
	ClipDataPositionMap mpClipMap;
	int iCount = cTrackInfo.GetClipDataAtFrame(iOut, mpClipMap);
	if (iCount == 1)
	{
		ClipDataPositionMap::iterator itr = mpClipMap.begin();
		ClipDataRect* pClipData = (*itr).second;
		if (pClipData->GetTimelineInPoint() == iOut)
		{
			mpClipMap.clear();
			return pClipData;
		}
	}
	mpClipMap.clear();
	return nullptr;
}




/*

座標、位置計算系

*/
// クリップ位置計算
BOOL TimelineDataOperator::CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{

	if (CalcClipRect(rcClipRect, const_cast<ClipDataRect*>(clClipData)->GetTimelineInPoint(), const_cast<ClipDataRect*>(clClipData)->GetDuration(),
		rcTrackRect, iMoveFrames, iIntrimFrames, iOuttrimFrames))
	{
		if (rcClipRect.left < m_pTimelineDataRect->left)
		{
			rcClipRect.left = m_pTimelineDataRect->left;
		}
		if (rcClipRect.right > m_pTimelineDataRect->right)
		{
			rcClipRect.right = m_pTimelineDataRect->right;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

// クリップ位置計算（はみ出し補正なし）
BOOL TimelineDataOperator::CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{
	rcClipRect.top = rcTrackRect.top + static_cast<int>(rcTrackRect.Height() * (1 - CLIPHIGHT));
	rcClipRect.bottom = rcClipRect.top + static_cast<int>(rcTrackRect.Height() * CLIPHIGHT);

	if (iInPoint + iMoveFrames + iIntrimFrames > m_iOperatingRightFrameNumber)
	{
		rcClipRect.SetRectEmpty();
		return FALSE;
	}
	if ((iInPoint + iDuration + iMoveFrames + iOuttrimFrames) < m_iOperatingLeftFrameNumber)
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
int TimelineDataOperator::ChangeTimelineFramePositionToDisplayPoint(const int iFrame)
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
int TimelineDataOperator::ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame)
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
		if (iHorizontalLengthFromTimelineCursor < 0)
		{
			// タイムラインカーソルからの相対座標を求める
			iFrame = static_cast<int>(floor(iHorizontalLengthFromTimelineCursor / m_iPointPerFrame)) + m_iTimelineCursorFramePosition - 1;
		}
		else
		{
			// タイムラインカーソルからの相対座標を求める
			iFrame = static_cast<int>(floor(iHorizontalLengthFromTimelineCursor / m_iPointPerFrame)) + m_iTimelineCursorFramePosition;
		}
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
int TimelineDataOperator::ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame /* = 0 */)
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

操作チェック系（コントローラーへ移動）

*/
// 操作がIn側トリム可能な範囲内かを判定して位置を調整する
BOOL TimelineDataOperator::CheckInTrim(void)
{
	int iDuration = m_pOperatingClipData->GetDuration();
	// 範囲チェック
	if (iDuration - m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = iDuration - 1;
		return FALSE;
	}

	// 重なりチェック
	m_iOperatingClipFrameCount = m_pOperateToTrackInfo->CheckClipInSingleInTrimRange(m_pOperatingClipData->GetTimelineInPoint(), 
		(m_pOperatingClipData->GetTimelineInPoint() + m_iOperatingClipFrameCount)) 
		- m_pOperatingClipData->GetTimelineInPoint();
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	BOOL fRet = TRUE;
	// クリップ限界チェック
	int iEnableOperateFrame = m_iOperatingClipFrameCount;
	int iIn = m_pOperatingClipData->GetInPoint();
	if (iIn + m_iOperatingClipFrameCount < 0)
	{
			iEnableOperateFrame = iIn * -1;
		fRet = FALSE;
	}
	// 範囲チェック
	if (m_pOperatingClipData->GetTimelineInPoint() + m_iOperatingClipFrameCount < 0)
	{
		int iZeroFrameDistance = m_pOperatingClipData->GetTimelineInPoint() * -1;
		if (iEnableOperateFrame < iZeroFrameDistance)
		{
			m_iOperatingClipFrameCount = iZeroFrameDistance;
		}
		else
		{
			m_iOperatingClipFrameCount = iEnableOperateFrame;
		}
		fRet = FALSE;
	}
	else
	{
		m_iOperatingClipFrameCount = iEnableOperateFrame;
	}
	return fRet;
}

// 操作がOut側トリム可能な範囲内かを判定して位置を調整する
BOOL TimelineDataOperator::CheckOutTrim(void)
{
	// 範囲チェック
	int iDuration = m_pOperatingClipData->GetDuration();
	// 範囲チェック
	if (iDuration + m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = (iDuration * -1) + 1;
		return FALSE;
	}

	// 重なりチェック
	int iStartFrame = m_pOperatingClipData->GetTimelineOutPoint() - 1;
	m_iOperatingClipFrameCount = m_pOperateToTrackInfo->CheckClipInSingleOutTrimRange(iStartFrame, iStartFrame + m_iOperatingClipFrameCount) - iStartFrame;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	if ((m_pOperatingClipData->GetOutPoint() + m_iOperatingClipFrameCount) > m_pOperatingClipData->GetFileDuration())
	{
		m_iOperatingClipFrameCount = m_pOperatingClipData->GetFileDuration() - m_pOperatingClipData->GetOutPoint();
	}

	return TRUE;
}

// 操作がMove可能な範囲内かを判定して位置を調整する
BOOL TimelineDataOperator::CheckMove(CPoint& point)
{
	int iMovingClipInFrame, iMovingClipOutFrame, iWorkOperatingClipFrameCount, iDropInPoint = 0;
	CRect rcWorkRect;
	ClipDataRect* pClipData;
	// 範囲チェック
	if (m_pOperatingClipData->GetTimelineInPoint() + m_iOperatingClipFrameCount < 0)
	{
		iWorkOperatingClipFrameCount = m_pOperatingClipData->GetTimelineInPoint() * -1;
		// ワークに操作中の矩形領域をコピー
		rcWorkRect.CopyRect(m_pOperatingClipData->GetOperatingRect());
		// 仮置きした位置に合わせてワークの矩形を移動する
		CalcClipRect(rcWorkRect, iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
		// 仮置きした場所でクリップと衝突しないかを再判定
		iMovingClipInFrame = m_pOperatingClipData->GetTimelineInPoint() + iWorkOperatingClipFrameCount;
		iMovingClipOutFrame = m_pOperatingClipData->GetTimelineOutPoint() + iWorkOperatingClipFrameCount;
		pClipData = m_pOperateToTrackInfo->CheckPlaceInRange(m_pOperatingClipData, iMovingClipInFrame, iMovingClipOutFrame);
		if ((pClipData != nullptr) && (pClipData != m_pOperatingClipData))
		{
			// 重なりがあった場合
			m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
			m_pOperateToTrack = m_pEnableMovingTrack;
			CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData->GetTimelineInPoint(), m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			return FALSE;
		}
		else
		{
			// 重なりがない場合、その場所に配置する
			m_iOperatingClipFrameCount = m_pOperatingClipData->GetTimelineInPoint() * -1;
			m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
			m_pEnableMovingTrack = m_pOperateToTrack;
			CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			return TRUE;
		}
	}

	// 重なりチェック
	iMovingClipInFrame = m_pOperatingClipData->GetTimelineInPoint() + m_iOperatingClipFrameCount;
	iMovingClipOutFrame = m_pOperatingClipData->GetTimelineOutPoint() + m_iOperatingClipFrameCount;
	pClipData = m_pOperateToTrackInfo->CheckPlaceInRange(m_pOperatingClipData, iMovingClipInFrame, iMovingClipOutFrame);
	if ((pClipData != nullptr) && (pClipData != m_pOperatingClipData))
	{
		// 重なりがあった場合
		int iStaticClipCenterFrame = pClipData->GetTimelineInPoint() + static_cast<int>(floor(pClipData->GetDuration() / 2));
		int iDropInPoint = 0;
		// 重なっているクリップの左右に仮配置
		if (iMovingClipInFrame <= iStaticClipCenterFrame)
		{
			// 中心より左の場合、重なっているクリップのIn点-自身のDurationを配置点に仮置き
			iDropInPoint = pClipData->GetTimelineInPoint() - m_pOperatingClipData->GetDuration();
		}
		else
		{
			// 中心より右の場合、重なっているクリップのOut点を配置点に仮置き
			iDropInPoint = pClipData->GetTimelineOutPoint();
		}
		// 仮置きしたポイントへの移動量を算出
		iWorkOperatingClipFrameCount = iDropInPoint - m_pOperatingClipData->GetTimelineInPoint();
		// 移動の結果In点が0未満になる場合は移動しない
		if (m_pOperatingClipData->GetTimelineInPoint() + iWorkOperatingClipFrameCount > 0)
		{
			// ワークに操作中の矩形領域をコピー
			rcWorkRect.CopyRect(m_pOperatingClipData->GetOperatingRect());
			// 仮置きした位置に合わせてワークの矩形を移動する
			CalcClipRect(rcWorkRect, iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			// 仮置きした場所でクリップと衝突しないかを再判定
			iMovingClipInFrame = m_pOperatingClipData->GetTimelineInPoint() + iWorkOperatingClipFrameCount;
			iMovingClipOutFrame = m_pOperatingClipData->GetTimelineOutPoint() + iWorkOperatingClipFrameCount;
			pClipData = m_pOperateToTrackInfo->CheckPlaceInRange(m_pOperatingClipData, iMovingClipInFrame, iMovingClipOutFrame);
			if ((pClipData != nullptr) && (pClipData != m_pOperatingClipData))
			{
				// 重なりがあった場合
			}
			else
			{
				// 重なりがない場合、その場所に配置する
				m_iOperatingClipFrameCount = iDropInPoint - m_pOperatingClipData->GetTimelineInPoint();
				m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
				m_pEnableMovingTrack = m_pOperateToTrack;
				CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			}
		}
		// 移動先が配置できない場合、前回移動可能だった場所に戻す。
		m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
		m_pOperateToTrack = m_pEnableMovingTrack;
		CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData->GetTimelineInPoint(), m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
		return FALSE;
	}


	m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
	m_pEnableMovingTrack = m_pOperateToTrack;
	CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, m_pOperateToTrack, m_iOperatingClipFrameCount);
	return TRUE;
}



// タイムラインカーソルを所定の距離ずらす
void TimelineDataOperator::MoveTimelineCursor(int iLength)
{
	m_iTimelineCursorFramePosition += iLength;
	m_iLeftFrameNumber += iLength;
	m_iRightFrameNumber += iLength;
	m_iOperatingTimelineCursorFramePosition = m_iTimelineCursorFramePosition;
	m_iOperatingLeftFrameNumber = m_iLeftFrameNumber;
	m_iOperatingRightFrameNumber = m_iRightFrameNumber;
}

// 入力のポイントを中心とした位置にクリップの配置を変更する。
void TimelineDataOperator::SetRectAroundPoint(ClipDataRect& cClipRect, const POINT point, const int& iInputHeight)
{
	int iHeight = iInputHeight;
	if (iHeight == 0)
	{
		iHeight = cClipRect.Height();
	}
	CRect rcRect;
	rcRect.top = point.y - static_cast<int>(floor(iHeight / 2));
	rcRect.bottom = point.y + static_cast<int>(ceil(iHeight / 2));
	CalcClipRectDisplayPoint(static_cast<CRect&>(cClipRect), &cClipRect, rcRect);
	int iWidth = m_pDnDClipDataRect->Width();
	m_pDnDClipDataRect->left = point.x - static_cast<int>(floor(iWidth / 2));
	m_pDnDClipDataRect->right = point.x + static_cast<int>(ceil(iWidth / 2));
}


// シャトルスピードの算定
BOOL TimelineDataOperator::CalcShuttleSpeed(const CPoint& point, const CSize& szMoveSize, int& iNumerator, int& iDenominator)
{
	int iWidth = m_pTimelineDataRect->Width();
	int iMove = abs(szMoveSize.cx);
	int iSign = 1;
	if (szMoveSize.cx < 0)
	{
		iSign = -1;
	}

	if (iMove < iWidth *0.05)
	{
		return TRUE;
	}
	else if (iMove < iWidth * 0.2)
	{
		iNumerator = 1 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.3)
	{
		iNumerator = 2 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.35)
	{
		iNumerator = 3 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.4)
	{
		iNumerator = 4 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.42)
	{
		iNumerator = 5 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.44)
	{
		iNumerator = 6 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.45)
	{
		iNumerator = 7 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.46)
	{
		iNumerator = 8 * iSign;
		iDenominator = 1;
	}
	else if (iMove < iWidth * 0.47)
	{
		iNumerator = 9 * iSign;
		iDenominator = 1;
	}
	else
	{
		iNumerator = 10 * iSign;
		iDenominator = 1;
	}
	return TRUE;
}

// クリップを削除する
BOOL TimelineDataOperator::DeleteClip(void)
{
	ASSERT(m_pOperatingClipData);
	ASSERT(m_pSelectedTrackInfo);
	if ((m_pOperatingClipData != nullptr) && (m_pSelectedTrackInfo != nullptr))
	{
		// トランジション情報を削除してペアのクリップを調整する
		if (m_pOperatingClipData->GetInTransition())
		{
			// トランジションのペアを解除
			ClipDataInfo* pClipInfo = m_pOperatingClipData->GetInTransitionClipInfo();
			ASSERT(pClipInfo);
			if (!pClipInfo)
			{
				return FALSE;
			}
			TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
			int iLength = pTransitionData->GetTransitionDuration();
			m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetOutTransitionUUID()));
			pClipInfo->ResetTransition(FALSE, iLength);
			m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetInTransitionUUID()));
		}
		if (m_pOperatingClipData->GetOutTransition())
		{
			// トランジションのペアを解除
			ClipDataInfo* pClipInfo = m_pOperatingClipData->GetOutTransitionClipInfo();
			ASSERT(pClipInfo);
			if (!pClipInfo)
			{
				return FALSE;
			}
			TransitionData* pTransitionData = m_pTransitionManager->GetTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
			int iLength = pTransitionData->GetTransitionDuration();
			m_pTransitionManager->DeleteTransitionData(static_cast<CString>(pClipInfo->GetInTransitionUUID()));
			m_pSelectedTrackInfo->DeleteClip(pClipInfo->GetTimelineInPoint());
			pClipInfo->ResetTransition(TRUE, iLength);
			m_pTransitionManager->DeleteTransitionData(static_cast<CString>(m_pOperatingClipData->GetOutTransitionUUID()));
			m_pSelectedTrackInfo->AddClip(pClipInfo->GetTimelineInPoint(), pClipInfo->GetClipDataRect(), pClipInfo);
		}
		m_pSelectedTrackInfo->DeleteClip(m_pOperatingClipData->GetTimelineInPoint());
		m_pSelectedTrack = nullptr;
		m_pSelectedTrackInfo = nullptr;
		m_pOperatingClipData = nullptr;
		return TRUE;
	}
	return FALSE;
}

// 入力の文字列と先頭一致するメニュー上の位置を取得する
CMenu* TimelineDataOperator::FindSubMenuFromString(CMenu* pMenu, LPCTSTR pszSearchString, int& iPos)
{
	iPos = -1;
	if (!pMenu)
	{
		return nullptr;
	}

	CMenu* pRetMenu = nullptr;
	CString strSearch = static_cast<CString>(pszSearchString);

	int iCnt = pMenu->GetMenuItemCount();
	CString	strMenu;
	int i = 0;
	while ((i < iCnt) && (pRetMenu == nullptr))
	{
		pMenu->GetMenuString(i, strMenu, MF_BYPOSITION);

		if (strSearch.CompareNoCase(strMenu.Left(strSearch.GetLength())) == 0)
		{
			pRetMenu = pMenu->GetSubMenu(i);
			iPos = i;
		}
		++i;
	}
	return pRetMenu;
}