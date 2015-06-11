
// CtrlUnitPreviewView.h : CCtrlUnitPreviewView �N���X�̃C���^�[�t�F�C�X
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
protected: // �V���A��������̂ݍ쐬���܂��B
	CCtrlUnitPreviewView();
	DECLARE_DYNCREATE(CCtrlUnitPreviewView)

// ����
public:
	CCtrlUnitPreviewDoc* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ����
public:
	virtual ~CCtrlUnitPreviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
	// TODO: OpenGLView�Ɉڐ�
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int);
	PFNWGLSWAPINTERVALEXTPROC m_pWglSwapIntervalEXT;

	OpenGLRect* m_pPlayButton;
	OpenGLRect* m_pStopButton;
	OpenGLRect* m_pTimelineEditorButton;

	int m_iTimelineCursorFrame;
	int m_iPlaySpeed;

	BOOL m_fPlay;
	//BOOL m_fThread;

	// �^�C���R�[�h�֘A
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

	// �Đ��@�\
	void ChangePlay(void);
	void Play(BOOL fOdd = TRUE);
	
	// TODO: OpenGLView�Ɉڐ�
	BOOL SetWglSwapIntervalEXT(void);

	// �`��֘A
	void DrawFullScreen(int iPlaySpeed, int iCount, CRect& rcRect);
	void DrawSmallScreen(CDC& dc);
	void DrawPlayButton(CDC& dc, int iHeight);
	void DrawStopButton(CDC& dc, int iHeight);
	void DrawTimelineEditorButton(CDC& dc, int iHeight);
	void SetButtonRect(void);

	// �^�C�����C���G�f�B�^�[�����֘A
	void ChangeDisplayTimelineEditor(void);
	void SetTimelineCursorFrame(int iFrame);

};

#ifndef _DEBUG  // CtrlUnitPreviewView.cpp �̃f�o�b�O �o�[�W����
inline CCtrlUnitPreviewDoc* CCtrlUnitPreviewView::GetDocument() const
   { return reinterpret_cast<CCtrlUnitPreviewDoc*>(m_pDocument); }
#endif

