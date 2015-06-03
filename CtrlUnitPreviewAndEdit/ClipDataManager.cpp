// ClipDataManager.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "ClipDataManager.h"
#include "ClipDataInfo.h"
#include "ClipDataRect.h"


// ClipDataManager

ClipDataManager::ClipDataManager()
{
	m_eClipDataManagerTag = CLIPDATAMANAGER;
}

ClipDataManager::~ClipDataManager()
{
	RpcStringFree(&m_rwsId);
}


// ClipDataManager メンバー関数

// UUID設定
BOOL ClipDataManager::InitializeClipDataManagerId(UUID& uiClipDataManagerId)
{
	if (RPC_S_OK == UuidCreate(&m_uiClipDataManagerId))
	{
		uiClipDataManagerId = m_uiClipDataManagerId;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// クリップ全データ削除
void ClipDataManager::DeleteClipDataManager(void)
{
	ClipDataInfoMap::iterator itrInfo = m_ClipDataInfoMap.begin();
	while (itrInfo != m_ClipDataInfoMap.end())
	{
		delete (*itrInfo).second;
		++itrInfo;
	}
	ClipDataRectMap::iterator itrRect = m_ClipDataRectMap.begin();
	while (itrRect != m_ClipDataRectMap.end())
	{
		delete (*itrRect).second;
		++itrRect;
	}
	// メモリも解放されます
	m_ClipDataInfoMap.clear();
	m_ClipDataRectMap.clear();
}

// クリップデータ削除
BOOL ClipDataManager::DeleteClipData(const UUID& uiClipId, const BOOL fInfoFlag /*= TRUE*/)
{	
	PCTSTR pszClipId = nullptr;
	if (ChangeUUIDToCString(uiClipId, pszClipId))
	{
		if (fInfoFlag)
		{
			ClipDataInfoMap::iterator itr = m_ClipDataInfoMap.find(static_cast<CString>(pszClipId));
			if (itr != m_ClipDataInfoMap.end())
			{
				ClipDataRect* pClipDataRect = (*itr).second->GetClipDataRect();
				delete pClipDataRect;
				delete (*itr).second;
				return TRUE;
			}
		}
		else
		{
			ClipDataRectMap::iterator itr = m_ClipDataRectMap.find(static_cast<CString>(pszClipId));
			if (itr != m_ClipDataRectMap.end())
			{
				ClipDataInfo* pClipDataInfo = (*itr).second->GetClipDataInfo();
				delete pClipDataInfo;
				delete (*itr).second;
				return TRUE;
			}
		}
	}
	return FALSE;
}


// 生成したクリップ情報データを管理情報に登録する。
BOOL ClipDataManager::SetClipData(const UUID& uiClipId, ClipDataInfo* pClipDataInfo, const UUID& uiClipRectId, ClipDataRect* pClipDataRect)
{
	ASSERT(pClipDataInfo != nullptr);
	ASSERT(pClipDataRect != nullptr);

	if ((pClipDataInfo != nullptr) && (pClipDataRect != nullptr))
	{
		PCTSTR pszClipId = nullptr, pszClipRectId = nullptr;

		if ((ChangeUUIDToCString(uiClipId, pszClipId)) && (ChangeUUIDToCString(uiClipRectId, pszClipRectId)))
		{
			m_ClipDataInfoMap.insert(std::make_pair(static_cast<CString>(pszClipId), pClipDataInfo));
			m_ClipDataRectMap.insert(std::make_pair(static_cast<CString>(pszClipRectId), pClipDataRect));
			return TRUE;
		}
	}
	return FALSE;
}

// クリップデータを生成する。
BOOL ClipDataManager::CreateClipData(UUID& uiClipId, UUID& uiClipRectId)
{
	ClipDataInfo* pClipDataInfo = new ClipDataInfo();
	ClipDataRect* pClipDataRect = new ClipDataRect();

	if (pClipDataInfo->InitializeClipId(uiClipId))
	{
		if (pClipDataRect->InitializeClipRectId(uiClipRectId))
		{
			pClipDataRect->InitClipData();
			pClipDataRect->SetClipDataInfo(uiClipId, pClipDataInfo);
			pClipDataInfo->SetClipDataRect(uiClipRectId, pClipDataRect);
			SetClipData(uiClipId, pClipDataInfo, uiClipRectId, pClipDataRect);
			return TRUE;
		}
	}
	DeleteObject(pClipDataInfo);
	DeleteObject(pClipDataRect);
	return FALSE;
}



// クリップ情報データのポインタを取得する
ClipDataInfo* ClipDataManager::GetClipDataInfo(const UUID& uiClipId)
{
	PCTSTR pszClipId = nullptr;
	if (ChangeUUIDToCString(uiClipId, pszClipId))
	{
		ClipDataInfoMap::iterator itr = m_ClipDataInfoMap.find(pszClipId);
		if (itr != m_ClipDataInfoMap.end())
		{
			return (*itr).second;
		}
		ASSERT(itr != m_ClipDataInfoMap.end());
	}
	return nullptr;
}

// クリップ表示データのポインタを取得する
ClipDataRect* ClipDataManager::GetClipDataRect(const UUID& uiClipDataRectId)
{
	PCTSTR pszClipRectId = nullptr;
	if (ChangeUUIDToCString(uiClipDataRectId, pszClipRectId))
	{
		ClipDataRectMap::iterator itr = m_ClipDataRectMap.find(pszClipRectId);
		if (itr != m_ClipDataRectMap.end())
		{
			return (*itr).second;
		}
		ASSERT(itr != m_ClipDataRectMap.end());
	}
	return nullptr;
}


// UUIDの型変換
BOOL ClipDataManager::ChangeUUIDToCString(const UUID& uiId, PCTSTR& pszId)
{
	if (RPC_S_OK == UuidToString(&uiId, &m_rwsId))
	{
		pszId = reinterpret_cast<PCTSTR>(m_rwsId);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}