
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
END_MESSAGE_MAP()

// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
{
	// TODO: �����o�[�������R�[�h�������ɒǉ����Ă��������B
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	CRect rcDesktopRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktopRect, 0);

	cs.x = rcDesktopRect.left + 50;								// x ���W �\���ʒu
	cs.y = rcDesktopRect.top + 50;								// y ���W �\���ʒu
	cs.cx = (rcDesktopRect.right - rcDesktopRect.left) - 100;	// ��
	cs.cy = (rcDesktopRect.bottom - rcDesktopRect.top) / 4;      // ����

	return TRUE;
}

// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h���[

