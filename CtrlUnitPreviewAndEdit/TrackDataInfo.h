#pragma once
#include "TrackDataManager.h"

// TrackDataInfo コマンド ターゲット

class TrackDataInfo : public CObject
{
public:
	TrackDataInfo();
	virtual ~TrackDataInfo();

private:
	TrackDataTag m_eTrackDataInfoTag;
	UUID m_uiTrackId;
	InfoKind m_eTrackKind;
	UUID m_uiInputChannel;
	UUID m_uiOutputChannel;
	ClipDataPositionMap m_mpClipDataRectMap;

	// 再生時参照項目
	BOOL m_fSolo;
	BOOL m_fAudioRecordable;
	BOOL m_fMuted;
	BOOL m_fLevelPointRecordable;
	
	// トラック表示データとのリンク項目
	UUID m_uiTrackRectId;
	TrackDataRect* m_pTrackDataRect;

public:
	BOOL InitTrackData(void);
	BOOL InitializeTrackId(UUID& uiClipId);


	void AddClip(const int iInPoint, ClipDataRect* pClipData);
	void DeleteClip(const int iInPoint);
	void ChangeClip(const int iOldInPoint, const int iNewInPoint, ClipDataRect* pClipData);
	void SetTrackDataRect(const UUID& uiTrackRectId, TrackDataRect* pTrackDataRect) { m_pTrackDataRect = pTrackDataRect; m_uiTrackRectId = uiTrackRectId; }
	void DeleteTrackDataInfo(void);

	// Getter
	ClipDataRect* GetClipDataInfo(const int iFrame, int& iInPoint);
	int GetClipDataAtFrame(const int iFrame, ClipDataPositionMap& mpClipMap);
	int GetClipDataInRange(const int iStartFrame, const int iEndFrame, ClipDataPositionMap& mpClipMap);
	UUID GetTrackRectId(void) { return m_uiTrackRectId; }
	TrackDataRect* GetTrackDataRect(void) { return m_pTrackDataRect; }

	// Setter
	//void SetTrackRectId(UUID uiTrackRectId) { m_uiTrackRectId = uiTrackRectId; }
	//void SetTrackDataRect(TrackDataRect* pTrackDataRect) { m_pTrackDataRect = pTrackDataRect; }



	// TODO: これはコントローラーに？
	int CheckClipInSingleOutTrimRange(int iStartFrame, int iEndFrame);
	int CheckClipInSingleInTrimRange(int iStartFrame, int iEndFrame);
	ClipDataRect* CheckMove(ClipDataRect* pCheckClipData, const int iInPoint, const int iOutPoint);

};


