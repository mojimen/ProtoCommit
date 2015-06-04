#pragma once

// DragAndDropOperator コマンド ターゲット

class OpenGLRect;
class TimelineDataOperator;
class TimelineDataManager;
class TrackDataManager;
class TrackDataRect;
class TrackDataInfo;
class ClipDataManager;
class ClipDataRect;

class DragAndDropOperator : public CObject
{
public:
	DragAndDropOperator();
	DragAndDropOperator(TimelineDataOperator* pTimelineDataOperator){ m_pTimelineDataOperator = pTimelineDataOperator; }
	virtual ~DragAndDropOperator();

	OpenGLRect* m_pTimelineEditPanelRect;
	OpenGLRect* m_pTimelineEditHeaderRect;
	OpenGLRect* m_pTimelineControlPanelRect;
	OpenGLRect* m_pSeekBarRect;
	OpenGLRect* m_pTrackHeaderRect;
	OpenGLRect* m_pTimelineDataRect;
	OpenGLRect* m_pTimelineCursorHitArea;
	//TODO: これはいらないかも？
	OpenGLRect* m_pTransisionRect;

	// タイムラインデータ管理
	TimelineDataOperator* m_pTimelineDataOperator;
	UUID m_uiTimelineDataOperatorId;
	TimelineDataManager* m_pTimelineDataManager;
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
	UUID m_uiClipDataManagerId;

	// ドラッグ＆ドロップ操作
	TrackDataRect* GetDropTrack(const POINT& point);
	BOOL Initialize(UUID& uiDragAndDropOperatorId);
	BOOL CheckFileNameExtension(const CString& strFileName);
	BOOL CheckDropFile(PCTSTR pszFileName, CString& strClipFileName, UINT& uIn, UINT& uOut);
	int CreateClipData(ClipDataRect& pClipDataRect, PCTSTR pszClipFileName, const UINT& uIn, const UINT& uOut);
	BOOL CreateClipDataFromDropFile(TrackDataInfo& pTrackDataInfo, const UINT& uFrame, PCTSTR pszClipFileName, const UINT& uIn, const UINT& uOut);
	BOOL SetClipDataInOutPoint(ClipDataRect& pClipDataRect, const UINT& uFrame, const UINT& uDuration);

private:
	UUID m_uiDragAndDropOperatorId;

};


