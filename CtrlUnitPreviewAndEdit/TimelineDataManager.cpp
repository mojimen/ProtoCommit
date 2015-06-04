// TimelineDataManager.cpp : �����t�@�C��
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
	m_pTrackDataVideoManager = nullptr;
	m_pTrackDataAudioManager = nullptr;
	m_pTrackDataInfoManager = nullptr;
	m_pTrackDataMasterManager = nullptr;
	m_pClipDataManager = nullptr;
}

TimelineDataManager::~TimelineDataManager()
{
}


// TimelineDataManager �����o�[�֐�

// �����ݒ�
BOOL TimelineDataManager::InitializeTimelineDataManager(UUID& uiTimelineDataManagerId)
{
	if (RPC_S_OK == UuidCreate(&m_uiTimelineDataManagerId))
	{
		uiTimelineDataManagerId = m_uiTimelineDataManagerId;
	}
	else
	{
		return FALSE;
	}

	if (m_pTrackDataVideoManager)
	{
		delete m_pTrackDataVideoManager;
	}
	m_pTrackDataVideoManager = new TrackDataManager(TRACKDATAMANAGER_VIDEO);
	m_pTrackDataVideoManager->InitializeTrackDataManagerId(m_uiTrackDataVideoManagerId);

	if (m_pTrackDataAudioManager)
	{
		delete m_pTrackDataAudioManager;
	}
	m_pTrackDataAudioManager = new TrackDataManager(TRACKDATAMANAGER_AUDIO);
	m_pTrackDataAudioManager->InitializeTrackDataManagerId(m_uiTrackDataAudioManagerId);

	if (m_pClipDataManager)
	{
		delete m_pClipDataManager;
	}
	m_pClipDataManager = new ClipDataManager();
	m_pClipDataManager->InitializeClipDataManagerId(m_uiClipDataManagerUUID);

	return TRUE;
}

// �^�C�����C���f�[�^���폜����
void TimelineDataManager::DeleteTimelineDataManager(void)
{
	if (m_pTrackDataVideoManager)
	{
		m_pTrackDataVideoManager->DeleteTrackDataManager();
		delete m_pTrackDataVideoManager;
		UuidCreateNil(&m_uiTrackDataVideoManagerId);
		m_pTrackDataVideoManager = nullptr;
	}
	if (m_pTrackDataAudioManager)
	{
		m_pTrackDataAudioManager->DeleteTrackDataManager();
		delete m_pTrackDataAudioManager;
		UuidCreateNil(&m_uiTrackDataAudioManagerId);
		m_pTrackDataAudioManager = nullptr;
	}
	if (m_pTrackDataInfoManager)
	{
		m_pTrackDataInfoManager->DeleteTrackDataManager();
		delete m_pTrackDataInfoManager;
		UuidCreateNil(&m_uiTrackDataInfoManagerId);
		m_pTrackDataInfoManager = nullptr;
	}
	if (m_pTrackDataMasterManager)
	{
		m_pTrackDataMasterManager->DeleteTrackDataManager();
		delete m_pTrackDataMasterManager;
		UuidCreateNil(&m_uiTrackDataMasterManagerId);
		m_pTrackDataMasterManager = nullptr;
	}
	if (m_pClipDataManager)
	{
		m_pClipDataManager->DeleteClipDataManager();
		delete m_pClipDataManager;
		UuidCreateNil(&m_uiClipDataManagerUUID);
		m_pClipDataManager = nullptr;
	}
}

// �g���b�N�f�[�^�}�l�[�W���[���擾����
TrackDataManager* TimelineDataManager::GetTrackDataManager(const TrackDataTag eTrackDataTag, UUID &uiTrackDataManagerId)
{
	switch (eTrackDataTag)
	{
		case TRACKDATAMANAGER_VIDEO:
			uiTrackDataManagerId = m_uiTrackDataVideoManagerId;
			return m_pTrackDataVideoManager;
		case TRACKDATAMANAGER_AUDIO:
			uiTrackDataManagerId = m_uiTrackDataAudioManagerId;
			return m_pTrackDataAudioManager;
		case TRACKDATAMANAGER_INFO:
			uiTrackDataManagerId = m_uiTrackDataInfoManagerId;
			return m_pTrackDataInfoManager;
		case TRACKDATAMANAGER_MASTER:
			uiTrackDataManagerId = m_uiTrackDataMasterManagerId;
			return m_pTrackDataMasterManager;
		default:
			return nullptr;
	}

}

// �N���b�v�f�[�^�}�l�[�W���[���擾����
ClipDataManager* TimelineDataManager::GetClipDataManager(UUID &uiClipDataManagerUUID)
{
	uiClipDataManagerUUID = m_uiClipDataManagerUUID;
	return m_pClipDataManager;
}

