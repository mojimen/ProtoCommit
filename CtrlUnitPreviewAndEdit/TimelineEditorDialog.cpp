// TimelineEditorDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditorDialog.h"
#include "TimelineEditorView.h"
#include "afxdialogex.h"

#include "CtrlUnitPreviewView.h"


// TimelineEditorDialog �_�C�A���O

IMPLEMENT_DYNAMIC(TimelineEditorDialog, CDialogEx)

TimelineEditorDialog::TimelineEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TimelineEditorDialog::IDD, pParent)
{
	m_pParentWnd = pParent;
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
	ON_WM_KEYUP()
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// TimelineEditorDialog ���b�Z�[�W �n���h���[


// ��\��
void TimelineEditorDialog::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	ShowWindow(SW_HIDE);
	m_pParentWnd->PostMessageW(WM_COMMAND, MAKEWPARAM(ID_HIDE_EDITOR, 0), NULL);
	//CDialogEx::OnClose();
}

// �����\���ʒu����
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

// �����ݒ�
BOOL TimelineEditorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	//ModifyStyle(WS_CAPTION, 0, 0);
	ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0, 0);
	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//pSysMenu->AppendMenu(SC_SIZE);

	// �t���[���E�B���h�E�̕\���̈�
	CRect rcFrameWndRect;
	GetWindowRect(&rcFrameWndRect);
	ScreenToClient(&rcFrameWndRect);

	m_pEditorView = static_cast<TimelineEditorView*>((RUNTIME_CLASS(TimelineEditorView))->CreateObject());
	m_pEditorView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rcFrameWndRect, this, 0, NULL);
	m_pEditorView->SendMessage(WM_INITIALUPDATE);

	m_pEditorView->SetMainWnd(m_pParentWnd);
	m_pEditorView->SetParentDialog(this);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

// �T�C�Y�ύX
void TimelineEditorDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CRect rcMyRect;
	GetClientRect(rcMyRect);
	//GetWindowRect(&rcMyRect);
	//ScreenToClient(&rcMyRect);
	//rcMyRect.left += 50;
	//rcMyRect.top += 50;
	//rcMyRect.right -= 50;
	//rcMyRect.bottom -= 50;


	m_pEditorView->MoveWindow(rcMyRect);
}

// �w�i�N���A�𖳌���
BOOL TimelineEditorDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

// �Đ��J�n
void TimelineEditorDialog::OnPlay(int iSpeed /* = 1*/)
{
	m_pEditorView->OnPlay(iSpeed);
	//PostMessage(WM_COMMAND, MAKEWPARAM(ID_PLAY, 0), NULL);
}

// �Đ���~
void TimelineEditorDialog::OnStop(void)
{
	m_pEditorView->OnStop();
}

// �^�C�����C���J�[�\���ʒu�����i�v���r���[����擾�j
int TimelineEditorDialog::GetTimelineCursorFrame(void)
{
	return m_pEditorView->GetTimelineCursorFrame();
}

// �^�C�����C���J�[�\���ʒu�����i�_�C�A���O����ݒ�j
void TimelineEditorDialog::SetTimelineCursorFrame(int iFrame)
{
	if (m_pMainView)
	{
		m_pMainView->SetTimelineCursorFrame(iFrame);
	}
}

// �Đ���Ԃ̐؂�ւ�
void TimelineEditorDialog::ChangePlay(const int iNumerator /*= 1*/, const int iDenominator /*= 1*/)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlay(iNumerator, iDenominator);
	}
}

// �Đ����x�ύX
void TimelineEditorDialog::ChangePlaySpeed(const int& iNumerator, const int& iDenominator)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlaySpeed(iNumerator, iDenominator);
	}
}

// �Đ����x�A�b�v
void TimelineEditorDialog::PlaySpeedUp(const int iSpeed /*= 1*/)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlaySpeed(iSpeed);
	}
}

// �Đ����x�_�E��
void TimelineEditorDialog::PlaySpeedDown(const int iSpeed /*= 1*/)
{
	if (m_pMainView)
	{
		m_pMainView->ChangePlaySpeed(iSpeed * -1);
	}
}

// �Đ��|�[�Y
void TimelineEditorDialog::PausePlay(void)
{
	if (m_pMainView)
	{
		m_pMainView->PausePlay();
	}
}

// ESC�ŕ���𖳌���
void TimelineEditorDialog::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnCancel();
}

// Enter�ŕ���𖳌���
void TimelineEditorDialog::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}

// �L�[���͂������n��
void TimelineEditorDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pMainView)
	{
		m_pMainView->KeyUp(nChar, nRepCnt, nFlags);
	}
}


void TimelineEditorDialog::OnNcPaint()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CDialogEx::OnNcPaint() ���Ăяo���Ȃ��ł��������B

	int icxFrame = GetSystemMetrics(SM_CXFRAME);//�@�t���[���̕�
	int icyFrame = GetSystemMetrics(SM_CYFRAME);//�@�t���[���̍���
	int iCaptionHeight = GetSystemMetrics(SM_CYCAPTION);

	CDC* pWDC = GetWindowDC();

	CRect rcRect;
	GetWindowRect(&rcRect);
	CString str;
	str.Format(_T("L%d T%d R%d B%d C%d X%d Y%d\n"), rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, iCaptionHeight, icxFrame, icyFrame);
	ScreenToClient(&rcRect);
	int cxFrame = 0 - rcRect.left;
	int cyFrame = 0 - rcRect.top - iCaptionHeight;
	str.Format(_T("L%d T%d R%d B%d C%d X%d Y%d\n"), rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, iCaptionHeight, cxFrame, cyFrame);
	OutputDebugString(str);

	////rcRect.left += 2;
	////rcRect.right -= 2;
	////rcRect.top += 2;
	////rcRect.bottom -= 2;

	//HBRUSH hBrs = CreateSolidBrush(GRAY_BRUSH);

	pWDC->FillSolidRect(0, 0, rcRect.Width(), cyFrame, RGB(100, 100, 230));

	pWDC->FillSolidRect(0, cyFrame, rcRect.Width(), iCaptionHeight, RGB(100, 100, 230));

	pWDC->FillSolidRect(0, rcRect.bottom + iCaptionHeight + cyFrame - WINDOWBORDER_THICKNESS, rcRect.Width(), WINDOWBORDER_THICKNESS, RGB(100, 100, 230));
	pWDC->FillSolidRect(WINDOWBORDER_THICKNESS, rcRect.bottom + iCaptionHeight, rcRect.Width() - WINDOWBORDER_THICKNESS, cyFrame - WINDOWBORDER_THICKNESS, RGB(255, 255, 255));

	pWDC->FillSolidRect(0, 0, WINDOWBORDER_THICKNESS, rcRect.Height(), RGB(100, 100, 230));
	pWDC->FillSolidRect(WINDOWBORDER_THICKNESS, cyFrame + iCaptionHeight, cxFrame - WINDOWBORDER_THICKNESS, rcRect.Height() - WINDOWBORDER_THICKNESS - cyFrame - iCaptionHeight, RGB(255, 255, 255));

	pWDC->FillSolidRect(rcRect.right + cxFrame - WINDOWBORDER_THICKNESS, 0, WINDOWBORDER_THICKNESS, rcRect.Height(), RGB(100, 100, 230));
	pWDC->FillSolidRect(rcRect.right, cyFrame + iCaptionHeight, cxFrame - WINDOWBORDER_THICKNESS, rcRect.Height() - WINDOWBORDER_THICKNESS - cyFrame - iCaptionHeight, RGB(255, 255, 255));

	ReleaseDC(pWDC);

	//m_pEditorView->OnDraw(GetDC());

}


//LRESULT TimelineEditorDialog::OnNcHitTest(CPoint point)
//{
//	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
//
//	CRect rcRect;
//	GetWindowRect(&rcRect);
//
//	//if (point.x < rcRect.left + WINDOWBORDER_THICKNESS)
//	//{
//		//return HTLEFT;
//	//}
//
//	return CDialogEx::OnNcHitTest(point);
//}
