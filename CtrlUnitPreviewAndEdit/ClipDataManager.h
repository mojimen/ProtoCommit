#pragma once
#include "TimelineDataManager.h"

class ClipDataInfo;
class ClipDataRect;

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
	void DeleteClipDataManager(void);
	BOOL DeleteClipData(const UUID& uiClipId, const BOOL fInfoFlag = TRUE);

	BOOL SetClipData(const UUID& uiClipId, ClipDataInfo* pClipDataInfo, const UUID& uiClipRectId, ClipDataRect* pClipDataRect);
	ClipDataInfo* GetClipDataInfo(const UUID& uiClipId);
	ClipDataRect* GetClipDataRect(const UUID& uiClipDataRectId);
	BOOL CreateClipData(UUID& uiClipId, UUID& uiClipRectId);
	BOOL ChangeUUIDToCString(const UUID& uiId, PCTSTR& pszId);

};


