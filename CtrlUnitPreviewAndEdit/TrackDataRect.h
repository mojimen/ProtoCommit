#pragma once

#include <list>
#include <map>
#include <rpc.h>
#pragma comment(lib ,"rpcrt4.lib")

#include "OpenGLRect.h"

// 共通カラー
#define BLACKCOLOR_BRUSH_FLOAT 0.0f, 0.0f, 0.0f, 1.0f
#define WHITECOLOR_BRUSH_FLOAT 1.0f, 1.0f, 1.0f, 1.0f
#define REDCOLOR_BRUSH_FLOAT 1.0f, 0.0f, 0.0f, 1.0f
#define BLUECOLOR_BRUSH_FLOAT 0.0f, 0.0f, 1.0f, 1.0f
#define GREENCOLOR_BRUSH_FLOAT 0.0f, 1.0f, 0.0f, 1.0f
#define DARKGRAYCOLOR_BRUSH_FLOAT 0.3f, 0.3f, 0.3f, 1.0f
#define DARKGRAYCOLOR2_BRUSH_FLOAT 0.2f, 0.2f, 0.2f, 1.0f
#define DARKGRAYCOLOR3_BRUSH_FLOAT 0.1f, 0.1f, 0.1f, 1.0f
#define GRAYCOLOR_BRUSH_FLOAT 0.5f, 0.5f, 0.5f, 1.0f
#define LIGHTGRAYCOLOR_BRUSH_FLOAT 0.7f, 0.7f, 0.7f, 1.0f
#define LIGHTGRAYCOLOR2_BRUSH_FLOAT 0.8f, 0.8f, 0.8f, 1.0f
#define LIGHTGRAYCOLOR3_BRUSH_FLOAT 0.9f, 0.9f, 0.9f, 1.0f

#define ACCENTCOLOR_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 1.0f
#define ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 0.5f
#define ACCENTCOLOR2_BRUSH_FLOAT 0.3f,0.4f,1.0f,1.0f
#define ACCENTCOLOR2_HALFALPHA_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.5f
#define ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.7f
#define ACCENTCOLOR3_BRUSH_FLOAT 0.8f,0.9f,1.0f,1.0f
//#define ACCENTCOLOR_BRUSH4 RGB(51,255,255)

#define BASECOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define CAUTIONCOLOR_BRUSH_FLOAT REDCOLOR_BRUSH_FLOAT

// 固有カラー
#define TIMELINETRACKCOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH2_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH2_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT


class TrackDataInfo;

// TrackDataRect コマンド ターゲット


class TrackDataRect : public OpenGLRect
{
public:
	TrackDataRect();
	virtual ~TrackDataRect();

	enum TrackDataTag{ TRACKDATARECT };
	enum TrackKind {
		VIDEO,
		AUDIO,
		TITLE,
		INFO,
		MASTER_VIDEO,
		MASTER_AUDIO
	};

private:
	TrackDataTag m_eTrackDataInfoTag;
	UUID m_uiTrackRectId;
	CString m_strTrackName;
	int m_iHeight;

	// 編集時参照項目
	BOOL m_fLocked;
	BOOL m_fGapless;
	// 画面表示参照項目
	BOOL m_fDisplayOverlay;
	BOOL m_fDisplayLevelPoint;

	// データとのリンク項目
	UUID m_uiTrackId;
	TrackDataInfo* m_pTrackDataInfo;

public:
	BOOL InitTrackData(void);
	BOOL InitializeTrackRectId(UUID& uiClipId);

	// Getter
	int GetHeight(void) { return m_iHeight; }
	PCTSTR GetTrackName(void) { return static_cast<PCTSTR>(m_strTrackName); }
	UUID GetTrackId(void) { return m_uiTrackId; }
	TrackDataInfo* GetTrackDataInfo(void) { return m_pTrackDataInfo; }

	// Setter
	void SetTrackName(PCTSTR pszTrackName) { m_strTrackName = static_cast<CString>(pszTrackName); }
	void SetHeight(int iHeight) { m_iHeight = iHeight; }
	void SetTrackId(UUID uiTrackId) { m_uiTrackId = uiTrackId; }
	void SetTrackDataInfo(TrackDataInfo* pTrackDataInfo) { m_pTrackDataInfo = pTrackDataInfo; }

};
