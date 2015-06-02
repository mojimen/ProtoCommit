#pragma once
#include "TimelineDataManager.h"

// TrackDataManager コマンド ターゲット

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

	RPC_WSTR m_rwsId;				//UUID変換用項目


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
	// リスト種別の引渡しが必要
	TrackDataRectList* GetTrackDataRectList(void){ return &m_TrackDataRectList; }

};


