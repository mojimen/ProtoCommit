
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

#include "TimelineEditorDialog.h"
//#include "PlaybackController.h"
#include "TimecodeOperator.h"

#include "MainFrm.h"

#include <windows.h>
#include <mmsystem.h>
#include <random>
#include <thread>
#pragma comment(lib,"winmm.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCtrlUnitPreviewView

IMPLEMENT_DYNCREATE(CCtrlUnitPreviewView, OpenGLView)

BEGIN_MESSAGE_MAP(CCtrlUnitPreviewView, OpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_HIDE_EDITOR, &CCtrlUnitPreviewView::OnHideEditor)
	ON_COMMAND(ID_CHANGE_TIMELINECURSOR, &CCtrlUnitPreviewView::OnChangeTimelineCursor)
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CCtrlUnitPreviewView コンストラクション/デストラクション

CCtrlUnitPreviewView::CCtrlUnitPreviewView()
{
	// TODO: 構築コードをここに追加します。
	m_pMainWnd = nullptr;
	m_pMainFrame = nullptr;
	m_pWglSwapIntervalEXT = nullptr;
	m_dlgTimelineEditor = nullptr;
	//m_pPlaybackController = nullptr;
	m_pPlayButton = nullptr;
	m_pStopButton = nullptr;
	m_pFullScreenButton = nullptr;
	m_pTimelineEditorButton = nullptr;
	m_fPlay = FALSE;
	//m_fThread = FALSE;
	m_fInitialize = FALSE;
	m_iTimelineCursorFrame = 0;
	m_iPlaySpeed = 0;
}

CCtrlUnitPreviewView::~CCtrlUnitPreviewView()
{
}

BOOL CCtrlUnitPreviewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return OpenGLView::PreCreateWindow(cs);
}

// CCtrlUnitPreviewView 描画
void CCtrlUnitPreviewView::OnDraw(CDC* pDC)
{
	CCtrlUnitPreviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	
	if ((!m_pMainFrame->GetScreenMode()) && !m_fPlay)
	{
		DrawSmallScreen(*pDC);
	}
	else
	{
		DrawFullScreen(*pDC);
	}
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

// 初期設定
int CCtrlUnitPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (OpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	DWORD dwExStyle = GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
	dwExStyle &= ~WS_EX_WINDOWEDGE;
	dwExStyle &= ~WS_EX_CLIENTEDGE;
	SetWindowLong(this->m_hWnd, GWL_EXSTYLE, dwExStyle);

	m_pMainWnd = static_cast<CMDIFrameWnd *>(AfxGetMainWnd());
	m_pMainFrame = static_cast<CMainFrame *>(AfxGetMainWnd());

	SetWglSwapIntervalEXT();
	//m_pPlaybackController = new PlaybackController();
	//m_pPlaybackController->InitializePlaybackController(m_pDC, &m_hRC, m_strPlaybackControllerUUID);

	timeBeginPeriod(1);

	return 0;

}

// 終了処理
void CCtrlUnitPreviewView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_fPlay = FALSE;
	timeEndPeriod(1);

	if (m_dlgTimelineEditor != nullptr)
	{
		m_dlgTimelineEditor->EndDialog(0);
		m_dlgTimelineEditor->DestroyWindow();
		delete m_dlgTimelineEditor;
	}
	if (m_pTimecodeOperator != nullptr)
	{
		delete m_pTimecodeOperator;
	}

	if (m_pPlayButton)
	{
		delete m_pPlayButton;
	}
	if (m_pStopButton)
	{
		delete m_pStopButton;
	}
	if (m_pFullScreenButton)
	{
		delete m_pFullScreenButton;
	}
	if (m_pTimelineEditorButton)
	{
		delete m_pTimelineEditorButton;
	}

	//if (m_pPlaybackController != nullptr)
	//{
	//	delete m_pPlaybackController;
	//}

}

// ウィンドウサイズ変更
void CCtrlUnitPreviewView::OnSize(UINT nType, int cx, int cy)
{
	OpenGLView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	if (m_fInitialize)
	{
		SetButtonRect();
	}
}

// 初期設定
void CCtrlUnitPreviewView::OnInitialUpdate()
{

	OpenGLView::OnInitialUpdate();

	m_fInitialize = TRUE;

	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	m_dlgTimelineEditor = new TimelineEditorDialog(m_pMainWnd);
	m_dlgTimelineEditor->Create(TimelineEditorDialog::IDD, GetDesktopWindow());
	CRect rcRect;
	m_pMainWnd->GetWindowRect(rcRect);
	m_dlgTimelineEditor->SetPosition(rcRect);
	m_dlgTimelineEditor->SetMainView(this);

	m_pPlayButton = new OpenGLRect();
	m_pStopButton = new OpenGLRect();
	m_pFullScreenButton = new OpenGLRect();
	m_pTimelineEditorButton = new OpenGLRect();
	m_pPlayButton->SetBothColor(PLAYBUTTON_BRUSH, PLAYBUTTON_BRUSH, PLAYBUTTON_BRUSH, PLAYBUTTON_BRUSH);
	m_pStopButton->SetBothColor(STOPBUTTON_BRUSH, STOPBUTTON_BRUSH, STOPBUTTON_BRUSH, STOPBUTTON_BRUSH);
	m_pFullScreenButton->SetBothColor(FULLSCREENBUTTON_BRUSH, FULLSCREENBUTTON_BRUSH, FULLSCREENBUTTON_BRUSH, FULLSCREENBUTTON_BRUSH);
	m_pTimelineEditorButton->SetBothColor(TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH);
	SetButtonRect();

	m_pTimecodeOperator = new TimecodeOperator();
	m_pTimecodeOperator->InitializeTimecodeOperator(m_strTimecodeOperator);
}



// Ｌクリック
void CCtrlUnitPreviewView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if (m_pMainFrame->GetScreenMode())
	{
		return;
	}

	if (m_pPlayButton->PtInRect(point))
	{
		ChangePlay();
	}
	else if (m_pStopButton->PtInRect(point))
	{
		// 実際は機能なし
		m_fPlay = FALSE;
	}
	else if (m_pFullScreenButton->PtInRect(point))
	{
		ChangeScreenSize();
	}
	else if (m_pTimelineEditorButton->PtInRect(point))
	{
		ChangeDisplayTimelineEditor();
	}
}

// Ｒクリック
void CCtrlUnitPreviewView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if ((m_pPlayButton->PtInRect(point)) || (m_pStopButton->PtInRect(point)) || (m_pFullScreenButton->PtInRect(point)))
	{
		return;
	}
	ChangeDisplayTimelineEditor();
}

// Ｌボタンダブルクリック
void CCtrlUnitPreviewView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if (!m_fPlay)
	{
		if ((m_pTimelineEditorButton->PtInRect(point)) || (m_pPlayButton->PtInRect(point)) || (m_pStopButton->PtInRect(point)) || (m_pFullScreenButton->PtInRect(point)))
		{
			// ボタン上は無効
			return;
		}
	}

	// 再生開始
	ChangePlay();
}

// キー操作
void CCtrlUnitPreviewView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	switch (nChar)
	{

	case VK_SPACE:
		if (!m_fPlay)
		{
			ChangePlay();
		}
		else
		{
			PausePlay();
		}
		break;

	case VK_RETURN:
		ChangePlay();
		break;

	case VK_ESCAPE:
		SetSmallScreen();
		if (m_fPlay)
		{
			ChangePlay();
		}
		break;

	case VK_PRIOR:
		if (m_fPlay)
		{
			ChangePlaySpeed(1);
		}
		break;

	case VK_NEXT:
		if (m_fPlay)
		{
			ChangePlaySpeed(-1);
		}
		break;

	case 'T':
		ChangeDisplayTimelineEditor();
		break;

	default:
		break;

	}

	OpenGLView::OnKeyUp(nChar, nRepCnt, nFlags);
}



// タイムラインエディター閉じるボタン押下
void CCtrlUnitPreviewView::OnHideEditor(void)
{
	Invalidate();
}

// タイムラインカーソル位置同期
void CCtrlUnitPreviewView::OnChangeTimelineCursor(void)
{
	m_iTimelineCursorFrame = m_dlgTimelineEditor->GetTimelineCursorFrame();
	Invalidate();
}




// OpenGLTest

// 全画面表示終了
void CCtrlUnitPreviewView::SetSmallScreen(void)
{
	if (m_pMainFrame->GetScreenMode())
	{
		m_pMainWnd->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_FULLSCREEN, 0), NULL);
	}
}

// 画面サイズ切り替え
void CCtrlUnitPreviewView::ChangeScreenSize(void)
{
	m_pMainWnd->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_FULLSCREEN, 0), NULL);
}

// 再生状態の切り替え
void CCtrlUnitPreviewView::ChangePlay(const int iNumerator /*= 1*/, const int iDenominator /*= 1*/)
{
	// ウィンドウサイズとフラグの切り替え
	m_fPlay = !m_fPlay;
	if (!m_fPlay)
	{
		// 停止時は少し待つ（スレッドの描画とかち合わないように）
		Sleep(100);
	}
	Invalidate();

	if (m_fPlay)
	{
		if (!(m_pMainFrame->GetScreenMode()))
		{
			ChangeScreenSize();
		}
		// TODO: ここはメッセージ？
		m_iTimelineCursorFrame = m_dlgTimelineEditor->GetTimelineCursorFrame();

		m_iPlaySpeed = iNumerator;
		std::thread thPlay(&CCtrlUnitPreviewView::Play, this, TRUE);
		thPlay.detach();
	}
}

// 再生速度変更
void CCtrlUnitPreviewView::ChangePlaySpeed(const int& iNumerator, const int& iDenominator)
{
	if (!m_fPlay)
	{
		return;
	}

	m_iPlaySpeed = iNumerator;
}

// 再生速度変更（相対値）
void CCtrlUnitPreviewView::ChangePlaySpeed(const int& iSpeed)
{
	if (!m_fPlay)
	{
		return;
	}
	if (m_iPlaySpeed == 0)
	{
		return;
	}
	else if ((m_iPlaySpeed + iSpeed) == 0)
	{
		m_iPlaySpeed += (iSpeed * 2);
	}
	else
	{
		m_iPlaySpeed += iSpeed;
	}
}

// 再生ポーズ
void CCtrlUnitPreviewView::PausePlay(void)
{
	if (m_iPlaySpeed == 0)
	{
		m_iPlaySpeed = m_iLastPlaySpeed;
		m_iLastPlaySpeed = 0;
	}
	else
	{
		m_iLastPlaySpeed = m_iPlaySpeed;
		m_iPlaySpeed = 0;
	}
}

// 暫定　再生スレッド
void CCtrlUnitPreviewView::Play(BOOL fOdd /*= TRUE*/)
{
	DWORD dwTime,dwTimeZ;
	CString strTime;
	//int iHz = GetDeviceCaps(m_pDC->GetSafeHdc(), VREFRESH);
	//strTime.Format(_T("%d\n"), iHz);
	//OutputDebugString(strTime);

	CRect rcRect;
	m_pDC->GetWindow()->GetClientRect(&rcRect);
	float left, right, top, bottom, red, green, blue, alpha;
	left = (rcRect.Width() / 2) - 50.0f;
	right = left + 100.0f;
	top = (rcRect.Height() / 2) + 50.0f;
	bottom = top - 100.0f;
	red = 0.5f;
	green = 0.5f;
	blue = 0.5f;
	alpha = 1.0f;

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glLineWidth(1);
	glColor4f(red, green, blue, alpha);
	glBegin(GL_QUADS);
	glVertex2f(left, top);
	glVertex2f(right, top);
	glVertex2f(right, bottom);
	glVertex2f(left, bottom);
	glEnd();

	rcRect.left = static_cast<long>(left);
	rcRect.top = static_cast<long>(top);
	rcRect.right = static_cast<long>(right);
	rcRect.bottom = static_cast<long>(bottom);

	glFlush();
	SwapBuffers(m_pDC->GetSafeHdc());


	if (m_pWglSwapIntervalEXT)
	{
		m_pWglSwapIntervalEXT(0);	// 0/1 で VSYNC OFF/ON
	}



	dwTimeZ = timeGetTime();
	//strTime.Format(_T("b%d\n"), dwTimeZ);
	//OutputDebugString(strTime);

	int iCount = 1, iCount2 = 0, iFrameCount = 0;
	int iNext = 0;
	int iCheckMilSecond = 100;
	int iFps = 6;
	int iWait = static_cast<int>(floor(static_cast<float>(iCheckMilSecond) / iFps));
	DWORD dwTimeS, dwTimeE, dwTimeN, dwTimeCheck;

	dwTimeCheck = timeGetTime();
	//while (TRUE)
	while (m_fPlay)
	{
		iCount2 += m_iPlaySpeed;
		if (iCount % 3 == 0)
		{

			dwTimeS = timeGetTime();
			//while (m_fThread)
			//{
			//	Sleep(0);
			//}
			//m_fThread = TRUE;
			//std::thread thDraw(&CCtrlUnitPreviewView::DrawFullScreen, this, rcRect);
			//thDraw.detach();
			DrawFullScreen(m_iPlaySpeed, iCount2, iFrameCount, rcRect);
			dwTimeN = timeGetTime();
			dwTimeE = dwTimeN - dwTimeS;

			if (iCount % iFps == 0)
			{
				iNext = iCheckMilSecond - (dwTimeN - dwTimeCheck);
				iCount = 1;
				if (iNext < 0)
				{
					Sleep(0);
				}
				else
				{
					//strTime.Format(_T("Sleep%d,%d,%d,%d\n"), iNext, iCheckMilSecond, dwTimeN, dwTimeCheck);
					//OutputDebugString(strTime);
					Sleep(iNext);
				}
				dwTimeCheck = timeGetTime();
			}
			else
			{
				++iCount;
				iNext = iWait - dwTimeE;
				if (iNext < 0)
				{
					iNext = 0;
				}
				//strTime.Format(_T("Sleep%d\n"), iNext);
				//OutputDebugString(strTime);
				Sleep(iNext);
			}
		}
		else
		{
			dwTimeS = timeGetTime();
			//while (m_fThread)
			//{
			//	Sleep(0);
			//}
			//m_fThread = TRUE;
			//std::thread thDraw(&CCtrlUnitPreviewView::DrawFullScreen, this, rcRect);
			//thDraw.detach();
			DrawFullScreen(m_iPlaySpeed, iCount2, iFrameCount, rcRect);
			++iCount;
			dwTimeE = timeGetTime() - dwTimeS;
			iNext = iWait - dwTimeE;
			if (iNext < 0)
			{
				iNext = 0;
			}
			//strTime.Format(_T("Sleep%d\n"), iNext);
			//OutputDebugString(strTime);

			Sleep(iNext);
		}
		if (m_iPlaySpeed != 0)
		{
			++iFrameCount;
		}
		if (m_iPlaySpeed < 0)
		{
			if (m_iTimelineCursorFrame + iCount2 == 0)
			{
				ChangePlay();
			}
			else
			{
				if (m_iTimelineCursorFrame + iCount2 + m_iPlaySpeed < 0)
				{
					m_iPlaySpeed = 0 - (iCount2 + m_iTimelineCursorFrame);
				}
			}
		}
		//dwTime = timeGetTime();
		////strTime.Format(_T("cc%d\n"), (dwTime - dwTimeW));
		////OutputDebugString(strTime);
		//strTime.Format(_T("%d,%d\n"), iCount2, (dwTime - dwTimeZ));
		//OutputDebugString(strTime);
	}
	dwTime = timeGetTime();
	strTime.Format(_T("PlayDuration %d\n"), (dwTime - dwTimeZ));
	OutputDebugString(strTime);
	strTime.Format(_T("PlayFrame %d\n"), iFrameCount);
	OutputDebugString(strTime);
	m_dlgTimelineEditor->OnStop();
	m_iPlaySpeed = 0;

	m_iTimelineCursorFrame += iCount2;
	ASSERT(m_dlgTimelineEditor->GetTimelineCursorFrame() == m_iTimelineCursorFrame);

	if (m_pWglSwapIntervalEXT)
	{
		m_pWglSwapIntervalEXT(1);	// 0/1 で VSYNC OFF/ON
	}

	wglMakeCurrent(NULL, NULL);
}

// TODO: OpenGLViewに移設 OpenGL スワップインターバル変更関数の取得
BOOL CCtrlUnitPreviewView::SetWglSwapIntervalEXT(void)
{
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);


	m_pWglSwapIntervalEXT = nullptr;

	const char * extensions = (const char *)glGetString(GL_EXTENSIONS);
	if (strstr(extensions, "WGL_EXT_swap_control") == 0)
	{
		return FALSE; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
	}
	else
	{
		m_pWglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	}
	wglMakeCurrent(NULL, NULL);
	return TRUE;

}

// 暫定　全画面表示時の画面描画
void CCtrlUnitPreviewView::DrawFullScreen(int iPlaySpeed, int iCount, int iFrame, CRect& rcRect)
{
	CRect rcClientRect;
	GetClientRect(rcClientRect);

	std::random_device rnd;     // 非決定的な乱数生成器
	std::mt19937 mt(rnd());            // メルセンヌ・ツイスタの32ビット版、引数は初期シード
	std::normal_distribution<> color(0.5, 0.01);
	//std::mt19937 gen(1729);
	std::uniform_int_distribution<> pos(-3, 3);


	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("Timeline Cursor Frame %d"), m_iTimelineCursorFrame + iCount);
	CreateDrawFont(30, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(100, 100, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	
	m_pTimecodeOperator->ChangeFrameToTimecode(m_iTimelineCursorFrame + iCount, strFrameNumber, DF_MODE);
	ChangeScreenPointToOpenGLPoint(430, 130, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Play Frame %d"), iFrame);
	ChangeScreenPointToOpenGLPoint(100, 160, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strFrameNumber.Format(_T("Play Speed x%d"), iPlaySpeed);
	ChangeScreenPointToOpenGLPoint(100, 190, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	if (iPlaySpeed == 0)
	{
		strFrameNumber = _T("Pause");
		ChangeScreenPointToOpenGLPoint(100, 220, rcClientRect.Height(), dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	}
	else
	{
		strFrameNumber = _T("Play");
		ChangeScreenPointToOpenGLPoint(100, 220, rcClientRect.Height(), dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

		rcRect.left += pos(mt);
		rcRect.right += pos(mt);
		rcRect.top += pos(mt);
		rcRect.bottom += pos(mt);

	}
	DeleteObject(hfDrawFont);

	float red = static_cast<float>(color(mt));
	float blue = static_cast<float>(color(mt));
	float green = static_cast<float>(color(mt));
	float alpha = static_cast<float>(color(mt));

	if (red > 1.0){ red = 1.0; }
	if (blue > 1.0){ blue = 1.0; }
	if (green > 1.0){ green = 1.0; }
	if (alpha > 1.0){ alpha = 1.0; }
	if (red < -1.0){ red = -1.0; }
	if (blue < -1.0){ blue = -1.0; }
	if (green < -1.0){ green = -1.0; }
	if (alpha < -1.0){ alpha = -1.0; }

	glLineWidth(1);
	glColor4f(red, green, blue, 1.0f);
	glBegin(GL_QUADS);
	glVertex2i(rcRect.left, rcRect.top);
	glVertex2i(rcRect.right, rcRect.top);
	glVertex2i(rcRect.right, rcRect.bottom);
	glVertex2i(rcRect.left, rcRect.bottom);
	glEnd();

	//glFlush();
	SwapBuffers(m_pDC->GetSafeHdc());
	//glFinish();

	wglMakeCurrent(NULL, NULL);

	// TODO: ここはメッセージ？
	if (iPlaySpeed != 0)
	{
		m_dlgTimelineEditor->OnPlay(iPlaySpeed);
	}
}



// 全画面（停止）時の描画
void CCtrlUnitPreviewView::DrawFullScreen(CDC& dc)
{
	CRect rcClientRect;
	GetClientRect(rcClientRect);

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// 背景塗りつぶし
	glClearColor(PREVIEW_FULLSCREEN_BASE_BRUSH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// タイムコード表示
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("Timeline Cursor Frame %d"), m_iTimelineCursorFrame);
	CreateDrawFont(30, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(100, 100, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	m_pTimecodeOperator->ChangeFrameToTimecode(m_iTimelineCursorFrame, strFrameNumber, DF_MODE);
	ChangeScreenPointToOpenGLPoint(430, 130, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Play Frame %d"), 0);
	ChangeScreenPointToOpenGLPoint(100, 160, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strFrameNumber.Format(_T("Play Speed x%d"), 0);
	ChangeScreenPointToOpenGLPoint(100, 190, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber = _T("Stop");
	ChangeScreenPointToOpenGLPoint(100, 220, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);

	glFlush();
	SwapBuffers(m_pDC->GetSafeHdc());

	wglMakeCurrent(NULL, NULL);
}

// 非全画面時の描画
void CCtrlUnitPreviewView::DrawSmallScreen(CDC& dc)
{
	CRect rcView;
	GetClientRect(rcView);
	int iHeight = rcView.Height();

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// 背景塗りつぶし
	glClearColor(PREVIEW_SMALLSCREEN_BASE_BRUSH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawPlayButton(dc, iHeight);
	DrawStopButton(dc, iHeight);
	DrawFullScreenButton(dc, iHeight);
	DrawTimelineEditorButton(dc, iHeight);

	// タイムコード表示
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("Timeline Cursor Frame %d"), m_iTimelineCursorFrame);
	CreateDrawFont(30, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(20, 170, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	m_pTimecodeOperator->ChangeFrameToTimecode(m_iTimelineCursorFrame, strFrameNumber, DF_MODE);
	ChangeScreenPointToOpenGLPoint(20, 200, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);

	glFlush();
	SwapBuffers(m_pDC->GetSafeHdc());

	wglMakeCurrent(NULL, NULL);


}

// 再生ボタン描画
void CCtrlUnitPreviewView::DrawPlayButton(CDC& dc, int iHeight)
{
	m_pPlayButton->DrawMyFillRect();

	CString strButtonName;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(BUTTON_FONTSIZE, 0, BUTTON_FONTFACE, hfDrawFont);
	int iFontX = m_pPlayButton->left + 5, 
		iFontY = m_pPlayButton->bottom - 8;
	
	// TODO: 初回だけ位置ずれを起こすので空描画
	strButtonName = " ";
	ChangeScreenPointToOpenGLPoint(iFontX, iFontY, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONT_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strButtonName = PLAYBUTTON_NAME;
	ChangeScreenPointToOpenGLPoint(iFontX + BUTTON_FONTSHADOW_WIDTH, iFontY + BUTTON_FONTSHADOW_WIDTH, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONTSHADOW_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	ChangeScreenPointToOpenGLPoint(iFontX, iFontY, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONT_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);

}

// 停止ボタン描画
void CCtrlUnitPreviewView::DrawStopButton(CDC& dc, int iHeight)
{
	m_pStopButton->DrawMyFillRect();

	CString strButtonName;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(BUTTON_FONTSIZE, 0, BUTTON_FONTFACE, hfDrawFont);
	int iFontX = m_pStopButton->left + 5, 
		iFontY = m_pStopButton->bottom - 8;

	strButtonName = STOPBUTTON_NAME;
	ChangeScreenPointToOpenGLPoint(iFontX + BUTTON_FONTSHADOW_WIDTH, iFontY + BUTTON_FONTSHADOW_WIDTH, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONTSHADOW_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	ChangeScreenPointToOpenGLPoint(iFontX, iFontY, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONT_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);

}

// 全画面表示ボタン描画
void CCtrlUnitPreviewView::DrawFullScreenButton(CDC& dc, int iHeight)
{
	m_pFullScreenButton->DrawMyFillRect();

	CString strButtonName;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(BUTTON_FONTSIZE, 0, BUTTON_FONTFACE, hfDrawFont);
	int iFontX = m_pFullScreenButton->left + 5,
		iFontY = m_pFullScreenButton->bottom - 8;

	strButtonName = FULLSCREENBUTTON_NAME;
	ChangeScreenPointToOpenGLPoint(iFontX + BUTTON_FONTSHADOW_WIDTH, iFontY + BUTTON_FONTSHADOW_WIDTH, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONTSHADOW_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	ChangeScreenPointToOpenGLPoint(iFontX, iFontY, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONT_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);

}

// タイムラインエディタ表示ボタン描画
void CCtrlUnitPreviewView::DrawTimelineEditorButton(CDC& dc, int iHeight)
{
	if (m_dlgTimelineEditor->IsWindowVisible())
	{
		m_pTimelineEditorButton->SetBothColor(TIMELINEEDITORBUTTON_TOGGLED_BRUSH, TIMELINEEDITORBUTTON_TOGGLED_BRUSH, TIMELINEEDITORBUTTON_TOGGLED_BRUSH, TIMELINEEDITORBUTTON_TOGGLED_BRUSH);
		m_pTimelineEditorButton->DrawMyFillRect();
		m_pTimelineEditorButton->SetBothColor(TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH);
	}
	else
	{
		m_pTimelineEditorButton->DrawMyFillRect();
	}

	CString strButtonName;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(BUTTON_FONTSIZE, 0, BUTTON_FONTFACE, hfDrawFont);
	int iFontX = m_pTimelineEditorButton->left + 5,
		iFontY = m_pTimelineEditorButton->bottom - 20,
		iFontLowerX = m_pTimelineEditorButton->left + 32,
		iFontLowerY = m_pTimelineEditorButton->bottom - 7;

	strButtonName = TIMELINEEDITORBUTTON_NAME_UPPER;
	ChangeScreenPointToOpenGLPoint(iFontX + BUTTON_FONTSHADOW_WIDTH, iFontY + BUTTON_FONTSHADOW_WIDTH, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONTSHADOW_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strButtonName = TIMELINEEDITORBUTTON_NAME_LOWER;
	ChangeScreenPointToOpenGLPoint(iFontLowerX + BUTTON_FONTSHADOW_WIDTH, iFontLowerY + BUTTON_FONTSHADOW_WIDTH, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONTSHADOW_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strButtonName = TIMELINEEDITORBUTTON_NAME_UPPER;
	ChangeScreenPointToOpenGLPoint(iFontX, iFontY, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONT_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strButtonName = TIMELINEEDITORBUTTON_NAME_LOWER;
	ChangeScreenPointToOpenGLPoint(iFontLowerX, iFontLowerY, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strButtonName), dc.GetSafeHdc(), hfDrawFont, BUTTON_FONT_BRUSH,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	DeleteObject(hfDrawFont);
}

// ボタン配置
void CCtrlUnitPreviewView::SetButtonRect(void)
{
	CRect rcViewRect;
	GetClientRect(&rcViewRect);
	double dLeft = 0.0, dRight = 0.0, dTop = 0.0, dBottom = 0.0;

	int iViewHeight = static_cast<int>(rcViewRect.Height());

	m_pTimelineEditorButton->left = 20;
	m_pTimelineEditorButton->top = 20;
	m_pTimelineEditorButton->right = m_pTimelineEditorButton->left + PREVIEWBUTTON_SIZE;
	m_pTimelineEditorButton->bottom = m_pTimelineEditorButton->top + PREVIEWBUTTON_SIZE;
	m_pTimelineEditorButton->SetVert(iViewHeight);

	m_pPlayButton->CopyRect(static_cast<CRect>(m_pTimelineEditorButton));
	m_pPlayButton->left = m_pTimelineEditorButton->right + 1;
	m_pPlayButton->right = m_pPlayButton->left + PREVIEWBUTTON_SIZE;
	m_pPlayButton->SetVert(iViewHeight);

	m_pStopButton->CopyRect(static_cast<CRect>(m_pTimelineEditorButton));
	m_pStopButton->left = m_pPlayButton->right + 1;
	m_pStopButton->right = m_pStopButton->left + PREVIEWBUTTON_SIZE;
	m_pStopButton->SetVert(iViewHeight);

	m_pFullScreenButton->CopyRect(static_cast<CRect>(m_pTimelineEditorButton));
	m_pFullScreenButton->left = m_pStopButton->right + 1;
	m_pFullScreenButton->right = m_pFullScreenButton->left + PREVIEWBUTTON_SIZE;
	m_pFullScreenButton->SetVert(iViewHeight);
}

// タイムラインエディターの表示切替
void CCtrlUnitPreviewView::ChangeDisplayTimelineEditor(void)
{
	if (m_dlgTimelineEditor->IsWindowVisible())
	{
		m_dlgTimelineEditor->ShowWindow(SW_HIDE);
	}
	else
	{
		m_dlgTimelineEditor->ShowWindow(SW_SHOW);
	}
	Invalidate();
}


// タイムラインカーソル位置同期
void CCtrlUnitPreviewView::SetTimelineCursorFrame(int iFrame)
{
	m_iTimelineCursorFrame = iFrame;

	CDC* pDC = GetDC();
	if ((!m_pMainFrame->GetScreenMode()) && !m_fPlay)
	{
		DrawSmallScreen(*pDC);
	}
	else
	{
		DrawFullScreen(*pDC);
	}
	ReleaseDC(pDC);

}

