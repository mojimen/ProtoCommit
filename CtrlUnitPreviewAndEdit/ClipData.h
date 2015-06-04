#pragma once

#include "ClipDataRect.h"
#include "ClipDataInfo.h"


// ClipData コマンド ターゲット

class TrackDataInfo;
class TrackDataRect;

class ClipData : public CObject
{
public:
	ClipData();
	virtual ~ClipData();

private:
	ClipDataTag m_eClipDataTag;
	UUID m_uiClipDataId;

	ClipDataManager* m_pClipDataManager;
	UUID m_uiClipDataManagerId;

	ClipDataInfo* m_pClipDataInfo;
	UUID m_uiClipDataInfoId;
	ClipDataRect* m_pClipDataRect;
	UUID m_uiClipDataRectId;
	TrackDataInfo* m_pTrackDataInfo;
	UUID m_uiTrackDataInfoId;
	TrackDataRect* m_pTrackDataRect;
	UUID m_uiTrackDataRectId;

public:
	BOOL SetClipData(ClipDataManager* pClipManager, ClipDataInfo* pClipInfo, ClipDataRect* pClipRect, TrackDataInfo* pTrackInfo, TrackDataRect* pTrackRect, UUID& uiId);

	// Getter
	ClipDataInfo* GetClipDataInfo(void){ return m_pClipDataInfo; }
	UUID GetClipDataInfoId(void){ return m_uiClipDataInfoId; }
	ClipDataRect* GetClipDataRect(void){ return m_pClipDataRect; }
	UUID GetClipDataRectId(void){ return m_uiClipDataRectId; }
	TrackDataInfo* GetTrackDataInfo(void){ return m_pTrackDataInfo; }
	UUID GetTrackDataInfoId(void){ return m_uiTrackDataInfoId; }
	TrackDataRect* GetTrackDataRect(void){ return m_pTrackDataRect; }
	UUID GetTrackDataRectId(void){ return m_uiTrackDataRectId; }

	// ClipDataRectGetter
	ClipDataTag GetClipRectTag(void) { return m_pClipDataRect->GetTag(); }
	UUID GetClipRectId(void) { return m_pClipDataRect->GetClipRectId(); }
	//InfoKind GetClipKind(void) { return m_pClipDataRect->GetClipKind(); }
	//int GetDuration(void) { return m_pClipDataRect->GetDuration(); }
	int GetTimelineInPoint(void) { return m_pClipDataRect->GetTimelineInPoint(); }
	int GetTimelineOutPoint(void) { return m_pClipDataRect->GetTimelineOutPoint(); }

	CRect* GetOperatingRect(void) { return m_pClipDataRect->GetOperatingRect(); }
	float(*GetOverlappingVert(void))[3] { return m_pClipDataRect->GetOverlappingVert(); }
	void GetOverlappingVert(float(&fVert)[4][3]) { return m_pClipDataRect->GetOverlappingVert(fVert); }
	void GetMovingColor(float(&fColor)[4][4]) { return m_pClipDataRect->GetMovingColor(fColor); };
	void GetSingleInTrimColor(float(&fColor)[4][4]) { return m_pClipDataRect->GetSingleInTrimColor(fColor); };
	void GetSingleOutTrimColor(float(&fColor)[4][4]) { return m_pClipDataRect->GetSingleOutTrimColor(fColor); };
	void GetOperatingOldColor(float(&fColor)[4][4]) { return m_pClipDataRect->GetOperatingOldColor(fColor); };
	void GetOverlappingColor(float(&fColor)[4][4]) { return m_pClipDataRect->GetOverlappingColor(fColor); };
	void GetSingleTrimBorderColor(float(&fColor)[4][4]) { return m_pClipDataRect->GetSingleTrimBorderColor(fColor); };

	// ClipDataInfo Getter
	ClipDataTag GetClipInfoTag(void) { return m_pClipDataInfo->GetTag(); }
	UUID GetClipInfoId(void) { return m_pClipDataInfo->GetClipId(); }
	InfoKind GetClipKind(void) { return m_pClipDataInfo->GetClipKind(); }
	UUID GetFileId(void) { return m_pClipDataInfo->GetFileId(); }
	int GetInPoint(void) { return m_pClipDataInfo->GetInPoint(); }
	int GetOutPoint(void) { return m_pClipDataInfo->GetOutPoint(); }
	int GetDuration(void) { return m_pClipDataInfo->GetDuration(); }
	int GetInOffset(void) { return m_pClipDataInfo->GetInOffset(); }
	// どっちに持たせよう？
	//int GetTimelineInPoint(void) { return m_pClipDataInfo->m_iTimelineInPoint; }
	//int GetTimelineOutPoint(void) { return m_pClipDataInfo->m_iTimelineOutPoint; }
#ifdef PROTOTYPEMODE
	CString GetFilePath(void) { return m_pClipDataInfo->GetFilePath(); }
#endif



	// Setter
	void SetClipDataInfo(ClipDataInfo* pClipDataInfo){ m_pClipDataInfo = pClipDataInfo; }
	void SetClipDataInfoId(UUID uiId){ m_uiClipDataInfoId = uiId; }
	void GetClipDataRect(ClipDataRect* pClipDataRect){ m_pClipDataRect = pClipDataRect; }
	void SetClipDataRectId(UUID uiId){ m_uiClipDataRectId = uiId; }
	void GetTrackDataInfo(TrackDataInfo* pTrackDataInfo){ m_pTrackDataInfo = pTrackDataInfo; }
	void SetTrackDataInfoId(UUID uiId){ m_uiTrackDataInfoId = uiId; }
	void GetTrackDataRect(TrackDataRect* pTrackDataRect){ m_pTrackDataRect = pTrackDataRect; }
	void SetTrackDataRectId(UUID uiId){ m_uiTrackDataRectId = uiId; }

	// ClipDataRect Setter
	void SetCliprectTag(ClipDataTag eTag) { m_pClipDataRect->SetTag(eTag); }
	void SetClipRectId(UUID uiClipId) { m_pClipDataRect->SetClipRectId(uiClipId); }
	//void SetClipKind(InfoKind eClipKind) { m_pClipDataRect->m_eClipKind = eClipKind; }
	//void SetDuration(int iDuration) { m_pClipDataRect->m_iDuration = iDuration; }
	void SetTimelineInPoint(int iTimelineInPoint) { m_pClipDataRect->SetTimelineInPoint(iTimelineInPoint); }
	void SetTimelineOutPoint(int iTimelineOutPoint) { m_pClipDataRect->SetTimelineOutPoint(iTimelineOutPoint); }
	void SetOperatingRect(const CRect& rcRect) { m_pClipDataRect->SetOperatingRect(rcRect); }
	void CopyOperatingRectToOriginalRect(void) { m_pClipDataRect->CopyOperatingRectToOriginalRect(); }
	void SetOverlappingVert(const float fLeft, const float fTop, const float fRight, const float fBottom){ m_pClipDataRect->SetOverlappingVert(fLeft, fTop, fRight, fBottom); }
	void SetMovingColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4)
	{ m_pClipDataRect->SetMovingColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3, fR4, fG4, fB4, fA4); }
	void SetSingleInTrimColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4)
	{ m_pClipDataRect->SetSingleInTrimColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3, fR4, fG4, fB4, fA4); }
	void SetSingleOutTrimColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4)
	{ m_pClipDataRect->SetSingleOutTrimColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3, fR4, fG4, fB4, fA4); }
	void SetOperatingOldColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4)
	{ m_pClipDataRect->SetOperatingOldColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3, fR4, fG4, fB4, fA4); }
	void SetOverlappingColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4)
	{ m_pClipDataRect->SetOverlappingColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3, fR4, fG4, fB4, fA4); }
	void SetSingleTrimBorderColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4)
	{ m_pClipDataRect->SetSingleTrimBorderColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3, fR4, fG4, fB4, fA4); }

	// ClipDataInfo Setter
	void SetClipInfoTag(ClipDataTag eTag) { m_pClipDataInfo->SetTag(eTag); }
	void SetClipInfoId(UUID uiClipId) { m_pClipDataInfo->SetClipId(uiClipId); }
	void SetClipKind(InfoKind eClipKind) { m_pClipDataInfo->SetClipKind(eClipKind); }
	void SetFileId(UUID uiFileId) { m_pClipDataInfo->SetFileId(uiFileId); }
	void SetInPoint(int iInPoint) { m_pClipDataInfo->SetInPoint(iInPoint); }
	void SetOutPoint(int iOutPoint) { m_pClipDataInfo->SetOutPoint(iOutPoint); }
	void SetDuration(int iDuration) { m_pClipDataInfo->SetDuration(iDuration); }
	void SetInOffset(int iInOffset) { m_pClipDataInfo->SetInOffset(iInOffset); }
	//void SetTimelineInPoint(int iTimelineInPoint) { m_pClipDataInfo->SetTimelineInPoint(iTimelineInPoint); }
	//void SetTimelineOutPoint(int iTimelineOutPoint) { m_pClipDataInfo->SetTimelineOutPoint(iTimelineOutPoint); }
#ifdef PROTOTYPEMODE
	void SetFilePath(PCTSTR pszFilePath) { m_pClipDataInfo->SetFilePath(pszFilePath); }
#endif
};


