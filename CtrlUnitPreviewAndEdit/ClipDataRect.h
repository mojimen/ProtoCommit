#pragma once
#include <list>
#include <map>
#include <rpc.h>

#pragma comment(lib ,"rpcrt4.lib")

#include "OpenGLRect.h"

// TODO: デバッグ用
#include <random>


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


// 標準カラー固定値
#define ACCENTCOLOR_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 1.0f
#define ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 0.5f
#define ACCENTCOLOR2_BRUSH_FLOAT 0.3f,0.4f,1.0f,1.0f
#define ACCENTCOLOR2_HALFALPHA_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.5f
#define ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.7f
#define ACCENTCOLOR3_BRUSH_FLOAT 0.8f,0.9f,1.0f,1.0f
//#define ACCENTCOLOR_BRUSH4 RGB(51,255,255)

#define BASECOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define CAUTIONCOLOR_BRUSH_FLOAT REDCOLOR_BRUSH_FLOAT


// 固有カラー固定値
#define CLIPCOLOR_BRUSH_FLOAT ACCENTCOLOR_BRUSH_FLOAT
#define CLIPBORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define CLIPCOLOR_OPERATING_BRUSH_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT
#define CLIPCOLOR_MOVING_BRUSH_FLOAT ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT
#define CLIPCOLOR_SINGLEINTRIMING_BRUSH_FLOAT ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT
#define CLIPCOLOR_SINGLEOUTTRIMING_BRUSH_FLOAT ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT
#define CLIPCOLOR_OVERLAPPING_BRUSH_FLOAT 1.0f, 0.0f, 0.0f, 0.2f
#define CLIPCOLOR_SINGLETRIMINGBORDER_BRUSH_FLOAT 1.0f, 0.7f, 0.3f, 0.7f

// 固定値
#define SINGLETRIMINGBORDER_WIDTH 10


// ClipDataRect コマンド ターゲット

struct FilterInfo
{
	int iOutPoint;
	int iDuration;
	UUID uiFilterId;
};


class ClipDataRect : public OpenGLRect
{

public:
	ClipDataRect();
	virtual ~ClipDataRect();


	enum ClipDataTag{ CLIPDATARECT };
	enum ClipKind {
		VIDEO,
		AUDIO,
		TITLE,
		MASTER_VIDEO,
		MASTER_AUDIO
	};

private:
	ClipDataTag m_eClipDataRectTag;
	UUID m_uiClipRectId;
	ClipKind m_eClipKind;
	//UUID m_uiFileId;
	//int m_iInPoint;
	//int m_iOutPoint;
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

	// For ProtoType
	//CString strFilePath;

public:
	// TODO: private化
	int m_iTimelineInPoint;
	int m_iTimelineOutPoint;

	BOOL InitClipData(void);


public:

	BOOL InitializeClipRectId(UUID& uiClipRectId);
	void DrawSingleTrimRect(int iHeight, BOOL fInTrim);
	void DrawMovingRect(int iHeight);
	void DrawOperatingOldRect(int iHeight);
	void DrawOverlappingRect(int iHeight);
	void InitializeOperatingRect(void) { m_rcOperatingRect.SetRectEmpty(); }

	// Setter
	void SetTag(ClipDataTag eTag) { m_eClipDataRectTag = eTag; }
	void SetClipId(UUID uiClipId) { m_uiClipRectId = uiClipId; }
	void SetClipKind(ClipKind eClipKind) { m_eClipKind = eClipKind; }
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

	// Getter
	ClipDataTag GetTag(void) { return m_eClipDataRectTag; }
	UUID GetClipId(void) { return m_uiClipRectId; }
	ClipKind GetClipKind(void) { return m_eClipKind; }
	int GetDuration(void) { return m_iDuration; }
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
