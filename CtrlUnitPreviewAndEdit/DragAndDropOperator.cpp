// DragAndDropOperator.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "DragAndDropOperator.h"

#include "TimelineDataOperator.h"
#include "TimelineDataManager.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
#include "ClipDataManager.h"
#include "ClipDataRect.h"
#include "ClipDataInfo.h"

// DragAndDropOperator

DragAndDropOperator::DragAndDropOperator()
{
}

DragAndDropOperator::~DragAndDropOperator()
{
}

BOOL DragAndDropOperator::Initialize(UUID& uiDragAndDropOperatorId)
{
	m_pTimelineEditPanelRect = m_pTimelineDataOperator->GetTimelineEditPanelRect();
	m_pTimelineEditHeaderRect = m_pTimelineDataOperator->GetTimelineEditHeaderRect();
	m_pTimelineControlPanelRect = m_pTimelineDataOperator->GetTimelineControlPanelRect();
	m_pSeekBarRect = m_pTimelineDataOperator->GetSeekBarRect();
	m_pTrackHeaderRect = m_pTimelineDataOperator->GetTrackHeaderRect();
	m_pTimelineDataRect = m_pTimelineDataOperator->GetTimelineDataRect();
	m_pTimelineCursorHitArea = m_pTimelineDataOperator->GetTimelineCursorHitArea();
	m_pTransisionRect = m_pTimelineDataOperator->GetTransisionRect();

	m_pTimelineDataManager = m_pTimelineDataOperator->GetTimelineDataManager();
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManagerId);
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManagerId);
	m_pTrackDataInfoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_INFO, m_uiTrackDataInfoManagerId);
	m_pTrackDataMasterManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_MASTER, m_uiTrackDataMasterManagerId);
	m_pClipDataManager = m_pTimelineDataManager->GetClipDataManager(m_uiClipDataManagerId);

	if (RPC_S_OK == UuidCreate(&m_uiDragAndDropOperatorId))
	{
		uiDragAndDropOperatorId = m_uiDragAndDropOperatorId;
		return TRUE;
	}
	return FALSE;
}

// DragAndDropOperator メンバー関数

// ドロップ先のトラックを取得する
TrackDataRect* DragAndDropOperator::GetDropTrack(const POINT& point)
{
	if (m_pTimelineDataRect->PtInRect(point))
	{
		return m_pTimelineDataOperator->IsPointInAnyTrack(point);
	}
	return nullptr;
}

// ファイル拡張子チェック
BOOL DragAndDropOperator::CheckFileNameExtension(const CString& strFileName)
{
	int iHit = strFileName.ReverseFind('.');
	if (iHit > 0)
	{
		CString strExt = strFileName.Mid(iHit);
		if (strExt.CompareNoCase(_T(".txt")) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// ドロップされたファイルの形式をチェックする
BOOL DragAndDropOperator::CheckDropFile(PCTSTR pszFileName, CString& strClipFileName, UINT& uIn, UINT& uOut)
{
	CStdioFile sfOpenFile;
	if (!(sfOpenFile.Open(static_cast<LPCTSTR>(pszFileName), CFile::modeRead)))
	{
		return FALSE;
	}
	CString strIn, strOut;
	sfOpenFile.ReadString(strClipFileName);
	sfOpenFile.ReadString(strIn);
	sfOpenFile.ReadString(strOut);
	sfOpenFile.Close();

	if ((strClipFileName.IsEmpty()) || (strIn.IsEmpty()) || (strOut.IsEmpty()))
	{
		return FALSE;
	}
	CString strCheck = strIn.SpanIncluding(_T("0123456789"));
	if (strCheck.Compare(strIn) != 0)
	{
		return FALSE;
	}
	strCheck = strOut.SpanIncluding(_T("0123456789"));
	if (strCheck.Compare(strOut) != 0)
	{
		return FALSE;
	}
	uIn = _ttoi(strIn);
	uOut = _ttoi(strOut);
	return TRUE;
}

// クリップデータの基本情報を作成する
int DragAndDropOperator::CreateClipData(ClipDataRect& pClipDataRect, PCTSTR pszClipFileName, const UINT& uIn, const UINT& uOut)
{
	ClipDataInfo* pClipDataInfo = pClipDataRect.GetClipDataInfo();
	int iDuration = uOut - uIn;
	if (iDuration < 0)
	{
		iDuration = iDuration * -1 + 1;
	}
	else
	{
		++iDuration;
	}
	pClipDataRect.SetDuration(iDuration);
	pClipDataInfo->SetInPoint(uIn);
	pClipDataInfo->SetOutPoint(uOut);
	pClipDataInfo->SetDuration(iDuration);
#ifdef PROTOTYPEMODE
	pClipDataInfo->SetFilePath(pszClipFileName);
#endif
	return iDuration;
}

// ドロップされたファイルからクリップデータを作成する
BOOL DragAndDropOperator::CreateClipDataFromDropFile(TrackDataInfo& pTrackDataInfo, const UINT& uFrame, PCTSTR pszClipFileName, const UINT& uIn, const UINT& uOut)
{
	UUID uiClipId, uiClipRectId;
	m_pClipDataManager->CreateClipData(uiClipId, uiClipRectId);
	ClipDataRect* pClipDataRect = m_pClipDataManager->GetClipDataRect(uiClipRectId);
	int iDuration =  CreateClipData(*pClipDataRect, pszClipFileName, uIn, uOut);

	SetClipDataInOutPoint(*pClipDataRect, uFrame, static_cast<UINT>(iDuration));

	pClipDataRect->m_iTimelineInPoint = uFrame - static_cast<int>(floor(iDuration / 2));
	pClipDataRect->m_iTimelineOutPoint = uFrame + static_cast<int>(ceil(iDuration / 2)) - 1;
	pTrackDataInfo.AddClip(pClipDataRect->m_iTimelineInPoint, pClipDataRect);
	return TRUE;
}

// クリップイデータのInOut点を設定する
BOOL DragAndDropOperator::SetClipDataInOutPoint(ClipDataRect& pClipDataRect, const UINT& uFrame, const UINT& uDuration)
{
	ASSERT(uDuration > 0);
	pClipDataRect.m_iTimelineInPoint = uFrame - static_cast<int>(floor(uDuration / 2));
	pClipDataRect.m_iTimelineOutPoint = uFrame + static_cast<int>(ceil(uDuration / 2)) - 1;
	if (pClipDataRect.m_iTimelineInPoint < 0)
	{
		pClipDataRect.m_iTimelineInPoint = 0;
		pClipDataRect.m_iTimelineOutPoint = uDuration - 1;
		return FALSE;
	}
	return TRUE;
}


