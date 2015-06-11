#pragma once

#include "DataObjectBase.h"

// TimecodeOperator コマンド ターゲット

struct Timecode
{
	int iHH;
	int iMM;
	int iSS;
	int iFrame;
};

#define TIMECODE_DF_STRING _T("%02d:%02d:%02d;%02d")
#define TIMECODE_NDF_STRING _T("%02d:%02d:%02d:%02d")

class TimecodeOperator : public DataObjectBase
{
public:
	TimecodeOperator();
	virtual ~TimecodeOperator();

private:
	Timecode* m_pTc;
	const int m_kDFDropFrameCount = 4;
	const int m_kDFFramePerMin = 3596;
	const int m_kDFFramePerTenMin = 35964;
	const int m_kDFFramePerHour = 215784;
	const int m_kDFNotDropIntervalMiniute = 10;



public:
	BOOL InitializeTimecodeOperator(CString& strTimecodeOperatorUUID);
	Timecode* ChangeFrameToTimecode(const int& iFrame, BOOL fDF = TRUE);
	Timecode* ChangeFrameToTimecode(const int& iFrame, CString& strTimecode, BOOL fDF = TRUE);
	int ChangeTimeCodeToFrame(const Timecode& tc, BOOL fDF = TRUE);
	void clearTimecode(Timecode& tc);
};


