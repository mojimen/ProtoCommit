#pragma once
#include<list>
#include<map>
#include<vector>

#include <rpc.h>
#pragma comment(lib ,"rpcrt4.lib")

#include "ConstantDefine.h"

class TrackDataManager;
class TrackDataInfo;
class TrackDataRect;
class ClipDataManager;
class ClipDataInfo;
class ClipDataRect;

typedef std::map<CString, TrackDataInfo*> TrackDataInfoMap;
typedef std::map<CString, TrackDataRect*> TrackDataRectMap;
//typedef std::vector<TrackDataInfo*> TrackDataInfoList;
//typedef std::vector<TrackDataRect*> TrackDataRectList;
typedef std::vector<UUID> TrackDataInfoList;
typedef std::vector<UUID> TrackDataRectList;

typedef std::map<int, ClipDataRect*> ClipDataPositionMap;	//InPoint,ClipDataPointer 
//typedef std::map<int, UUID> ClipDataPositionMap;	//InPoint,ClipId 

typedef std::map<CString, ClipDataInfo*> ClipDataInfoMap;
typedef std::map<CString, ClipDataRect*> ClipDataRectMap;

typedef enum TimelineDataTag{
	TIMELINEDATAMANAGER,
	TIMELINEDATAINFO
};

typedef enum TrackDataTag{
	TRACKDATAMANAGER_VIDEO,
	TRACKDATAMANAGER_AUDIO,
	TRACKDATAMANAGER_INFO,
	TRACKDATAMANAGER_MASTER,
	TRACKDATAINFO,
	TRACKDATARECT
};

typedef enum ClipDataTag{
	CLIPDATAMANAGER,
	CLIPDATAINFO,
	CLIPDATARECT
};

typedef enum InfoKind {
	VIDEO,
	AUDIO,
	TITLE,
	INFO,
	MASTER_VIDEO,
	MASTER_AUDIO
};

struct FilterInfo
{
	int iOutPoint;
	int iDuration;
	UUID uiFilterId;
};


// TimelineDataManager コマンド ターゲット

class TimelineDataManager : public CObject
{
public:
	TimelineDataManager();
	virtual ~TimelineDataManager();




	TimelineDataTag m_eTimelineDataManagerTag;
	UUID m_uiTimelineDataManagerId;

	TrackDataManager* m_pTrackDataVideoManager;
	UUID m_uiTrackDataVideoManagerId;

	TrackDataManager* m_pTrackDataAudioManager;
	UUID m_uiTrackDataAudioManagerId;

	TrackDataManager* m_pTrackDataInfoManager;
	UUID m_uiTrackDataInfoManagerId;

	TrackDataManager* m_pTrackDataMasterManager;
	UUID m_uiTrackDataMasterManagerId;

	ClipDataManager* m_pClipDataManager;
	UUID m_uiClipDataManagerUUID;

	BOOL InitializeTimelineDataManager(UUID& uiTrackDataManagerId);
	void DeleteTimelineDataManager(void);
	TrackDataManager* GetTrackDataManager(const TrackDataTag eTrackDataTag, UUID &TrackDataManagerId);
	ClipDataManager* GetClipDataManager(UUID &ClipDataManagerId);


};


