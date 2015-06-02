#pragma once
#include "TimelineDataManager.h"

// TrackDataManager �R�}���h �^�[�Q�b�g

class TrackDataManager : public CObject
{
public:
	TrackDataManager();
	TrackDataManager(TrackDataTag eTrackDataTag);
	virtual ~TrackDataManager();

private:

	TrackDataTag m_eTrackDataManagerTag;
	UUID m_uiTrackDataManagerId;
	int m_iSequentialNumber;
	TrackDataInfoMap m_TrackDataInfoMap;
	TrackDataRectMap m_TrackDataRectMap;
	TrackDataInfoList m_TrackDataInfoList;
	TrackDataRectList m_TrackDataRectList;

	int m_iTimelineLastFrame;

	RPC_WSTR m_rwsId;				//UUID�ϊ��p����


public:
	BOOL InitializeTrackDataManagerId(UUID& uiTrackDataManagerId);
	BOOL CreateTrackData(const UINT iLayerNumber, UUID& uiTrackId, UUID& uiTrackRectId);
	BOOL ChangeUUIDToCString(const UUID& uiId, PCTSTR& pszId);
	int GetTrackCount(void);
	BOOL SetTrackData(const UINT iLayerNumber, const UUID& uiTrackId, TrackDataInfo* pTrackDataInfo, const UUID& uiTrackRectId, TrackDataRect* pTrackDataRect);
	void DeleteTrackDataManager(void);
	BOOL DeleteTrackData(const UINT iLayerNumber);
	TrackDataInfo* GetTrackDataInfo(const UUID& uiTrackDataId);
	TrackDataRect* GetTrackDataRect(const UUID& uiTrackDataRectId);
	TrackDataInfo* GetTrackDataInfo(const UINT iLayerNumber);
	TrackDataRect* GetTrackDataRect(const UINT iLayerNumber);
	UUID GetTrackDataInfoId(const UINT iLayerNumber);
	UUID GetTrackDataRectId(const UINT iLayerNumber);

	// Setter

	// Getter
	// ���X�g��ʂ̈��n�����K�v
	TrackDataRectList* GetTrackDataRectList(void){ return &m_TrackDataRectList; }

};


