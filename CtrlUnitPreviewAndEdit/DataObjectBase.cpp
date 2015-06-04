// DataObjectBase.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "DataObjectBase.h"


// DataObjectBase

DataObjectBase::DataObjectBase()
{
}

DataObjectBase::~DataObjectBase()
{
}


// DataObjectBase メンバー関数
BOOL DataObjectBase::InitializeData(DataTag eTag)
{
	m_eTag = eTag;

	if (RPC_S_OK != UuidCreate(&m_uiUUID))
	{
		return FALSE;
	}

	RPC_WSTR rwsId;
	if (RPC_S_OK != UuidToString(&m_uiUUID, &rwsId))
	{
		RpcStringFree(&rwsId);
		return FALSE;
	}
	m_strUUID = static_cast<CString>(reinterpret_cast<PCTSTR>(rwsId));
	RpcStringFree(&rwsId);
	return TRUE;
}
