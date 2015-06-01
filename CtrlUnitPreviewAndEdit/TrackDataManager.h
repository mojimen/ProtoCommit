#pragma once
#include<map>
#include<vector>

class TrackDataInfo;
class TrackDataRect;

typedef std::map<CString, TrackDataInfo*> TrackDataInfoMap;
typedef std::map<CString, TrackDataRect*> TrackDataRectMap;
//typedef std::vector<TrackDataInfo*> TrackDataInfoList;
//typedef std::vector<TrackDataRect*> TrackDataRectList;
typedef std::vector<UUID> TrackDataInfoList;
typedef std::vector<UUID> TrackDataRectList;

typedef enum TrackDataTag{
	TRACKDATAMANAGER_VIDEO,
	TRACKDATAMANAGER_AUDIO,
	TRACKDATAMANAGER_INFO,
	TRACKDATAMANAGER_MASTER,
	TRACKDATAINFO,
	TRACKDATARECT
};

typedef enum TrackKind {
	TRACK_VIDEO,
	TRACK_AUDIO,
	TRACK_TITLE,
	TRACK_INFO,
	TRACK_MASTER_VIDEO,
	TRACK_MASTER_AUDIO
};



// 固定値
#define TRACK_HIEGHT_DEFAULT 50

// 共通カラー
#define BLACKCOLOR_BRUSH_FLOAT 0.0f, 0.0f, 0.0f, 1.0f
#define WHITECOLOR_BRUSH_FLOAT 1.0f, 1.0f, 1.0f, 1.0f
#define REDCOLOR_BRUSH_FLOAT 1.0f, 0.0f, 0.0f, 1.0f
#define BLUECOLOR_BRUSH_FLOAT 0.0f, 0.0f, 1.0f, 1.0f
#define GREENCOLOR_BRUSH_FLOAT 0.0f, 1.0f, 0.0f, 1.0f
#define DARKGRAYCOLOR_BRUSH_FLOAT 0.3f, 0.3f, 0.3f, 1.0f
#define DARKGRAYCOLOR2_BRUSH_FLOAT 0.2f, 0.2f, 0.2f, 1.0f
#define DARKGRAYCOLOR3_BRUSH_FLOAT 0.1f, 0.1f, 0.1f, 1.0f
#define GRAYCOLOR_BRUSH_FLOAT 0.5f, 0.5f, 0.5f, 1.0f
#define LIGHTGRAYCOLOR_BRUSH_FLOAT 0.7f, 0.7f, 0.7f, 1.0f
#define LIGHTGRAYCOLOR2_BRUSH_FLOAT 0.8f, 0.8f, 0.8f, 1.0f
#define LIGHTGRAYCOLOR3_BRUSH_FLOAT 0.9f, 0.9f, 0.9f, 1.0f

#define ACCENTCOLOR_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 1.0f
#define ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 0.5f
#define ACCENTCOLOR2_BRUSH_FLOAT 0.3f,0.4f,1.0f,1.0f
#define ACCENTCOLOR2_HALFALPHA_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.5f
#define ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.7f
#define ACCENTCOLOR3_BRUSH_FLOAT 0.8f,0.9f,1.0f,1.0f
//#define ACCENTCOLOR_BRUSH4 RGB(51,255,255)

#define BASECOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define CAUTIONCOLOR_BRUSH_FLOAT REDCOLOR_BRUSH_FLOAT

// 固有カラー
#define TIMELINETRACKCOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH2_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH2_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT





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
	TrackDataInfoMap m_TrackDataInfoVideoMap;
	TrackDataRectMap m_TrackDataRectVideoMap;
	TrackDataInfoList m_TrackDataInfoVideoList;
	TrackDataRectList m_TrackDataRectVideoList;

	int m_iTimelineLastFrame;

	RPC_WSTR m_rwsId;				//UUID変換用項目


public:
	BOOL InitializeTrackDataManagerId(UUID& uiTrackDataManagerId);
	BOOL CreateTrackData(const int iLayerNumber, UUID& uiTrackId, UUID& uiTrackrectId);
	BOOL ChangeUUIDToCString(const UUID& uiId, PCTSTR& pszId);
	int GetTrackCount(void);
	BOOL SetTrackData(const int iLayerNumber, const UUID& uiTrackId, TrackDataInfo* pTrackDataInfo, const UUID& uiTrackRectId, TrackDataRect* pTrackDataRect);

	// Setter

	// Getter
	TrackDataInfo* GetTrackDataInfo(const UUID& uiTrackDataId);
	TrackDataRect* GetTrackDataRect(const UUID& uiTrackDataRectId);
	// リスト種別の引渡しが必要
	TrackDataRectList* GetTrackDataRectList(void){ return &m_TrackDataRectVideoList; }

};


