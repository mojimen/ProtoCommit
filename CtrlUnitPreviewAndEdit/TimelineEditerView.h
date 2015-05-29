#pragma once

#include <map>
#include "OpenGLView.h"

class OpenGLRect;
class ClipDataRect;
class TrackDataInfo;
class TrackDataRect;

typedef std::map<int, int> DisplayScaleMap;

// TimelineEditerView ビュー

class TimelineEditerView : public OpenGLView
{
	DECLARE_DYNCREATE(TimelineEditerView)

protected:
	TimelineEditerView();           // 動的生成で使用される protected コンストラクター
	virtual ~TimelineEditerView();

public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);




private:
	// 動作モード
#define PROTOTYPEMODE
#define SEEKBAR_MIDDLESCALELINE_DRAW
#define SEEKBAR_SMALLSCALELINE_DRAW

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
#define ACCENTCOLOR4_BRUSH_FLOAT 0.5f,0.8f,1.0f,1.0f

#define BASECOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define CAUTIONCOLOR_BRUSH_FLOAT REDCOLOR_BRUSH_FLOAT

	// 固有カラー
#define PREVIEWPANELBORDER_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define TIMELINEBASECOLOR_BRUSH_FLOAT BASECOLOR_BRUSH_FLOAT

#define TIMELINECONTROLPANELBACKGROUNDCOLOR_BRUSH_FLOAT ACCENTCOLOR2_BRUSH_FLOAT
#define SEEKBARBACKGROUNDCOLOR_BRUSH_FLOAT ACCENTCOLOR2_BRUSH_FLOAT
#define TIMELINETRACKHEADERBORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH2_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH2_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINEDATAAREABORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define SEEKBARBIGSCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define SEEKBARMIDDLESCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define SEEKBARSMALLSCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT

#define SEEKBARBIGSCALELINECOLOR_BRUSH_FLOAT SEEKBARBIGSCALECOLOR_BRUSH_FLOAT
#define SEEKBARMIDDLESCALELINECOLOR_BRUSH_FLOAT 1.0f, 0.8f, 1.0f, 1.0f
#define SEEKBARSMALLSCALELINECOLOR_BRUSH_FLOAT 1.0f, 1.0f, 0.9f, 1.0f

#define TIMELINECURSORCOLOR_BRUSH_FLOAT 1.0f, 0.0f, 1.0f, 0.6f
#define TIMELINECURSORDRAGGUIDELINECOLOR_BRUSH ACCENTCOLOR_BRUSH



	// 文字種別／サイズ
#define FONTFACE_MSMINCHOU _T("ＭＳ 明朝")
#define FONTFACE_MSGOTHIC _T("ＭＳ ゴシック")
#define FONTFACE_MEIRIO _T("メイリオ")
#define DEFAULT_FONTFACE FONTFACE_MSGOTHIC
#define SEEKBARTIMECODE_FONTSIZE 15
#define SEEKBARTIMECODE_MIDDLE_FONTSIZE 12

	// 線
#define PREVIEWPANELBORDER_LINEWIDTH 1.0f
#define SEEKBARBIGSCALE_LINEWIDTH 1.0f
#define SEEKBARMIDDLESCALE_LINEWIDTH 1.0f
#define SEEKBARSMALLSCALE_LINEWIDTH 1.0f
#define SEEKBARBIGSCALELINE_LINEWIDTH SEEKBARBIGSCALE_LINEWIDTH
#define SEEKBARMIDDLESCALELINE_LINEWIDTH SEEKBARMIDDLESCALE_LINEWIDTH
#define SEEKBARSMALLSCALELINE_LINEWIDTH SEEKBARSMALLSCALE_LINEWIDTH
#define TIMELINECURSOR_LINEWIDTH 1.0f


	//マージン、位置補正
#define SEEKBARTIMECODE_MARGINX 2
#define SEEKBARTIMECODE_MARGINY 16
#define SEEKBARTIMECODE_MIDDLE_MARGINY 17
#define SEEKBARBIGSCALE_TOPMARGIN 10
#define SEEKBARMIDDLESCALE_TOPMARGIN 20
#define SEEKBARSMALLSCALE_TOPMARGIN 25
#define TIMELINE_DEFAULTZ 0.0f
#define CLIPHIGHT 0.9f


	const int kSplitterWidth = 5;					// スプリッタの幅
	const int kSplitterHeight = 5;					// スプリッタの幅
	const double kSlideTrimAreaRate = 0.1;			// スライドトリムエリアとして使用する比率
	const double kTrimAreaRate = 0.2;				// トリムエリアとして使用する比率
	const int kClipHitCheckMinWidth = 15;			// クリップ当たり判定の最小保証幅（これよりクリップが短い場合に使用する幅）
	const int kTrimHitCheckMinWidth = 5;			// トリムチェックで使用する最小幅（これよりトリムチェック範囲が短い場合に使用する幅）
	const int kTrimHitCheckMaxWidth = 30;			// トリムチェックで使用する最大幅（これよりトリムチェック範囲が長い場合に使用する幅）
	const int kPreviewPanelDefaltHeight = 300;		// 未使用になる
	const int kPreviewPanelMinHeight = 100;			// 未使用になる

	const int kTimelineEditDefaltHeight = 300;		// タイムラインデータ表示パネルの初期高さ
	const int kTimelineEditPanelMinHeight = 100;	// タイムラインデータ表示パネルの最小高さ
	const int kTimelineEditHeaderDefaltHeight = 30;	// タイムラインデータ表示パネルヘッダー行の初期高さ
	const int kTimelineControlPanelDefaultWidth = 300;	// タイムラインデータ表示パネル見出しの初期幅

	const int kTrackDefaultHeight = 100;			// トラックの初期高さ（TODO: トラックマネージャーに移動すべき）
	const int kTrackMinHeight = 10;					// トラックの最小高さ
	const int kTrackMaxHeight = 1000;				// トラックの最大高さ


	const int kTimelineCursorDragArea = 5;					// タイムラインカーソルドラッグ時のヒット領域（左右にこの値をプラスして判定）
	const int kTimelineCursorDragGuideLineThickness = 1;	// タイムラインカーソルドラッグ時のガイドラインの太さ
	const float kTimelineCursorDragOneSpeed = 0.1f;			// タイムラインカーソルドラッグ時の×１倍移動範囲
	const float kTimelineCursorDragTowSpeed = 0.2f;			// タイムラインカーソルドラッグ時の×２倍移動範囲
	const float kTimelineCursorDragFourSpeed = 0.28f;		// タイムラインカーソルドラッグ時の×４倍移動範囲
	const float kTimelineCursorDragEightSpeed = 0.36f;		// タイムラインカーソルドラッグ時の×８倍移動範囲
	const float kTimelineCursorDragSixteenSpeed = 0.42f;	// タイムラインカーソルドラッグ時の×１６倍移動範囲
	const float kTimelineCursorDragThirtyTwoSpeed = 0.47f;	// タイムラインカーソルドラッグ時の×３２倍移動範囲
	const float kTimelineCursorDragSixtyFourSpeed = 0.5f;	// タイムラインカーソルドラッグ時の×６４倍移動範囲


	//表示情報
private:
	OpenGLRect* m_prcPreviewPanelRect;
	OpenGLRect* m_prcTimelineEditPanelRect;
	OpenGLRect* m_prcTimelineEditHeaderRect;
	OpenGLRect* m_prcTimelineControlPanelRect;
	OpenGLRect* m_prcSeekBarRect;
	OpenGLRect* m_prcTrackHeaderRect;
	OpenGLRect* m_prcTimelineDataRect;
	OpenGLRect* m_prcTimelineCursorHitArea;
	OpenGLRect* m_prcTransisionRect;

	int m_iTimelineCursorFramePosition;	// タイムラインカーソル位置のフレーム番号
	int m_iLeftFrameNumber;				// タイムラインデータ表示範囲の先頭フレーム
	int m_iRightFrameNumber;			// タイムラインデータ表示範囲の最終フレーム
	int m_iOperatingFrameCount;			// 操作中の移動フレーム数
	int m_iOperatingTimelineCursorFramePosition;	// タイムラインカーソル位置のフレーム番号
	int m_iOperatingLeftFrameNumber;	// タイムラインデータ表示範囲の先頭フレーム
	int m_iOperatingRightFrameNumber;	// タイムラインデータ表示範囲の最終フレーム
	int m_iOperatingClipFrameCount;		// 操作中クリップの移動／伸縮フレーム数
	int m_fSuttleSpeed;					// シャトル操作中の移動スピード（倍）
	int m_iFramePerPoint;				// １ポイントあたりのフレーム数
	int m_iPointPerFrame;				// １フレームあたりのポイント数
	int m_iSmallScaleDrawInterval;		// 小目盛りあたりのフレーム数
	int m_iMiddleScaleDrawInterval;		// 中目盛りあたりの小目盛り数
	int m_iBigScaleDrawInterval;		// 大目盛りあたりの中目盛り数
	DisplayScaleMap m_mpScalePoint;	// 表示倍率変更情報検索用
	DisplayScaleMap m_mpScaleFrame;	// 表示倍率変更情報検索用
	int m_iDisplayScaleArray[2][30][4];		// 表示倍率変更情報
	//0: m_iSmallScaleDrawInterval加減算値
	//1: m_iMiddleScaleDrawInterval加減算値
	//2: m_iBigScaleDrawInterval加減算値
	//3: m_iPointPerFrame/m_iFramePerPoint加減算値
	int m_iTimelineCursorPoint;			// タイムラインカーソルの描画位置

	void SetPanelRect(void);
	BOOL CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	BOOL CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	BOOL ChangeDisplayScale(void);
	int ChangeTimelineFramePositionToDisplayPoint(const int iFrame);
	int ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame);
	int ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame = 0);

	// 仮想クリップ
	ClipDataRect* m_clClipData1;
	ClipDataRect* m_clClipData2;
	ClipDataRect* m_clMovingClipData;
	ClipDataRect* m_clStaticClipData;
	// 仮想トラック
	TrackDataRect* m_clTrack1;
	TrackDataRect* m_clTrack2;
	TrackDataInfo* m_clTrackInfo1;
	TrackDataInfo* m_clTrackInfo2;
	TrackDataRect* m_clSelectedTrack;
	TrackDataRect* m_clOperateToTrack;
	TrackDataInfo* m_clSelectedTrackInfo;
	TrackDataInfo* m_clOperateToTrackInfo;

	// 図形ドラッグ用
private:
	CRect m_rcMousePointRect;		// 移動中のイメージ（重なり中のイメージ）
	CPoint m_poMousePointerLocation; // マウスボタンが押されたときの位置
	BOOL m_fLButtonClicking;	// マウスボタンが押されているかどうかを記録
	BOOL m_fMoving;				// Move操作中
	BOOL m_fSingleInTriming;	// In側SingleTrim操作中
	BOOL m_fSingleOutTriming;	// Out側SingleTrim操作中
	BOOL m_fScrubing;			// Scrub操作中
	BOOL m_fDragShuttling;		// DragShuttle操作中

	TrackDataRect* IsPointInAnyTrack(const CPoint& point);
	BOOL IsPointInAnyClipRect(const CPoint& point);
	BOOL IsPointInClipRect(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect);
	BOOL CheckInTrim(void);
	BOOL CheckOutTrim(void);
	BOOL CheckMove(CPoint& point);

	BOOL IsPointInSeekBar(const CPoint& point);
	BOOL IsPointInTimelineControlPanel(const CPoint& point);
	//float SetShuttleSpeed(const CPoint& point, CSize& szMoveSize);
	//float SetShuttleSpeedByMoveLength(int iMoveLength);

	// 描画

	void DrawTimelineControlPanel(void);
	void DrawSeekBar(const CDC& dcPaintDC, const int iHeight);
	void DrawBigScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawMiddleScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawSmallScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawTrackHeader(void);
	void DrawTrack(void);
	BOOL DrawClip(const int iHeight);
	void DrawClipInTrack(TrackDataRect* pTrackDataRect, const int iHeight);
	void DrawTimelineDataRect(void);
	BOOL DrawOperatingClip(const CDC& dcPaintDC, const int iHeight);
	BOOL DrawTimelineCursor(const CDC& dcPaintDC, const int iHeight);
	//void DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcShuttleLineRect, float fGuideAreaWidth);
	//void DrawAnimation(const int iFrame);


public:

private:
	// 初期設定
	void InitAreaRect(void);
	void InitTestObject(void);
	void CreateZoomMap(void);

	// 表示可能なタイムラインの範囲（左端、右端）を計算する
	void CalcTimelineDisplayRange();
};
