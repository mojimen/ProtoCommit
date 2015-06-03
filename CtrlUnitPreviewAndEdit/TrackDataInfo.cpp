// TrackDataInfo.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TrackDataInfo.h"
#include "ClipDataRect.h"

// TrackDataInfo

TrackDataInfo::TrackDataInfo()
{
	m_eTrackDataInfoTag = TRACKDATAINFO;
}

TrackDataInfo::~TrackDataInfo()
{
}


// TrackDataInfo メンバー関数

BOOL TrackDataInfo::InitTrackData(void)
{
	return TRUE;
}

// UUID設定
BOOL TrackDataInfo::InitializeTrackId(UUID& uiClipId)
{
	if (RPC_S_OK == UuidCreate(&m_uiTrackId))
	{
		uiClipId = m_uiTrackId;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 保有するクリップを全て削除する
void TrackDataInfo::DeleteTrackDataInfo(void)
{
	ClipDataPositionMap::iterator itr = m_mpClipDataRectMap.begin();
	while (itr != m_mpClipDataRectMap.end())
	{
		(*itr).second->DeleteClipData();
		delete (*itr).second;
	}
	m_mpClipDataRectMap.clear();
}

// 入力のフレームに存在するクリップを取得する（１つのみ）
ClipDataRect* TrackDataInfo::GetClipDataInfo(const int iFrame, int& iInPoint)
{
	ASSERT(iFrame >= 0);

	if (m_mpClipDataRectMap.size() == 0)
	{
		return nullptr;
	}
	ClipDataPositionMap::iterator itr = m_mpClipDataRectMap.upper_bound(iFrame);
	if (itr == m_mpClipDataRectMap.begin())
	{
		return nullptr;
	}
	--itr;
	ClipDataRect* pClipData = (*itr).second;
	if (((*itr).first <= iFrame) && (iFrame <= ((*itr).first + pClipData->GetDuration() - 1)))
	{
		iInPoint = (*itr).first;
		return pClipData;
	}
	else
	{
		return nullptr;
	}

}

// 入力のフレームに存在するクリップを全て取得
int TrackDataInfo::GetClipDataAtFrame(const int iFrame, ClipDataPositionMap& mpClipMap)
{
	ASSERT(iFrame >= 0);

	int iSize = 0;
	if (m_mpClipDataRectMap.size() == 0)
	{
		return iSize;
	}
	ClipDataPositionMap::iterator itr = m_mpClipDataRectMap.upper_bound(iFrame);
	if (itr == m_mpClipDataRectMap.begin())
	{
		return 0;
	}
	else
	{
		ClipDataRect* pClipData;
		--itr;
		pClipData = (*itr).second;
		if (iFrame <= ((*itr).first + pClipData->GetDuration() - 1))
		{
			mpClipMap[(*itr).first] = pClipData;
			++iSize;
		}
		--itr;
		if (itr == m_mpClipDataRectMap.begin())
		{
			pClipData = (*itr).second;
			if (iFrame <= ((*itr).first + pClipData->GetDuration() - 1))
			{
				mpClipMap[(*itr).first] = pClipData;
				++iSize;
			}
		}
	}
	return iSize;
}

// 入力のフレーム範囲に存在するクリップを取得
int TrackDataInfo::GetClipDataInRange(const int iStartFrame, const int iEndFrame, ClipDataPositionMap& mpClipMap)
{
	ASSERT(iStartFrame >= 0);
	ASSERT(iEndFrame >= iStartFrame);

	int iSize = 0;
	if (m_mpClipDataRectMap.size() == 0)
	{
		return iSize;
	}
	ClipDataRect* pClipData;
	ClipDataPositionMap::iterator itr = m_mpClipDataRectMap.upper_bound(iStartFrame);
	if (itr != m_mpClipDataRectMap.begin())
	{
		--itr;
		pClipData = (*itr).second;
		if (iStartFrame <= ((*itr).first + pClipData->GetDuration() - 1))
		{
			mpClipMap[(*itr).first] = pClipData;
			++iSize;
		}
		++itr;
	}
	while (itr != m_mpClipDataRectMap.end())
	{
		pClipData = (*itr).second;
		if ((*itr).first <= iEndFrame)
		{
			mpClipMap[(*itr).first] = pClipData;
			++itr;
			++iSize;
		}
		else
		{
			break;
		}
	}

	return iSize;
}






// 入力のフレーム位置にクリップを追加する
void TrackDataInfo::AddClip(const int iInPoint, ClipDataRect* pClipData)
{
	m_mpClipDataRectMap[iInPoint] = pClipData;
}

// 入力のフレーム位置にIn点があるクリップを削除する
void TrackDataInfo::DeleteClip(const int iInPoint)
{
	m_mpClipDataRectMap.erase(iInPoint);
}

// 入力のフレーム位置にIn点があるクリップを削除して新しい位置に指定したクリップを追加する。
void TrackDataInfo::ChangeClip(const int iOldInPoint, const int iNewInPoint, ClipDataRect* pClipData)
{
	DeleteClip(iOldInPoint);
	AddClip(iNewInPoint, pClipData);
}


// TODO: これはコントローラーにあるべき？
// SingleOutTrimの操作範囲にクリップが存在するかをチェック
int TrackDataInfo::CheckClipInSingleOutTrimRange(int iStartFrame, int iEndFrame)
{
	if (m_mpClipDataRectMap.size() <= 1)
	{
		return iEndFrame;
	}
	ClipDataPositionMap::iterator itr = m_mpClipDataRectMap.upper_bound(iStartFrame);
	if (itr != m_mpClipDataRectMap.end())
	{
		if ((*itr).first <= iEndFrame)
		{
			return ((*itr).second)->m_iTimelineInPoint - 1;
		}
	}
	return iEndFrame;
}

// SingleInTrimの操作範囲にクリップが存在するかをチェック
int TrackDataInfo::CheckClipInSingleInTrimRange(int iStartFrame, int iEndFrame)
{
	if (m_mpClipDataRectMap.size() <= 1)
	{
		return iEndFrame;
	}
	ClipDataPositionMap::iterator itr = m_mpClipDataRectMap.lower_bound(iStartFrame);
	if (itr != m_mpClipDataRectMap.begin())
	{
		--itr;
		ClipDataRect* pClipData = (*itr).second;
		int iOutFrame = pClipData->m_iTimelineInPoint + pClipData->GetDuration() - 1;
		if (iEndFrame <= iOutFrame)
		{
			return iOutFrame + 1;
		}
	}
	return iEndFrame;
}



ClipDataRect* TrackDataInfo::CheckMove(ClipDataRect* pCheckClipData, const int iInPoint, const int iOutPoint)
{
	ClipDataRect* pClipData;
	int iPoint;
	pClipData = GetClipDataInfo(iInPoint, iPoint);
	if ((pClipData != nullptr) && (pClipData != pCheckClipData))
	{
		return pClipData;
	}
	pClipData = GetClipDataInfo(iOutPoint, iPoint);
	if ((pClipData != nullptr) && (pClipData != pCheckClipData))
	{
		return pClipData;
	}
	// 移動中のクリップに含まれるクリップがないかをチェック


	ClipDataPositionMap::iterator itr = m_mpClipDataRectMap.upper_bound(iInPoint);
	if (itr == m_mpClipDataRectMap.end())
	{
		return nullptr;
	}
	if (iOutPoint < (*itr).first)
	{
		return nullptr;
	}
	else
	{
		return (*itr).second;
	}

}