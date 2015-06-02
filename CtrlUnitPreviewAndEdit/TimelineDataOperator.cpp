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



// TimelineDataOperator

TimelineDataOperator::TimelineDataOperator()
{
}

TimelineDataOperator::~TimelineDataOperator()
{
}

void TimelineDataOperator::InitializeTimelineDataOperator(void)
{
	m_pTimelineEditPanelRect = new OpenGLRect();
	m_pTimelineEditHeaderRect = new OpenGLRect();
	m_pTimelineControlPanelRect = new OpenGLRect();
	m_pSeekBarRect = new OpenGLRect();
	m_pTrackHeaderRect = new OpenGLRect();
	m_pTimelineDataRect = new OpenGLRect();
	m_pTimelineCursorHitArea = new OpenGLRect();
	m_pTransisionRect = new OpenGLRect();
}

void TimelineDataOperator::DeleteTimelineDataOperator(void)
{
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
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	
	// �^�C�����C���f�[�^�G���A������
	if (m_pTimelineDataRect->PtInRect(point))
	{
		// �g���b�N����
		m_pSelectedTrack = IsPointInAnyTrack(point);
		m_pSelectedTrackInfo = m_pSelectedTrack->GetTrackDataInfo();
		if (m_pSelectedTrack != nullptr)
		{
			m_pOperateToTrack = m_pSelectedTrack;
			m_pOperateToTrackInfo = m_pSelectedTrackInfo;

			if (IsPointInAnyClipRect(point))
			{
				m_fLButtonClicking = TRUE;
				//SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
				m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
				m_clMovingClipData->SetOperatingRect(static_cast<CRect>(m_clMovingClipData));
				m_rcMousePointRect.CopyRect(static_cast<CRect>(m_clMovingClipData));
				m_iOperatingClipFrameCount = 0;
				m_iEnableMovingFrameCount = 0;
				m_pEnableMovingTrack = nullptr;
				return TRUE;
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
		if (m_pTimelineCursorHitArea->PtInRect(point))
		{
			m_fLButtonClicking = TRUE;
			m_fDragShuttling = TRUE;
			//SetCapture(); // �}�E�X���L���v�`���[( OnLButtonUp()�ŉ��)
			m_poMousePointerLocation = point;	// �ړ��ʌv�Z�̂��߁A�������W��ۑ�
			m_iOperatingFrameCount = 0;
			m_fSuttleSpeed = 0;
			return TRUE;
		}
		// �V�[�N�o�[������
		else if (IsPointInSeekBar(point))
		{
			m_fLButtonClicking = TRUE;
			m_fScrubing = TRUE;
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
	m_iRightFrameNumber = m_iTimelineCursorFramePosition + static_cast<int>(ceil((iDisplayFrameCount / 2.0f))) + 1;
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
	m_fMoving = FALSE;

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
				CalcClipRectDisplayPoint(static_cast<CRect>(m_clMovingClipData), m_clMovingClipData, static_cast<CRect>(m_pSelectedTrack));
			}
		}
	}

	return (m_fMoving || m_fSingleInTriming || m_fSingleOutTriming);
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
		m_fMoving = !(IsPointInTrimRange(point, &rcClipRect));
	}
	return (m_fMoving || m_fSingleInTriming || m_fSingleOutTriming);
}

// �N���b�N�ӏ����N���b�v���̃g��������G���A�����𔻒�
BOOL TimelineDataOperator::IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect)
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


