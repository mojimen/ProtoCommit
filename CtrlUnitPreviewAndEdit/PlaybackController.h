#pragma once

#include "DataObjectBase.h"
#include "OpenGLView.h"



// PlaybackController コマンド ターゲット

class PlaybackController : public DataObjectBase, public OpenGLView
{
public:
	PlaybackController();
	virtual ~PlaybackController();

private:
	int m_iStartFrame;
	int m_iFrameCount;
	int m_iFps;
	BOOL m_fPlay;
	int m_iPlaySpeed;
	CDC* m_pDC;
	HGLRC m_hRCOdd;
	HGLRC m_hRCEven;
	HGLRC* m_pRC;

	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int);
	PFNWGLSWAPINTERVALEXTPROC m_pWglSwapIntervalEXT;


public:
	BOOL InitializePlaybackController(CDC* pDC, HGLRC* pRC, CString& strPlaybackControllerUUID);
	BOOL Run(CDC* pDC, int iStartFrame, int iFps, int iPlaySpeed = 1);
	void Play(BOOL fOdd = TRUE);

};


