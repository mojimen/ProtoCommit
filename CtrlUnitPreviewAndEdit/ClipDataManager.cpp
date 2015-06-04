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
}

ClipDataManager::~ClipDataManager()
{
	RpcStringFree(&m_rwsId);
}


// ClipDataManager メンバー関数

// 初期設定
BOOL ClipDataManager::InitializeClipDataManagerId(UUID& uiClipDataManagerUUID)
{
	if (InitializeData(A_CLIPDATAMANAGER))
	{
		uiClipDataManagerUUID = m_uiUUID;
		return TRUE;
	}
	return FALSE;
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
BOOL ClipDataManager::DeleteClipData(PCTSTR pszClipUUID, const BOOL fInfoFlag /*= FALSE*/)
{	
	ASSERT(pszClipUUID != nullptr);
	if ((pszClipUUID == nullptr) || (static_cast<CString>(pszClipUUID).IsEmpty()))
	{
		return FALSE;
	}
	if (fInfoFlag)
	{
		ClipDataInfoMap::iterator itr = m_ClipDataInfoMap.find(static_cast<CString>(pszClipUUID));
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
		ClipDataRectMap::iterator itr = m_ClipDataRectMap.find(static_cast<CString>(pszClipUUID));
		if (itr != m_ClipDataRectMap.end())
		{
			ClipDataInfo* pClipDataInfo = (*itr).second->GetClipDataInfo();
			delete pClipDataInfo;
			delete (*itr).second;
			return TRUE;
		}
	}
	return FALSE;
}


// 生成したクリップ情報データを管理情報に登録する。
BOOL ClipDataManager::SetClipDataMap(PCTSTR pszClipInfoUUID, ClipDataInfo& cClipDataInfo, PCTSTR pszClipRectUUID, ClipDataRect& cClipDataRect)
{
	ASSERT(pszClipInfoUUID != nullptr);
	ASSERT(pszClipRectUUID != nullptr);
	if ((pszClipInfoUUID == nullptr) || (static_cast<CString>(pszClipInfoUUID).IsEmpty()) || (pszClipRectUUID == nullptr) || (static_cast<CString>(pszClipRectUUID).IsEmpty()))
	{
		return FALSE;
	}
	m_ClipDataInfoMap.insert(std::make_pair(static_cast<CString>(pszClipInfoUUID), &cClipDataInfo));
	m_ClipDataRectMap.insert(std::make_pair(static_cast<CString>(pszClipRectUUID), &cClipDataRect));
	return TRUE;
}

// クリップデータを生成する。
BOOL ClipDataManager::CreateClipData(PCTSTR& pszClipInfoUUID, PCTSTR& pszClipRectUUID)
{
	ClipDataInfo* pClipDataInfo = new ClipDataInfo();
	ClipDataRect* pClipDataRect = new ClipDataRect();

	UUID uiClipInfoUUID, uiClipRectUUID;

	if (pClipDataInfo->InitializeClipDataInfo(uiClipInfoUUID))
	{
		if (pClipDataRect->InitializeClipDataRect(uiClipRectUUID))
		{
			pClipDataRect->SetClipDataInfo(uiClipInfoUUID, pClipDataInfo);
			pClipDataInfo->SetClipDataRect(uiClipRectUUID, pClipDataRect);
			SetClipDataMap(pClipDataInfo->GetClipInfoStrUUID(), *pClipDataInfo, pClipDataRect->GetClipRectStrUUID(), *pClipDataRect);
			pszClipInfoUUID = pClipDataInfo->GetClipInfoStrUUID();
			pszClipRectUUID = pClipDataRect->GetClipRectStrUUID();
			return TRUE;
		}
	}
	DeleteObject(pClipDataInfo);
	DeleteObject(pClipDataRect);
	return FALSE;
}



// クリップ情報データのポインタを取得する
ClipDataInfo* ClipDataManager::GetClipDataInfo(PCTSTR pszClipUUID)
{
	ASSERT(pszClipUUID != nullptr);
	if ((pszClipUUID == nullptr) || (static_cast<CString>(pszClipUUID).IsEmpty()))
	{
		return nullptr;
	}
	ClipDataInfoMap::iterator itr = m_ClipDataInfoMap.find(pszClipUUID);
	if (itr == m_ClipDataInfoMap.end())
	{
		ASSERT(itr != m_ClipDataInfoMap.end());
		return nullptr;
	}
	return (*itr).second;
}

// クリップ表示データのポインタを取得する
ClipDataRect* ClipDataManager::GetClipDataRect(PCTSTR pszClipUUID)
{
	ASSERT(pszClipUUID != nullptr);
	if ((pszClipUUID == nullptr) || (static_cast<CString>(pszClipUUID).IsEmpty()))
	{
		return nullptr;
	}
	ClipDataRectMap::iterator itr = m_ClipDataRectMap.find(pszClipUUID);
	if (itr == m_ClipDataRectMap.end())
	{
		ASSERT(itr != m_ClipDataRectMap.end());
		return nullptr;
	}
	return (*itr).second;
}
