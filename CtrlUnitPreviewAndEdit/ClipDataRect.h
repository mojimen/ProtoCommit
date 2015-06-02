#pragma once
#include "OpenGLRect.h"
#include "ClipDataManager.h"

// ClipDataRect コマンド ターゲット

class ClipDataRect : public OpenGLRect
{

public:
	ClipDataRect();
	virtual ~ClipDataRect();

private:
	ClipDataTag m_eClipDataRectTag;
	UUID m_uiClipRectId;
	InfoKind m_eClipKind;
	int m_iDuration;
	//int m_iInOffset;
	//UUID m_uiMetaInfoId;
	//std::list<UUID> m_uiVideoLevelPointList;
	//std::list<UUID> m_uiAudioLevelPointList;
	//std::list<UUID> m_uiVALinkList;
	BOOL m_fLocked;
	//std::map < int, FilterInfo > m_FilterMap;	//InPoint, FilterInfo 

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
	// TODO: private化
	int m_iTimelineInPoint;
	int m_iTimelineOutPoint;

private:
	// クリップ情報データとのリンク項目
	UUID m_uiClipId;
	ClipDataInfo* m_pClipDataInfo;

	// For ProtoType




public:
	BOOL InitClipData(void);
	BOOL InitializeClipRectId(UUID& uiClipRectId);
	BOOL DeleteClipData(void);
	void DrawSingleTrimRect(int iHeight, BOOL fInTrim);
	void DrawMovingRect(int iHeight);
	void DrawOperatingOldRect(int iHeight);
	void DrawOverlappingRect(int iHeight);
	void InitializeOperatingRect(void) { m_rcOperatingRect.SetRectEmpty(); }

	// Setter
	void SetTag(ClipDataTag eTag) { m_eClipDataRectTag = eTag; }
	void SetClipRectId(UUID uiClipId) { m_uiClipRectId = uiClipId; }
	void SetClipKind(InfoKind eClipKind) { m_eClipKind = eClipKind; }
	void SetDuration(int iDuration) { m_iDuration = iDuration; }
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
	void SetClipDataInfo(const UUID& uiClipId, ClipDataInfo* pClipDataInfo) { m_pClipDataInfo = pClipDataInfo; m_uiClipId = uiClipId; }


	// Getter
	ClipDataTag GetTag(void) { return m_eClipDataRectTag; }
	UUID GetClipRectId(void) { return m_uiClipRectId; }
	InfoKind GetClipKind(void) { return m_eClipKind; }
	int GetDuration(void) { return m_iDuration; }
	UUID GetClipDataInfoId(void){ return m_uiClipId; }
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

};
