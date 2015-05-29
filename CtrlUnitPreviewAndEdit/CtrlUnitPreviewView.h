
// CtrlUnitPreviewView.h : CCtrlUnitPreviewView �N���X�̃C���^�[�t�F�C�X
//

#pragma once

#include "OpenGLView.h"

class TimelineEditerDialog;

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
	TimelineEditerDialog* m_dlgTimelineEditer;

public:
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // CtrlUnitPreviewView.cpp �̃f�o�b�O �o�[�W����
inline CCtrlUnitPreviewDoc* CCtrlUnitPreviewView::GetDocument() const
   { return reinterpret_cast<CCtrlUnitPreviewDoc*>(m_pDocument); }
#endif

