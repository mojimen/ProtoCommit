#pragma once
#include<map>

class ClipDataInfo;
class ClipDataRect;

typedef std::map<CString, ClipDataInfo*> ClipDataInfoMap;
typedef std::map<CString, ClipDataRect*> ClipDataRectMap;

typedef enum ClipDataTag{ 
	CLIPDATAMANAGER,
	CLIPDATAINFO,
	CLIPDATARECT
};

typedef enum ClipKind {
	CLIP_VIDEO,
	CLIP_AUDIO,
	CLIP_TITLE,
	CLIP_MASTER_VIDEO,
	CLIP_MASTER_AUDIO
};


// ClipDataManager コマンド ターゲット

class ClipDataManager : public CObject
{
public:
	ClipDataManager();
	virtual ~ClipDataManager();

private:
	ClipDataTag m_eClipDataManagerTag;
	UUID m_uiClipDataManagerId;
	ClipDataInfoMap m_ClipDataInfoMap;
	ClipDataRectMap m_ClipDataRectMap;

	RPC_WSTR m_rwsId;				//UUID変換用項目

public:
	BOOL InitializeClipDataManagerId(UUID& uiClipDataManagerId);

	BOOL SetClipData(const UUID& uiClipId, ClipDataInfo* pClipDataInfo, const UUID& uiClipRectId, ClipDataRect* pClipDataRect);
	ClipDataInfo* GetClipDataInfo(const UUID& uiClipId);
	ClipDataRect* GetClipDataRect(const UUID& uiClipDataRectId);
	BOOL CreateClipData(UUID& uiClipId, UUID& uiClipRectId);
	BOOL ChangeUUIDToCString(const UUID& uiId, PCTSTR& pszId);

	
	//BOOL DeleteClipData(const UUID& uiClipDataRectId);
};


