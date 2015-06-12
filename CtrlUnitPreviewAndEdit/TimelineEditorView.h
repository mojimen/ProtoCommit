#pragma once

#include <map>
#include "OpenGLView.h"
#include "ConstantDefine.h"
#include "TimelineDataManager.h"

class OpenGLRect;
class TimelineDataOperator;
class TimelineDataManager;
class TrackDataManager;
class TrackDataRect;
class ClipDataRect;
class TimelineEditorDialog;

class TimecodeOperator;

typedef std::map<int, int> DisplayScaleMap;

// TimelineEditorView ビュー

class TimelineEditorView : public OpenGLView
{
	DECLARE_DYNCREATE(TimelineEditorView)

protected:
	TimelineEditorView();           // 動的生成で使用される protected コンストラクター
	virtual ~TimelineEditorView();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

private:
	// 暫定固定値
	const int kSplitterWidth = 5;					// スプリッタの幅
	const int kSplitterHeight = 5;					// スプリッタの幅
	const int kDebugInfoPanelDefaltHeight = 300;		// 未使用になる
	const int kDebugInfoPanelMinHeight = 100;			// 未使用になる

	const int kTimelineEditDefaltHeight = 300;		// タイムラインデータ表示パネルの初期高さ
	const int kTimelineEditPanelMinHeight = 100;	// タイムラインデータ表示パネルの最小高さ
	const int kTimelineEditHeaderDefaltHeight = 30;	// タイムラインデータ表示パネルヘッダー行の初期高さ
	const int kTimelineControlPanelDefaultWidth = 300;	// タイムラインデータ表示パネル見出しの初期幅

	const int kTimelineCursorDragArea = 5;					// タイムラインカーソルドラッグ時のヒット領域（左右にこの値をプラスして判定）
	const int kTimelineCursorDragGuideLineThickness = 1;	// タイムラインカーソルドラッグ時のガイドラインの太さ
	//const float kTimelineCursorDragOneSpeed = 0.1f;			// タイムラインカーソルドラッグ時の×１倍移動範囲
	//const float kTimelineCursorDragTowSpeed = 0.2f;			// タイムラインカーソルドラッグ時の×２倍移動範囲
	//const float kTimelineCursorDragFourSpeed = 0.28f;		// タイムラインカーソルドラッグ時の×４倍移動範囲
	//const float kTimelineCursorDragEightSpeed = 0.36f;		// タイムラインカーソルドラッグ時の×８倍移動範囲
	//const float kTimelineCursorDragSixteenSpeed = 0.42f;	// タイムラインカーソルドラッグ時の×１６倍移動範囲
	//const float kTimelineCursorDragThirtyTwoSpeed = 0.47f;	// タイムラインカーソルドラッグ時の×３２倍移動範囲
	//const float kTimelineCursorDragSixtyFourSpeed = 0.5f;	// タイムラインカーソルドラッグ時の×６４倍移動範囲

	BOOL m_fInitialize;
	CWnd* m_pMainWnd;
	TimelineEditorDialog* m_pParentDialog;

	//表示情報
	OpenGLRect* m_pDebugInfoPanelRect;
	OpenGLRect* m_pTimelineEditPanelRect;
	OpenGLRect* m_pTimelineEditHeaderRect;
	OpenGLRect* m_pTimelineControlPanelRect;
	OpenGLRect* m_pSeekBarRect;
	OpenGLRect* m_pTrackHeaderRect;
	OpenGLRect* m_pTimelineDataRect;
	OpenGLRect* m_pTimelineCursorHitArea;
	OpenGLRect* m_pTransitionRect;

	int m_iTimelineCursorPoint;			// タイムラインカーソルの描画位置

	ClipDataPositionMap m_mpClipMap;

	// 操作中クリップへのポインタ
	ClipDataRect* m_pOperatingClipData;

	// タイムラインデータ管理
	TimelineDataOperator* m_pTimelineDataOperator;
	UUID m_uiTimelineDataOperatorId;
	TimelineDataManager* m_pTimelineDataManager;
	UUID m_uiTimelineDataManager;
	TrackDataManager* m_pTrackDataVideoManager;
	UUID m_uiTrackDataVideoManager;
	TrackDataManager* m_pTrackDataAudioManager;
	UUID m_uiTrackDataAudioManager;
	TrackDataManager* m_pTrackDataInfoManager;
	UUID m_uiTrackDataInfoManager;
	TrackDataManager* m_pTrackDataMasterManager;
	UUID m_uiTrackDataMasterManager;

	TimecodeOperator* m_pTimecodeOperator;
	CString m_strTimecodeOperator;


	// 描画
	void DrawTimelineEditorView(const CDC& dcDc);
	void DrawTimelineControlPanel(void);
	void DrawSeekBar(const CDC& dcDC, const int& iHeight);
	void DrawBigScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine);
	void DrawMiddleScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine);
	void DrawSmallScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine);
	void DrawTrackHeader(void);
	void DrawTrack(const CDC& dcDC, const int& iHeight);
	BOOL DrawClip(const CDC& dcDC, const int& iHeight);
	int DrawClipInTrack(const CDC& dcDC, TrackDataRect* pTrackDataRect, const int& iHeight, int iClipTotalCount = 0);
	void DrawTimelineDataRect(void);
	BOOL DrawOperatingClip(const CDC& dcDC, const int& iHeight);
	BOOL DrawDragAndDropClip(const CDC& dcDC, const int& iHeight);
	BOOL DrawTimelineCursor(const CDC& dcDC, const int& iHeight);
	void TimelineEditorView::DrawJumpFrame(int iMoveFrame, int iMoveDitance);
	//void DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcShuttleLineRect, float fGuideAreaWidth);

	// 表示位置計算
	void SetPanelRect(void);
	// 初期設定
	void InitAreaRect(void);
	void InitTestObject(void);
	void CreateZoomMap(void);

	// ドラッグ＆ドロップ用
	COleDropTarget m_DropTarget;

public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(void);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	//virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnClipDelete(void);
	afx_msg void OnTransitionSetIn(void);
	afx_msg void OnTransitionSetOut(void);
	afx_msg void OnTransitionResetInCenter(void);
	afx_msg void OnTransitionResetInStart(void);
	afx_msg void OnTransitionResetInEnd(void);
	afx_msg void OnTransitionResetOutCenter(void);
	afx_msg void OnTransitionResetOutStart(void);
	afx_msg void OnTransitionResetOutEnd(void);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	void OnPlay(int iSpeed);
	void OnStop(void);
	int GetTimelineCursorFrame(void);

	void SetMainWnd(CWnd* pWnd) { m_pMainWnd = pWnd; }
	void SetParentDialog(TimelineEditorDialog* pDlg) { m_pParentDialog = pDlg; }

	afx_msg LRESULT OnNcHitTest(CPoint point);
};
