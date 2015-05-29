
// CtrlUnitPreviewView.cpp : CCtrlUnitPreviewView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
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

// CCtrlUnitPreviewView コンストラクション/デストラクション

CCtrlUnitPreviewView::CCtrlUnitPreviewView()
{
	// TODO: 構築コードをここに追加します。

}

CCtrlUnitPreviewView::~CCtrlUnitPreviewView()
{
}

BOOL CCtrlUnitPreviewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CCtrlUnitPreviewView 描画

void CCtrlUnitPreviewView::OnDraw(CDC* /*pDC*/)
{
	CCtrlUnitPreviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CCtrlUnitPreviewView 診断

#ifdef _DEBUG
void CCtrlUnitPreviewView::AssertValid() const
{
	CView::AssertValid();
}

void CCtrlUnitPreviewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCtrlUnitPreviewDoc* CCtrlUnitPreviewView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCtrlUnitPreviewDoc)));
	return (CCtrlUnitPreviewDoc*)m_pDocument;
}
#endif //_DEBUG


// CCtrlUnitPreviewView メッセージ ハンドラー

int CCtrlUnitPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (OpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	return 0;

}



void CCtrlUnitPreviewView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	if (m_dlgTimelineEditer != nullptr)
	{
		m_dlgTimelineEditer->EndDialog(0);
		delete m_dlgTimelineEditer;
	}

}


void CCtrlUnitPreviewView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	m_dlgTimelineEditer->ShowWindow(SW_SHOW);

	OpenGLView::OnRButtonUp(nFlags, point);
}


void CCtrlUnitPreviewView::OnInitialUpdate()
{

	OpenGLView::OnInitialUpdate();

	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
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
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで OpenGLView::OnPaint() を呼び出さないでください。

	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// 背景塗りつぶし
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




