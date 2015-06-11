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
	ClipDataPositionInfoMap m_mpClipDataInfoMap;

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


	BOOL AddClip(const int iInPoint, ClipDataRect* pClipRect = nullptr, ClipDataInfo* pClipInfo = nullptr);
	void DeleteClip(const int iInPoint, BOOL fRect = TRUE );
	void ChangeClip(const int iOldInPoint, const int iNewInPoint, ClipDataRect* pClipData = nullptr, ClipDataInfo* pClipInfo = nullptr, BOOL fRect = TRUE);
	void SetTrackDataRect(const UUID& uiTrackRectId, TrackDataRect* pTrackDataRect) { m_pTrackDataRect = pTrackDataRect; m_uiTrackRectId = uiTrackRectId; }
	void DeleteTrackDataInfo(void);

	// Getter
	ClipDataRect* GetClipDataRect(const int iFrame, int& iInPoint);
	int GetClipDataAtFrame(const int iFrame, ClipDataPositionMap& mpClipMap);
	int GetClipDataInfoAtFrame(const int iFrame, ClipDataPositionInfoMap& mpClipMap);
	int GetClipDataInRange(const int iStartFrame, const int iEndFrame, ClipDataPositionMap& mpClipMap);
	UUID GetTrackRectId(void) { return m_uiTrackRectId; }
	TrackDataRect* GetTrackDataRect(void) { return m_pTrackDataRect; }

	// Setter

	// TODO: これはコントローラーに？
	int CheckClipInSingleOutTrimRange(int iStartFrame, int iEndFrame);
	int CheckClipInSingleInTrimRange(int iStartFrame, int iEndFrame);
	ClipDataRect* CheckPlaceInRange(ClipDataRect* pCheckClipData, const int iInPoint, const int iOutPoint);

};


