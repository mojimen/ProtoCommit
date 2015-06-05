// ClipDataInfo.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "ClipDataInfo.h"

#include "ClipDataRect.h"


// ClipDataInfo

ClipDataInfo::ClipDataInfo()
{
	m_eClipKind = VIDEO;
	m_iInPoint = 0;
	m_iOutPoint = 0;
	m_iDuration = 0;
	m_iInOffset = 0;
	m_fInTransition = FALSE;
	m_pInTransitionClipInfo = nullptr;
	m_fOutTransition = FALSE;
	m_pOutTransitionClipInfo = nullptr;
	m_pClipDataRect = nullptr;
}

ClipDataInfo::ClipDataInfo(InfoKind eKind)
{
	ClipDataInfo();
	m_eClipKind = eKind;
}

ClipDataInfo::~ClipDataInfo()
{
}


// ClipDataInfo メンバー関数

// 初期設定
BOOL ClipDataInfo::InitializeClipDataInfo(UUID& uiClipUUID)
{
	if (InitializeData(A_CLIPDATAINFO))
	{
		uiClipUUID = m_uiUUID;
		return TRUE;
	}
	return FALSE;
}

// クリップ表示データ削除
BOOL ClipDataInfo::DeleteClipData(void)
{
	if (m_pClipDataRect)
	{
		delete m_pClipDataRect;
		return TRUE;
	}
	return FALSE;
}

void ClipDataInfo::ResetTransition(BOOL fIn, int iLength)
{
	if (fIn)
	{
		m_fInTransition = FALSE;
		m_strInTransitionUUID.Empty();
		m_pInTransitionClipInfo = nullptr;
		m_iInPoint += iLength;
		m_iTimelineInPoint += iLength;
		m_iDuration -= iLength;
	}
	else
	{
		m_fOutTransition = FALSE;
		m_strOutTransitionUUID.Empty();
		m_pOutTransitionClipInfo = nullptr;
		m_iOutPoint -= iLength;
		m_iTimelineOutPoint -= iLength;
		m_iDuration -= iLength;
	}
}