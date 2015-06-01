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

// 生成したトラック情報データを管理情報に登録する。
BOOL TrackDataManager::SetTrackData(const int iLayerNumber, const UUID& uiTrackId, TrackDataInfo* pTrackDataInfo, const UUID& uiTrackRectId, TrackDataRect* pTrackDataRect)
{
	assert(pTrackDataInfo != nullptr);
	assert(pTrackDataRect != nullptr);

	if ((pTrackDataInfo != nullptr) && (pTrackDataRect != nullptr))
	{
		PCTSTR pszTrackId = nullptr, pszTrackRectId = nullptr;

		if ((ChangeUUIDToCString(uiTrackId, pszTrackId)) && (ChangeUUIDToCString(uiTrackRectId, pszTrackRectId)))
		{
			m_TrackDataInfoVideoList.insert(m_TrackDataInfoVideoList.begin() + iLayerNumber, uiTrackId);
			m_TrackDataRectVideoList.insert(m_TrackDataRectVideoList.begin() + iLayerNumber, uiTrackRectId);
			m_TrackDataInfoVideoMap.insert(std::make_pair(static_cast<CString>(pszTrackId), pTrackDataInfo));
			m_TrackDataRectVideoMap.insert(std::make_pair(static_cast<CString>(pszTrackRectId), pTrackDataRect));
			return TRUE;
		}
	}
	return FALSE;
}

// トラック情報データを生成する。
BOOL TrackDataManager::CreateTrackData(const int iLayerNumber, UUID& uiTrackId, UUID& uiTrackrectId)
{
	TrackDataInfo* pTrackDataInfo = new TrackDataInfo();
	TrackDataRect* pTrackDataRect = new TrackDataRect();

	if (pTrackDataInfo->InitializeTrackId(uiTrackId))
	{
		if (pTrackDataRect->InitializeTrackRectId(uiTrackrectId))
		{
			pTrackDataRect->InitTrackData();
			pTrackDataRect->SetTrackDataInfo(uiTrackId, pTrackDataInfo);

			CString strTrackName;
			strTrackName.Format(_T("%d"), m_iSequentialNumber);
			pTrackDataRect->SetTrackName(_T("TrackNo.") + strTrackName);
			++m_iSequentialNumber;
			SetTrackData(iLayerNumber, uiTrackId, pTrackDataInfo, uiTrackrectId, pTrackDataRect);
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
	return static_cast<int>(m_TrackDataInfoVideoList.size());
}

// トラック情報データのポインタを取得する
TrackDataInfo* TrackDataManager::GetTrackDataInfo(const UUID& uiTrackDataId)
{
	PCTSTR pszTrackInfoId = nullptr;
	if (ChangeUUIDToCString(uiTrackDataId, pszTrackInfoId))
	{
		TrackDataInfoMap::iterator itr = m_TrackDataInfoVideoMap.find(pszTrackInfoId);
		if (itr != m_TrackDataInfoVideoMap.end())
		{
			return (*itr).second;
		}
		assert(itr != m_TrackDataInfoVideoMap.end());
	}
	return nullptr;
}

// トラック表示データのポインタを取得する
TrackDataRect* TrackDataManager::GetTrackDataRect(const UUID& uiTrackDataRectId)
{
	PCTSTR pszTrackRectId = nullptr;
	if (ChangeUUIDToCString(uiTrackDataRectId, pszTrackRectId))
	{
		TrackDataRectMap::iterator itr = m_TrackDataRectVideoMap.find(pszTrackRectId);
		if (itr != m_TrackDataRectVideoMap.end())
		{
			return (*itr).second;
		}
		assert(itr != m_TrackDataRectVideoMap.end());
	}
	return nullptr;
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