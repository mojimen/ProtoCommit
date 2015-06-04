#pragma once
#include "ClipDataInfo.h"
#include "ClipDataManager.h"
#include "DataObjectBase.h"
#include "OpenGLRect.h"

// ClipDataRect コマンド ターゲット

class ClipDataRect : public DataObjectBase, public OpenGLRect
{

public:
	ClipDataRect();
	virtual ~ClipDataRect();

private:
	//std::map < int, FilterInfo > m_FilterMap;	//InPoint, FilterInfo 

	// クリップ情報データとのリンク項目
	UUID m_uiClipInfoUUID;
	ClipDataInfo* m_pClipDataInfo;

	// For Edit
	BOOL m_fLocked;

	// ForView
	OpenGLRect m_rcOperatingRect;
	OpenGLRect m_rcOperatingBorderRect;
	float m_fOverlappingVert[4][3];
	float m_fMovingColor[4][4];			// 全クリップ共通。コントローラーで設定すべき？
	float m_fSingleInTrimColor[4][4];	// 全クリップ共通。コントローラーで設定すべき？
	float m_fSingleOutTrimColor[4][4];	// 全クリップ共通。コントローラーで設定すべき？
	float m_fOperatingOldColor[4][4];	// 全クリップ共通。コントローラーで設定すべき？
	float m_fOverlappingColor[4][4];	// 全クリップ共通。コントローラーで設定すべき？
	float m_fSingleTrimBorderColor[4][4];	// 全クリップ共通。コントローラーで設定すべき？

public:
	BOOL InitializeClipDataRect(UUID& uiClipRectId);
	BOOL DeleteClipData(void);
	void DrawSingleTrimRect(int iHeight, BOOL fInTrim);
	void DrawMovingRect(int iHeight);
	void DrawOperatingOldRect(int iHeight);
	void DrawOverlappingRect(int iHeight);
	void InitializeOperatingRect(void) { m_rcOperatingRect.SetRectEmpty(); }

	// Setter
	void SetOperatingRect(const CRect& rcRect) { m_rcOperatingRect.CopyRect(rcRect); }
	void CopyOperatingRectToOriginalRect(void) { CopyRect(m_rcOperatingRect); m_rcOperatingRect.SetRectEmpty(); }
	void SetOverlappingVert(const float fLeft, const float fTop, const float fRight, const float fBottom);
	void SetMovingColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetSingleInTrimColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetSingleOutTrimColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetOperatingOldColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetOverlappingColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetSingleTrimBorderColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetClipDataInfo(const UUID& uiClipInfoUUID, ClipDataInfo* pClipDataInfo) { m_pClipDataInfo = pClipDataInfo; m_uiClipInfoUUID = uiClipInfoUUID; }

	// ClipDataInfo Setter
	void SetFileId(UUID uiFileId) { m_pClipDataInfo->SetFileId(uiFileId); }
	void SetInPoint(int iInPoint) { m_pClipDataInfo->SetInPoint(iInPoint); }
	void SetOutPoint(int iOutPoint) { m_pClipDataInfo->SetOutPoint(iOutPoint); }
	void SetDuration(int iDuration) { m_pClipDataInfo->SetDuration(iDuration); }
	void SetInOffset(int iInOffset) { m_pClipDataInfo->SetInOffset(iInOffset); }
	void SetTimelineInPoint(int iTimelineInPoint) { m_pClipDataInfo->SetTimelineInPoint(iTimelineInPoint); }
	void SetTimelineOutPoint(int iTimelineOutPoint) { m_pClipDataInfo->SetTimelineOutPoint(iTimelineOutPoint); }
#ifdef PROTOTYPEMODE
	void SetFilePath(PCTSTR pszFilePath) { m_pClipDataInfo->SetFilePath(pszFilePath); }
#endif


	// Getter
	DataTag GetClipRectTag(void) { return m_eTag; }
	UUID GetClipRectUUID(void) { return m_uiUUID; }
	PCTSTR GetClipRectStrUUID(void) { return static_cast<PCTSTR>(m_strUUID); }
	UUID GetClipDataInfoUUID(void){ return m_uiClipInfoUUID; }
	ClipDataInfo* GetClipDataInfo(void){ return m_pClipDataInfo; }

	CRect* GetOperatingRect(void) { return &m_rcOperatingRect; }
	float(*GetOverlappingVert(void))[3] { return m_fOverlappingVert; }
	void GetOverlappingVert(float(&fVert)[4][3]);
	void GetMovingColor(float(&fColor)[4][4]);
	void GetSingleInTrimColor(float(&fColor)[4][4]);
	void GetSingleOutTrimColor(float(&fColor)[4][4]);
	void GetOperatingOldColor(float(&fColor)[4][4]);
	void GetOverlappingColor(float(&fColor)[4][4]);
	void GetSingleTrimBorderColor(float(&fColor)[4][4]);

	// ClipDataInfo Getter
	DataTag GetClipInfoTag(void) { return m_pClipDataInfo->GetTag(); }
	UUID GetClipInfoId(void) { return m_pClipDataInfo->GetClipInfoUUID(); }
	InfoKind GetClipKind(void) { return m_pClipDataInfo->GetClipKind(); }
	UUID GetFileId(void) { return m_pClipDataInfo->GetFileId(); }
	int GetInPoint(void) { return m_pClipDataInfo->GetInPoint(); }
	int GetOutPoint(void) { return m_pClipDataInfo->GetOutPoint(); }
	int GetDuration(void) { return m_pClipDataInfo->GetDuration(); }
	int GetInOffset(void) { return m_pClipDataInfo->GetInOffset(); }
	int GetTimelineInPoint(void) { return m_pClipDataInfo->GetTimelineInPoint(); }
	int GetTimelineOutPoint(void) { return m_pClipDataInfo->GetTimelineOutPoint(); }
#ifdef PROTOTYPEMODE
	CString GetFilePath(void) { return m_pClipDataInfo->GetFilePath(); }
#endif

};
