#pragma once

class ClipDataRect;
class TrackDataInfo;

// SingleTrimOperator コマンド ターゲット

class SingleTrimOperator : public CObject
{
public:
	SingleTrimOperator();
	virtual ~SingleTrimOperator();

private:
	ClipDataRect* m_clMovingClipData;		// 操作中クリップへのポインタ
	TrackDataInfo* m_clOperateToTrackInfo;	// 操作先トラックへのポインタ
	int m_iOperatingClipFrameCount;			// 操作中のフレーム数

	BOOL CheckInTrim(void);
	BOOL CheckOutTrim(void);



};


