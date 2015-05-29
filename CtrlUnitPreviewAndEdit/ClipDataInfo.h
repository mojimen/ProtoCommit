#pragma once
#include <list>
#include <map>

// TODO: デバッグ用
#include <random>


// ClipDataInfo コマンド ターゲット

struct FilterInfo
{
	int iOutPoint;
	int iDuration;
	UUID uiFilterId;
};

class ClipDataInfo : public CObject
{
public:
	ClipDataInfo();
	virtual ~ClipDataInfo();

	enum ClipDataTag{ CLIPDATAINFO };
	enum ClipKind {
		VIDEO,
		AUDIO,
		TITLE,
		MASTER_VIDEO,
		MASTER_AUDIO
	};

private:
	ClipDataTag m_eClipDataInfoTag;
	UUID m_uiClipId;
	ClipKind m_eClipKind;
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

public:
	// TODO: private化
	int m_iTimelineInPoint;

	BOOL InitClipData(void);
	void CreateClipInOutPoint(void);


public:

	BOOL InitializeClipId(UUID& uiClipId);

	// Setter
	void SetTag(ClipDataTag eTag) { m_eClipDataInfoTag = eTag; }
	void SetClipId(UUID uiClipId) { m_uiClipId = uiClipId; }
	void SetClipKind(ClipKind eClipKind) { m_eClipKind = eClipKind; }
	void SetFileId(UUID uiFileId) { m_uiFileId = uiFileId; }
	void SetInPoint(int iInPoint) { m_iInPoint = iInPoint; }
	void SetOutPoint(int iOutPoint) { m_iOutPoint = iOutPoint; }
	void SetDuration(int iDuration) { m_iDuration = iDuration; }
	void SetInOffset(int iInOffset) { m_iInOffset = iInOffset; }

	// Getter
	ClipDataTag GetTag(void) { return m_eClipDataInfoTag; }
	UUID GetClipId(void) { return m_uiClipId; }
	ClipKind GetClipKind(void) { return m_eClipKind; }
	UUID GetFileId(void) { return m_uiFileId; }
	int GetInPoint(void) { return m_iInPoint; }
	int GetOutPoint(void) { return m_iOutPoint; }
	int GetDuration(void) { return m_iDuration; }
	int GetInOffset(void) { return m_iInOffset; }

};


