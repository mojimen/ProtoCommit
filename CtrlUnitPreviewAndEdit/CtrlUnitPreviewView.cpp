
// CtrlUnitPreviewView.cpp : CCtrlUnitPreviewView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "CtrlUnitPreviewAndEdit.h"
#endif

#include "CtrlUnitPreviewDoc.h"
#include "CtrlUnitPreviewView.h"

#include "TimelineEditerDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCtrlUnitPreviewView

IMPLEMENT_DYNCREATE(CCtrlUnitPreviewView, OpenGLView)

BEGIN_MESSAGE_MAP(CCtrlUnitPreviewView, OpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CCtrlUnitPreviewView �R���X�g���N�V����/�f�X�g���N�V����

CCtrlUnitPreviewView::CCtrlUnitPreviewView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CCtrlUnitPreviewView::~CCtrlUnitPreviewView()
{
}

BOOL CCtrlUnitPreviewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CCtrlUnitPreviewView �`��

void CCtrlUnitPreviewView::OnDraw(CDC* /*pDC*/)
{
	CCtrlUnitPreviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}


// CCtrlUnitPreviewView �f�f

#ifdef _DEBUG
void CCtrlUnitPreviewView::AssertValid() const
{
	CView::AssertValid();
}

void CCtrlUnitPreviewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCtrlUnitPreviewDoc* CCtrlUnitPreviewView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCtrlUnitPreviewDoc)));
	return (CCtrlUnitPreviewDoc*)m_pDocument;
}
#endif //_DEBUG


// CCtrlUnitPreviewView ���b�Z�[�W �n���h���[

int CCtrlUnitPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (OpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	return 0;

}



void CCtrlUnitPreviewView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	if (m_dlgTimelineEditer != nullptr)
	{
		m_dlgTimelineEditer->EndDialog(0);
		delete m_dlgTimelineEditer;
	}

}


void CCtrlUnitPreviewView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	m_dlgTimelineEditer->ShowWindow(SW_SHOW);

	OpenGLView::OnRButtonUp(nFlags, point);
}


void CCtrlUnitPreviewView::OnInitialUpdate()
{

	OpenGLView::OnInitialUpdate();

	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	m_dlgTimelineEditer = new TimelineEditerDialog();
	m_dlgTimelineEditer->Create(TimelineEditerDialog::IDD, GetDesktopWindow());
	CRect rcRect;
	CWnd* pParentWnd = GetParent();
	pParentWnd->GetWindowRect(rcRect);
	m_dlgTimelineEditer->SetPosition(rcRect);

}



// OpenGLTest





void CCtrlUnitPreviewView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� OpenGLView::OnPaint() ���Ăяo���Ȃ��ł��������B

	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// �w�i�h��Ԃ�
	::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	::glLineWidth(1);
	::glColor4f(1, 0, 1, 1);
	::glBegin(GL_QUADS);
	::glVertex2d(0, 100);
	::glVertex2d(100, 100);
	::glVertex2d(100, 0);
	::glVertex2d(0, 0);
	::glEnd();

	::glFlush();
	::SwapBuffers(m_pDC->GetSafeHdc());

	::wglMakeCurrent(NULL, NULL);
}




