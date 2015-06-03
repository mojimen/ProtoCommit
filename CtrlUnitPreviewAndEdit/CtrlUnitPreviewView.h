
// CtrlUnitPreviewView.h : CCtrlUnitPreviewView クラスのインターフェイス
//

#pragma once

#include "OpenGLView.h"

class CMainFrame;
class TimelineEditorDialog;

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
	TimelineEditorDialog* m_dlgTimelineEditor;

public:
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
private:
	CMDIFrameWnd* m_pMainWnd;
	CMainFrame* m_pMainFrame;
};

#ifndef _DEBUG  // CtrlUnitPreviewView.cpp のデバッグ バージョン
inline CCtrlUnitPreviewDoc* CCtrlUnitPreviewView::GetDocument() const
   { return reinterpret_cast<CCtrlUnitPreviewDoc*>(m_pDocument); }
#endif

