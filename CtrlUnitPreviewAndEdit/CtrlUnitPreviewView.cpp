
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

// CCtrlUnitPreviewView �R���X�g���N�V����/�f�X�g���N�V����

CCtrlUnitPreviewView::CCtrlUnitPreviewView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B
	m_pMainWnd = nullptr;
	m_pMainFrame = nullptr;
	m_pWglSwapIntervalEXT = nullptr;
	m_dlgTimelineEditor = nullptr;
	//m_pPlaybackController = nullptr;
	m_pPlayButton = nullptr;
	m_pStopButton = nullptr;
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
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return OpenGLView::PreCreateWindow(cs);
}

// CCtrlUnitPreviewView �`��
void CCtrlUnitPreviewView::OnDraw(CDC* pDC)
{
	CCtrlUnitPreviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!m_pMainFrame->GetScreenMode())
	{
		DrawSmallScreen(*pDC);
	}
	else
	{
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		//timeBeginPeriod(1);
		//DWORD dwTime;
		//CString strTime;
		//dwTime = timeGetTime();
		//strTime.Format(_T("a%d\n"), dwTime);
		//OutputDebugString(strTime);
		// �w�i�h��Ԃ�
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glFlush();
		SwapBuffers(m_pDC->GetSafeHdc());
		wglMakeCurrent(NULL, NULL);
		//timeEndPeriod(1);


	}


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

// �����ݒ�
int CCtrlUnitPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (OpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

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

// �I������
void CCtrlUnitPreviewView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
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
	if (m_pTimelineEditorButton)
	{
		delete m_pTimelineEditorButton;
	}

	//if (m_pPlaybackController != nullptr)
	//{
	//	delete m_pPlaybackController;
	//}

}

// �E�B���h�E�T�C�Y�ύX
void CCtrlUnitPreviewView::OnSize(UINT nType, int cx, int cy)
{
	OpenGLView::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	if (m_fInitialize)
	{
		SetButtonRect();
	}
}

// �����ݒ�
void CCtrlUnitPreviewView::OnInitialUpdate()
{

	OpenGLView::OnInitialUpdate();

	m_fInitialize = TRUE;

	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	m_dlgTimelineEditor = new TimelineEditorDialog(m_pMainWnd);
	m_dlgTimelineEditor->Create(TimelineEditorDialog::IDD, GetDesktopWindow());
	CRect rcRect;
	m_pMainWnd->GetWindowRect(rcRect);
	m_dlgTimelineEditor->SetPosition(rcRect);
	m_dlgTimelineEditor->SetMainView(this);

	m_pPlayButton = new OpenGLRect();
	m_pStopButton = new OpenGLRect();
	m_pTimelineEditorButton = new OpenGLRect();
	m_pPlayButton->SetBothColor(PLAYBUTTON_BRUSH, PLAYBUTTON_BRUSH, PLAYBUTTON_BRUSH, PLAYBUTTON_BRUSH);
	m_pStopButton->SetBothColor(STOPBUTTON_BRUSH, STOPBUTTON_BRUSH, STOPBUTTON_BRUSH, STOPBUTTON_BRUSH);
	m_pTimelineEditorButton->SetBothColor(TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH, TIMELINEEDITORBUTTON_BRUSH);
	SetButtonRect();

	m_pTimecodeOperator = new TimecodeOperator();
	m_pTimecodeOperator->InitializeTimecodeOperator(m_strTimecodeOperator);
}



// �k�N���b�N
void CCtrlUnitPreviewView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
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
		// ���ۂ͋@�\�Ȃ�
		m_fPlay = FALSE;
	}
	else if (m_pTimelineEditorButton->PtInRect(point))
	{
		ChangeDisplayTimelineEditor();
	}
}

// �q�N���b�N
void CCtrlUnitPreviewView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if ((m_pPlayButton->PtInRect(point)) || (m_pStopButton->PtInRect(point)))
	{
		return;
	}
	ChangeDisplayTimelineEditor();
}

// �k�{�^���_�u���N���b�N
void CCtrlUnitPreviewView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if ((m_pTimelineEditorButton->PtInRect(point)) || (m_pPlayButton->PtInRect(point)) || (m_pStopButton->PtInRect(point)))
	{
		// �{�^����͖���
		return;
	}

	// �Đ��J�n
	ChangePlay();
}

// �L�[����
void CCtrlUnitPreviewView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	switch (nChar)
	{

	case VK_SPACE:
	case VK_RETURN:
		ChangePlay();
		break;

	case VK_ESCAPE:
		if (m_fPlay)
		{
			ChangePlay();
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



// �^�C�����C���G�f�B�^�[����{�^������
void CCtrlUnitPreviewView::OnHideEditor(void)
{
	Invalidate();
}

// �^�C�����C���J�[�\���ʒu����
void CCtrlUnitPreviewView::OnChangeTimelineCursor(void)
{
	m_iTimelineCursorFrame = m_dlgTimelineEditor->GetTimelineCursorFrame();
	Invalidate();
}




// OpenGLTest


// �Đ���Ԃ̐؂�ւ�
void CCtrlUnitPreviewView::ChangePlay(void)
{
	// �E�B���h�E�T�C�Y�ƃt���O�̐؂�ւ�
	m_pMainWnd->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_FULLSCREEN, 0), NULL);
	m_fPlay = !m_fPlay;
	if (!m_fPlay)
	{
		// ��~���͏����҂i�X���b�h�̕`��Ƃ�������Ȃ��悤�Ɂj
		Sleep(100);
	}
	Invalidate();

	if (m_fPlay)
	{
		// TODO: �����̓��b�Z�[�W�H
		m_iTimelineCursorFrame = m_dlgTimelineEditor->GetTimelineCursorFrame();

		m_iPlaySpeed = 1;
		std::thread thPlay(&CCtrlUnitPreviewView::Play, this, TRUE);
		thPlay.detach();
	}
}


// �b��@�Đ��X���b�h
void CCtrlUnitPreviewView::Play(BOOL fOdd /*= TRUE*/)
{
	//timeBeginPeriod(1);
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
		m_pWglSwapIntervalEXT(0);	// 0/1 �� VSYNC OFF/ON
	}



	dwTimeZ = timeGetTime();
	//strTime.Format(_T("b%d\n"), dwTimeZ);
	//OutputDebugString(strTime);

	int iCount = 1, iCount2 = 0;
	int iNext = 0;
	int iCheckMilSecond = 100;
	int iFps = 6;
	int iWait = static_cast<int>(floor(static_cast<float>(iCheckMilSecond) / iFps));
	DWORD dwTimeS, dwTimeE, dwTimeN, dwTimeCheck;

	dwTimeCheck = timeGetTime();
	//while (TRUE)
	while (m_fPlay)
	//for (int i=1; i <= 600; ++i)
	{


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
			DrawFullScreen(m_iPlaySpeed, iCount2, rcRect);
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
			DrawFullScreen(m_iPlaySpeed, iCount2, rcRect);
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

		++iCount2;
		//dwTime = timeGetTime();
		////strTime.Format(_T("cc%d\n"), (dwTime - dwTimeW));
		////OutputDebugString(strTime);
		//strTime.Format(_T("%d,%d\n"), iCount2, (dwTime - dwTimeZ));
		//OutputDebugString(strTime);

	}
	dwTime = timeGetTime();
	strTime.Format(_T("PlayDuration %d\n"), (dwTime - dwTimeZ));
	OutputDebugString(strTime);
	strTime.Format(_T("PlayFrame %d\n"), iCount2);
	OutputDebugString(strTime);
	//timeEndPeriod(1);
	m_dlgTimelineEditor->OnStop();
	m_iPlaySpeed = 0;

	m_iTimelineCursorFrame += iCount2;
	//strTime.Format(_T("PreviewCursor %d\n"), m_iTimelineCursorFrame);
	//OutputDebugString(strTime);
	//strTime.Format(_T("EditorCursor %d\n"), m_dlgTimelineEditor->GetTimelineCursorFrame());
	//OutputDebugString(strTime);
	ASSERT(m_dlgTimelineEditor->GetTimelineCursorFrame() == m_iTimelineCursorFrame);

	if (m_pWglSwapIntervalEXT)
	{
		m_pWglSwapIntervalEXT(1);	// 0/1 �� VSYNC OFF/ON
	}

	wglMakeCurrent(NULL, NULL);
}

// TODO: OpenGLView�Ɉڐ� OpenGL �X���b�v�C���^�[�o���ύX�֐��̎擾
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

// �b��@�S��ʕ\�����̉�ʕ`��
void CCtrlUnitPreviewView::DrawFullScreen(int iPlaySpeed, int iCount, CRect& rcRect)
{
	//CString strTime;
	//DWORD dwTimeW = timeGetTime();
	//strTime.Format(_T("c%d\n"), dwTimeW);
	//OutputDebugString(strTime);

	CRect rcClientRect;
	GetClientRect(rcClientRect);

	std::random_device rnd;     // �񌈒�I�ȗ���������
	std::mt19937 mt(rnd());            // �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h
	std::normal_distribution<> color(0.5, 0.01);
	//std::mt19937 gen(1729);
	std::uniform_int_distribution<> pos(-3, 3);

	rcRect.left += pos(mt);
	rcRect.right += pos(mt);
	rcRect.top += pos(mt);
	rcRect.bottom += pos(mt);

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

	strFrameNumber.Format(_T("Play Frame %d"), iCount);
	ChangeScreenPointToOpenGLPoint(100, 160, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strFrameNumber.Format(_T("Play Speed x%d"), iPlaySpeed);
	ChangeScreenPointToOpenGLPoint(100, 190, rcClientRect.Height(), dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), m_pDC->GetSafeHdc(), hfDrawFont, WHITECOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
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

	// TODO: �����̓��b�Z�[�W�H
	m_dlgTimelineEditor->OnPlay(iPlaySpeed);
	//DWORD dwTime = timeGetTime();
	//strTime.Format(_T("write %d\n"), dwTime);
	//OutputDebugString(strTime);


	wglMakeCurrent(NULL, NULL);

	//m_fThread = FALSE;

}



// ��S��ʎ��̕`��
void CCtrlUnitPreviewView::DrawSmallScreen(CDC& dc)
{
	CRect rcView;
	GetClientRect(rcView);
	int iHeight = rcView.Height();

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// �w�i�h��Ԃ�
	glClearColor(PREVIEW_SMALLSCREEN_BASE_BRUSH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawPlayButton(dc, iHeight);
	DrawStopButton(dc, iHeight);
	DrawTimelineEditorButton(dc, iHeight);

	// �^�C���R�[�h�\��
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

// �Đ��{�^���`��
void CCtrlUnitPreviewView::DrawPlayButton(CDC& dc, int iHeight)
{
	m_pPlayButton->DrawMyFillRect();

	CString strButtonName;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(BUTTON_FONTSIZE, 0, BUTTON_FONTFACE, hfDrawFont);
	int iFontX = m_pPlayButton->left + 5, 
		iFontY = m_pPlayButton->bottom - 8;
	
	// TODO: ���񂾂��ʒu������N�����̂ŋ�`��
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

// ��~�{�^���`��
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

// �^�C�����C���G�f�B�^�\���{�^���`��
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

// �{�^���z�u
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
}

// �^�C�����C���G�f�B�^�[�̕\���ؑ�
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


// �^�C�����C���J�[�\���ʒu����
void CCtrlUnitPreviewView::SetTimelineCursorFrame(int iFrame)
{
	m_iTimelineCursorFrame = iFrame;

	CDC* pDC = GetDC();
	DrawSmallScreen(*pDC);
	ReleaseDC(pDC);

}

