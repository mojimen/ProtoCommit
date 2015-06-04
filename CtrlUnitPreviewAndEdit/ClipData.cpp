// ClipData.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "ClipData.h"


// ClipData

ClipData::ClipData()
{
	m_pClipDataInfo = nullptr;
	m_pClipDataRect = nullptr;
	m_pTrackDataInfo = nullptr;
	m_pTrackDataRect = nullptr;
}

ClipData::~ClipData()
{
}


// ClipData メンバー関数

BOOL ClipData::SetClipData(ClipDataManager* pClipManager, ClipDataInfo* pClipInfo, ClipDataRect* pClipRect, TrackDataInfo* pTrackInfo, TrackDataRect* pTrackRect, UUID& uiId)
{
	m_pClipDataManager = pClipManager;
	m_uiClipDataManagerId = m_pClipDataManager->GetClipDataManagerId();
	m_pClipDataInfo = pClipInfo;
	m_uiClipDataInfoId = GetClipInfoId();
	m_pClipDataRect = pClipRect;
	m_uiClipDataRectId = GetClipRectId();
	m_pTrackDataInfo = pTrackInfo;
	m_uiTrackDataInfoId = GetTrackDataInfoId();
	m_pTrackDataRect = pTrackRect;
	m_uiTrackDataRectId = GetTrackDataRectId();

	if (RPC_S_OK == UuidCreate(&m_uiClipDataId))
	{
		uiId = m_uiClipDataManagerId;
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

