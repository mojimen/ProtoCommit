// SingleTrimOperator.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "SingleTrimOperator.h"

#include "ClipDataRect.h"
#include "TrackDataInfo.h"


// SingleTrimOperator

SingleTrimOperator::SingleTrimOperator()
{
}

SingleTrimOperator::~SingleTrimOperator()
{
}


// SingleTrimOperator メンバー関数

// 操作がIn側トリム可能な範囲内かを判定して位置を調整する
BOOL SingleTrimOperator::CheckInTrim(void)
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
	m_iOperatingClipFrameCount = m_clOperateToTrackInfo->CheckClipInSingleInTrimRange(m_clMovingClipData->GetTimelineInPoint(), 
																						m_clMovingClipData->GetTimelineInPoint() + m_iOperatingClipFrameCount)
		- m_clMovingClipData->GetTimelineInPoint();
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	// 範囲チェック（重なりチェックより後にやらないと0を超えてドラッグされた場合に0に設定されてしまう）
	if (m_clMovingClipData->GetTimelineInPoint() + m_iOperatingClipFrameCount < 0)
	{
		m_iOperatingClipFrameCount = m_clMovingClipData->GetTimelineInPoint() * -1;
		return FALSE;
	}

	return TRUE;
}

// 操作がOut側トリム可能な範囲内かを判定して位置を調整する
BOOL SingleTrimOperator::CheckOutTrim(void)
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
	int iStartFrame = m_clMovingClipData->GetTimelineOutPoint();
	m_iOperatingClipFrameCount = m_clOperateToTrackInfo->CheckClipInSingleOutTrimRange(iStartFrame, iStartFrame + m_iOperatingClipFrameCount) - iStartFrame;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	return TRUE;
}
