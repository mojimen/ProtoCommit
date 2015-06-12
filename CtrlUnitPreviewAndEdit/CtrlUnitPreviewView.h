
// CtrlUnitPreviewView.h : CCtrlUnitPreviewView クラスのインターフェイス
//

#pragma once

#include "OpenGLRect.h"
#include "OpenGLView.h"
#include "ConstantDefine.h"
#include "CtrlUnitPreviewDoc.h"

class CMainFrame;
class TimelineEditorDialog;
class TimecodeOperator;

class CCtrlUnitPreviewView : public OpenGLView
{
protected: // シリアル化からのみ作成します。
	CCtrlUnitPreviewView();
	DECLARE_DYNCREATE(CCtrlUnitPreviewView)

// 属性
public:
	CCtrlUnitPreviewDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CCtrlUnitPreviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


private:
	BOOL m_fInitialize;

	TimelineEditorDialog* m_dlgTimelineEditor;
	//PlaybackController* m_pPlaybackController;
	//CString m_strPlaybackControllerUUID;

	CMDIFrameWnd* m_pMainWnd;
	CMainFrame* m_pMainFrame;
	// TODO: OpenGLViewに移設
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int);
	PFNWGLSWAPINTERVALEXTPROC m_pWglSwapIntervalEXT;

	OpenGLRect* m_pPlayButton;
	OpenGLRect* m_pStopButton;
	OpenGLRect* m_pFullScreenButton;
	OpenGLRect* m_pTimelineEditorButton;

	int m_iTimelineCursorFrame;
	int m_iPlaySpeed;
	int m_iLastPlaySpeed;

	BOOL m_fPlay;
	//BOOL m_fThread;

	// タイムコード関連
	TimecodeOperator* m_pTimecodeOperator;
	CString m_strTimecodeOperator;

public:
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHideEditor(void);
	afx_msg void OnChangeTimelineCursor(void);

	void SetSmallScreen(void);
	void ChangeScreenSize(void);

	// 再生機能
	void ChangePlay(const int iNumerator = 1, const int iDenominator = 1);
	void ChangePlaySpeed(const int& iNumerator, const int& iDenominator);
	void ChangePlaySpeed(const int& iSpeed);
	void PausePlay(void);
	void Play(BOOL fOdd = TRUE);
	
	// TODO: OpenGLViewに移設
	BOOL SetWglSwapIntervalEXT(void);

	// 描画関連
	void DrawFullScreen(int iPlaySpeed, int iCount, int iFrame, CRect& rcRect);
	void DrawFullScreen(CDC& dc);
	void DrawSmallScreen(CDC& dc);
	void DrawPlayButton(CDC& dc, int iHeight);
	void DrawStopButton(CDC& dc, int iHeight);
	void DrawFullScreenButton(CDC& dc, int iHeight);
	void DrawTimelineEditorButton(CDC& dc, int iHeight);
	void SetButtonRect(void);

	// タイムラインエディター同期関連
	void ChangeDisplayTimelineEditor(void);
	void SetTimelineCursorFrame(int iFrame);

	void KeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) { OnKeyUp(nChar, nRepCnt, nFlags); }

};

#ifndef _DEBUG  // CtrlUnitPreviewView.cpp のデバッグ バージョン
inline CCtrlUnitPreviewDoc* CCtrlUnitPreviewView::GetDocument() const
   { return reinterpret_cast<CCtrlUnitPreviewDoc*>(m_pDocument); }
#endif

