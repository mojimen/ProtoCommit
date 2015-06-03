// ClipDataManager.cpp : �����t�@�C��
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


// ClipDataManager �����o�[�֐�

// UUID�ݒ�
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

// �N���b�v�S�f�[�^�폜
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
	// ���������������܂�
	m_ClipDataInfoMap.clear();
	m_ClipDataRectMap.clear();
}

// �N���b�v�f�[�^�폜
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


// ���������N���b�v���f�[�^���Ǘ����ɓo�^����B
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

// �N���b�v�f�[�^�𐶐�����B
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



// �N���b�v���f�[�^�̃|�C���^���擾����
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

// �N���b�v�\���f�[�^�̃|�C���^���擾����
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


// UUID�̌^�ϊ�
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