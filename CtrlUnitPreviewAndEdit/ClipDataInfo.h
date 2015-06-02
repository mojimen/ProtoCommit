#pragma once
#include "ClipDataManager.h"

class ClipDataRect;

// ClipDataInfo コマンド ターゲット

class ClipDataInfo : public CObject
{
public:
	ClipDataInfo();
	ClipDataInfo(InfoKind eKind);
	virtual ~ClipDataInfo();

private:
	ClipDataTag m_eClipDataInfoTag;
	UUID m_uiClipId;
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

	// For ProtoType
	CString strFilePath;

private:
	// クリップ表示データとのリンク項目
	UUID m_uiClipRectId;
	ClipDataRect* m_pClipDataRect;


public:
	// TODO: private化
	int m_iTimelineInPoint;

	BOOL InitClipData(void);

public:

	BOOL InitializeClipId(UUID& uiClipId);
	void SetClipDataRect(const UUID& uiClipRectId, ClipDataRect* pClipDataRect) { m_pClipDataRect = pClipDataRect; m_uiClipRectId = uiClipRectId; }
	BOOL DeleteClipData(void);


	// Setter
	void SetTag(ClipDataTag eTag) { m_eClipDataInfoTag = eTag; }
	void SetClipId(UUID uiClipId) { m_uiClipId = uiClipId; }
	void SetClipKind(InfoKind eClipKind) { m_eClipKind = eClipKind; }
	void SetFileId(UUID uiFileId) { m_uiFileId = uiFileId; }
	void SetInPoint(int iInPoint) { m_iInPoint = iInPoint; }
	void SetOutPoint(int iOutPoint) { m_iOutPoint = iOutPoint; }
	void SetDuration(int iDuration) { m_iDuration = iDuration; }
	void SetInOffset(int iInOffset) { m_iInOffset = iInOffset; }


	// Getter
	ClipDataTag GetTag(void) { return m_eClipDataInfoTag; }
	UUID GetClipId(void) { return m_uiClipId; }
	InfoKind GetClipKind(void) { return m_eClipKind; }
	UUID GetFileId(void) { return m_uiFileId; }
	int GetInPoint(void) { return m_iInPoint; }
	int GetOutPoint(void) { return m_iOutPoint; }
	int GetDuration(void) { return m_iDuration; }
	int GetInOffset(void) { return m_iInOffset; }

	UUID GetClipDataRectId(void){ return m_uiClipRectId; }
	ClipDataRect* GetClipDataRect(void){ return m_pClipDataRect; }

};


