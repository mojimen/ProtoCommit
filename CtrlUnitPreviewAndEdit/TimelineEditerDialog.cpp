// TimelineEditerDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditerDialog.h"
#include "TimelineEditerView.h"
#include "afxdialogex.h"


// TimelineEditerDialog �_�C�A���O

IMPLEMENT_DYNAMIC(TimelineEditerDialog, CDialogEx)

TimelineEditerDialog::TimelineEditerDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TimelineEditerDialog::IDD, pParent)
{

}

TimelineEditerDialog::~TimelineEditerDialog()
{
}

void TimelineEditerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TimelineEditerDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// TimelineEditerDialog ���b�Z�[�W �n���h���[



void TimelineEditerDialog::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


void TimelineEditerDialog::SetPosition(CRect& rcParentRect)
{
	CRect rcDesktopRect, rcMyRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktopRect, 0);
	int iHeight;
	if ((rcDesktopRect.bottom - rcParentRect.bottom - 50) < 400)
	{
		iHeight = 400;
	}
	else
	{
		iHeight = rcDesktopRect.bottom - rcParentRect.bottom - 50;
	}
	SetWindowPos(&wndTop, rcParentRect.left, rcParentRect.bottom, (rcParentRect.right - rcParentRect.left), iHeight, SWP_NOACTIVATE);
	GetClientRect(&rcMyRect);
	m_pEditerView->MoveWindow(rcMyRect);
}

BOOL TimelineEditerDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	// �t���[���E�B���h�E�̕\���̈�
	CRect rcFrameWndRect;
	GetWindowRect(&rcFrameWndRect);
	ScreenToClient(&rcFrameWndRect);

	m_pEditerView = static_cast<TimelineEditerView*>((RUNTIME_CLASS(TimelineEditerView))->CreateObject());
	m_pEditerView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rcFrameWndRect, this, 0, NULL);
	m_pEditerView->SendMessage(WM_INITIALUPDATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void TimelineEditerDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CRect rcMyRect;
	GetClientRect(&rcMyRect);
	m_pEditerView->MoveWindow(rcMyRect);
}



BOOL TimelineEditerDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
