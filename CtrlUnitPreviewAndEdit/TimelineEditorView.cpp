// TimelineEditorView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditorView.h"

#include "OpenGLRect.h"
#include "TimelineDataOperator.h"
#include "ClipDataRect.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
#include "TrackDataManager.h"

#include <map>

// TimelineEditorView

IMPLEMENT_DYNCREATE(TimelineEditorView, OpenGLView)

TimelineEditorView::TimelineEditorView()
{
	m_fInitialize = FALSE;
}

TimelineEditorView::~TimelineEditorView()
{
}

BEGIN_MESSAGE_MAP(TimelineEditorView, OpenGLView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_DROPFILES()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// TimelineEditorView �`��

void TimelineEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}



// TimelineEditorView �f�f

#ifdef _DEBUG
void TimelineEditorView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void TimelineEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// �E�B���h�E�X�^�C�������l�ύX
BOOL TimelineEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	cs.dwExStyle |= WS_EX_ACCEPTFILES;

	return OpenGLView::PreCreateWindow(cs);
}

// �����ݒ�
int TimelineEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (OpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
	// �h���b�v�^�[�Q�b�g�ɓo�^
	if (m_DropTarget.Register(this))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}



// TimelineEditorView ���b�Z�[�W �n���h���[

// ��ʕ`��
void TimelineEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� OpenGLView::OnPaint() ���Ăяo���Ȃ��ł��������B

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// �w�i�h��Ԃ�
	glClearColor(TIMELINEBASECOLOR_BRUSH_FLOAT);
	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	DrawTimelineEditorView(dc);

	glFlush();
	SwapBuffers(m_pDC->GetSafeHdc());

	wglMakeCurrent(NULL, NULL);
}

// �k�{�^���_�E��
void TimelineEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	BOOL fRet = m_pTimelineDataOperator->OnLButtonDown(nFlags, point);
	m_pOperatingClipData = m_pTimelineDataOperator->GetOperatingClipData();
	if (fRet)
	{
		Invalidate();
	}
	if (m_pTimelineDataOperator->IsLButtonDown())
	{
		SetCapture();
	}

	OpenGLView::OnLButtonDown(nFlags, point);
}

// �k�{�^���A�b�v
void TimelineEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	BOOL fRet = m_pTimelineDataOperator->OnLButtonUp(nFlags, point);
	m_pOperatingClipData = m_pTimelineDataOperator->GetOperatingClipData();
	if (fRet)
	{
		Invalidate();
	}
	ReleaseCapture();

	OpenGLView::OnLButtonUp(nFlags, point);
}

// �q�{�^���A�b�v
void TimelineEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_pTimelineDataOperator->OnRButtonUp(nFlags, point))
	{
		Invalidate();
	}
	//else
	//{
	//	ClientToScreen(&point);
	//	//OnContextMenu(this, point);
	//}
	OpenGLView::OnRButtonUp(nFlags, point);
}

// �}�E�X�ړ�
void TimelineEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if (m_pTimelineDataOperator->OnMouseMove(nFlags, point))
	{
		Invalidate();
	}
	m_pOperatingClipData = m_pTimelineDataOperator->GetOperatingClipData();

	OpenGLView::OnMouseMove(nFlags, point);
}

// �T�C�Y�ύX
void TimelineEditorView::OnSize(UINT nType, int cx, int cy)
{
	OpenGLView::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	if (m_fInitialize)
	{
		SetPanelRect();
	}

}

// �����ݒ�
void TimelineEditorView::OnInitialUpdate()
{
	OpenGLView::OnInitialUpdate();

	m_fInitialize = TRUE;

	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	m_pTimelineDataOperator = new TimelineDataOperator();
	m_pTimelineDataOperator->InitializeTimelineDataOperator(m_uiTimelineDataOperatorId);

	m_pTimelineDataManager = m_pTimelineDataOperator->GetTimelineDataManager();
	m_uiTimelineDataManager = m_pTimelineDataOperator->GetTimelineDataManagerId();
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManager);
	ASSERT(m_pTrackDataVideoManager);
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManager);
	ASSERT(m_pTrackDataAudioManager);

	InitAreaRect();
	m_pDebugInfoPanelRect->bottom = m_pDebugInfoPanelRect->top + kDebugInfoPanelDefaltHeight;

	//TODO: ����m�F�p	
	InitTestObject();
}

// �j��
void TimelineEditorView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	m_pTimelineDataOperator->DeleteTimelineDataOperator();
	delete m_pTimelineDataOperator;

	delete m_pDebugInfoPanelRect;

}

// �t�@�C���h���b�v
void TimelineEditorView::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CString strFileName;

	if (m_pTimelineDataOperator->OnDropFiles(hDropInfo, strFileName))
	{
		OutputDebugString(strFileName + "\n");
		GetParent()->SetForegroundWindow();
		Invalidate();
	}
	else
	{
		OutputDebugString(_T("DROP FALSE\n"));
	}

	OpenGLView::OnDropFiles(hDropInfo);
}


/*

�`��n

*/
//��ʕ`��
void TimelineEditorView::DrawTimelineEditorView(CPaintDC& dcPaintDC)
{
	CRect rcRect;
	GetClientRect(&rcRect);
	int iHeight = rcRect.Height();
	CString strPoint;

	// �v���r���[�G���A�g���`��
	m_pDebugInfoPanelRect->DrawMyBorderRect(3.0f);
	
	// �^�C�����C���w�b�_�[�R���g���[���p�l���`��
	DrawTimelineControlPanel();
	
	// �V�[�N�o�[�G���A�`��
	DrawSeekBar(dcPaintDC, iHeight);
	
	// �Î~�N���b�v�`��
	DrawClip(iHeight, dcPaintDC);
	
	// ����C���[�W�`��
	if ((m_pTimelineDataOperator->IsSingleInTrim() || m_pTimelineDataOperator->IsSingleOutTrim() || m_pTimelineDataOperator->IsMove())
		&& (!(m_pOperatingClipData->GetOperatingRect()->IsRectEmpty())))
	{
		DrawOperatingClip(dcPaintDC, iHeight);
	}

	if (m_pTimelineDataOperator->EnableDrawDragRect())
	{
		DrawDragAndDropClip(dcPaintDC, iHeight);
	}


	// �g���b�N�w�b�_�[�`��
	DrawTrackHeader();

	// �^�C�����C���f�[�^�G���A�`��
	DrawTimelineDataRect();

	// �g���b�N�g�`��
	DrawTrack(iHeight, dcPaintDC);

	// �^�C�����C���J�[�\���^�V���g������⏕���`��
	DrawTimelineCursor(dcPaintDC, iHeight);

#ifdef _DEBUG
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);

	strFrameNumber.Format(_T("TLCursor %d"), m_pTimelineDataOperator->GetTimelineCursorFramePosition());
	ChangeScreenPointToOpenGLPoint(5, 15, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Frame Left %d Right %d"), m_pTimelineDataOperator->GetLeftFrameNumber(), m_pTimelineDataOperator->GetRightFrameNumber());
	ChangeScreenPointToOpenGLPoint(5, 30, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating TLCursor %d"), m_pTimelineDataOperator->GetOperatingTimelineCursorFramePosition());
	ChangeScreenPointToOpenGLPoint(5, 45, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating Left %d Right %d"), m_pTimelineDataOperator->GetOperatingLeftFrameNumber(), m_pTimelineDataOperator->GetOperatingRightFrameNumber());
	ChangeScreenPointToOpenGLPoint(5, 60, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("CursorLine Point %d"), m_iTimelineCursorPoint);
	ChangeScreenPointToOpenGLPoint(5, 75, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("f/p %d p/f %d"), m_pTimelineDataOperator->GetFramePerPoint(), m_pTimelineDataOperator->GetPointPerFrame());
	ChangeScreenPointToOpenGLPoint(5, 90, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	DeleteObject(hfDrawFont);
#endif

}

// �^�C�����C���R���g���[���p�l���`��
void TimelineEditorView::DrawTimelineControlPanel(void)
{
	m_pTimelineControlPanelRect->DrawMyFillRect();
}

// �V�[�N�o�[�`��
void TimelineEditorView::DrawSeekBar(const CDC& dcPaintDC, const int iHeight)
{

	// �w�i�h��Ԃ�
	m_pSeekBarRect->DrawMyFillRect();

	int iDrawFrame = m_pTimelineDataOperator->GetOperatingLeftFrameNumber();
	if (iDrawFrame < 0)
	{
		iDrawFrame = 0;
	}
	else
	{
		int iSurplusFrame = iDrawFrame % m_pTimelineDataOperator->GetSmallScaleDrawInterval();
		if (iSurplusFrame != 0)
		{
			iDrawFrame += (m_pTimelineDataOperator->GetSmallScaleDrawInterval() - iSurplusFrame);
		}
	}

	POINT pScaleLine;
	pScaleLine.y = m_pSeekBarRect->top;
	pScaleLine.x = m_pTimelineDataOperator->ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);

	while (pScaleLine.x < m_pSeekBarRect->right)
	{
		if (pScaleLine.x > m_pSeekBarRect->left)
		{
			// ��ڐ���
			if ((iDrawFrame % m_pTimelineDataOperator->GetBigScaleDrawInterval()) == 0)
			{
				DrawBigScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
			// ���ڐ���
			else if ((iDrawFrame % m_pTimelineDataOperator->GetMiddleScaleDrawInterval()) == 0)
			{
				DrawMiddleScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
			// ���ڐ���
			else
			{
				DrawSmallScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
		}
		iDrawFrame += m_pTimelineDataOperator->GetSmallScaleDrawInterval();
		pScaleLine.x = m_pTimelineDataOperator->ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);
	}

}

// ��ڐ���`��
void TimelineEditorView::DrawBigScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{

	// TODO: ���i�̓^�C���R�[�h��\��
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("%d"), iDrawFrame);
	CreateDrawFont(SEEKBARTIMECODE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_pSeekBarRect->top + SEEKBARTIMECODE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	// �ڐ���A���C���`��
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->top + SEEKBARBIGSCALE_TOPMARGIN, pScaleLine.x, m_pSeekBarRect->bottom,
		SEEKBARBIGSCALE_LINEWIDTH, SEEKBARBIGSCALECOLOR_BRUSH_FLOAT);
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->bottom + 1, pScaleLine.x, m_pTimelineDataRect->bottom,
		SEEKBARBIGSCALELINE_LINEWIDTH, SEEKBARBIGSCALELINECOLOR_BRUSH_FLOAT);

	DeleteObject(hfDrawFont);
	return;
}

// ���ڐ���`��
void TimelineEditorView::DrawMiddleScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("%d"), iDrawFrame);
	CreateDrawFont(SEEKBARTIMECODE_MIDDLE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_pSeekBarRect->top + SEEKBARTIMECODE_MIDDLE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);
#endif

	// �ڐ���A���C���`��
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->top + SEEKBARMIDDLESCALE_TOPMARGIN, pScaleLine.x, m_pSeekBarRect->bottom,
		SEEKBARMIDDLESCALE_LINEWIDTH, SEEKBARMIDDLESCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->bottom + 1, pScaleLine.x, m_pTimelineDataRect->bottom,
		SEEKBARMIDDLESCALELINE_LINEWIDTH, SEEKBARMIDDLESCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// ���ڐ���`��
void TimelineEditorView::DrawSmallScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{
	// �ڐ���A���C���`��
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->top + SEEKBARSMALLSCALE_TOPMARGIN, pScaleLine.x, m_pSeekBarRect->bottom,
		SEEKBARSMALLSCALE_LINEWIDTH, SEEKBARSMALLSCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_SMALLSCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_pSeekBarRect->bottom + 1, pScaleLine.x, m_pTimelineDataRect->bottom,
		SEEKBARSMALLSCALELINE_LINEWIDTH, SEEKBARSMALLSCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// �g���b�N�w�b�_�[�`��
void TimelineEditorView::DrawTrackHeader(void)
{
	// TODO: �Ƃ肠�������͘g����
	m_pTrackHeaderRect->DrawMyBorderRect();
}

// �g���b�N�`��
void TimelineEditorView::DrawTrack(const int iHeight, const CPaintDC& dcPaintDC)
{
	// TODO: �Ƃ肠�������͘g�Ɩ��O����
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRect;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
	for (int i = 0; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		pTrackDataRect = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i));
		pTrackDataRect->DrawMyBottomLine();
		ChangeScreenPointToOpenGLPoint(pTrackDataRect->left + TRACK_NAME_MARGINLEFT, (pTrackDataRect->top + TRACK_NAME_MARGINTOP), iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(pTrackDataRect->GetTrackName()), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	}
	DeleteObject(hfDrawFont);
}

// �^�C�����C���f�[�^�G���A�`��
void TimelineEditorView::DrawTimelineDataRect(void)
{
	// TODO: �Ƃ肠�������͘g����
	m_pTimelineDataRect->DrawMyBorderRect();
}

// �N���b�v�̕`����s��
BOOL TimelineEditorView::DrawClip(const int iHeight, CPaintDC& dcPaintDC)
{
	int iClipCount = 0;
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	for (int i = 0; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		iClipCount = DrawClipInTrack(m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i)), iHeight, dcPaintDC, iClipCount);

	}
	return TRUE;
}

// �g���b�N���̕\���͈͓��N���b�v���T�[�`���ĕ`��
int TimelineEditorView::DrawClipInTrack(TrackDataRect* pTrackDataRect, const int iHeight, CPaintDC& dcPaintDC, int iClipTotalCount)
{
	//TODO: ����S�T�[�`����̂ł͂Ȃ���vector�Ƃ��ɕ\���͈͂̃I�u�W�F�N�g��ݒ肵�Ă����đ���̂��т�vector���X�V����
	int iStartFrame = m_pTimelineDataOperator->GetOperatingLeftFrameNumber();
	if (iStartFrame < 0)
	{
		iStartFrame = 0;
	}
	ClipDataPositionMap mpClipDataMap;
	int iClipCount = pTrackDataRect->GetTrackDataInfo()->GetClipDataInRange(iStartFrame, m_pTimelineDataOperator->GetOperatingRightFrameNumber(), mpClipDataMap);
	ClipDataRect* pClipData;
	ClipDataRect* pClipDataLeft;
	pClipDataLeft = nullptr;
	if (iClipCount > 0)
	{
#ifdef _DEBUG
		CString strFrameNumber;
		double dPointX, dPointY;
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
#endif
		ClipDataPositionMap::iterator itr = mpClipDataMap.begin();
		while (itr != mpClipDataMap.end())
		{
			pClipData = (*itr).second;
			m_pTimelineDataOperator->CalcClipRectDisplayPoint(static_cast<CRect&>(*pClipData), pClipData, static_cast<CRect>(pTrackDataRect));
			pClipData->SetVert(iHeight);
			pClipData->DrawMyFillRect();
			if (pClipDataLeft != nullptr)
			{
				// �g�����W�V���������h��ւ�
				if (pClipData->left < pClipDataLeft->right)
				{
					m_pTransisionRect->CopyRect(pClipData);
					m_pTransisionRect->right = pClipDataLeft->right;
					m_pTransisionRect->SetVert(iHeight);
					m_pTransisionRect->DrawMyFillRect();
					m_pTransisionRect->DrawMyLeftLine();
					m_pTransisionRect->DrawMyRightLine();
					m_pTransisionRect->SetRectEmpty();

				}
				else if (pClipData->left == pClipDataLeft->right)
				{
					pClipDataLeft->DrawMyRightLine();
				}
			}
			pClipDataLeft = pClipData;
			++itr;

#ifdef _DEBUG
			int iOutPoint = pClipData->GetTimelineInPoint() + pClipData->GetDuration() - 1;
			strFrameNumber.Format(_T(" L %d T %d R %d B %d I %d O %d D %d"), pClipData->left, pClipData->top, pClipData->right, pClipData->bottom, pClipData->GetTimelineInPoint(), iOutPoint, pClipData->GetDuration());
			ChangeScreenPointToOpenGLPoint(5, 105 + (iClipTotalCount * 15), iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(pTrackDataRect->GetTrackName() + strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
#endif
			++iClipTotalCount;
		}
#ifdef _DEBUG
		DeleteObject(hfDrawFont);
#endif
	}
	return iClipTotalCount;
}

// ���쒆�N���b�v�̕`����s��
BOOL TimelineEditorView::DrawOperatingClip(const CDC& dcPaintDC, const int iHeight)
{
	// TODO: ���̃N���b�v�̐F��ς���@���X�e�^�C�~���O�Ŏ��{����悤�ύX
	m_pOperatingClipData->DrawOperatingOldRect(iHeight);

	CRect* rcOperatingRect = m_pOperatingClipData->GetOperatingRect();

	if (m_pTimelineDataOperator->IsSingleInTrim() || m_pTimelineDataOperator->IsSingleOutTrim())
	{
		m_pOperatingClipData->DrawSingleTrimRect(iHeight, m_pTimelineDataOperator->IsSingleInTrim());

#ifdef _DEBUG
		//TODO: �f�o�b�O
		CString strText;
		double dPointX, dPointY;
		int iPoint;
		int iDuration;
		if (m_pTimelineDataOperator->IsSingleInTrim())
		{
			iPoint = m_pOperatingClipData->GetTimelineInPoint() + m_pTimelineDataOperator->GetOperatingClipFrameCount();
		}
		else
		{
			iPoint = m_pOperatingClipData->GetTimelineInPoint();
		}
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
		strText.Format(_T("TrimingClipInPoint  %d"), iPoint);
		ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipLeftPoint  %d"), rcOperatingRect->left);
		ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		if (m_pTimelineDataOperator->IsSingleInTrim())
		{
			iPoint = m_pOperatingClipData->GetTimelineInPoint() + m_pOperatingClipData->GetDuration() - 1;
			iDuration = m_pOperatingClipData->GetDuration() - m_pTimelineDataOperator->GetOperatingClipFrameCount();
		}
		else
		{
			iPoint = m_pOperatingClipData->GetTimelineInPoint() + m_pOperatingClipData->GetDuration() - 1 + m_pTimelineDataOperator->GetOperatingClipFrameCount();
			iDuration = m_pOperatingClipData->GetDuration() + m_pTimelineDataOperator->GetOperatingClipFrameCount();
		}
		strText.Format(_T("TrimingClipOutPoint  %d"), iPoint);
		ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipRightPoint  %d"), rcOperatingRect->right);
		ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("TrimingClipDuration  %d"), iDuration);
		ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		DeleteObject(hfDrawFont);
#endif

	}
	else
	{
		// �h���b�v�ʒu�p
		m_pOperatingClipData->DrawMovingRect(iHeight);

#ifdef _DEBUG
		//TODO: �f�o�b�O
		CString strText;
		double dPointX, dPointY;
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
		strText.Format(_T("MovingClipInPoint  %d"), m_pOperatingClipData->GetTimelineInPoint() + m_pTimelineDataOperator->GetOperatingClipFrameCount());
		ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("MovingClipLeftPoint  %d"), rcOperatingRect->left);
		ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("MovingClipRightPoint  %d"), rcOperatingRect->right);
		ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
#endif

		// �}�E�X�ʒu�ǐ��p
		CRect rcMousePointRect = m_pTimelineDataOperator->GetMousePointRect();
		if (!(rcMousePointRect.EqualRect(rcOperatingRect)))
		{
			m_pOperatingClipData->SetOverlappingVert(static_cast<float>(rcMousePointRect.left), static_cast<float>(iHeight - rcMousePointRect.top),
				static_cast<float>(rcMousePointRect.right), static_cast<float>(iHeight - rcMousePointRect.bottom));
			m_pOperatingClipData->DrawOverlappingRect(iHeight);

#ifdef _DEBUG
			strText.Format(_T("MouseMoveClipLeftPoint  %d"), rcMousePointRect.left);
			ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			strText.Format(_T("MouseMoveClipRightPoint  %d"), rcMousePointRect.right);
			ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			DeleteObject(hfDrawFont);
#endif
		}
	}
	return TRUE;
}

// �h���b�O���h���b�v���̃C���[�W��`�悷��
BOOL TimelineEditorView::DrawDragAndDropClip(const CDC& dcPaintDC, const int iHeight)
{
	ClipDataRect* pClipRect = m_pTimelineDataOperator->GetDragAndDropClipDataRect();
	pClipRect->DrawMovingRect(iHeight);

#ifdef _DEBUG
	//TODO: �f�o�b�O
	CString strText;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
	strText.Format(_T("DropClipInPoint  %d"), pClipRect->GetTimelineInPoint());
	ChangeScreenPointToOpenGLPoint(700, 15, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipLeftPoint  %d"), pClipRect->left);
	ChangeScreenPointToOpenGLPoint(700, 30, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipOutPoint  %d"), pClipRect->GetTimelineOutPoint());
	ChangeScreenPointToOpenGLPoint(700, 45, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipRightPoint  %d"), pClipRect->right);
	ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	strText.Format(_T("DropClipDuration  %d"), pClipRect->GetBorderColor());
	ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);
#endif

	return TRUE;
}

// �^�C�����C���J�[�\���̕`����s��
BOOL TimelineEditorView::DrawTimelineCursor(const CDC& dcPaintDC, const int iHeight)
{
	// ���C����`��
	DrawLine(iHeight, m_iTimelineCursorPoint, m_pSeekBarRect->top, m_iTimelineCursorPoint, m_pTimelineDataRect->bottom,
		TIMELINECURSOR_LINEWIDTH, TIMELINECURSORCOLOR_BRUSH_FLOAT);

	//if (m_fDragShuttling)
	//{
	//	blAlphaBlend.SourceConstantAlpha = kTimelineCursorDragGuideLineAlpha;

	//	//TODO: �ʒu�͌������K�v
	//	CString strText;
	//	int iOldBkMode = dcMemDc.SetBkMode(TRANSPARENT);
	//	COLORREF crOldTextColor = dcMemDc.SetTextColor(SEEKBARTIMECODETEXTCOLOR_BRUSH);
	//	strText.Format(_T("�~  %d"), m_fSuttleSpeed);
	//	dcMemDc.TextOutW(90, m_pTimelineControlPanelRect.bottom - 20, strText);
	//	dcMemDc.SetBkMode(iOldBkMode);
	//	dcMemDc.SetTextColor(crOldTextColor);

	//	//�⏕���̕`��
	//	CRect rcShuttleLineRect;
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragOneSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragTowSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragFourSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragEightSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragSixteenSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragThirtyTwoSpeed);
	//	DrawShuttleGuideLine(dcMemDc, dcMovingMemDc, blAlphaBlend, rcShuttleLineRect, kTimelineCursorDragSixtyFourSpeed);

	//}

	return TRUE;
}

//// �V���g�����쎞�̃K�C�h���C����\������
//void TimelineEditorView::DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcLineRect, float fGuideAreaWidth)
//{
//	int iGuideAreaWidth = static_cast<int>(floor(m_pTimelineDataRect.Width() * fGuideAreaWidth));
//
//	rcLineRect.left = m_iTimelineCursorPoint + iGuideAreaWidth - kTimelineCursorDragGuideLineThickness;
//	rcLineRect.right = rcLineRect.left + kTimelineCursorDragGuideLineThickness;
//	rcLineRect.top = m_pTimelineDataRect.top;
//	rcLineRect.bottom = m_pTimelineDataRect.bottom;
//
//	// ���C����`��
//	CBrush brShuttleGuidLineBrush(TIMELINECURSORDRAGGUIDELINECOLOR_BRUSH);
//	dcMovingMemDc.FillRect(rcLineRect, &brShuttleGuidLineBrush);
//	dcMemDc.AlphaBlend(rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		&dcMovingMemDc, rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		blAlphaBlend);
//
//	rcLineRect.left = m_iTimelineCursorPoint - iGuideAreaWidth;
//	rcLineRect.right = rcLineRect.left + kTimelineCursorDragGuideLineThickness;
//
//	// ���C����`��
//	dcMovingMemDc.FillRect(rcLineRect, &brShuttleGuidLineBrush);
//	brShuttleGuidLineBrush.DeleteObject();
//	dcMemDc.AlphaBlend(rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		&dcMovingMemDc, rcLineRect.left, rcLineRect.top, rcLineRect.Width(), rcLineRect.Height(),
//		blAlphaBlend);
//}



/*

�\���ʒu�v�Z

*/
// View�̃T�C�Y����e�\���p�l���̍��W���v�Z���Đݒ肷��B
void TimelineEditorView::SetPanelRect(void)
{
	CRect rcViewRect;
	GetClientRect(&rcViewRect);
	double dLeft = 0.0, dRight = 0.0, dTop = 0.0, dBottom = 0.0;

	float fViewHeight = static_cast<float>(rcViewRect.Height());
	long lViewHeight = static_cast<long>(floor((fViewHeight - static_cast<float>(m_pSeekBarRect->Height())) / 2));

	// TODO: ������̓����o�ϐ��ɁI
	int iTimelineEditHeaderDefaltHeight = kTimelineEditHeaderDefaltHeight;
	int iTimelineControlPanelDefaultWidth = kTimelineControlPanelDefaultWidth;

	// �v���r���[�G���A�z�u
	m_pDebugInfoPanelRect->CopyRect(rcViewRect);
	m_pDebugInfoPanelRect->bottom = rcViewRect.top + lViewHeight;
	m_pDebugInfoPanelRect->SetVert(rcViewRect.Height());

	// �^�C�����C���ҏW�G���A�z�u
	m_pTimelineEditPanelRect->CopyRect(rcViewRect);
	m_pTimelineEditPanelRect->top = m_pDebugInfoPanelRect->bottom + kSplitterHeight;
	m_pTimelineEditPanelRect->SetVert(rcViewRect.Height());

	// �^�C�����C���w�b�_�[�G���A�̔z�u
	m_pTimelineEditHeaderRect->CopyRect(m_pTimelineEditPanelRect);
	m_pTimelineEditHeaderRect->bottom = m_pTimelineEditHeaderRect->top + iTimelineEditHeaderDefaltHeight;
	m_pTimelineEditHeaderRect->SetVert(rcViewRect.Height());

	// �^�C�����C���R���g���[���G���A�̔z�u
	m_pTimelineControlPanelRect->CopyRect(m_pTimelineEditHeaderRect);
	m_pTimelineControlPanelRect->right = m_pTimelineControlPanelRect->left + iTimelineControlPanelDefaultWidth;
	m_pTimelineControlPanelRect->SetVert(rcViewRect.Height());

	// �V�[�N�o�[�G���A�̔z�u
	m_pSeekBarRect->CopyRect(m_pTimelineEditHeaderRect);
	m_pSeekBarRect->left = m_pTimelineControlPanelRect->right + kSplitterWidth;
	m_pSeekBarRect->SetVert(rcViewRect.Height());

	// �g���b�N�w�b�_�̔z�u
	m_pTrackHeaderRect->CopyRect(m_pTimelineEditPanelRect);
	m_pTrackHeaderRect->top = m_pTimelineEditHeaderRect->bottom + kSplitterHeight;
	m_pTrackHeaderRect->right = m_pTimelineControlPanelRect->right;
	m_pTrackHeaderRect->SetVert(rcViewRect.Height());

	// �^�C�����C���f�[�^�G���A�̔z�u
	m_pTimelineDataRect->CopyRect(m_pTimelineEditPanelRect);
	m_pTimelineDataRect->left = m_pSeekBarRect->left;
	m_pTimelineDataRect->top = m_pTrackHeaderRect->top;
	m_pTimelineDataRect->SetVert(rcViewRect.Height());

	// �^�C�����C���J�[�\���q�b�g�̈�̔z�u
	m_pTimelineCursorHitArea->CopyRect(m_pTimelineEditPanelRect);
	m_pTimelineCursorHitArea->top = m_pSeekBarRect->top;
	m_pTimelineCursorHitArea->left = m_iTimelineCursorPoint - kTimelineCursorDragArea;
	m_pTimelineCursorHitArea->right = m_iTimelineCursorPoint + kTimelineCursorDragArea;
	m_pTimelineCursorHitArea->SetVert(rcViewRect.Height());

	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRectBefor = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(0));
	pTrackDataRectBefor->CopyRect(m_pTimelineEditPanelRect);
	pTrackDataRectBefor->top = m_pTrackHeaderRect->top;
	pTrackDataRectBefor->bottom = pTrackDataRectBefor->top + pTrackDataRectBefor->GetHeight();
	pTrackDataRectBefor->SetVert(rcViewRect.Height());
	TrackDataRect* pTrackDataRect = nullptr;
	for (int i = 1; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		pTrackDataRect = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i));
		pTrackDataRect->CopyRect(pTrackDataRectBefor);
		pTrackDataRect->top = pTrackDataRectBefor->bottom + 1;
		pTrackDataRect->bottom = pTrackDataRect->top + pTrackDataRect->GetHeight();
		pTrackDataRect->SetVert(rcViewRect.Height());
		pTrackDataRectBefor = pTrackDataRect;
	}

	m_pTimelineDataOperator->CalcTimelineDisplayRange();
	m_iTimelineCursorPoint = m_pTimelineDataOperator->GetTimelineCursorPoint();

	return;
}

/*

�����ݒ�n

*/
// �\���G���A�̏����ݒ�
void TimelineEditorView::InitAreaRect(void)
{
	// TODO: �F�͌�Œ萔�����
	m_pDebugInfoPanelRect = new OpenGLRect();
	m_pDebugInfoPanelRect->SetRectEmpty();
	m_pDebugInfoPanelRect->SetBothColor(ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT);

	m_pTimelineEditPanelRect = m_pTimelineDataOperator->GetTimelineEditPanelRect();
	m_pTimelineEditPanelRect->SetRectEmpty();
	m_pTimelineEditPanelRect->SetBothColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_pTimelineEditHeaderRect = m_pTimelineDataOperator->GetTimelineEditHeaderRect();
	m_pTimelineEditHeaderRect->SetRectEmpty();

	m_pTimelineControlPanelRect = m_pTimelineDataOperator->GetTimelineControlPanelRect();
	m_pTimelineControlPanelRect->SetRectEmpty();
	m_pTimelineControlPanelRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, REDCOLOR_BRUSH_FLOAT, GREENCOLOR_BRUSH_FLOAT);

	m_pSeekBarRect = m_pTimelineDataOperator->GetSeekBarRect();
	m_pSeekBarRect->SetRectEmpty();
	m_pSeekBarRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT);

	m_pTrackHeaderRect = m_pTimelineDataOperator->GetTrackHeaderRect();
	m_pTrackHeaderRect->SetRectEmpty();
	m_pTrackHeaderRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_pTimelineDataRect = m_pTimelineDataOperator->GetTimelineDataRect();
	m_pTimelineDataRect->SetRectEmpty();
	m_pTimelineDataRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_pTimelineCursorHitArea = m_pTimelineDataOperator->GetTimelineCursorHitArea();
	m_pTimelineCursorHitArea->SetRectEmpty();

	m_pTransisionRect = m_pTimelineDataOperator->GetTransisionRect();
	m_pTransisionRect->SetRectEmpty();
	m_pTransisionRect->SetColor(ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT);
	m_pTransisionRect->SetBorderColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);
}

// ����m�F�p�I�u�W�F�N�g�̏����ݒ�
void TimelineEditorView::InitTestObject(void)
{
	UUID uiTrackId, uiTrackRectId;

	m_pTrackDataVideoManager->CreateTrackData(0, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(1, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(2, uiTrackId, uiTrackRectId);

	UUID uiClipManagerId, uiClipId, uiClipRectId;

	ClipDataRect* pClipDataRect = nullptr;
	m_pTimelineDataManager->GetClipDataManager(uiClipManagerId)->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pTimelineDataManager->GetClipDataManager(uiClipManagerId)->GetClipDataRect(uiClipRectId);
	pClipDataRect->SetTimelineInPoint(101);
	pClipDataRect->SetDuration(10);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->GetTimelineInPoint(), pClipDataRect);

	m_pTimelineDataManager->GetClipDataManager(uiClipManagerId)->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pTimelineDataManager->GetClipDataManager(uiClipManagerId)->GetClipDataRect(uiClipRectId);
	pClipDataRect->SetTimelineInPoint(600);
	pClipDataRect->SetDuration(100);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->GetTimelineInPoint(), pClipDataRect);

	m_pTimelineDataManager->GetClipDataManager(uiClipManagerId)->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pTimelineDataManager->GetClipDataManager(uiClipManagerId)->GetClipDataRect(uiClipRectId);
	pClipDataRect->SetTimelineInPoint(300);
	pClipDataRect->SetDuration(50);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->GetTimelineInPoint(), pClipDataRect);



}

// �\���{���֘A�̃}�b�v�쐬
void TimelineEditorView::CreateZoomMap(void)
{
}











DROPEFFECT TimelineEditorView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	m_pTimelineDataOperator->OnDragEnter(pDataObject, dwKeyState, point);
	if (m_pTimelineDataOperator->EnableDrawDragRect())
	{
		Invalidate();
	}
	return OpenGLView::OnDragEnter(pDataObject, dwKeyState, point);
}



void TimelineEditorView::OnDragLeave()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	m_pTimelineDataOperator->OnDragLeave();
	Invalidate();

	OpenGLView::OnDragLeave();
}


DROPEFFECT TimelineEditorView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	Invalidate();
	return m_pTimelineDataOperator->OnDragOver(pDataObject, dwKeyState, point);

	//return OpenGLView::OnDragOver(pDataObject, dwKeyState, point);
}


BOOL TimelineEditorView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	OutputDebugString(_T("Drop\n"));

	return OpenGLView::OnDrop(pDataObject, dropEffect, point);
}
