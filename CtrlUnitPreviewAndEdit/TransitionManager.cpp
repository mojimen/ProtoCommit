// TransitionManager.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TransitionManager.h"


// TransitionManager

TransitionManager::TransitionManager()
{
}

TransitionManager::~TransitionManager()
{
}

// TransitionManager メンバー関数

// 初期設定
BOOL TransitionManager::InitializeTransitionManager(CString& strTransitionManagerUUID)
{
	if (InitializeData(TRANSITIONMANAGER))
	{
		strTransitionManagerUUID = m_strUUID;
		return TRUE;
	}
	return FALSE;
}

// 削除前処理
void TransitionManager::DeleteTransitionManager(void)
{
	DeleteAllTransitionData();
}

// トランジションデータ取得
TransitionData* TransitionManager::GetTransitionData(CString& strTransitionUUID)
{
	TransitionDataMap::iterator itr = m_mpTransitionDataMap.find(strTransitionUUID);
	if (itr == m_mpTransitionDataMap.end())
	{
		return nullptr;
	}
	return (*itr).second;
}

// トランジションデータ作成
BOOL TransitionManager::CreateTransitionData(CString& strTransitionUUID, int iDuration, BOOL fIn /* = TRUE */)
{
	BOOL fRet;
	TransitionData* pTransitionData = new TransitionData();

	if (fIn)
	{
		fRet = pTransitionData->InitializeInTransition(strTransitionUUID);
	}
	else
	{
		fRet = pTransitionData->InitializeOutTransition(strTransitionUUID);
	}
	if (!fRet)
	{
		return FALSE;
	}
	pTransitionData->SetTransitionDuration(iDuration);
	m_mpTransitionDataMap[strTransitionUUID] = pTransitionData;
	return TRUE;
}


// トランジションデータ設定
BOOL TransitionManager::SetTransitionData(TransitionData& cTransitionData)
{
	CString strUUID = static_cast<CString>(cTransitionData.GetStrUUID());
	if (strUUID.IsEmpty())
	{
		return FALSE;
	}
	m_mpTransitionDataMap[strUUID] = &cTransitionData;
	return TRUE;
}

// トランジションデータ削除
BOOL TransitionManager::DeleteTransitionData(CString& strTransitionUUID)
{
	if (strTransitionUUID.IsEmpty())
	{
		return FALSE;
	}
	TransitionDataMap::iterator itr = m_mpTransitionDataMap.find(strTransitionUUID);
	if (itr == m_mpTransitionDataMap.end())
	{
		return FALSE;
	}
	delete (*itr).second;
	return TRUE;
}

// トランジションデータ全削除
void TransitionManager::DeleteAllTransitionData(void)
{
	TransitionDataMap::iterator itr = m_mpTransitionDataMap.begin();
	while (itr != m_mpTransitionDataMap.end())
	{
		delete (*itr).second;
		++itr;
	}
	m_mpTransitionDataMap.clear();
}


