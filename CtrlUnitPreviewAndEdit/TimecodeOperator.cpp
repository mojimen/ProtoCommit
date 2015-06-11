// TimecodeOperator.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TimecodeOperator.h"


// TimecodeOperator

TimecodeOperator::TimecodeOperator()
{
	m_pTc = nullptr;
}

TimecodeOperator::~TimecodeOperator()
{
	if (m_pTc)
	{
		delete m_pTc;
	}
}


// TimecodeOperator メンバー関数
BOOL TimecodeOperator::InitializeTimecodeOperator(CString& strTimecodeOperatorUUID)
{
	m_pTc = new Timecode();
	if (InitializeData(TIMECODEOPERATOR))
	{
		strTimecodeOperatorUUID = m_strUUID;
		return TRUE;
	}
	return FALSE;
}

Timecode* TimecodeOperator::ChangeFrameToTimecode(const int& iFrame, BOOL fDF /*= TRUE*/)
{
	clearTimecode(*m_pTc);
	if (fDF)
	{
		// 59.94fps 想定
		m_pTc->iHH = iFrame / m_kDFFramePerHour;
		m_pTc->iMM = (iFrame / m_kDFFramePerTenMin) % 6 * 10 + (iFrame % m_kDFFramePerTenMin - 4) / m_kDFFramePerMin;
		m_pTc->iSS = ((iFrame % m_kDFFramePerTenMin - 4) % m_kDFFramePerMin + 4) / 60;
		m_pTc->iFrame = ((iFrame % m_kDFFramePerTenMin - 4) % m_kDFFramePerMin + 4) % 60;
	}
	else
	{
		m_pTc->iFrame = iFrame % 60;
		int iTime = iFrame / 60;
		m_pTc->iSS = iTime % 60;
		iTime /= 60;
		m_pTc->iMM = iTime % 24;
		m_pTc->iHH = iTime / 24;
	}
	return m_pTc;
}

Timecode* TimecodeOperator::ChangeFrameToTimecode(const int& iFrame, CString& strTimecode, BOOL fDF /*= TRUE*/)
{
	ChangeFrameToTimecode(iFrame, fDF);
	if (fDF)
	{
		strTimecode.Format(TIMECODE_DF_STRING, m_pTc->iHH, m_pTc->iMM, m_pTc->iSS, m_pTc->iFrame);
	}
	else
	{
		strTimecode.Format(TIMECODE_NDF_STRING, m_pTc->iHH, m_pTc->iMM, m_pTc->iSS, m_pTc->iFrame);
	}
	return m_pTc;
}


int TimecodeOperator::ChangeTimeCodeToFrame(const Timecode& tc, BOOL fDF /*= TRUE*/)
{
	int iFrame;
	iFrame = tc.iHH * 24 * 60 * 60 +
			 tc.iMM * 60 * 60 +
			 tc.iSS * 60 +
			 tc.iFrame;
	return iFrame;
}

void TimecodeOperator::clearTimecode(Timecode& tc)
{
	tc.iHH = 0;
	tc.iMM = 0;
	tc.iSS = 0;
	tc.iFrame = 0;
}
