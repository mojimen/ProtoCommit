#pragma once

class ClipDataRect;
class TrackDataInfo;

// SingleTrimOperator �R�}���h �^�[�Q�b�g

class SingleTrimOperator : public CObject
{
public:
	SingleTrimOperator();
	virtual ~SingleTrimOperator();

private:
	ClipDataRect* m_clMovingClipData;		// ���쒆�N���b�v�ւ̃|�C���^
	TrackDataInfo* m_clOperateToTrackInfo;	// �����g���b�N�ւ̃|�C���^
	int m_iOperatingClipFrameCount;			// ���쒆�̃t���[����

	BOOL CheckInTrim(void);
	BOOL CheckOutTrim(void);



};


