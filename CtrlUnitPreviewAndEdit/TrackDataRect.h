#pragma once

#include <list>
#include <map>
#include <rpc.h>
#pragma comment(lib ,"rpcrt4.lib")

#include "OpenGLRect.h"
#include "TrackDataManager.h"

class TrackDataInfo;

// TrackDataRect コマンド ターゲット


class TrackDataRect : public OpenGLRect
{
public:
	TrackDataRect();
	virtual ~TrackDataRect();


private:
	TrackDataTag m_eTrackDataRectTag;
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
	void SetHeight(const int iHeight) { m_iHeight = iHeight; }
	void SetTrackId(const UUID& uiTrackId) { m_uiTrackId = uiTrackId; }
	void SetTrackDataInfo(const UUID& uiTrackId, TrackDataInfo* pTrackDataInfo) { m_pTrackDataInfo = pTrackDataInfo; m_uiTrackId = uiTrackId; }

};
