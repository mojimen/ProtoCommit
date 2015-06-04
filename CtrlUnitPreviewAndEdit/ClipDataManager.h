#pragma once
#include "DataObjectBase.h"
#include "TimelineDataManager.h"

class ClipDataInfo;
class ClipDataRect;

// ClipDataManager コマンド ターゲット

class ClipDataManager : public DataObjectBase
{
public:
	ClipDataManager();
	virtual ~ClipDataManager();

private:
	ClipDataInfoMap m_ClipDataInfoMap;
	ClipDataRectMap m_ClipDataRectMap;

	RPC_WSTR m_rwsId;				//UUID変換用項目

public:
	BOOL InitializeClipDataManagerId(UUID& uiClipDataManagerUUID);
	void DeleteClipDataManager(void);
	BOOL DeleteClipData(PCTSTR pszClipUUID, const BOOL fInfoFlag = FALSE);

	BOOL SetClipDataMap(PCTSTR pszClipInfoUUID, ClipDataInfo& cClipDataInfo, PCTSTR pszClipRectUUID, ClipDataRect& cClipDataRect);
	ClipDataInfo* GetClipDataInfo(PCTSTR pszClipUUID);
	ClipDataRect* GetClipDataRect(PCTSTR pszClipUUID);
	BOOL CreateClipData(PCTSTR& pszClipInfoUUID, PCTSTR& pszClipRectUUID);

	// Getter
	UUID GetClipDataManagerUUID(void) { return m_uiUUID; }
	PCTSTR GetClipDataManagerStrUUID(void) { return static_cast<PCTSTR>(m_strUUID); }

};


