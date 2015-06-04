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
	std::map < int, FilterInfo > m_FilterMap;	//InPoint, FilterInfo 
	int m_iTimelineInPoint;
	int m_iTimelineOutPoint;


	// For ProtoType
#ifdef PROTOTYPEMODE
	CString m_strFilePath;
#endif

private:
	// クリップ表示データとのリンク項目
	UUID m_uiClipRectUUID;
	ClipDataRect* m_pClipDataRect;

public:
	BOOL InitializeClipDataInfo(UUID& uiClipUUID);
	void SetClipDataRect(const UUID& uiClipRectUUID, ClipDataRect* pClipDataRect) { m_pClipDataRect = pClipDataRect; m_uiClipRectUUID = uiClipRectUUID; }
	BOOL DeleteClipData(void);


	// Setter
	void SetClipKind(InfoKind eClipKind) { m_eClipKind = eClipKind; }
	void SetFileId(UUID uiFileId) { m_uiFileId = uiFileId; }
	void SetInPoint(int iInPoint) { m_iInPoint = iInPoint; }
	void SetOutPoint(int iOutPoint) { m_iOutPoint = iOutPoint; }
	void SetDuration(int iDuration) { m_iDuration = iDuration; }
	void SetInOffset(int iInOffset) { m_iInOffset = iInOffset; }
	void SetTimelineInPoint(int iTimelineInPoint) { m_iTimelineInPoint = iTimelineInPoint; }
	void SetTimelineOutPoint(int iTimelineOutPoint) { m_iTimelineOutPoint = iTimelineOutPoint; }
#ifdef PROTOTYPEMODE
	void SetFilePath(PCTSTR pszFilePath) { m_strFilePath = pszFilePath; }
#endif

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
	int GetTimelineInPoint(void) { return m_iTimelineInPoint; }
	int GetTimelineOutPoint(void) { return m_iTimelineOutPoint; }
#ifdef PROTOTYPEMODE
	CString GetFilePath(void) { return m_strFilePath; }
#endif
	UUID GetClipDataRectUUID(void){ return m_uiClipRectUUID; }
	ClipDataRect* GetClipDataRect(void){ return m_pClipDataRect; }

};


