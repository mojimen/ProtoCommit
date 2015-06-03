// TimelineDataOperator.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimelineDataOperator.h"

#include "OpenGLRect.h"
#include "ClipDataRect.h"
#include "TrackDataRect.h"
#include "TrackDataInfo.h"
#include "TrackDataManager.h"
#include "DragAndDropOperator.h"



// TimelineDataOperator

TimelineDataOperator::TimelineDataOperator()
{
}

TimelineDataOperator::~TimelineDataOperator()
{
}

BOOL TimelineDataOperator::InitializeTimelineDataOperator(UUID& uiTimelineDataOperatorId)
{
	if (RPC_S_OK == UuidCreate(&m_uiTimelineDataOperatorId))
	{
		uiTimelineDataOperatorId = m_uiTimelineDataOperatorId;
	}
	else
	{
		return FALSE;
	}

	m_pTimelineEditPanelRect = new OpenGLRect();
	m_pTimelineEditHeaderRect = new OpenGLRect();
	m_pTimelineControlPanelRect = new OpenGLRect();
	m_pSeekBarRect = new OpenGLRect();
	m_pTrackHeaderRect = new OpenGLRect();
	m_pTimelineDataRect = new OpenGLRect();
	m_pTimelineCursorHitArea = new OpenGLRect();
	m_pTransisionRect = new OpenGLRect();

	m_pTimelineDataManager = new TimelineDataManager();
	if (!(m_pTimelineDataManager->InitializeTimelineDataManager(m_uiTimelineDataManagerId)))
	{
		return FALSE;
	}
	m_pTrackDataVideoManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_VIDEO, m_uiTrackDataVideoManagerId);
	ASSERT(m_pTrackDataVideoManager);
	if (m_pTrackDataVideoManager == nullptr)
	{
		return FALSE;
	}
	m_pTrackDataAudioManager = m_pTimelineDataManager->GetTrackDataManager(TRACKDATAMANAGER_AUDIO, m_uiTrackDataAudioManagerId);
	ASSERT(m_pTrackDataAudioManager);
	if (m_pTrackDataAudioManager == nullptr)
	{
		return FALSE;
	}
	m_pClipDataManager = m_pTimelineDataManager->GetClipDataManager(m_uiClipDataManagerId);
	ASSERT(m_pClipDataManager);
	if (m_pClipDataManager == nullptr)
	{
		return FALSE;
	}

	m_pDropAndDragOperator = new DragAndDropOperator(this);
	m_pDropAndDragOperator->Initialize(m_uiDropAndDragOperatorId);

	m_fLButtonDown = FALSE;
	m_fMove = FALSE;
	m_fSingleInTrim = FALSE;
	m_fSingleOutTrim = FALSE;
	m_fScrub = FALSE;
	m_fDragShuttle = FALSE;

	m_iLeftFrameNumber = 0;
	m_iRightFrameNumber = 0;
	m_iTimelineCursorFramePosition = 0;
	m_iOperatingTimelineCursorFramePosition = m_iTimelineCursorFramePosition;
	m_iOperatingFrameCount = 0;
	m_iOperatingClipFrameCount = 0;

	m_iFramePerPoint = 0;
	m_iPointPerFrame = 1;
	ChangeDisplayScale();

	return TRUE;
}

void TimelineDataOperator::DeleteTimelineDataOperator(void)
{

	if (m_pTimelineDataManager)
	{
		m_pTimelineDataManager->DeleteTimelineDataManager();
		delete m_pTimelineDataManager;
	}
	if (m_pTimelineEditPanelRect)
	{
		delete m_pTimelineEditPanelRect;
	}
	if (m_pTimelineEditHeaderRect)
	{
		delete m_pTimelineEditHeaderRect;
	}
	if (m_pTimelineControlPanelRect)
	{
		delete m_pTimelineControlPanelRect;
	}
	if (m_pSeekBarRect)
	{
		delete m_pSeekBarRect;
	}
	if (m_pTrackHeaderRect)
	{
		delete m_pTrackHeaderRect;
	}
	if (m_pTimelineDataRect)
	{
		delete m_pTimelineDataRect;
	}
	if (m_pTimelineCursorHitArea)
	{
		delete m_pTimelineCursorHitArea;
	}
	if (m_pTransisionRect)
	{
		delete m_pTransisionRect;
	}
}


// TimelineDataOperator �����o�[�֐�

// �k�{�^���_�E��
BOOL TimelineDataOperator::OnLButtonDown(UINT nFlags, CPoint point)
{
	// �^�C�����C���f�[�^�G���A������
	if (m_pTimelineDataRect->PtInRect(point))
	{
		// �g���b�N����
		m_pSelectedTrack = IsPointInAnyTrack(point);
		if (m_pSelectedTrack != nullptr)
		{
			m_pSelectedTrackInfo = m_pSelectedTrack->GetTrackDataInfo();
			m_pOperateToTrack = m_pSelectedTrack;
			m_pOperateToTrackInfo = m_pSelectedTrackInfo;

			if (IsPointInAnyClipRect(point))
			{
				m_fLButtonDown = TRUE;
				//SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
				m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
				m_pOperatingClipData->SetOperatingRect(static_cast<CRect>(m_pOperatingClipData));
				m_rcMousePointRect.CopyRect(static_cast<CRect>(m_pOperatingClipData));
				m_iOperatingClipFrameCount = 0;
				m_iEnableMovingFrameCount = 0;
				m_pEnableMovingTrack = m_pOperateToTrack;
				return TRUE;
			}
		}
		if (!m_fLButtonDown)
		{
			//// �^�C�����C���J�[�\������
			//else if (m_pTimelineCursorHitArea.PtInRect(point))
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
		if (m_pTimelineCursorHitArea->PtInRect(point))
		{
			m_fLButtonDown = TRUE;
			m_fDragShuttle = TRUE;
			//SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
			m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
			m_iOperatingFrameCount = 0;
			m_fSuttleSpeed = 0;
			return TRUE;
		}
		// �V�[�N�o�[������
		else if (IsPointInSeekBar(point))
		{
			m_fLButtonDown = TRUE;
			m_fScrub = TRUE;
			//SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
			m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
			m_iOperatingFrameCount = 0;
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
				return TRUE;
			}
		}
	}
	return FALSE;
}

// �k�{�^���A�b�v
BOOL TimelineDataOperator::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_fLButtonDown = FALSE;
	if (m_fMove)
	{
		m_pSelectedTrackInfo->DeleteClip(m_pOperatingClipData->m_iTimelineInPoint);
		m_pOperatingClipData->m_iTimelineInPoint += m_iOperatingClipFrameCount;
		(m_pEnableMovingTrack->GetTrackDataInfo())->AddClip(m_pOperatingClipData->m_iTimelineInPoint, m_pOperatingClipData);
		m_pOperatingClipData->CopyRect(m_pOperatingClipData->GetOperatingRect());
		//TODO: ������͕��������Ȃ��Ƃ����Ȃ��|���ړ����͂������ǐÎ~��Ԃ̎��Ƀg���b�N����In�_�������Ă���̂�����I
		//m_clMovingClipData->m_iTimelineInPoint = 0;
	}
	else if (m_fSingleInTrim)
	{
		m_pSelectedTrackInfo->ChangeClip(m_pOperatingClipData->m_iTimelineInPoint, m_pOperatingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount, m_pOperatingClipData);
		m_pOperatingClipData->m_iTimelineInPoint += m_iOperatingClipFrameCount;
		int iClipDuration = m_pOperatingClipData->GetDuration();
		// In���ɐL�т�i�}�C�i�X�����ւ̈ړ��j�͒��������Z
		m_pOperatingClipData->SetDuration(iClipDuration - m_iOperatingClipFrameCount);
		m_pOperatingClipData->CopyRect(m_pOperatingClipData->GetOperatingRect());
	}
	else if (m_fSingleOutTrim)
	{
		int iClipDuration = m_pOperatingClipData->GetDuration();
		m_pOperatingClipData->SetDuration(iClipDuration + m_iOperatingClipFrameCount);
		m_pOperatingClipData->CopyRect(m_pOperatingClipData->GetOperatingRect());
	}
	else if (m_fScrub)
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

	m_pOperatingClipData = nullptr;
	m_fMove = FALSE;
	m_fSingleInTrim = FALSE;
	m_fSingleOutTrim = FALSE;
	m_fScrub = FALSE;
	m_fDragShuttle = FALSE;

	return TRUE;
}

// �q�{�^���A�b�v
BOOL TimelineDataOperator::OnRButtonUp(UINT nFlags, CPoint point)
{
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
			return TRUE;
		}
	}
	return FALSE;
}

// �}�E�X�ړ�
BOOL TimelineDataOperator::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_fLButtonDown)
	{
		CSize szMoveSize(point - m_poMousePointerLocation);
		if (m_fMove)
		{
			m_pOperateToTrack = IsPointInAnyTrack(point);
			if (m_pOperateToTrack == nullptr)
			{
				m_pOperateToTrack = m_pSelectedTrack;
			}
			m_pOperateToTrackInfo = m_pOperateToTrack->GetTrackDataInfo();
			m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_pOperatingClipData->m_iTimelineInPoint);
			CalcClipRectDisplayPoint(m_rcMousePointRect, m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), m_iOperatingClipFrameCount);
			CheckMove(point);
			CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), m_iOperatingClipFrameCount);
			return TRUE;
		}
		else if (m_fSingleInTrim || m_fSingleOutTrim)
		{
			m_pOperatingClipData->SetOperatingRect(static_cast<CRect>(m_pOperatingClipData)); // �L�k���̃C���[�W���W
			if (m_fSingleInTrim)
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_pOperatingClipData->m_iTimelineInPoint);
				CheckInTrim();
				CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), 0, m_iOperatingClipFrameCount);
			}
			else
			{
				m_iOperatingClipFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize,
					(m_pOperatingClipData->m_iTimelineInPoint + m_pOperatingClipData->GetDuration()));
				CheckOutTrim();
				CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, static_cast<CRect>(m_pOperateToTrack), 0, 0, m_iOperatingClipFrameCount);
			}
			return TRUE;
		}
		else if (m_fScrub)
		{
			szMoveSize.cx *= -1;
			m_iOperatingFrameCount = ChangeOperatingDistanceToTimelineFrames(szMoveSize, m_iTimelineCursorFramePosition);
			// �ŏ��͈̓`�F�b�N
			// TODO: �ő�͈̓`�F�b�N�́H
			if ((m_iTimelineCursorFramePosition + m_iOperatingFrameCount) < 0)
			{
				m_iOperatingFrameCount = -1 * m_iTimelineCursorFramePosition;
			}
			m_iOperatingLeftFrameNumber = m_iLeftFrameNumber + m_iOperatingFrameCount;
			m_iOperatingRightFrameNumber = m_iRightFrameNumber + m_iOperatingFrameCount;
			m_iOperatingTimelineCursorFramePosition = m_iTimelineCursorFramePosition + m_iOperatingFrameCount;
			return TRUE;
		}
		else if (m_fDragShuttle)
		{
			//// �}�E�X�ʒu����{�����x���擾
			//m_fSuttleSpeed = SetShuttleSpeed(point, szMoveSize);
			//Invalidate();
		}
	}
	return FALSE;
}

// �h���b�v�ɂ��t�@�C���̃N���b�v��
BOOL TimelineDataOperator::OnDropFiles(const HDROP& hDropInfo, CString& strFileName)
{
	//�h���b�v���ꂽ�t�@�C���̌����`�F�b�N
	UINT uiCount = DragQueryFile(hDropInfo, ~0lu, NULL, 0);
	if (uiCount != 1)
	{
		return FALSE;
	}

	// �h���b�v��̃g���b�N�����
	POINT poDorpPoint;
	DragQueryPoint(hDropInfo, &poDorpPoint);
	m_pSelectedTrack = m_pDropAndDragOperator->GetDropTrack(poDorpPoint);
	if (m_pSelectedTrack == nullptr)
	{
		return FALSE;
	}

	// �t�@�C�������擾
	UINT uiLen = DragQueryFile(hDropInfo, 0, NULL, 0);
	DragQueryFile(hDropInfo, 0, strFileName.GetBuffer(uiLen + 1), uiLen + 1);
	strFileName.ReleaseBuffer();

	// �t�@�C���g���q�`�F�b�N
	if (!(m_pDropAndDragOperator->CheckFileNameExtension(strFileName)))
	{
		return FALSE;
	}

	// �t�@�C���`���`�F�b�N
	CString strFilePath;
	UINT uiIn = 0, uiOut = 0;
	if (!(m_pDropAndDragOperator->CheckDropFile(strFileName, strFilePath, uiIn, uiOut)))
	{
		return FALSE;
	}
	
	// �N���b�v�f�[�^���쐬����
	int iFrame = 0, iDropFrame = 0;
	m_pSelectedTrackInfo = m_pSelectedTrack->GetTrackDataInfo();
	iDropFrame = ChangeDisplayPointToTimelineFramePosition(poDorpPoint, iFrame);
	if (m_pDropAndDragOperator->CreateClipDataFromDropFile(m_pSelectedTrackInfo, iDropFrame, static_cast<PCTSTR>(strFilePath), uiIn, uiOut))
	{
		return TRUE;
	}
	return FALSE;
}


// �^�C�����C���f�[�^�\���{���̕ύX
BOOL TimelineDataOperator::ChangeDisplayScale(void)
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

// �\���\�ȃ^�C�����C���͈̔́i���[�A�E�[�j���v�Z����
void TimelineDataOperator::CalcTimelineDisplayRange(void)
{
	// �\���\�t���[���͈͂̌v�Z
	int iDisplayFrameCount = 0;
	if (m_iPointPerFrame < 1)
	{
		iDisplayFrameCount = m_pSeekBarRect->Width() * m_iFramePerPoint;
	}
	else
	{
		iDisplayFrameCount = static_cast<int>(ceil(m_pSeekBarRect->Width() / m_iPointPerFrame));
	}
	m_iTimelineCursorPoint = static_cast<int>(floor(m_pSeekBarRect->Width() / 2.0f)) + m_pSeekBarRect->left;
	m_iLeftFrameNumber = m_iTimelineCursorFramePosition - static_cast<int>(floor((iDisplayFrameCount / 2.0f)));
	m_iOperatingLeftFrameNumber = m_iLeftFrameNumber;
	m_iRightFrameNumber = m_iTimelineCursorFramePosition + static_cast<int>(ceil((iDisplayFrameCount / 2.0f))) + 1;
	m_iOperatingRightFrameNumber = m_iRightFrameNumber;
}


/*

�q�b�g�e�X�g�n

*/
// �N���b�N�|�C���g���g���b�N�̈�ɂ��邩�̔���
TrackDataRect* TimelineDataOperator::IsPointInAnyTrack(const CPoint& point)
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
BOOL TimelineDataOperator::IsPointInSeekBar(const CPoint& point)
{
	if (!(m_pSeekBarRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// �N���b�N�ʒu���^�C�����C���R���g���[���p�l�������𔻒肷��
BOOL TimelineDataOperator::IsPointInTimelineControlPanel(const CPoint& point)
{
	if (!(m_pTimelineControlPanelRect->PtInRect(point)))
	{
		return FALSE;
	}
	return TRUE;
}

// �N���b�N�ʒu���N���b�v�����𔻒肷��
BOOL TimelineDataOperator::IsPointInAnyClipRect(const CPoint& point)
{
	m_fMove = FALSE;

	// TODO: �}�C�i�X��Ԃ��p�^�[�����K�v
	int iFrame;
	ChangeDisplayPointToTimelineFramePosition(point, iFrame);
	if ((m_pSelectedTrack != nullptr) && (m_pSelectedTrackInfo != nullptr) && (iFrame >= 0))
	{
		int iInPoint = 0;
		ClipDataPositionMap mpClipMap;
		int iSize = m_pSelectedTrackInfo->GetClipDataAtFrame(iFrame, mpClipMap);
		if (iSize == 0)
		{
			m_pOperatingClipData = nullptr;
		}
		else if (iSize == 1)
		{
			ClipDataPositionMap::iterator itr = mpClipMap.begin();
			m_pOperatingClipData = (*itr).second;
		}
		else
		{
			ClipDataPositionMap::iterator itr = mpClipMap.begin();
			++itr;
			m_pOperatingClipData = (*itr).second;
		}

		if (m_pOperatingClipData != nullptr)
		{
			if (IsPointInClipRect(point, static_cast<CRect&>(*m_pOperatingClipData)))
			{
				CalcClipRectDisplayPoint(static_cast<CRect>(m_pOperatingClipData), m_pOperatingClipData, static_cast<CRect>(m_pSelectedTrack));
			}
		}
	}

	return (m_fMove || m_fSingleInTrim || m_fSingleOutTrim);
}

// Move/Trim�U�蕪��
BOOL TimelineDataOperator::IsPointInClipRect(const CPoint& point, const CRect& rcClipRect)
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
		m_fMove = !(IsPointInTrimRange(point, &rcClipRect));
	}
	return (m_fMove || m_fSingleInTrim || m_fSingleOutTrim);
}

// �N���b�N�ӏ����N���b�v���̃g��������G���A�����𔻒�
BOOL TimelineDataOperator::IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect)
{

	m_fSingleInTrim = FALSE;
	m_fSingleOutTrim = FALSE;

	CRect rcTrimRect;

	// In������
	rcTrimRect.CopyRect(rcClipRect);
	// �N���b�v�����K��l�����̏ꍇ��InTrim���߂���
	if (rcTrimRect.Width() < kClipHitCheckMinWidth)
	{
		//TODO: In���f�����Ȃ��ꍇ�iClipIn�_=0�jOutTrim�ɂӂ�
		m_fSingleInTrim = TRUE;
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
		m_fSingleInTrim = rcTrimRect.PtInRect(point);
	}

	if (!(m_fSingleInTrim || m_fSingleOutTrim))
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
		m_fSingleOutTrim = rcTrimRect.PtInRect(point);
	}

	return (m_fSingleInTrim || m_fSingleOutTrim);
}



/*

���W�A�ʒu�v�Z�n

*/
// �N���b�v�ʒu�v�Z
BOOL TimelineDataOperator::CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{

	if (CalcClipRect(rcClipRect, const_cast<ClipDataRect*>(clClipData)->m_iTimelineInPoint, const_cast<ClipDataRect*>(clClipData)->GetDuration(),
		rcTrackRect, iMoveFrames, iIntrimFrames, iOuttrimFrames))
	{
		if (rcClipRect.left < m_pTimelineDataRect->left)
		{
			rcClipRect.left = m_pTimelineDataRect->left;
		}
		if (rcClipRect.right > m_pTimelineDataRect->right)
		{
			rcClipRect.right = m_pTimelineDataRect->right;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

// �N���b�v�ʒu�v�Z�i�͂ݏo���␳�Ȃ��j
BOOL TimelineDataOperator::CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect, const int& iMoveFrames /* = 0 */,
	const int& iIntrimFrames /* = 0 */, const int& iOuttrimFrames/* = 0 */)
{
	rcClipRect.top = rcTrackRect.top + static_cast<int>(rcTrackRect.Height() * (1 - CLIPHIGHT));
	rcClipRect.bottom = rcClipRect.top + static_cast<int>(rcTrackRect.Height() * CLIPHIGHT);

	if (iInPoint + iMoveFrames + iIntrimFrames > m_iOperatingRightFrameNumber)
	{
		rcClipRect.SetRectEmpty();
		return FALSE;
	}
	if ((iInPoint + iDuration + iMoveFrames + iOuttrimFrames) < m_iOperatingLeftFrameNumber)
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
int TimelineDataOperator::ChangeTimelineFramePositionToDisplayPoint(const int iFrame)
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
int TimelineDataOperator::ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame)
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
int TimelineDataOperator::ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame /* = 0 */)
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

����`�F�b�N�n�i�R���g���[���[�ֈړ��j

*/
// ���삪In���g�����\�Ȕ͈͓����𔻒肵�Ĉʒu�𒲐�����
BOOL TimelineDataOperator::CheckInTrim(void)
{
	int iDuration = m_pOperatingClipData->GetDuration();
	// �͈̓`�F�b�N
	if (iDuration - m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = iDuration - 1;
		return FALSE;
	}

	// �d�Ȃ�`�F�b�N
	// TODO: In�_�̏ꏊ�ɃN���b�v�����邩���T�[�`����B
	m_iOperatingClipFrameCount = m_pOperateToTrackInfo->CheckClipInSingleInTrimRange(m_pOperatingClipData->m_iTimelineInPoint, m_pOperatingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount)
		- m_pOperatingClipData->m_iTimelineInPoint;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	// �͈̓`�F�b�N�i�d�Ȃ�`�F�b�N����ɂ��Ȃ���0�𒴂��ăh���b�O���ꂽ�ꍇ��0�ɐݒ肳��Ă��܂��j
	if (m_pOperatingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount < 0)
	{
		m_iOperatingClipFrameCount = m_pOperatingClipData->m_iTimelineInPoint * -1;
		return FALSE;
	}

	return TRUE;
}

// ���삪Out���g�����\�Ȕ͈͓����𔻒肵�Ĉʒu�𒲐�����
BOOL TimelineDataOperator::CheckOutTrim(void)
{
	// �͈̓`�F�b�N
	int iDuration = m_pOperatingClipData->GetDuration();
	// �͈̓`�F�b�N
	if (iDuration + m_iOperatingClipFrameCount < 1)
	{
		m_iOperatingClipFrameCount = (iDuration * -1) + 1;
		return FALSE;
	}

	// �d�Ȃ�`�F�b�N
	int iStartFrame = m_pOperatingClipData->m_iTimelineInPoint + iDuration - 1;
	m_iOperatingClipFrameCount = m_pOperateToTrackInfo->CheckClipInSingleOutTrimRange(iStartFrame, iStartFrame + m_iOperatingClipFrameCount) - iStartFrame;
	if (m_iOperatingClipFrameCount == 0)
	{
		return FALSE;
	}

	return TRUE;
}

// ���삪Move�\�Ȕ͈͓����𔻒肵�Ĉʒu�𒲐�����
BOOL TimelineDataOperator::CheckMove(CPoint& point)
{
	int iMovingClipInFrame, iMovingClipOutFrame, iWorkOperatingClipFrameCount, iDropInPoint = 0;
	CRect rcWorkRect;
	ClipDataRect* pClipData;
	// �͈̓`�F�b�N
	if (m_pOperatingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount < 0)
	{
		iWorkOperatingClipFrameCount = m_pOperatingClipData->m_iTimelineInPoint * -1;
		// ���[�N�ɑ��쒆�̋�`�̈���R�s�[
		rcWorkRect.CopyRect(m_pOperatingClipData->GetOperatingRect());
		// ���u�������ʒu�ɍ��킹�ă��[�N�̋�`���ړ�����
		CalcClipRect(rcWorkRect, iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
		// ���u�������ꏊ�ŃN���b�v�ƏՓ˂��Ȃ������Ĕ���
		iMovingClipInFrame = m_pOperatingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount;
		iMovingClipOutFrame = m_pOperatingClipData->m_iTimelineInPoint + m_pOperatingClipData->GetDuration() - 1 + iWorkOperatingClipFrameCount;
		pClipData = m_pOperateToTrackInfo->CheckMove(m_pOperatingClipData, iMovingClipInFrame, iMovingClipOutFrame);
		if ((pClipData != nullptr) && (pClipData != m_pOperatingClipData))
		{
			// �d�Ȃ肪�������ꍇ
			m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
			m_pOperateToTrack = m_pEnableMovingTrack;
			CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData->m_iTimelineInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			return FALSE;
		}
		else
		{
			// �d�Ȃ肪�Ȃ��ꍇ�A���̏ꏊ�ɔz�u����
			m_iOperatingClipFrameCount = m_pOperatingClipData->m_iTimelineInPoint * -1;
			m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
			m_pEnableMovingTrack = m_pOperateToTrack;
			CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			return TRUE;
		}
	}

	// �d�Ȃ�`�F�b�N
	iMovingClipInFrame = m_pOperatingClipData->m_iTimelineInPoint + m_iOperatingClipFrameCount;
	iMovingClipOutFrame = m_pOperatingClipData->m_iTimelineInPoint + m_pOperatingClipData->GetDuration() - 1 + m_iOperatingClipFrameCount;
	pClipData = m_pOperateToTrackInfo->CheckMove(m_pOperatingClipData, iMovingClipInFrame, iMovingClipOutFrame);
	if ((pClipData != nullptr) && (pClipData != m_pOperatingClipData))
	{
		// �d�Ȃ肪�������ꍇ
		int iStaticClipCenterFrame = pClipData->m_iTimelineInPoint + static_cast<int>(floor(pClipData->GetDuration() / 2));
		int iDropInPoint = 0;
		// �d�Ȃ��Ă���N���b�v�̍��E�ɉ��z�u
		if (iMovingClipInFrame <= iStaticClipCenterFrame)
		{
			// ���S��荶�̏ꍇ�A�d�Ȃ��Ă���N���b�v��In�_-���g��Duration��z�u�_�ɉ��u��
			iDropInPoint = pClipData->m_iTimelineInPoint - m_pOperatingClipData->GetDuration();
		}
		else
		{
			// ���S���E�̏ꍇ�A�d�Ȃ��Ă���N���b�v��Out�_��z�u�_�ɉ��u��
			iDropInPoint = pClipData->m_iTimelineInPoint + pClipData->GetDuration();
		}
		// ���u�������|�C���g�ւ̈ړ��ʂ��Z�o
		iWorkOperatingClipFrameCount = iDropInPoint - m_pOperatingClipData->m_iTimelineInPoint;
		// �ړ��̌���In�_��0�����ɂȂ�ꍇ�͈ړ����Ȃ�
		if (m_pOperatingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount > 0)
		{
			// ���[�N�ɑ��쒆�̋�`�̈���R�s�[
			rcWorkRect.CopyRect(m_pOperatingClipData->GetOperatingRect());
			// ���u�������ʒu�ɍ��킹�ă��[�N�̋�`���ړ�����
			CalcClipRect(rcWorkRect, iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			// ���u�������ꏊ�ŃN���b�v�ƏՓ˂��Ȃ������Ĕ���
			iMovingClipInFrame = m_pOperatingClipData->m_iTimelineInPoint + iWorkOperatingClipFrameCount;
			iMovingClipOutFrame = m_pOperatingClipData->m_iTimelineInPoint + m_pOperatingClipData->GetDuration() - 1 + iWorkOperatingClipFrameCount;
			pClipData = m_pOperateToTrackInfo->CheckMove(m_pOperatingClipData, iMovingClipInFrame, iMovingClipOutFrame);
			if ((pClipData != nullptr) && (pClipData != m_pOperatingClipData))
			{
				// �d�Ȃ肪�������ꍇ
			}
			else
			{
				// �d�Ȃ肪�Ȃ��ꍇ�A���̏ꏊ�ɔz�u����
				m_iOperatingClipFrameCount = iDropInPoint - m_pOperatingClipData->m_iTimelineInPoint;
				m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
				m_pEnableMovingTrack = m_pOperateToTrack;
				CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), iDropInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
			}
		}
		// �ړ��悪�z�u�ł��Ȃ��ꍇ�A�O��ړ��\�������ꏊ�ɖ߂��B
		m_iOperatingClipFrameCount = m_iEnableMovingFrameCount;
		m_pOperateToTrack = m_pEnableMovingTrack;
		CalcClipRect(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData->m_iTimelineInPoint, m_pOperatingClipData->GetDuration(), m_pOperateToTrack);
		return FALSE;
	}


	m_iEnableMovingFrameCount = m_iOperatingClipFrameCount;
	m_pEnableMovingTrack = m_pOperateToTrack;
	CalcClipRectDisplayPoint(*(m_pOperatingClipData->GetOperatingRect()), m_pOperatingClipData, m_pOperateToTrack, m_iOperatingClipFrameCount);
	return TRUE;
}
