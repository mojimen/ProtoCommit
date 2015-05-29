// TrackDataRect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TrackDataRect.h"
//#include "ClipDataRect.h"


// TrackDataRect

TrackDataRect::TrackDataRect()
{
}

TrackDataRect::~TrackDataRect()
{
}


// TrackDataRect メンバー関数

BOOL TrackDataRect::InitTrackData(void)
{
	SetColor(TIMELINETRACKCOLOR_BRUSH_FLOAT, TIMELINETRACKCOLOR_BRUSH_FLOAT, TIMELINETRACKCOLOR_BRUSH2_FLOAT, TIMELINETRACKCOLOR_BRUSH2_FLOAT);
	SetBorderColor(TIMELINETRACKBORDERCOLOR_BRUSH2_FLOAT, TIMELINETRACKBORDERCOLOR_BRUSH_FLOAT, TIMELINETRACKBORDERCOLOR_BRUSH2_FLOAT, TIMELINETRACKBORDERCOLOR_BRUSH_FLOAT);
	return TRUE;
}

BOOL TrackDataRect::InitializeTrackRectId(UUID& uiClipId)
{
	if (RPC_S_OK == UuidCreate(&m_uiTrackRectId))
	{
		uiClipId = m_uiTrackRectId;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

