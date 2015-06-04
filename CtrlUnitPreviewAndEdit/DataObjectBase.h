#pragma once

typedef enum DataTag{
	A_TRACKDATAMANAGER_VIDEO,
	A_TRACKDATAMANAGER_AUDIO,
	A_TRACKDATAMANAGER_INFO,
	A_TRACKDATAMANAGER_MASTER,
	A_TRACKDATAINFO,
	A_TRACKDATARECT,

	A_CLIPDATAMANAGER,
	A_CLIPDATAINFO,
	A_CLIPDATARECT
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
};


