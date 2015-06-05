#pragma once

typedef enum DataTag
{
	A_TRACKDATAMANAGER_VIDEO,
	A_TRACKDATAMANAGER_AUDIO,
	A_TRACKDATAMANAGER_INFO,
	A_TRACKDATAMANAGER_MASTER,
	A_TRACKDATAINFO,
	A_TRACKDATARECT,

	A_CLIPDATAMANAGER,
	A_CLIPDATAINFO,
	A_CLIPDATARECT,

	TRANSITIONMANAGER,
	TRANSITIONDATAIN,
	TRANSITIONDATAOUT
};

// DataObjectBase コマンド ターゲット

class DataObjectBase : public CObject
{
public:
	DataObjectBase();
	virtual ~DataObjectBase();

protected:
	DataTag m_eTag;
	UUID m_uiUUID;
	CString m_strUUID;

	BOOL InitializeData(DataTag m_eTag);

public:
	DataTag GetDataTag(void) { return m_eTag; }
	UUID GetUUID(void) { return m_uiUUID; }
	PCTSTR GetStrUUID(void) { return static_cast<PCTSTR>(m_strUUID); }
};


