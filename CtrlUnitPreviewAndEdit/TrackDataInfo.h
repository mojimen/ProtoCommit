#pragma once

#include <list>
#include <map>
#include <rpc.h>
#pragma comment(lib ,"rpcrt4.lib")

class ClipDataRect;

// TrackDataInfo コマンド ターゲット

typedef std::map<int, ClipDataRect*> ClipDataInfoMap;	//InPoint,ClipId 
//typedef std::map<int, UUID> ClipDataInfoMap;	//InPoint,ClipId 

class TrackDataInfo : public CObject
{
public:
	TrackDataInfo();
	virtual ~TrackDataInfo();

	// TODO: これはトラックマネージャーに！
	enum TrackDataTag{ TRACKDATATAG_INFO };
	enum TrackKind {
		VIDEO,
		AUDIO,
		TITLE,
		INFO,
		MASTER_VIDEO,
		MASTER_AUDIO
	};

private:
	TrackDataTag m_eTrackDataInfoTag;
	UUID m_uiTrackId;
	TrackKind m_eTrackKind;
	UUID m_uiInputChannel;
	UUID m_uiOutputChannel;
	ClipDataInfoMap m_mpClipDataInfoMap;

	// 再生時参照項目
	BOOL m_fSolo;
	BOOL m_fAudioRecordable;
	BOOL m_fMuted;
	BOOL m_fLevelPointRecordable;
	
	// ビューとのリンク項目
	//UUID m_uiTrackRectId;
	//TrackDataRect* m_pTrackDataRect;

public:
	BOOL InitTrackData(void);
	BOOL InitializeTrackId(UUID& uiClipId);


	void AddClip(const int iInPoint, ClipDataRect* pClipData);
	void DeleteClip(const int iInPoint);
	void ChangeClip(const int iOldInPoint, const int iNewInPoint, ClipDataRect* pClipData);

	// Getter
	ClipDataRect* GetClipDataInfo(int iFrame, int& iInPoint);
	int GetClipDataAtFrame(int iFrame, ClipDataInfoMap& mpClipMap);
	int GetClipDataInRange(int iStartFrame, int iEndFrame, ClipDataInfoMap& mpClipMap);
	//UUID GetTrackRectId(void) { return m_uiTrackRectId; }
	//TrackDataRect* GetTrackDataRect(void) { return m_pTrackDataRect; }

	// Setter
	//void SetTrackRectId(UUID uiTrackRectId) { m_uiTrackRectId = uiTrackRectId; }
	//void SetTrackDataRect(TrackDataRect* pTrackDataRect) { m_pTrackDataRect = pTrackDataRect; }



	// TODO: これはコントローラーに？
	int CheckClipInSingleOutTrimRange(int iStartFrame, int iEndFrame);
	int CheckClipInSingleInTrimRange(int iStartFrame, int iEndFrame);
	ClipDataRect* CheckMove(ClipDataRect* pCheckClipData, const int iInPoint, const int iOutPoint);

};


