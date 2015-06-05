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
	m_pTransitionRect = m_pTimelineDataOperator->GetTransitionRect();

	m_pTimelineDataManager = m_pTimelineDataOperator->GetTimelineDataManager();
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManagerId);
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManagerId);
	m_pTrackDataInfoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_INFO, m_uiTrackDataInfoManagerId);
	m_pTrackDataMasterManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_MASTER, m_uiTrackDataMasterManagerId);
	m_pClipDataManager = m_pTimelineDataManager->GetClipDataManager(m_uiClipDataManagerUUID);

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
BOOL DragAndDropOperator::CheckDropFile(PCTSTR pszFileName, CString& strClipFileName, UINT& uIn, UINT& uOut, int& iFileDuration)
{
	ASSERT(uIn >= 0);
	ASSERT(uOut >= 0);
	CStdioFile sfOpenFile;
	if (!(sfOpenFile.Open(static_cast<LPCTSTR>(pszFileName), CFile::modeRead)))
	{
		return FALSE;
	}
	CString strIn, strOut, strFileDuration;
	sfOpenFile.ReadString(strClipFileName);
	sfOpenFile.ReadString(strIn);
	sfOpenFile.ReadString(strOut);
	sfOpenFile.ReadString(strFileDuration);
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
	strCheck = strFileDuration.SpanIncluding(_T("0123456789"));
	if (strCheck.Compare(strFileDuration) != 0)
	{
		return FALSE;
	}
	uIn = _ttoi(strIn);
	uOut = _ttoi(strOut);
	iFileDuration = _ttoi(strFileDuration);
	return TRUE;
}

// クリップデータの基本情報を作成する
int DragAndDropOperator::CreateClipData(ClipDataRect& pClipDataRect, PCTSTR pszClipFileName, const UINT& uIn, const UINT& uOut, const int& iFileDuration)
{
	int iDuration = uOut - uIn;
	if (iDuration < 0)
	{
		iDuration = iDuration * -1;
	}
	pClipDataRect.SetDuration(iDuration);
	pClipDataRect.SetInPoint(uIn);
	pClipDataRect.SetOutPoint();
#ifdef PROTOTYPEMODE
	pClipDataRect.SetFilePath(pszClipFileName);
#endif
	pClipDataRect.SetFileDuration(iFileDuration);
	return iDuration;
}

// ドロップされたファイルからクリップデータを作成する
BOOL DragAndDropOperator::CreateClipDataFromDropFile(TrackDataInfo& pTrackDataInfo, const UINT& uFrame, PCTSTR pszClipFileName, 
	const UINT& uIn, const UINT& uOut, const int& iFileDuration)
{
	PCTSTR pszClipInfoUUID = nullptr, pszClipRectUUID = nullptr;
	m_pClipDataManager->CreateClipData(pszClipInfoUUID, pszClipRectUUID);
	ClipDataRect* pClipDataRect = m_pClipDataManager->GetClipDataRect(pszClipRectUUID);
	int iDuration =  CreateClipData(*pClipDataRect, pszClipFileName, uIn, uOut, iFileDuration);

	SetClipDataInOutPoint(*pClipDataRect, uFrame, static_cast<UINT>(iDuration));

	pTrackDataInfo.AddClip(pClipDataRect->GetTimelineInPoint(), pClipDataRect);
	return TRUE;
}

// クリップデータのInOut点を設定する
BOOL DragAndDropOperator::SetClipDataInOutPoint(ClipDataRect& pClipDataRect, const UINT& uFrame, const UINT& uDuration)
{
	ASSERT(uDuration > 0);
	int iIn = uFrame - static_cast<UINT>(floor(uDuration / 2));
	if (iIn < 0)
	{
		pClipDataRect.SetTimelineInPoint(0);
		pClipDataRect.SetTimelineOutPoint();
		return FALSE;
	}
	pClipDataRect.SetTimelineInPoint(iIn);
	pClipDataRect.SetTimelineOutPoint();
	return TRUE;
}


