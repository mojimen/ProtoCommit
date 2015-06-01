// TimelineDataManager.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineDataManager.h"

#include "TrackDataManager.h"
#include "ClipDataManager.h"


// TimelineDataManager

TimelineDataManager::TimelineDataManager()
{
	m_eTimelineDataManagerTag = TIMELINEDATAMANAGER;
}

TimelineDataManager::~TimelineDataManager()
{
}


// TimelineDataManager メンバー関数

// UUID設定
BOOL TimelineDataManager::InitializeTimelineDataManager(UUID& uiTimelineDataManagerId)
{
	if (RPC_S_OK == UuidCreate(&uiTimelineDataManagerId))
	{
		uiTimelineDataManagerId = m_uiTimelineDataManagerId;
	}
	else
	{
		return FALSE;
	}

	if (m_pTrackDataVideoManager)
	{
		DeleteObject(m_pTrackDataVideoManager);
	}
	m_pTrackDataVideoManager = new TrackDataManager(TRACKDATAMANAGER_VIDEO);
	m_pTrackDataVideoManager->InitializeTrackDataManagerId(m_uiTrackDataVideoManagerId);

	if (m_pTrackDataAudioManager)
	{
		DeleteObject(m_pTrackDataAudioManager);
	}
	m_pTrackDataAudioManager = new TrackDataManager(TRACKDATAMANAGER_AUDIO);
	m_pTrackDataAudioManager->InitializeTrackDataManagerId(m_uiTrackDataAudioManagerId);

	if (m_pClipDataManager)
	{
		DeleteObject(m_pClipDataManager);
	}
	m_pClipDataManager = new ClipDataManager();
	m_pClipDataManager->InitializeClipDataManagerId(m_uiClipDataManagerId);
}

