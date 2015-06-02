// SingleTrimOperator.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "SingleTrimOperator.h"

#include "ClipDataRect.h"
#include "TrackDataInfo.h"


// SingleTrimOperator

SingleTrimOperator::SingleTrimOperator()
{
}

SingleTrimOperator::~SingleTrimOperator()
{
}


// SingleTrimOperator �����o�[�֐�

// ���삪In���g�����\�Ȕ͈͓����𔻒肵�Ĉʒu�𒲐�����
BOOL SingleTrimOperator::CheckInTrim(void)
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
BOOL SingleTrimOperator::CheckOutTrim(void)
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
