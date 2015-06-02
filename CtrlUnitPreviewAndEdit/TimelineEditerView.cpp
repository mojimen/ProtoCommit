// TimelineEditerView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineEditerView.h"

#include "OpenGLRect.h"
#include "ClipDataRect.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
#include "TrackDataManager.h"
//#include "math.h"

#include <map>


// TimelineEditerView

IMPLEMENT_DYNCREATE(TimelineEditerView, OpenGLView)

TimelineEditerView::TimelineEditerView()
{

}

TimelineEditerView::~TimelineEditerView()
{
}

BEGIN_MESSAGE_MAP(TimelineEditerView, OpenGLView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// TimelineEditerView �`��

void TimelineEditerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �`��R�[�h�������ɒǉ����Ă��������B
}


// TimelineEditerView �f�f

#ifdef _DEBUG
void TimelineEditerView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void TimelineEditerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// TimelineEditerView ���b�Z�[�W �n���h���[
// ��ʕ`��
void TimelineEditerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� OpenGLView::OnPaint() ���Ăяo���Ȃ��ł��������B

	CRect rcRect;
	GetClientRect(&rcRect);
	int iHeight = rcRect.Height();
	CString strPoint;

	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// �w�i�h��Ԃ�
	::glClearColor(TIMELINEBASECOLOR_BRUSH_FLOAT);
	::glClear(GL_COLOR_BUFFER_BIT);
	//::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// �v���r���[�G���A�g���`��
	m_prcDebugInfoPanelRect->DrawMyBorderRect(3.0f);


	// �^�C�����C���w�b�_�[�R���g���[���p�l���`��
	DrawTimelineControlPanel();

	// �V�[�N�o�[�G���A�`��
	DrawSeekBar(dc, iHeight);

	//// �Î~�N���b�v�`��
	DrawClip(iHeight);

	// ����C���[�W�`��
	if ((m_fSingleInTriming || m_fSingleOutTriming || m_fMoving) && (!(m_clMovingClipData->GetOperatingRect()->IsRectEmpty())))
	{
		DrawOperatingClip(dc, iHeight);
	}

	// �g���b�N�w�b�_�[�`��
	DrawTrackHeader();

	// �^�C�����C���f�[�^�G���A�g���`��
	DrawTimelineDataRect();

	// �g���b�N�g�`��
	DrawTrack(iHeight, dc);

	// �^�C�����C���J�[�\���^�V���g������⏕���`��
	DrawTimelineCursor(dc, iHeight);



#ifdef PROTOTYPEMODE
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);

	strFrameNumber.Format(_T("TLCursor %d"), m_iTimelineCursorFramePosition);
	ChangeScreenPointToOpenGLPoint(5, 15, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Frame Left %d Right %d"), m_iLeftFrameNumber, m_iRightFrameNumber);
	ChangeScreenPointToOpenGLPoint(5, 30, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating TLCursor %d"), m_iTimelineCursorFramePosition + m_iOperatingFrameCount);
	ChangeScreenPointToOpenGLPoint(5, 45, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("Operating Left %d Right %d"), m_iLeftFrameNumber + m_iOperatingFrameCount, m_iRightFrameNumber + m_iOperatingFrameCount);
	ChangeScreenPointToOpenGLPoint(5, 60, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("CursorLine Point %d"), m_iTimelineCursorPoint);
	ChangeScreenPointToOpenGLPoint(5, 75, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	strFrameNumber.Format(_T("f/p %d p/f %d"), m_iFramePerPoint, m_iPointPerFrame);
	ChangeScreenPointToOpenGLPoint(5, 150, iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	DeleteObject(hfDrawFont);
#endif



	::glFlush();
	::SwapBuffers(m_pDC->GetSafeHdc());

	::wglMakeCurrent(NULL, NULL);
}

// �k�{�^���_�E��
void TimelineEditerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	// �^�C�����C���f�[�^�G���A������
	if (m_prcTimelineDataRect->PtInRect(point))
	{
		//BOOL fRet;
		//fRet = m_pTimelineDataOperator->OnLButtonDown(nFlags, point);
		// �g���b�N����
		m_clSelectedTrack = IsPointInAnyTrack(point);
		m_clSelectedTrackInfo = m_clSelectedTrack->GetTrackDataInfo();
		if (m_clSelectedTrack != nullptr)
		{
			m_clOperateToTrack = m_clSelectedTrack;
			m_clOperateToTrackInfo = m_clSelectedTrackInfo;

			if (IsPointInAnyClipRect(point))
			{
				m_fLButtonClicking = TRUE;
				SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
				m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
				m_clMovingClipData->SetOperatingRect(static_cast<CRect>(m_clMovingClipData));
				m_rcMousePointRect.CopyRect(static_cast<CRect>(m_clMovingClipData));
				m_iOperatingClipFrameCount = 0;
				m_iEnableMovingFrameCount = 0;
				m_pEnableMovingTrack = nullptr;
				Invalidate();
			}
		}
		if (!m_fLButtonClicking)
		{
			//// �^�C�����C���J�[�\������
			//else if (m_prcTimelineCursorHitArea.PtInRect(point))
			//{
			//	m_fLButtonClicking = TRUE;
			//	m_fDragShuttling = TRUE;
			//	SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
			//	m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
			//	m_iOperatingFrameCount = 0;
			//	m_fSuttleSpeed = 0;
			//	Invalidate();
			//}
		}
	}
	else
	{
		// �^�C�����C���J�[�\������
		if (m_prcTimelineCursorHitArea->PtInRect(point))
		{
			m_fLButtonClicking = TRUE;
			m_fDragShuttling = TRUE;
			SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
			m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
			m_iOperatingFrameCount = 0;
			m_fSuttleSpeed = 0;
			Invalidate();
		}
		// �V�[�N�o�[������
		else if (IsPointInSeekBar(point))
		{
			m_fLButtonClicking = TRUE;
			m_fScrubing = TRUE;
			SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
			m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
			m_iOperatingFrameCount = 0;

			//TODO: �f�o�b�O�p�Ȃ̂Ō�ō폜
#ifdef PROTOTYPEMODE
			double dPointX, dPointY;
			CPaintDC dc(this);
			int iDummy;
			int iFrame = ChangeDisplayPointToTimelineFramePosition(point, iDummy);
			CRect rcViewRect;
			GetClientRect(&rcViewRect);
			HFONT hfDrawFont;
			CreateDrawFont(10, 0, DEFAULT_FONTFACE, hfDrawFont);
			CString strFrameNumber;
			strFrameNumber.Format(_T("SEEKBAR CLICK FRAME %d"), iFrame);
			ChangeScreenPointToOpenGLPoint(350, 15, rcViewRect.Height(), dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			strFrameNumber.Format(_T("SEEKBAR CLICK POINT %d"), point.x);
			ChangeScreenPointToOpenGLPoint(350, 30, rcViewRect.Height(), dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dc.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			DeleteObject(hfDrawFont);
			DeleteObject(dc);
#endif
		}
		// �^�C�����C���R���g���[���p�l��������
		else if (IsPointInTimelineControlPanel(point))
		{
			if (m_iFramePerPoint < 1)
			{
				--m_iPointPerFrame;
				if (m_iPointPerFrame < 1)
				{
					m_iFramePerPoint = 2;
				}
			}
			else
			{
				if (m_iFramePerPoint <= 216000)
				{
					// TODO: �����ƌ����悭�I
					if (m_iFramePerPoint <10)
					{
						++m_iFramePerPoint;
					}
					else if (m_iFramePerPoint <60)
					{
						m_iFramePerPoint += 5;
					}
					else if (m_iFramePerPoint <600)
					{
						m_iFramePerPoint += 60;
					}
					else if (m_iFramePerPoint <3600)
					{
						m_iFramePerPoint += 300;
					}
					else if (m_iFramePerPoint <36000)
					{
						m_iFramePerPoint += 18000;
					}
					else
					{
						m_iFramePerPoint += 36000;
						if (m_iFramePerPoint > 216000)
						{
							m_iFramePerPoint = 216000;
						}
					}
				}
				else
				{
					m_iFramePerPoint = 216000;
				}
			}
			if (ChangeDisplayScale())
			{
				Invalidate();
			}
		}
	}


	OpenGLView::OnLButtonDown(nFlags, point);
}

// �k�{�^���A�b�v
void TimelineEditerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	m_fLButtonClicking = FALSE; // �t���O��������
	ReleaseCapture(); // �}�E�X��������܂��B

	if (m_fMoving)
	{
		m_clSelectedTrackInfo->DeleteClip(m_clMovingClipData->m_iTimelineInPoint);
		m_clMovingClipData->m_iTimelineInPoint += m_iOperatingClipFrameCount;
		(m_pEnableMovingTrack->GetTrackDataInfo())->AddClip(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData);
		//m_clOperateToTrackInfo->AddClip(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData);
		m_clMovingClipData->CopyRect(m_clMovingClipData->GetOperatingRect());
		//TODO: ������͕��������Ȃ��Ƃ����Ȃ��|���ړ����͂������ǐÎ~��Ԃ̎��Ƀg���b�N����In�_�������Ă���̂�����I
		//m_clMovingClipData->m_iTimelineInPoint = 0;
	}
	else if (m_fSingleInTriming)
	{
		m_clSelectedTrackInfo->ChangeClip(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount, m_clMovingClipData);
		m_clMovingClipData->m_iTimelineInPoint += m_iOperatingClipFrameCount;
		int iClipDuration = m_clMovingClipData->GetDuration();
		// In���ɐL�т�i�}�C�i�X�����ւ̈ړ��j�͒��������Z
		m_clMovingClipData->SetDuration(iClipDuration - m_iOperatingClipFrameCount);
		m_clMovingClipData->CopyRect(m_clMovingClipData->GetOperatingRect());
	}
	else if (m_fSingleOutTriming)
	{
		int iClipDuration = m_clMovingClipData->GetDuration();
		m_clMovingClipData->SetDuration(iClipDuration + m_iOperatingClipFrameCount);
		m_clMovingClipData->CopyRect(m_clMovingClipData->GetOperatingRect());
	}
	else if (m_fScrubing)
	{
		if ((point.x == m_poMousePointerLocation.x) && (point.y == m_poMousePointerLocation.y))
		{
			int iDummy;
			int iFrame = ChangeDisplayPointToTimelineFramePosition(point, iDummy);
			// TODO: �ł���Ύ���
			//DrawAnimation(iFrame - m_iTimelineCursorFramePosition);
			m_iOperatingFrameCount = iFrame - m_iTimelineCursorFramePosition;
			m_iTimelineCursorFramePosition = iFrame;
			m_iLeftFrameNumber += m_iOperatingFrameCount;
			m_iRightFrameNumber += m_iOperatingFrameCount;
		}
		else
		{
			m_iTimelineCursorFramePosition += m_iOperatingFrameCount;
			m_iLeftFrameNumber += m_iOperatingFrameCount;
			m_iRightFrameNumber += m_iOperatingFrameCount;
		}
	}

	m_rcMousePointRect.SetRectEmpty();
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;
	m_iEnableMovingFrameCount = 0;
	m_pEnableMovingTrack = nullptr;

	Invalidate();

	m_clMovingClipData = nullptr;
	m_fMoving = FALSE;
	m_fSingleInTriming = FALSE;
	m_fSingleOutTriming = FALSE;
	m_fScrubing = FALSE;
	m_fDragShuttling = FALSE;

	OpenGLView::OnLButtonUp(nFlags, point);
}

// �q�{�^���A�b�v
void TimelineEditerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if (IsPointInTimelineControlPanel(point))
	{
		if (m_iFramePerPoint < 1)
		{
			if (m_iPointPerFrame < 120)
			{
				++m_iPointPerFrame;
			}
		}
		else
		{
			// TODO: �����ƌ����悭�I
			if (m_iFramePerPoint <= 10)
			{
				--m_iFramePerPoint;
			}
			else if (m_iFramePerPoint <= 60)
			{
				m_iFramePerPoint -= 5;
			}
			else if (m_iFramePerPoint <= 600)
			{
				m_iFramePerPoint -= 60;
			}
			else if (m_iFramePerPoint <= 3600)
			{
				m_iFramePerPoint -= 300;
			}
			else if (m_iFramePerPoint <= 36000)
			{
				m_iFramePerPoint -= 18000;
			}
			else
			{
				m_iFramePerPoint -= 36000;
			}
			if (m_iFramePerPoint < 1)
			{
				m_iPointPerFrame = 2;
			}
		}
		if (ChangeDisplayScale())
		{
			Invalidate();
		}
	}
	else
	{
		ClientToScreen(&point);
		//OnContextMenu(this, point);
	}

	//OpenGLView::OnRButtonUp(nFlags, point);
}

// �}�E�X�ړ�
void TimelineEditerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if (m_fLButtonClicking)
	{
		CSize szMoveSize(point - m_poMousePointerLocation);
		if (m_fMoving)
		{
			m_clOperateToTrack = IsPointInAnyTrack(point);
			if (m_clOperateToTrack == nullptr)
			{
				m_clOperateToTrack = m_clSelectedTrack;
			}
			m_clOperateToTrackInfo = m_clOperateToTrack->GetTrackDataInfo();
			m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_clMovingClipData->m_iTimelineInPoint);
			CalcClipRectDisplayPoint(m_rcMousePointRect, m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), m_iOperatingClipFrameCount);
			CheckMove(point);
			CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), m_iOperatingClipFrameCount);
			Invalidate();
		}
		else if (m_fSingleInTriming || m_fSingleOutTriming)
		{
			m_clMovingClipData->SetOperatingRect(static_cast<CRect>(m_clMovingClipData)); // �L�k���̃C���[�W���W
			if (m_fSingleInTriming)
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_clMovingClipData->m_iTimelineInPoint);
				CheckInTrim();
				CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), 0, m_iOperatingClipFrameCount);
			}
			else
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize,
					(m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration()));
				CheckOutTrim();
				CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, static_cast<CRect>(m_clOperateToTrack), 0, 0, m_iOperatingClipFrameCount);
			}
			Invalidate();
		}
		else if (m_fScrubing)
		{
			szMoveSize.cx *= -1;
			m_iOperatingFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_iTimelineCursorFramePosition);
			// �ŏ��͈̓`�F�b�N
			// TODO: �ő�͈̓`�F�b�N�́H
			if ((m_iTimelineCursorFramePosition + m_iOperatingFrameCount) < 0)
			{
				m_iOperatingFrameCount = -1 * m_iTimelineCursorFramePosition;
			}
			Invalidate();
		}
		else if (m_fDragShuttling)
		{
			//// �}�E�X�ʒu����{�����x���擾
			//m_fSuttleSpeed = SetShuttleSpeed(point, szMoveSize);
			//Invalidate();
		}
	}

	OpenGLView::OnMouseMove(nFlags, point);
}

// �T�C�Y�ύX
void TimelineEditerView::OnSize(UINT nType, int cx, int cy)
{
	OpenGLView::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	SetPanelRect();

}

// �����ݒ�
void TimelineEditerView::OnInitialUpdate()
{
	OpenGLView::OnInitialUpdate();

	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	InitAreaRect();

	m_pTimelineDataManager = new TimelineDataManager();
	m_pTimelineDataManager->InitializeTimelineDataManager(m_uiTimelineDataManager);
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManager);
	assert(m_pTrackDataVideoManager);
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManager);
	assert(m_pTrackDataAudioManager);
	m_pClipDataManager = m_pTimelineDataManager->GetClipDataManager(m_uiClipDataManager);
	assert(m_pClipDataManager);
	InitTestObject();

	m_fLButtonClicking = FALSE;
	m_fMoving = FALSE;
	m_fSingleInTriming = FALSE;
	m_fSingleOutTriming = FALSE;
	m_fScrubing = FALSE;
	m_fDragShuttling = FALSE;

	m_prcDebugInfoPanelRect->bottom = m_prcDebugInfoPanelRect->top + kPreviewPanelDefaltHeight;

	m_iLeftFrameNumber = 0;
	m_iRightFrameNumber = 0;
	m_iTimelineCursorFramePosition = 0;
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;

	m_iFramePerPoint = 0;
	m_iPointPerFrame = 1;
	ChangeDisplayScale();


}

// �j��
void TimelineEditerView::OnDestroy()
{
	OpenGLView::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	m_pTimelineDataManager->DeleteTimelineDataManager();
	delete m_pTimelineDataManager;


	delete m_prcDebugInfoPanelRect;
	delete m_prcTimelineEditPanelRect;
	delete m_prcTimelineEditHeaderRect;
	delete m_prcTimelineControlPanelRect;
	delete m_prcSeekBarRect;
	delete m_prcTrackHeaderRect;
	delete m_prcTimelineDataRect;
	delete m_prcTimelineCursorHitArea;

}




/*

�`��n

*/
// �^�C�����C���R���g���[���p�l���`��
void TimelineEditerView::DrawTimelineControlPanel(void)
{
	m_prcTimelineControlPanelRect->DrawMyFillRect();
}

// �V�[�N�o�[�`��
void TimelineEditerView::DrawSeekBar(const CDC& dcPaintDC, const int iHeight)
{

	// �w�i�h��Ԃ�
	m_prcSeekBarRect->DrawMyFillRect();

	int iDrawFrame = m_iLeftFrameNumber + m_iOperatingFrameCount;
	if (iDrawFrame < 0)
	{
		iDrawFrame = 0;
	}
	else
	{
		int iSurplusFrame = iDrawFrame % m_iSmallScaleDrawInterval;
		if (iSurplusFrame != 0)
		{
			iDrawFrame += (m_iSmallScaleDrawInterval - iSurplusFrame);
		}
	}

	POINT pScaleLine;
	pScaleLine.y = m_prcSeekBarRect->top;
	pScaleLine.x = ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);

	while (pScaleLine.x < m_prcSeekBarRect->right)
	{
		if (pScaleLine.x > m_prcSeekBarRect->left)
		{
			// ��ڐ���
			if ((iDrawFrame % m_iBigScaleDrawInterval) == 0)
			{
				DrawBigScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
			// ���ڐ���
			else if ((iDrawFrame % m_iMiddleScaleDrawInterval) == 0)
			{
				DrawMiddleScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
			// ���ڐ���
			else
			{
				DrawSmallScale(dcPaintDC, iDrawFrame, iHeight, pScaleLine);
			}
		}
		iDrawFrame += m_iSmallScaleDrawInterval;
		pScaleLine.x = ChangeTimelineFramePositionToDisplayPoint(iDrawFrame);
	}

}

// ��ڐ���`��
void TimelineEditerView::DrawBigScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{

	// TODO: ���i�̓^�C���R�[�h��\��
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("%d"), iDrawFrame);
	CreateDrawFont(SEEKBARTIMECODE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_prcSeekBarRect->top + SEEKBARTIMECODE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);

	// �ڐ���A���C���`��
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->top + SEEKBARBIGSCALE_TOPMARGIN, pScaleLine.x, m_prcSeekBarRect->bottom,
		SEEKBARBIGSCALE_LINEWIDTH, SEEKBARBIGSCALECOLOR_BRUSH_FLOAT);
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->bottom + 1, pScaleLine.x, m_prcTimelineDataRect->bottom,
		SEEKBARBIGSCALELINE_LINEWIDTH, SEEKBARBIGSCALELINECOLOR_BRUSH_FLOAT);

	DeleteObject(hfDrawFont);
	return;
}

// ���ڐ���`��
void TimelineEditerView::DrawMiddleScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	CString strFrameNumber;
	double dPointX, dPointY;
	HFONT hfDrawFont;
	strFrameNumber.Format(_T("%d"), iDrawFrame);
	CreateDrawFont(SEEKBARTIMECODE_MIDDLE_FONTSIZE, 0, DEFAULT_FONTFACE, hfDrawFont);
	ChangeScreenPointToOpenGLPoint(pScaleLine.x + SEEKBARTIMECODE_MARGINX, m_prcSeekBarRect->top + SEEKBARTIMECODE_MIDDLE_MARGINY,
		iHeight, dPointX, dPointY);
	DrawTextOnGL(static_cast<PCTSTR>(strFrameNumber), dcPaintDC.GetSafeHdc(), hfDrawFont, SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT,
		static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
	DeleteObject(hfDrawFont);
#endif

	// �ڐ���A���C���`��
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->top + SEEKBARMIDDLESCALE_TOPMARGIN, pScaleLine.x, m_prcSeekBarRect->bottom,
		SEEKBARMIDDLESCALE_LINEWIDTH, SEEKBARMIDDLESCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_MIDDLESCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->bottom + 1, pScaleLine.x, m_prcTimelineDataRect->bottom,
		SEEKBARMIDDLESCALELINE_LINEWIDTH, SEEKBARMIDDLESCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// ���ڐ���`��
void TimelineEditerView::DrawSmallScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine)
{
	// �ڐ���A���C���`��
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->top + SEEKBARSMALLSCALE_TOPMARGIN, pScaleLine.x, m_prcSeekBarRect->bottom,
		SEEKBARSMALLSCALE_LINEWIDTH, SEEKBARSMALLSCALECOLOR_BRUSH_FLOAT);

#ifdef SEEKBAR_SMALLSCALELINE_DRAW
	DrawLine(iHeight, pScaleLine.x, m_prcSeekBarRect->bottom + 1, pScaleLine.x, m_prcTimelineDataRect->bottom,
		SEEKBARSMALLSCALELINE_LINEWIDTH, SEEKBARSMALLSCALELINECOLOR_BRUSH_FLOAT);
#endif

	return;
}

// �g���b�N�w�b�_�[�`��
void TimelineEditerView::DrawTrackHeader(void)
{
	// TODO: �Ƃ肠�������͘g����
	m_prcTrackHeaderRect->DrawMyBorderRect();
}

// �g���b�N�`��
void TimelineEditerView::DrawTrack(const int iHeight, const CPaintDC& dcPaintDC)
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
void TimelineEditerView::DrawTimelineDataRect(void)
{
	// TODO: �Ƃ肠�������͘g����
	m_prcTimelineDataRect->DrawMyBorderRect();
}

// �N���b�v�̕`����s��
BOOL TimelineEditerView::DrawClip(const int iHeight)
{
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	for (int i = 0; i < m_pTrackDataVideoManager->GetTrackCount(); ++i)
	{
		DrawClipInTrack(m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i)), iHeight);
	}

	return TRUE;
}

// �g���b�N���̕\���͈͓��N���b�v���T�[�`���ĕ`��
void TimelineEditerView::DrawClipInTrack(TrackDataRect* pTrackDataRect, const int iHeight)
{
	int iStartFrame = m_iLeftFrameNumber + m_iOperatingFrameCount;
	if (iStartFrame < 0)
	{
		iStartFrame = 0;
	}
	ClipDataPositionMap mpClipDataMap;
	int iClipCount = pTrackDataRect->GetTrackDataInfo()->GetClipDataInRange(iStartFrame, m_iRightFrameNumber + m_iOperatingFrameCount, mpClipDataMap);
	ClipDataRect* pClipData;
	ClipDataRect* pClipDataLeft;
	pClipDataLeft = nullptr;
	if (iClipCount > 0)
	{
		ClipDataPositionMap::iterator itr = mpClipDataMap.begin();
		while (itr != mpClipDataMap.end())
		{
			pClipData = (*itr).second;
			CalcClipRectDisplayPoint(static_cast<CRect&>(*pClipData), pClipData, static_cast<CRect>(pTrackDataRect));
			pClipData->SetVert(iHeight);
			pClipData->DrawMyFillRect();
			if (pClipDataLeft != nullptr)
			{
				// �g�����W�V���������h��ւ�
				if (pClipData->left < pClipDataLeft->right)
				{
					m_prcTransisionRect->CopyRect(pClipData);
					m_prcTransisionRect->right = pClipDataLeft->right;
					m_prcTransisionRect->SetVert(iHeight);
					m_prcTransisionRect->DrawMyFillRect();
					m_prcTransisionRect->DrawMyLeftLine();
					m_prcTransisionRect->DrawMyRightLine();
					m_prcTransisionRect->SetRectEmpty();

				}
				else if (pClipData->left == pClipDataLeft->right)
				{
					pClipDataLeft->DrawMyRightLine();
				}
			}
			pClipDataLeft = pClipData;
			++itr;
		}
	}

}

// ���쒆�N���b�v�̕`����s��
BOOL TimelineEditerView::DrawOperatingClip(const CDC& dcPaintDC, const int iHeight)
{
	// TODO: ���̃N���b�v�̐F��ς���@���X�e�^�C�~���O�Ŏ��{����悤�ύX
	m_clMovingClipData->DrawOperatingOldRect(iHeight);

	CRect* rcOperatingRect = m_clMovingClipData->GetOperatingRect();

	if (m_fSingleInTriming || m_fSingleOutTriming)
	{
		m_clMovingClipData->DrawSingleTrimRect(iHeight, m_fSingleInTriming);

#ifdef PROTOTYPEMODE
		//TODO: �f�o�b�O
		CString strText;
		double dPointX, dPointY;
		int iPoint;
		int iDuration;
		if (m_fSingleInTriming)
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount;
		}
		else
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint;
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
		if (m_fSingleInTriming)
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1;
			iDuration = m_clMovingClipData->GetDuration() - m_iOperatingClipFrameCount;
		}
		else
		{
			iPoint = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + m_iOperatingClipFrameCount;
			iDuration = m_clMovingClipData->GetDuration() + m_iOperatingClipFrameCount;
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
		m_clMovingClipData->DrawMovingRect(iHeight);

#ifdef PROTOTYPEMODE
		//TODO: �f�o�b�O
		CString strText;
		double dPointX, dPointY;
		HFONT hfDrawFont;
		CreateDrawFont(13, 0, DEFAULT_FONTFACE, hfDrawFont);
		strText.Format(_T("MovingClipInPoint  %d"), m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount);
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
		strText.Format(_T("BorderClipLeftPoint  %d"), m_rcMousePointRect.left);
		ChangeScreenPointToOpenGLPoint(700, 60, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
		strText.Format(_T("BorderClipRightPoint  %d"), m_rcMousePointRect.right);
		ChangeScreenPointToOpenGLPoint(700, 75, iHeight, dPointX, dPointY);
		DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
			static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
#endif

		// �}�E�X�ʒu�ǐ��p
		if ((!(m_rcMousePointRect.EqualRect(rcOperatingRect))) /*&& ((m_rcMousePointRect->m_iTimelineInPoint + m_iOperatingClipFrameCount) > 0)*/)
		{
			double dLeft, dRight, dTop, dBottom;
			ChangeScreenRectToOpenGLPoint(m_rcMousePointRect, iHeight, dLeft, dRight, dTop, dBottom);
			m_clMovingClipData->SetOverlappingVert(static_cast<float>(m_rcMousePointRect.left), static_cast<float>(iHeight - m_rcMousePointRect.top),
				static_cast<float>(m_rcMousePointRect.right), static_cast<float>(iHeight - m_rcMousePointRect.bottom));
			m_clMovingClipData->DrawOverlappingRect(iHeight);

#ifdef PROTOTYPEMODE
			strText.Format(_T("ClipBorderLeftPoint  %d"), m_rcMousePointRect.left);
			ChangeScreenPointToOpenGLPoint(700, 90, iHeight, dPointX, dPointY);
			DrawTextOnGL(static_cast<PCTSTR>(strText), dcPaintDC.GetSafeHdc(), hfDrawFont, BLACKCOLOR_BRUSH_FLOAT,
				static_cast<float>(dPointX), static_cast<float>(dPointY), TIMELINE_DEFAULTZ, 1.0f);
			DeleteObject(hfDrawFont);
#endif
		}
	}

	return TRUE;
}

// �^�C�����C���J�[�\���̕`����s��
BOOL TimelineEditerView::DrawTimelineCursor(const CDC& dcPaintDC, const int iHeight)
{
	// ���C����`��
	DrawLine(iHeight, m_iTimelineCursorPoint, m_prcSeekBarRect->top, m_iTimelineCursorPoint, m_prcTimelineDataRect->bottom,
		TIMELINECURSOR_LINEWIDTH, TIMELINECURSORCOLOR_BRUSH_FLOAT);

	//if (m_fDragShuttling)
	//{
	//	blAlphaBlend.SourceConstantAlpha = kTimelineCursorDragGuideLineAlpha;

	//	//TODO: �ʒu�͌������K�v
	//	CString strText;
	//	int iOldBkMode = dcMemDc.SetBkMode(TRANSPARENT);
	//	COLORREF crOldTextColor = dcMemDc.SetTextColor(SEEKBARTIMECODETEXTCOLOR_BRUSH);
	//	strText.Format(_T("�~  %d"), m_fSuttleSpeed);
	//	dcMemDc.TextOutW(90, m_prcTimelineControlPanelRect.bottom - 20, strText);
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
//void TimelineEditerView::DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcLineRect, float fGuideAreaWidth)
//{
//	int iGuideAreaWidth = static_cast<int>(floor(m_prcTimelineDataRect.Width() * fGuideAreaWidth));
//
//	rcLineRect.left = m_iTimelineCursorPoint + iGuideAreaWidth - kTimelineCursorDragGuideLineThickness;
//	rcLineRect.right = rcLineRect.left + kTimelineCursorDragGuideLineThickness;
//	rcLineRect.top = m_prcTimelineDataRect.top;
//	rcLineRect.bottom = m_prcTimelineDataRect.bottom;
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

�\���ʒu�v�Z�n

*/
// View�̃T�C�Y����e�\���p�l���̍��W���v�Z���Đݒ肷��B
void TimelineEditerView::SetPanelRect(void)
{
	CRect rcViewRect;
	GetClientRect(&rcViewRect);
	double dLeft = 0.0, dRight = 0.0, dTop = 0.0, dBottom = 0.0;

	float fViewHeight = static_cast<float>(rcViewRect.Height());
	long lViewHeight = static_cast<long>(floor((fViewHeight - static_cast<float>(m_prcSeekBarRect->Height())) / 2));

	// TODO: ������̓����o�ϐ��ɁI
	int iTimelineEditHeaderDefaltHeight = kTimelineEditHeaderDefaltHeight;
	int iTimelineControlPanelDefaultWidth = kTimelineControlPanelDefaultWidth;

	// �v���r���[�G���A�z�u
	m_prcDebugInfoPanelRect->CopyRect(rcViewRect);
	m_prcDebugInfoPanelRect->bottom = rcViewRect.top + lViewHeight;
	m_prcDebugInfoPanelRect->SetVert(rcViewRect.Height());

	// �^�C�����C���ҏW�G���A�z�u
	m_prcTimelineEditPanelRect->CopyRect(rcViewRect);
	m_prcTimelineEditPanelRect->top = m_prcDebugInfoPanelRect->bottom + kSplitterHeight;
	m_prcTimelineEditPanelRect->SetVert(rcViewRect.Height());

	// �^�C�����C���w�b�_�[�G���A�̔z�u
	m_prcTimelineEditHeaderRect->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTimelineEditHeaderRect->bottom = m_prcTimelineEditHeaderRect->top + iTimelineEditHeaderDefaltHeight;
	m_prcTimelineEditHeaderRect->SetVert(rcViewRect.Height());

	// �^�C�����C���R���g���[���G���A�̔z�u
	m_prcTimelineControlPanelRect->CopyRect(m_prcTimelineEditHeaderRect);
	m_prcTimelineControlPanelRect->right = m_prcTimelineControlPanelRect->left + iTimelineControlPanelDefaultWidth;
	m_prcTimelineControlPanelRect->SetVert(rcViewRect.Height());

	// �V�[�N�o�[�G���A�̔z�u
	m_prcSeekBarRect->CopyRect(m_prcTimelineEditHeaderRect);
	m_prcSeekBarRect->left = m_prcTimelineControlPanelRect->right + kSplitterWidth;
	m_prcSeekBarRect->SetVert(rcViewRect.Height());

	// �g���b�N�w�b�_�̔z�u
	m_prcTrackHeaderRect->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTrackHeaderRect->top = m_prcTimelineEditHeaderRect->bottom + kSplitterHeight;
	m_prcTrackHeaderRect->right = m_prcTimelineControlPanelRect->right;
	m_prcTrackHeaderRect->SetVert(rcViewRect.Height());

	// �^�C�����C���f�[�^�G���A�̔z�u
	m_prcTimelineDataRect->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTimelineDataRect->left = m_prcSeekBarRect->left;
	m_prcTimelineDataRect->top = m_prcTrackHeaderRect->top;
	m_prcTimelineDataRect->SetVert(rcViewRect.Height());

	// �^�C�����C���J�[�\���q�b�g�̈�̔z�u
	m_prcTimelineCursorHitArea->CopyRect(m_prcTimelineEditPanelRect);
	m_prcTimelineCursorHitArea->top = m_prcSeekBarRect->top;
	m_prcTimelineCursorHitArea->left = m_iTimelineCursorPoint - kTimelineCursorDragArea;
	m_prcTimelineCursorHitArea->right = m_iTimelineCursorPoint + kTimelineCursorDragArea;
	m_prcTimelineCursorHitArea->SetVert(rcViewRect.Height());

	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRectBefor = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(0));
	pTrackDataRectBefor->CopyRect(m_prcTimelineEditPanelRect);
	pTrackDataRectBefor->top = m_prcTrackHeaderRect->top;
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

	CalcTimelineDisplayRange();

	return;
}

// �\���\�ȃ^�C�����C���͈̔́i���[�A�E�[�j���v�Z����
void TimelineEditerView::CalcTimelineDisplayRange()
{
	// �\���\�t���[���͈͂̌v�Z
	int iDisplayFrameCount = 0;
	if (m_iPointPerFrame < 1)
	{
		iDisplayFrameCount = m_prcSeekBarRect->Width() * m_iFramePerPoint;
	}
	else
	{
		iDisplayFrameCount = static_cast<int>(ceil(m_prcSeekBarRect->Width() / m_iPointPerFrame));
	}
	m_iTimelineCursorPoint = static_cast<int>(floor(m_prcSeekBarRect->Width() / 2.0f)) + m_prcSeekBarRect->left;
	m_iLeftFrameNumber = m_iTimelineCursorFramePosition - static_cast<int>(floor((iDisplayFrameCount / 2.0f)));
	m_iRightFrameNumber = m_iTimelineCursorFramePosition + static_cast<int>(ceil((iDisplayFrameCount / 2.0f))) + 1;
}

// �^�C�����C���f�[�^�\���{���̕ύX
BOOL TimelineEditerView::ChangeDisplayScale(void)
{
	//int kDisplayScaleArray[13] = { 1, 2, 3, 6, 10, 30, 60, 300, 600, 1800, 3600, 18000, 36000 };
	//int iArrayCount = sizeof kDisplayScaleArray / sizeof kDisplayScaleArray[0];

	if ((m_iFramePerPoint == 1) || (m_iPointPerFrame == 1))
	{
		m_iSmallScaleDrawInterval = 20;
		m_iMiddleScaleDrawInterval = 60;
		m_iBigScaleDrawInterval = 300;
	}
	else if (m_iFramePerPoint < 1)
	{
		if (m_iPointPerFrame >= 96)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 1;
			m_iBigScaleDrawInterval = 10;
		}
		else if (m_iPointPerFrame >= 64)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 2;
			m_iBigScaleDrawInterval = 10;
		}
		else if (m_iPointPerFrame >= 56)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 3;
			m_iBigScaleDrawInterval = 30;
		}
		else if (m_iPointPerFrame >= 48)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 4;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 40)
		{
			m_iSmallScaleDrawInterval = 1;
			m_iMiddleScaleDrawInterval = 6;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 32)
		{
			m_iSmallScaleDrawInterval = 2;
			m_iMiddleScaleDrawInterval = 10;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 16)
		{
			m_iSmallScaleDrawInterval = 2;
			m_iMiddleScaleDrawInterval = 12;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 8)
		{
			m_iSmallScaleDrawInterval = 4;
			m_iMiddleScaleDrawInterval = 20;
			m_iBigScaleDrawInterval = 60;
		}
		else if (m_iPointPerFrame >= 4)
		{
			m_iSmallScaleDrawInterval = 5;
			m_iMiddleScaleDrawInterval = 30;
			m_iBigScaleDrawInterval = 60;
		}
		else
		{
			m_iSmallScaleDrawInterval = 10;
			m_iMiddleScaleDrawInterval = 60;
			m_iBigScaleDrawInterval = 120;
		}
	}
	else
	{
		// TODO: �����ƌ����悭�I
		if (m_iFramePerPoint <= 2)
		{
			m_iSmallScaleDrawInterval = 30;
			m_iMiddleScaleDrawInterval = 120;
			m_iBigScaleDrawInterval = 600;
		}
		else if (m_iFramePerPoint <= 4)
		{
			m_iSmallScaleDrawInterval = 60;
			m_iMiddleScaleDrawInterval = 300;
			m_iBigScaleDrawInterval = 1200;
		}
		else if (m_iFramePerPoint <= 6)
		{
			m_iSmallScaleDrawInterval = 120;
			m_iMiddleScaleDrawInterval = 600;
			m_iBigScaleDrawInterval = 1800;
		}
		else if (m_iFramePerPoint <= 8)
		{
			m_iSmallScaleDrawInterval = 180;
			m_iMiddleScaleDrawInterval = 900;
			m_iBigScaleDrawInterval = 1800;
		}
		else if (m_iFramePerPoint <= 10)
		{
			m_iSmallScaleDrawInterval = 300;
			m_iMiddleScaleDrawInterval = 1200;
			m_iBigScaleDrawInterval = 3600;
		}
		else if (m_iFramePerPoint <= 20)
		{
			m_iSmallScaleDrawInterval = 600;
			m_iMiddleScaleDrawInterval = 1800;
			m_iBigScaleDrawInterval = 3600;
		}
		else if (m_iFramePerPoint <= 40)
		{
			m_iSmallScaleDrawInterval = 600;
			m_iMiddleScaleDrawInterval = 1800;
			m_iBigScaleDrawInterval = 7200;
		}
		else if (m_iFramePerPoint <= 60)
		{
			m_iSmallScaleDrawInterval = 1200;
			m_iMiddleScaleDrawInterval = 3600;
			m_iBigScaleDrawInterval = 7200;
		}
		else if (m_iFramePerPoint <= 90)
		{
			m_iSmallScaleDrawInterval = 1200;
			m_iMiddleScaleDrawInterval = 3600;
			m_iBigScaleDrawInterval = 10800;
		}
		else if (m_iFramePerPoint <= 120)
		{
			m_iSmallScaleDrawInterval = 1200;
			m_iMiddleScaleDrawInterval = 3600;
			m_iBigScaleDrawInterval = 14400;
		}
		else if (m_iFramePerPoint <= 180)
		{
			m_iSmallScaleDrawInterval = 3600;
			m_iMiddleScaleDrawInterval = 18000;
			m_iBigScaleDrawInterval = 108000;
		}
		else if (m_iFramePerPoint <= 300)
		{
			m_iSmallScaleDrawInterval = 108000;
			m_iMiddleScaleDrawInterval = 432000;
			m_iBigScaleDrawInterval = 1080000;
		}
		else
		{
			m_iSmallScaleDrawInterval = 1080000;
			m_iMiddleScaleDrawInterval = 4320000;
			m_iBigScaleDrawInterval = 10800000;
		}
	}

	CalcTimelineDisplayRange();

	return TRUE;
}



/*

���W�A�ʒu�v�Z�n

*/
// �N���b�v�ʒu�v�Z
BOOL TimelineEditerView::CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{

	if (CalcClipRect(rcClipRect, const_cast<ClipDataRect*>(clClipData)->m_iTimelineInPoint, const_cast<ClipDataRect*>(clClipData)->GetDuration(),
		rcTrackRect, iMoveFrames, iIntrimFrames, iOuttrimFrames))
	{
		if (rcClipRect.left < m_prcTimelineDataRect->left)
		{
			rcClipRect.left = m_prcTimelineDataRect->left;
		}
		if (rcClipRect.right > m_prcTimelineDataRect->right)
		{
			rcClipRect.right = m_prcTimelineDataRect->right;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

// �N���b�v�ʒu�v�Z�i�͂ݏo���␳�Ȃ��j
BOOL TimelineEditerView::CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{
	rcClipRect.top = rcTrackRect.top + static_cast<int>(rcTrackRect.Height() * (1 - CLIPHIGHT));
	rcClipRect.bottom = rcClipRect.top + static_cast<int>(rcTrackRect.Height() * CLIPHIGHT);
	int iLeftScrubingFrameCount = m_iLeftFrameNumber + m_iOperatingFrameCount;
	int iRightScrubingFrameCount = m_iRightFrameNumber + m_iOperatingFrameCount;

	if (iInPoint + iMoveFrames + iIntrimFrames > iRightScrubingFrameCount)
	{
		rcClipRect.SetRectEmpty();
		return FALSE;
	}
	if ((iInPoint + iDuration + iMoveFrames + iOuttrimFrames) < iLeftScrubingFrameCount)
	{
		rcClipRect.SetRectEmpty();
		return FALSE;
	}
	int iDisplayInPoint = iInPoint + iMoveFrames + iIntrimFrames;
	int iDisplayOutPoint = iInPoint + iDuration + iMoveFrames + iOuttrimFrames;

	rcClipRect.left = ChangeTimelineFramePositionToDisplayPoint(iDisplayInPoint);
	rcClipRect.right = ChangeTimelineFramePositionToDisplayPoint(iDisplayOutPoint);

	return TRUE;
}

// �t���[���ʒu����ʏ�̍��W�ɕϊ�����
int TimelineEditerView::ChangeTimelineFramePositionToDisplayPoint(const int iFrame)
{
	int iXPoint;
	int iFrameCountFromTimelineCursor = iFrame - m_iTimelineCursorFramePosition - m_iOperatingFrameCount;

	// �P�|�C���g������̃t���[�������P�����̏ꍇ�i�P�t���[���������|�C���g�Ɍׂ�j
	if (m_iFramePerPoint < 1)
	{
		// �^�C�����C���J�[�\������̑��΍��W�����߂�
		iXPoint = iFrameCountFromTimelineCursor * m_iPointPerFrame;
	}
	else
	{
		// �^�C�����C���J�[�\������̑��΍��W�����߂�
		iXPoint = static_cast<int>(floor(iFrameCountFromTimelineCursor / m_iFramePerPoint));
	}
	return (iXPoint + m_iTimelineCursorPoint);
}

// �N���b�N�ʒu���t���[���ʒu�ɕϊ�����
int TimelineEditerView::ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame)
{
	int iFrame;
	int iHorizontalLengthFromTimelineCursor = point.x - m_iTimelineCursorPoint;

	// �P�t���[��������̃|�C���g�����P�����̏ꍇ
	if (m_iPointPerFrame < 1)
	{
		// �^�C�����C���J�[�\������̃t���[���������߂�
		iFrame = iHorizontalLengthFromTimelineCursor * m_iFramePerPoint + m_iTimelineCursorFramePosition;
	}
	else
	{
		// �^�C�����C���J�[�\������̑��΍��W�����߂�
		iFrame = static_cast<int>(floor(iHorizontalLengthFromTimelineCursor / m_iPointPerFrame)) + m_iTimelineCursorFramePosition;
	}
	iActualFrame = iFrame;
	if (iFrame < 0)
	{
		return 0;
	}
	else
	{
		return iFrame;
	}
}

// ����ʂ𑀍�t���[�����ɕϊ�����
int TimelineEditerView::ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame /* = 0 */)
{

	int iFrames = 0;
	if (szMoveSize.cx == 0)
	{
		return iFrames;
	}

	// �P�|�C���g������̃t���[�������P�����̏ꍇ�i�P�t���[���������|�C���g�Ɍׂ�j
	if (m_iFramePerPoint < 1)
	{
		// �ړ��t���[�����͎��ۂ̈ړ����~�P�|�C���g������̃t���[�����i�K�v�ȕ��𓮂����Ȃ��ƃt���[���͓����Ȃ��j
		iFrames = static_cast<int>(floor(szMoveSize.cx / m_iPointPerFrame));
	}
	else
	{
		// �\���ɐ؂�̗ǂ��t���[���ʒu�łȂ��ꍇ�͒�������
		int iSurPlus = iStartFrame % m_iFramePerPoint;
		if (iSurPlus == 0)
		{
			// �ړ��t���[�����͎��ۂ̈ړ����~�P�|�C���g������̃t���[�����i�P�|�C���g�ŕ����t���[�������j
			iFrames = szMoveSize.cx * m_iFramePerPoint;
		}
		else
		{
			// �i�ړ��t���[�����͎��ۂ̈ړ����|�P�j�~�P�|�C���g������̃t���[�����i�ŏ��̂P�|�C���g�͒[�������Ɏg���j
			if (szMoveSize.cx < 0)
			{
				iFrames = ((szMoveSize.cx + 1) * m_iFramePerPoint) - iSurPlus;
			}
			else
			{
				iFrames = ((szMoveSize.cx - 1) * m_iFramePerPoint) + m_iFramePerPoint - iSurPlus;
			}
		}
	}
	return iFrames;
}



/*

�q�b�g�e�X�g�n

*/
// �N���b�N�|�C���g���g���b�N�̈�ɂ��邩�̔���
TrackDataRect* TimelineEditerView::IsPointInAnyTrack(const CPoint& point)
{
	TrackDataRectList* pTrackDataRectList = m_pTrackDataVideoManager->GetTrackDataRectList();
	TrackDataRect* pTrackDataRect = nullptr;
	int i = 0;
	while ((i < m_pTrackDataVideoManager->GetTrackCount()) && (pTrackDataRect == nullptr))
	{
		pTrackDataRect = m_pTrackDataVideoManager->GetTrackDataRect(pTrackDataRectList->at(i));
		if (!(pTrackDataRect->PtInRect(point)))
		{
			pTrackDataRect = nullptr;
			++i;
		}
	}
	return pTrackDataRect;
}

// �N���b�N�ʒu���V�[�N�o�[�����𔻒肷��
BOOL TimelineEditerView::IsPointInSeekBar(const CPoint& point)
{
	if (!(m_prcSeekBarRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// �N���b�N�ʒu���^�C�����C���R���g���[���p�l�������𔻒肷��
BOOL TimelineEditerView::IsPointInTimelineControlPanel(const CPoint& point)
{
	if (!(m_prcTimelineControlPanelRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// �N���b�N�ʒu���N���b�v�����𔻒肷��
BOOL TimelineEditerView::IsPointInAnyClipRect(const CPoint& point)
{
	m_fMoving = FALSE;

	// TODO: �}�C�i�X��Ԃ��p�^�[�����K�v
	int iFrame;
	ChangeDisplayPointToTimelineFramePosition(point, iFrame);
	if ((m_clSelectedTrack != nullptr) && (m_clSelectedTrackInfo != nullptr) && (iFrame >= 0))
	{
		int iInPoint = 0;
		ClipDataPositionMap mpClipMap;
		int iSize = m_clSelectedTrackInfo->GetClipDataAtFrame(iFrame, mpClipMap);
		if (iSize == 0)
		{
			m_clMovingClipData = nullptr;
		}
		else if (iSize == 1)
		{
			ClipDataPositionMap::iterator itr = mpClipMap.begin();
			m_clMovingClipData = (*itr).second;
		}
		else
		{
			ClipDataPositionMap::iterator itr = mpClipMap.begin();
			++itr;
			m_clMovingClipData = (*itr).second;
		}

		if (m_clMovingClipData != nullptr)
		{
			if (IsPointInClipRect(point, static_cast<CRect&>(*m_clMovingClipData)))
			{
				CalcClipRectDisplayPoint(static_cast<CRect>(m_clMovingClipData), m_clMovingClipData, static_cast<CRect>(m_clSelectedTrack));
			}
		}
	}

	return (m_fMoving || m_fSingleInTriming || m_fSingleOutTriming);
}

// Move/Trim�U�蕪��
BOOL TimelineEditerView::IsPointInClipRect(const CPoint& point, const CRect& rcClipRect)
{
	CRect rcHitTestRect;
	rcHitTestRect.CopyRect(rcClipRect);
	// �N���b�v�����K��l���Z���ꍇ�͋K��l���Ŕ��肷��
	if (rcClipRect.Width() < kClipHitCheckMinWidth)
	{
		int iAdditionalLength = static_cast<int>(floor((kClipHitCheckMinWidth - rcHitTestRect.Width()) / 2));
		rcHitTestRect.left -= iAdditionalLength;
		rcHitTestRect.right += iAdditionalLength;
	}
	// ���͂�ClipRect���Ƀ|�C���^�����݂��邩�H
	if (rcHitTestRect.PtInRect(point))
	{
		// Trim����ŘR�ꂽ��Move
		m_fMoving = !(IsPointInTrimRange(point, &rcClipRect));
	}
	return (m_fMoving || m_fSingleInTriming || m_fSingleOutTriming);
}

// �N���b�N�ӏ����N���b�v���̃g��������G���A�����𔻒�
BOOL TimelineEditerView::IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect)
{

	m_fSingleInTriming = FALSE;
	m_fSingleOutTriming = FALSE;

	CRect rcTrimRect;

	// In������
	rcTrimRect.CopyRect(rcClipRect);
	// �N���b�v�����K��l�����̏ꍇ��InTrim���߂���
	if (rcTrimRect.Width() < kClipHitCheckMinWidth)
	{
		//TODO: In���f�����Ȃ��ꍇ�iClipIn�_=0�jOutTrim�ɂӂ�
		m_fSingleInTriming = TRUE;
	}
	else
	{
		rcTrimRect.right = rcTrimRect.left + static_cast<long>(floor(rcTrimRect.Width() * kTrimAreaRate));
		if (rcTrimRect.Width() < kTrimHitCheckMinWidth)
		{
			rcTrimRect.right = rcTrimRect.left + kTrimHitCheckMinWidth;
		}
		else if (rcTrimRect.Width() > kTrimHitCheckMaxWidth)
		{
			rcTrimRect.right = rcTrimRect.left + kTrimHitCheckMaxWidth;
		}
		m_fSingleInTriming = rcTrimRect.PtInRect(point);
	}

	if (!(m_fSingleInTriming || m_fSingleOutTriming))
	{
		// Out������
		rcTrimRect.CopyRect(rcClipRect);
		rcTrimRect.left = rcTrimRect.right - static_cast<long>(floor(rcTrimRect.Width() * kTrimAreaRate));
		if (rcTrimRect.Width() < kTrimHitCheckMinWidth)
		{
			rcTrimRect.left = rcTrimRect.right - kTrimHitCheckMinWidth;
		}
		else if (rcTrimRect.Width() > kTrimHitCheckMaxWidth)
		{
			rcTrimRect.left = rcTrimRect.right - kTrimHitCheckMaxWidth;
		}
		m_fSingleOutTriming = rcTrimRect.PtInRect(point);
	}

	return (m_fSingleInTriming || m_fSingleOutTriming);
}



/*

����`�F�b�N�n�i�R���g���[���[�ֈړ��j

*/
// ���삪In���g�����\�Ȕ͈͓����𔻒肵�Ĉʒu�𒲐�����
BOOL TimelineEditerView::CheckInTrim(void)
{
	int iDuration = m_clMovingClipData->GetDuration();
	// �͈̓`�F�b�N
	if (iDuration - m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = iDuration - 1;
		return FALSE;
	}

	// �d�Ȃ�`�F�b�N
	// TODO: In�_�̏ꏊ�ɃN���b�v�����邩���T�[�`����B
	m_iOperatingClipFrameCount = m_clOperateToTrackInfo->CheckClipInSingleInTrimRange(m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount)
		- m_clMovingClipData->m_iTimelineInPoint;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	// �͈̓`�F�b�N�i�d�Ȃ�`�F�b�N����ɂ��Ȃ���0�𒴂��ăh���b�O���ꂽ�ꍇ��0�ɐݒ肳��Ă��܂��j
	if (m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount < 0)
	{
		m_iOperatingClipFrameCount = m_clMovingClipData->m_iTimelineInPoint * -1;
		return FALSE;
	}

	return TRUE;
}

// ���삪Out���g�����\�Ȕ͈͓����𔻒肵�Ĉʒu�𒲐�����
BOOL TimelineEditerView::CheckOutTrim(void)
{
	// �͈̓`�F�b�N
	int iDuration = m_clMovingClipData->GetDuration();
	// �͈̓`�F�b�N
	if (iDuration + m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = (iDuration * -1) + 1;
		return FALSE;
	}

	// �d�Ȃ�`�F�b�N
	int iStartFrame = m_clMovingClipData->m_iTimelineInPoint + iDuration - 1;
	m_iOperatingClipFrameCount = m_clOperateToTrackInfo->CheckClipInSingleOutTrimRange(iStartFrame, iStartFrame + m_iOperatingClipFrameCount) - iStartFrame;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	return TRUE;
}

// ���삪Move�\�Ȕ͈͓����𔻒肵�Ĉʒu�𒲐�����
BOOL TimelineEditerView::CheckMove(CPoint& point)
{
	int iMovingClipInFrame, iMovingClipOutFrame, iWorkOperatingClipFrameCount, iDropInPoint = 0;
	CRect rcWorkRect;
	ClipDataRect* pClipData;
	// �͈̓`�F�b�N
	if (m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount < 0)
	{
		iWorkOperatingClipFrameCount = m_clMovingClipData->m_iTimelineInPoint * -1;
		// ���[�N�ɑ��쒆�̋�`�̈���R�s�[
		rcWorkRect.CopyRect(m_clMovingClipData->GetOperatingRect());
		// ���u�������ʒu�ɍ��킹�ă��[�N�̋�`���ړ�����
		CalcClipRect(rcWorkRect, iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
		// ���u�������ꏊ�ŃN���b�v�ƏՓ˂��Ȃ������Ĕ���
		iMovingClipInFrame = m_clMovingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount;
		iMovingClipOutFrame = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + iWorkOperatingClipFrameCount;
		pClipData = m_clOperateToTrackInfo->CheckMove(m_clMovingClipData, iMovingClipInFrame, iMovingClipOutFrame);
		if ((pClipData != nullptr) && (pClipData != m_clMovingClipData))
		{
			// �d�Ȃ肪�������ꍇ
			m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
			m_clOperateToTrack = m_pEnableMovingTrack;
			CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			return FALSE;
		}
		else
		{
			// �d�Ȃ肪�Ȃ��ꍇ�A���̏ꏊ�ɔz�u����
			m_iOperatingClipFrameCount = m_clMovingClipData->m_iTimelineInPoint * -1;
			m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
			m_pEnableMovingTrack = m_clOperateToTrack;
			CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			return TRUE;
		}
	}

	// �d�Ȃ�`�F�b�N
	// TODO: �d�Ȃ�����̍Ĕ��肪�K�v�i�E�ړ��ƍ��ړ��ŕ����čl���Ȃ��Ƃ��߂����i�`�F�b�N�m�f��������n�_�ɖ߂��Ă����j
	iMovingClipInFrame = m_clMovingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount;
	iMovingClipOutFrame = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + m_iOperatingClipFrameCount;
	pClipData = m_clOperateToTrackInfo->CheckMove(m_clMovingClipData, iMovingClipInFrame, iMovingClipOutFrame);
	if ((pClipData != nullptr) && (pClipData != m_clMovingClipData))
	{
		// �d�Ȃ肪�������ꍇ
		int iStaticClipCenterFrame = pClipData->m_iTimelineInPoint + static_cast<int>(floor(pClipData->GetDuration() / 2));
		int iDropInPoint = 0;
		// �d�Ȃ��Ă���N���b�v�̍��E�ɉ��z�u
		if (iMovingClipInFrame <= iStaticClipCenterFrame)
		{
			// ���S��荶�̏ꍇ�A�d�Ȃ��Ă���N���b�v��In�_-���g��Duration��z�u�_�ɉ��u��
			iDropInPoint = pClipData->m_iTimelineInPoint - m_clMovingClipData->GetDuration();
		}
		else
		{
			// ���S���E�̏ꍇ�A�d�Ȃ��Ă���N���b�v��Out�_��z�u�_�ɉ��u��
			iDropInPoint = pClipData->m_iTimelineInPoint + pClipData->GetDuration();
		}
		// ���u�������|�C���g�ւ̈ړ��ʂ��Z�o
		iWorkOperatingClipFrameCount = iDropInPoint - m_clMovingClipData->m_iTimelineInPoint;
		// �ړ��̌���In�_��0�����ɂȂ�ꍇ�͈ړ����Ȃ�
		if (m_clMovingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount > 0)
		{
			// ���[�N�ɑ��쒆�̋�`�̈���R�s�[
			rcWorkRect.CopyRect(m_clMovingClipData->GetOperatingRect());
			// ���u�������ʒu�ɍ��킹�ă��[�N�̋�`���ړ�����
			CalcClipRect(rcWorkRect, iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			// ���u�������ꏊ�ŃN���b�v�ƏՓ˂��Ȃ������Ĕ���
			iMovingClipInFrame = m_clMovingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount;
			iMovingClipOutFrame = m_clMovingClipData->m_iTimelineInPoint + m_clMovingClipData->GetDuration() - 1 + iWorkOperatingClipFrameCount;
			pClipData = m_clOperateToTrackInfo->CheckMove(m_clMovingClipData, iMovingClipInFrame, iMovingClipOutFrame);
			if ((pClipData != nullptr) && (pClipData != m_clMovingClipData))
			{
				// �d�Ȃ肪�������ꍇ
			}
			else
			{
				// �d�Ȃ肪�Ȃ��ꍇ�A���̏ꏊ�ɔz�u����
				m_iOperatingClipFrameCount = iDropInPoint - m_clMovingClipData->m_iTimelineInPoint;
				m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
				m_pEnableMovingTrack = m_clOperateToTrack;
				CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), iDropInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
			}
		}
		// �ړ��悪�z�u�ł��Ȃ��ꍇ�A�O��ړ��\�������ꏊ�ɖ߂��B
		m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
		m_clOperateToTrack = m_pEnableMovingTrack;
		CalcClipRect(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData->m_iTimelineInPoint, m_clMovingClipData->GetDuration(), m_clOperateToTrack);
		return FALSE;
	}


	m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
	m_pEnableMovingTrack = m_clOperateToTrack;
	CalcClipRectDisplayPoint(*(m_clMovingClipData->GetOperatingRect()), m_clMovingClipData, m_clOperateToTrack, m_iOperatingClipFrameCount);
	return TRUE;
}



/*

�����ݒ�n

*/
// �\���G���A�̏����ݒ�
void TimelineEditerView::InitAreaRect(void)
{
	// TODO: �F�͌�Œ萔�����
	m_prcDebugInfoPanelRect = new OpenGLRect();
	m_prcDebugInfoPanelRect->SetRectEmpty();
	m_prcDebugInfoPanelRect->SetBothColor(ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, WHITECOLOR_BRUSH_FLOAT);

	m_prcTimelineEditPanelRect = new OpenGLRect();
	m_prcTimelineEditPanelRect->SetRectEmpty();
	m_prcTimelineEditPanelRect->SetBothColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_prcTimelineEditHeaderRect = new OpenGLRect();
	m_prcTimelineEditHeaderRect->SetRectEmpty();

	m_prcTimelineControlPanelRect = new OpenGLRect();
	m_prcTimelineControlPanelRect->SetRectEmpty();
	m_prcTimelineControlPanelRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, REDCOLOR_BRUSH_FLOAT, GREENCOLOR_BRUSH_FLOAT);

	m_prcSeekBarRect = new OpenGLRect();
	m_prcSeekBarRect->SetRectEmpty();
	m_prcSeekBarRect->SetBothColor(ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT, ACCENTCOLOR2_BRUSH_FLOAT);

	m_prcTrackHeaderRect = new OpenGLRect();
	m_prcTrackHeaderRect->SetRectEmpty();
	m_prcTrackHeaderRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_prcTimelineDataRect = new OpenGLRect();
	m_prcTimelineDataRect->SetRectEmpty();
	m_prcTimelineDataRect->SetBothColor(GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, GRAYCOLOR_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);

	m_prcTimelineCursorHitArea = new OpenGLRect();
	m_prcTimelineCursorHitArea->SetRectEmpty();

	m_prcTransisionRect = new OpenGLRect();
	m_prcTransisionRect->SetRectEmpty();
	m_prcTransisionRect->SetColor(ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT, ACCENTCOLOR4_BRUSH_FLOAT, ACCENTCOLOR3_BRUSH_FLOAT);
	m_prcTransisionRect->SetBorderColor(LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT, LIGHTGRAYCOLOR3_BRUSH_FLOAT);
}

// ����m�F�p�I�u�W�F�N�g�̏����ݒ�
void TimelineEditerView::InitTestObject(void)
{
	// TODO: �K�؂ȍ�����񋟂���
	UUID uiTrackId, uiTrackRectId;

	m_pTrackDataVideoManager->CreateTrackData(0, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(1, uiTrackId, uiTrackRectId);
	m_pTrackDataVideoManager->CreateTrackData(2, uiTrackId, uiTrackRectId);

	UUID uiClipId, uiClipRectId;

	ClipDataRect* pClipDataRect = nullptr;
	m_pClipDataManager->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pClipDataManager->GetClipDataRect(uiClipRectId);
	pClipDataRect->m_iTimelineInPoint = 101;
	pClipDataRect->SetDuration(10);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->m_iTimelineInPoint, pClipDataRect);

	m_pClipDataManager->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pClipDataManager->GetClipDataRect(uiClipRectId);
	pClipDataRect->m_iTimelineInPoint = 600;
	pClipDataRect->SetDuration(100);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->m_iTimelineInPoint, pClipDataRect);

	m_pClipDataManager->CreateClipData(uiClipId, uiClipRectId);
	pClipDataRect = m_pClipDataManager->GetClipDataRect(uiClipRectId);
	pClipDataRect->m_iTimelineInPoint = 300;
	pClipDataRect->SetDuration(50);
	m_pTrackDataVideoManager->GetTrackDataInfo(uiTrackId)->AddClip(pClipDataRect->m_iTimelineInPoint, pClipDataRect);



}

// �\���{���֘A�̃}�b�v�쐬
void TimelineEditerView::CreateZoomMap(void)
{
}



