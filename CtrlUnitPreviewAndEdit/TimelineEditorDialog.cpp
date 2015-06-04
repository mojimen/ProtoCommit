// TimelineEditorDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditorDialog.h"
#include "TimelineEditorView.h"
#include "afxdialogex.h"


// TimelineEditorDialog �_�C�A���O

IMPLEMENT_DYNAMIC(TimelineEditorDialog, CDialogEx)

TimelineEditorDialog::TimelineEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TimelineEditorDialog::IDD, pParent)
{

}

TimelineEditorDialog::~TimelineEditorDialog()
{
}

void TimelineEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TimelineEditorDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// TimelineEditorDialog ���b�Z�[�W �n���h���[



void TimelineEditorDialog::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


void TimelineEditorDialog::SetPosition(CRect& rcParentRect)
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
	m_pEditorView->MoveWindow(rcMyRect);
}

BOOL TimelineEditorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	// �t���[���E�B���h�E�̕\���̈�
	CRect rcFrameWndRect;
	GetWindowRect(&rcFrameWndRect);
	ScreenToClient(&rcFrameWndRect);

	m_pEditorView = static_cast<TimelineEditorView*>((RUNTIME_CLASS(TimelineEditorView))->CreateObject());
	m_pEditorView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rcFrameWndRect, this, 0, NULL);
	m_pEditorView->SendMessage(WM_INITIALUPDATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void TimelineEditorDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CRect rcMyRect;
	GetClientRect(&rcMyRect);
	m_pEditorView->MoveWindow(rcMyRect);
}



BOOL TimelineEditorDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}