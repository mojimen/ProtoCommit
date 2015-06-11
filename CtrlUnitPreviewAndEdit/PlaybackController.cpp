// PlaybackController.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "PlaybackController.h"

#include <windows.h>
#include <future>
#include <mmsystem.h>
#include <random>

#pragma comment(lib,"winmm.lib")

// PlaybackController

PlaybackController::PlaybackController()
{
	m_iStartFrame = 0;
	m_iFrameCount = 0;
	m_iFps = 0;
	m_fPlay = FALSE;
	m_iPlaySpeed = 1;
}

PlaybackController::~PlaybackController()
{
}


// PlaybackController メンバー関数

// 初期設定
BOOL PlaybackController::InitializePlaybackController(CDC* pDC, HGLRC* pRC, CString& strPlaybackControllerUUID)
{
	if (InitializeData(TAG_NONE))
	{
		strPlaybackControllerUUID = m_strUUID;
	}
	else
	{
		return FALSE;
	}

	m_pDC = pDC;
	m_pRC = pRC;
	//if ((m_hRCOdd = wglCreateContext(m_pDC->GetSafeHdc())) == 0)
	//{
	//	return FALSE;
	//}
	//if ((m_hRCEven = wglCreateContext(m_pDC->GetSafeHdc())) == 0)
	//{
	//	return FALSE;
	//}
	wglMakeCurrent(m_pDC->GetSafeHdc(), *m_pRC);

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

BOOL PlaybackController::Run(CDC* pDC, int iStartFrame, int iFps, int iPlaySpeed /* = 1*/)
{
	m_iStartFrame = iStartFrame;
	m_iFrameCount = 0;
	m_iFps = iFps;
	m_fPlay = !m_fPlay;

	if (m_fPlay)
	{
		std::thread thPlay(&PlaybackController::Play, this, TRUE);
		thPlay.detach();
	}

	return TRUE;
}


void PlaybackController::Play(BOOL fOdd /*= TRUE*/)
{
	//timeBeginPeriod(1);
	DWORD dwTime;
	CString strTime;
	int iHz = GetDeviceCaps(m_pDC->GetSafeHdc(), VREFRESH);
	strTime.Format(_T("%d\n"), iHz);
	OutputDebugString(strTime);

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

	wglMakeCurrent(m_pDC->GetSafeHdc(), *m_pRC);

	glLineWidth(1);
	glColor4f(red, green, blue, alpha);
	glBegin(GL_QUADS);
	glVertex2f(left, top);
	glVertex2f(right, top);
	glVertex2f(right, bottom);
	glVertex2f(left, bottom);
	glEnd();

	glFlush();
	SwapBuffers(m_pDC->GetSafeHdc());

	std::random_device rnd;     // 非決定的な乱数生成器
	std::mt19937 mt(rnd());            // メルセンヌ・ツイスタの32ビット版、引数は初期シード
	std::normal_distribution<> color(0.0, 0.001);
	std::normal_distribution<> pos(0.0, 1.0);

	wglMakeCurrent(m_pDC->GetSafeHdc(), *m_pRC);
	if (m_pWglSwapIntervalEXT)
	{
		m_pWglSwapIntervalEXT(1);	// 0/1 で VSYNC OFF/ON
	}



	dwTime = timeGetTime();
	strTime.Format(_T("b%d\n"), dwTime);
	OutputDebugString(strTime);




	//while (m_fPlay)
	for (int i = 0; i<600; ++i)
	{

		//if (!m_fPlay)
		//{
		//	break;
		//}

		//wglMakeCurrent(m_pDC->GetSafeHdc(), *m_pRC);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//left += pos(mt);
		//right += pos(mt);
		//top += pos(mt);
		//bottom += pos(mt);

		//red += color(mt);
		//blue += color(mt);
		//green += color(mt);
		//alpha += color(mt);

		//if (red > 1.0){ red = 1.0; }
		//if (blue > 1.0){ blue = 1.0; }
		//if (green > 1.0){ green = 1.0; }
		//if (alpha > 1.0){ alpha = 1.0; }
		//if (red < -1.0){ red = -1.0; }
		//if (blue < -1.0){ blue = -1.0; }
		//if (green < -1.0){ green = -1.0; }
		//if (alpha < -1.0){ alpha = -1.0; }


		//glLineWidth(1);
		//glColor4f(red, green, blue, alpha);
		//glBegin(GL_QUADS);
		//glVertex2f(left, top);
		//glVertex2f(right, top);
		//glVertex2f(right, bottom);
		//glVertex2f(left, bottom);
		//glEnd();

		dwTime = timeGetTime();
		strTime.Format(_T("c%d\n"), dwTime);
		OutputDebugString(strTime);
		SwapBuffers(m_pDC->GetSafeHdc());


		//wglMakeCurrent(NULL, NULL);
		//glFinish();


	}
	dwTime = timeGetTime();
	strTime.Format(_T("b%d\n"), dwTime);
	OutputDebugString(strTime);
	//timeEndPeriod(1);
}

//void PlaybackController::Play(BOOL fOdd /*= TRUE*/)
//{
//	timeBeginPeriod(1);
//	DWORD dwTime;
//	CString strTime;
//	while (m_fPlay)
//	{
//		dwTime = timeGetTime();
//		strTime.Format(_T("%d\n"), dwTime);
//		OutputDebugString(strTime);
//		Sleep(16);
//	}
//	timeEndPeriod(1);
//}