#pragma once
#include "ClipDataManager.h"
#include "DataObjectBase.h"

class ClipDataRect;

// ClipDataInfo コマンド ターゲット

class ClipDataInfo : public DataObjectBase
{
public:
	ClipDataInfo();
	ClipDataInfo(InfoKind eKind);
	virtual ~ClipDataInfo();

private:
	InfoKind m_eClipKind;
	UUID m_uiFileId;
	int m_iInPoint;
	int m_iOutPoint;
	int m_iDuration;
	int m_iInOffset;
	UUID m_uiMetaInfoId;
	std::list<UUID> m_uiVideoLevelPointList;
	std::list<UUID> m_uiAudioLevelPointList;
	std::list<UUID> m_uiVALinkList;
	BOOL m_fInTransition;
	CString m_strInTransitionUUID;
	ClipDataInfo* m_pInTransitionClipInfo;
	BOOL m_fOutTransition;
	CString m_strOutTransitionUUID;
	ClipDataInfo* m_pOutTransitionClipInfo;
	std::map < int, FilterInfo > m_FilterMap;	//InPoint, FilterInfo 
	int m_iTimelineInPoint;
	int m_iTimelineOutPoint;


	// For ProtoType
#ifdef PROTOTYPEMODE
	CString m_strFilePath;
#endif
	int m_iFileDuration;

private:
	// クリップ表示データとのリンク項目
	UUID m_uiClipRectUUID;
	ClipDataRect* m_pClipDataRect;

public:
	BOOL InitializeClipDataInfo(UUID& uiClipUUID);
	void SetClipDataRect(const UUID& uiClipRectUUID, ClipDataRect* pClipDataRect) { m_pClipDataRect = pClipDataRect; m_uiClipRectUUID = uiClipRectUUID; }
	BOOL DeleteClipData(void);
	void AddInPoint(int iNum) { m_iInPoint += iNum; }
	void AddOutPoint(int iNum) { m_iOutPoint += iNum; }
	void AddDuration(int iNum) { m_iDuration += iNum; }
	void AddTimelineInPoint(int iNum) { m_iTimelineInPoint += iNum; }
	void AddTimelineOutPoint(int iNum) { m_iTimelineOutPoint += iNum; }
	void ResetTransition(BOOL fIn, int iLength);

	// Setter
	void SetClipKind(InfoKind eClipKind) { m_eClipKind = eClipKind; }
	void SetFileId(UUID uiFileId) { m_uiFileId = uiFileId; }
	void SetInPoint(int iInPoint) { m_iInPoint = iInPoint; }
	void SetInPoint(void) { m_iInPoint = m_iOutPoint - m_iDuration; }
	void SetOutPoint(int iOutPoint) { m_iOutPoint = iOutPoint; }
	void SetOutPoint(void) { m_iOutPoint = m_iInPoint + m_iDuration; }
	void SetDuration(int iDuration) { m_iDuration = iDuration; }
	void SetDuration(void) { m_iDuration = m_iOutPoint - m_iInPoint; }
	void SetInOffset(int iInOffset) { m_iInOffset = iInOffset; }
	void SetInTransition(BOOL fTransition) { m_fInTransition = fTransition; }
	void SetInTransitionUUID(PCTSTR pszUUID) { m_strInTransitionUUID = static_cast<CString>(pszUUID); }
	void SetInTransitionClipInfo(ClipDataInfo* pClipInfo) { m_pInTransitionClipInfo = pClipInfo; }
	void SetOutTransition(BOOL fTransition) { m_fOutTransition = fTransition; }
	void SetOutTransitionUUID(PCTSTR pszUUID) { m_strOutTransitionUUID = static_cast<CString>(pszUUID); }
	void SetOutTransitionClipInfo(ClipDataInfo* pClipInfo) { m_pOutTransitionClipInfo = pClipInfo; }
	void SetTimelineInPoint(int iTimelineInPoint) { m_iTimelineInPoint = iTimelineInPoint; }
	void SetTimelineInPoint(void) { m_iTimelineInPoint = m_iTimelineOutPoint - m_iDuration; }
	void SetTimelineOutPoint(int iTimelineOutPoint) { m_iTimelineOutPoint = iTimelineOutPoint; }
	void SetTimelineOutPoint(void) { m_iTimelineOutPoint = m_iTimelineInPoint + m_iDuration; }
#ifdef PROTOTYPEMODE
	void SetFilePath(PCTSTR pszFilePath) { m_strFilePath = static_cast<CString>(pszFilePath); }
#endif
	void SetFileDuration(int iFileDuration) { m_iFileDuration = iFileDuration; }

	// Getter
	DataTag GetTag(void) { return m_eTag; }
	UUID GetClipInfoUUID(void) { return m_uiUUID; }
	PCTSTR GetClipInfoStrUUID(void) { return static_cast<PCTSTR>(m_strUUID); }
	InfoKind GetClipKind(void) { return m_eClipKind; }
	UUID GetFileId(void) { return m_uiFileId; }
	int GetInPoint(void) { return m_iInPoint; }
	int GetOutPoint(void) { return m_iOutPoint; }
	int GetDuration(void) { return m_iDuration; }
	int GetInOffset(void) { return m_iInOffset; }
	BOOL GetInTransition(void) { return m_fInTransition; }
	PCTSTR GetInTransitionUUID(void) { return static_cast<PCTSTR>(m_strInTransitionUUID); }
	ClipDataInfo* GetInTransitionClipInfo(void) { return m_pInTransitionClipInfo; }
	BOOL GetOutTransition(void) { return m_fOutTransition; }
	PCTSTR GetOutTransitionUUID(void) { return static_cast<PCTSTR>(m_strOutTransitionUUID); }
	ClipDataInfo* GetOutTransitionClipInfo(void) { return m_pOutTransitionClipInfo; }
	int GetTimelineInPoint(void) { return m_iTimelineInPoint; }
	int GetTimelineOutPoint(void) { return m_iTimelineOutPoint; }
#ifdef PROTOTYPEMODE
	PCTSTR GetFilePath(void) { return static_cast<PCTSTR>(m_strFilePath); }
#endif
	int GetFileDuration(void) { return m_iFileDuration; }

	UUID GetClipDataRectUUID(void){ return m_uiClipRectUUID; }
	ClipDataRect* GetClipDataRect(void){ return m_pClipDataRect; }

};


