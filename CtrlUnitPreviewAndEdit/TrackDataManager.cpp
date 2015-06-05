// TrackDataManager.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TrackDataManager.h"
#include "TrackDataInfo.h"
#include "TrackDataRect.h"

// TrackDataManager

TrackDataManager::TrackDataManager()
{
	// TODO: 種別ごとのタグに対応する必要あり
	m_eTrackDataManagerTag = TRACKDATAMANAGER_VIDEO;
	m_iSequentialNumber = 0;
	m_iTimelineLastFrame = 0;
}

TrackDataManager::TrackDataManager(TrackDataTag eTrackDataTag)
{
	// TODO: 種別ごとのタグに対応する必要あり
	m_eTrackDataManagerTag = eTrackDataTag;
	m_iSequentialNumber = 0;
	m_iTimelineLastFrame = 0;
}

TrackDataManager::~TrackDataManager()
{
	RpcStringFree(&m_rwsId);
}


// TrackDataManager メンバー関数

// UUID設定
BOOL TrackDataManager::InitializeTrackDataManagerId(UUID& uiTrackDataManagerId)
{
	if (RPC_S_OK == UuidCreate(&m_uiTrackDataManagerId))
	{
		uiTrackDataManagerId = m_uiTrackDataManagerId;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// トラックデータの全削除
void TrackDataManager::DeleteTrackDataManager(void)
{
	while (m_TrackDataInfoList.size() > 0)
	{
		DeleteTrackData(0);
	}
	ASSERT(m_TrackDataInfoList.size() == 0);
	ASSERT(m_TrackDataRectList.size() == 0);
	ASSERT(m_TrackDataInfoMap.size() == 0);
	ASSERT(m_TrackDataRectMap.size() == 0);
}

// トラックデータの削除
BOOL TrackDataManager::DeleteTrackData(const UINT iLayerNumber)
{
	if (m_TrackDataInfoList.size() < iLayerNumber + 1)
	{
		return FALSE;
	}
	UUID uiTrackInfoId = m_TrackDataInfoList.at(iLayerNumber);
	
	if (m_TrackDataRectList.size() < iLayerNumber + 1)
	{
		return FALSE;
	}
	UUID uiTrackRectId = m_TrackDataRectList.at(iLayerNumber);

	PCTSTR pszTrackId = nullptr, pszRectId = nullptr;
	if (!(ChangeUUIDToCString(uiTrackInfoId, pszTrackId)))
	{
		return FALSE;
	}
	TrackDataInfoMap::iterator itrInfo = m_TrackDataInfoMap.find(static_cast<CString>(pszTrackId));
	RpcStringFree(&m_rwsId);
	if (itrInfo == m_TrackDataInfoMap.end())
	{
		return FALSE;
	}
	TrackDataInfo* pTrackDataInfo = (*itrInfo).second;

	if (!(ChangeUUIDToCString(uiTrackRectId, pszRectId)))
	{
		return FALSE;
	}
	TrackDataRectMap::iterator itrRect = m_TrackDataRectMap.find(static_cast<CString>(pszRectId));
	RpcStringFree(&m_rwsId);
	if (itrRect == m_TrackDataRectMap.end())
	{
		return FALSE;
	}
	TrackDataRect* pTrackDataRect = (*itrRect).second;

	delete pTrackDataInfo;
	delete pTrackDataRect;
	m_TrackDataInfoMap.erase(itrInfo);
	m_TrackDataRectMap.erase(itrRect);
	TrackDataInfoList::iterator itrInfoList = find(m_TrackDataInfoList.begin(), m_TrackDataInfoList.end(), uiTrackInfoId);
	m_TrackDataInfoList.erase(itrInfoList);
	TrackDataRectList::iterator itrRectList = find(m_TrackDataRectList.begin(), m_TrackDataRectList.end(), uiTrackRectId);
	m_TrackDataRectList.erase(itrRectList);

	ASSERT(m_TrackDataInfoList.size() == m_TrackDataRectList.size());
	ASSERT(m_TrackDataInfoList.size() == m_TrackDataInfoMap.size());
	ASSERT(m_TrackDataInfoList.size() == m_TrackDataRectMap.size());

	return TRUE;
}

// 生成したトラック情報データを管理情報に登録する。
BOOL TrackDataManager::SetTrackData(const UINT iLayerNumber, const UUID& uiTrackId, TrackDataInfo* pTrackDataInfo, const UUID& uiTrackRectId, TrackDataRect* pTrackDataRect)
{
	ASSERT(pTrackDataInfo != nullptr);
	ASSERT(pTrackDataRect != nullptr);

	if ((pTrackDataInfo != nullptr) && (pTrackDataRect != nullptr))
	{
		PCTSTR pszTrackId = nullptr, pszTrackRectId = nullptr;

		if ((ChangeUUIDToCString(uiTrackId, pszTrackId)) && (ChangeUUIDToCString(uiTrackRectId, pszTrackRectId)))
		{
			m_TrackDataInfoList.insert(m_TrackDataInfoList.begin() + iLayerNumber, uiTrackId);
			m_TrackDataRectList.insert(m_TrackDataRectList.begin() + iLayerNumber, uiTrackRectId);
			m_TrackDataInfoMap.insert(std::make_pair(static_cast<CString>(pszTrackId), pTrackDataInfo));
			m_TrackDataRectMap.insert(std::make_pair(static_cast<CString>(pszTrackRectId), pTrackDataRect));
			return TRUE;
		}
		RpcStringFree(&m_rwsId);
	}
	return FALSE;
}

// トラック情報データを生成する。
BOOL TrackDataManager::CreateTrackData(const UINT iLayerNumber, UUID& uiTrackId, UUID& uiTrackRectId)
{
	TrackDataInfo* pTrackDataInfo = new TrackDataInfo();
	TrackDataRect* pTrackDataRect = new TrackDataRect();

	if (pTrackDataInfo->InitializeTrackId(uiTrackId))
	{
		if (pTrackDataRect->InitializeTrackRectId(uiTrackRectId))
		{
			pTrackDataRect->InitTrackData();
			pTrackDataRect->SetTrackDataInfo(uiTrackId, pTrackDataInfo);
			pTrackDataInfo->SetTrackDataRect(uiTrackRectId, pTrackDataRect);

			CString strTrackName;
			strTrackName.Format(_T("%d-%d"), m_eTrackDataManagerTag, m_iSequentialNumber);
			pTrackDataRect->SetTrackName(_T("TrackNo.") + strTrackName);
			++m_iSequentialNumber;
			SetTrackData(iLayerNumber, uiTrackId, pTrackDataInfo, uiTrackRectId, pTrackDataRect);
			return TRUE;
		}
	}
	DeleteObject(pTrackDataRect);
	DeleteObject(pTrackDataInfo);
	return FALSE;
}

// トラック数を取得する。
int TrackDataManager::GetTrackCount(void)
{
	return static_cast<int>(m_TrackDataInfoList.size());
}

// トラック情報データのポインタを取得する
TrackDataInfo* TrackDataManager::GetTrackDataInfo(const UUID& uiTrackDataId)
{
	PCTSTR pszTrackInfoId = nullptr;
	if (ChangeUUIDToCString(uiTrackDataId, pszTrackInfoId))
	{
		TrackDataInfoMap::iterator itr = m_TrackDataInfoMap.find(pszTrackInfoId);
		RpcStringFree(&m_rwsId);
		if (itr != m_TrackDataInfoMap.end())
		{
			return (*itr).second;
		}
		ASSERT(itr != m_TrackDataInfoMap.end());
	}
	return nullptr;
}

// トラック表示データのポインタを取得する
TrackDataRect* TrackDataManager::GetTrackDataRect(const UUID& uiTrackDataRectId)
{
	PCTSTR pszTrackRectId = nullptr;
	if (ChangeUUIDToCString(uiTrackDataRectId, pszTrackRectId))
	{
		TrackDataRectMap::iterator itr = m_TrackDataRectMap.find(pszTrackRectId);
		RpcStringFree(&m_rwsId);
		if (itr != m_TrackDataRectMap.end())
		{
			return (*itr).second;
		}
		ASSERT(itr != m_TrackDataRectMap.end());
	}
	return nullptr;
}

TrackDataInfo* TrackDataManager::GetTrackDataInfo(const UINT iLayerNumber)
{
	UUID uiTrackDataId = GetTrackDataInfoId(iLayerNumber);
	RPC_STATUS rpcStatus;
	UuidIsNil(&uiTrackDataId, &rpcStatus);
	if (rpcStatus != RPC_S_OK)
	{
		return nullptr;
	}

	PCTSTR pszTrackInfoId = nullptr;
	if (ChangeUUIDToCString(uiTrackDataId, pszTrackInfoId))
	{
		TrackDataInfoMap::iterator itr = m_TrackDataInfoMap.find(pszTrackInfoId);
		RpcStringFree(&m_rwsId);
		if (itr != m_TrackDataInfoMap.end())
		{
			return (*itr).second;
		}
		ASSERT(itr != m_TrackDataInfoMap.end());
	}
	return nullptr;
}

UUID TrackDataManager::GetTrackDataInfoId(const UINT iLayerNumber)
{
	if (m_TrackDataInfoList.size() >= (iLayerNumber + 1))
	{
		return m_TrackDataInfoList.at(iLayerNumber);
	}
	UUID uiRetId;
	UuidCreateNil(&uiRetId);
	return uiRetId;
}

TrackDataRect* TrackDataManager::GetTrackDataRect(const UINT iLayerNumber)
{
	UUID uiTrackDataId = GetTrackDataRectId(iLayerNumber);
	RPC_STATUS rpcStatus;
	UuidIsNil(&uiTrackDataId, &rpcStatus);
	if (rpcStatus != RPC_S_OK)
	{
		return nullptr;
	}

	PCTSTR pszTrackInfoId = nullptr;
	if (ChangeUUIDToCString(uiTrackDataId, pszTrackInfoId))
	{
		TrackDataRectMap::iterator itr = m_TrackDataRectMap.find(pszTrackInfoId);
		RpcStringFree(&m_rwsId);
		if (itr != m_TrackDataRectMap.end())
		{
			return (*itr).second;
		}
		ASSERT(itr != m_TrackDataRectMap.end());
	}
	return nullptr;
}

UUID TrackDataManager::GetTrackDataRectId(const UINT iLayerNumber)
{
	if (m_TrackDataRectList.size() >= iLayerNumber + 1)
	{
		return m_TrackDataRectList.at(iLayerNumber);
	}
	UUID uiRetId;
	UuidCreateNil(&uiRetId);
	return uiRetId;
}



// UUIDの型変換
BOOL TrackDataManager::ChangeUUIDToCString(const UUID& uiId, PCTSTR& pszId)
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