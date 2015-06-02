#pragma once

class OpenGLRect;
class TimelineDataManager;
class TrackDataManager;
class TrackDataInfo;
class TrackDataRect;
class ClipDataManager;
class ClipDataRect;

// TimelineDataOperator �R�}���h �^�[�Q�b�g

class TimelineDataOperator : public CObject
{
public:
	TimelineDataOperator();
	virtual ~TimelineDataOperator();

private:
	const double kSlideTrimAreaRate = 0.1;			// �X���C�h�g�����G���A�Ƃ��Ďg�p����䗦
	const double kTrimAreaRate = 0.2;				// �g�����G���A�Ƃ��Ďg�p����䗦
	const int kClipHitCheckMinWidth = 15;			// �N���b�v�����蔻��̍ŏ��ۏؕ��i������N���b�v���Z���ꍇ�Ɏg�p���镝�j
	const int kTrimHitCheckMinWidth = 5;			// �g�����`�F�b�N�Ŏg�p����ŏ����i������g�����`�F�b�N�͈͂��Z���ꍇ�Ɏg�p���镝�j
	const int kTrimHitCheckMaxWidth = 30;			// �g�����`�F�b�N�Ŏg�p����ő啝�i������g�����`�F�b�N�͈͂������ꍇ�Ɏg�p���镝�j


	// ���쒆�N���b�v�ւ̃|�C���^
	ClipDataRect* m_clMovingClipData;

	// TODO: ���쒆�g���b�N�ւ̃|�C���^			�����I�����̍l�����K�v
	TrackDataRect* m_pSelectedTrack;		// �I�������N���b�v�̂������g���b�N
	TrackDataInfo* m_pSelectedTrackInfo;
	TrackDataRect* m_pOperateToTrack;		// �ړ���g���b�N
	TrackDataInfo* m_pOperateToTrackInfo;

	OpenGLRect* m_pTimelineEditPanelRect;
	OpenGLRect* m_pTimelineEditHeaderRect;
	OpenGLRect* m_pTimelineControlPanelRect;
	OpenGLRect* m_pSeekBarRect;
	OpenGLRect* m_pTrackHeaderRect;
	OpenGLRect* m_pTimelineDataRect;
	OpenGLRect* m_pTimelineCursorHitArea;
	//TODO: ����͂���Ȃ������H
	OpenGLRect* m_pTransisionRect;

	CRect m_rcMousePointRect;			// �ړ����̃C���[�W�i�d�Ȃ蒆�̃C���[�W�j
	CPoint m_poMousePointerLocation;	// �}�E�X�{�^���������ꂽ�Ƃ��̈ʒu
	BOOL m_fLButtonClicking;			// �}�E�X�{�^����������Ă��邩�ǂ������L�^
	BOOL m_fMoving;						// Move���쒆
	BOOL m_fSingleInTriming;			// In��SingleTrim���쒆
	BOOL m_fSingleOutTriming;			// Out��SingleTrim���쒆
	BOOL m_fScrubing;					// Scrub���쒆
	BOOL m_fDragShuttling;				// DragShuttle���쒆

	// �^�C�����C���f�[�^�Ǘ�
	TimelineDataManager* m_pTimelineDataManager;
	UUID m_uiTimelineDataManager;
	TrackDataManager* m_pTrackDataVideoManager;
	UUID m_uiTrackDataVideoManager;
	TrackDataManager* m_pTrackDataAudioManager;
	UUID m_uiTrackDataAudioManager;
	TrackDataManager* m_pTrackDataInfoManager;
	UUID m_uiTrackDataInfoManager;
	TrackDataManager* m_pTrackDataMasterManager;
	UUID m_uiTrackDataMasterManager;
	ClipDataManager* m_pClipDataManager;
	UUID m_uiClipDataManager;

	int m_iTimelineCursorFramePosition;	// �^�C�����C���J�[�\���ʒu�̃t���[���ԍ�
	int m_iLeftFrameNumber;				// �^�C�����C���f�[�^�\���͈͂̐擪�t���[��
	int m_iRightFrameNumber;			// �^�C�����C���f�[�^�\���͈͂̍ŏI�t���[��
	int m_iOperatingFrameCount;			// ���쒆�̈ړ��t���[����
	int m_iOperatingTimelineCursorFramePosition;	// �^�C�����C���J�[�\���ʒu�̃t���[���ԍ�
	int m_iOperatingLeftFrameNumber;	// �^�C�����C���f�[�^�\���͈͂̐擪�t���[��
	int m_iOperatingRightFrameNumber;	// �^�C�����C���f�[�^�\���͈͂̍ŏI�t���[��
	int m_iOperatingClipFrameCount;		// ���쒆�N���b�v�̈ړ��^�L�k�t���[����
	int m_fSuttleSpeed;					// �V���g�����쒆�̈ړ��X�s�[�h�i�{�j
	int m_iEnableMovingFrameCount;		// Move���쒆�Ɉړ����\�ł��������߂̈ړ��t���[����
	TrackDataRect* m_pEnableMovingTrack;// Move���쒆�Ɉړ����\�ł��������߂̃g���b�N�ʒu

	int m_iFramePerPoint;				// �P�|�C���g������̃t���[����
	int m_iPointPerFrame;				// �P�t���[��������̃|�C���g��
	int m_iSmallScaleDrawInterval;		// ���ڐ��肠����̃t���[����
	int m_iMiddleScaleDrawInterval;		// ���ڐ��肠����̏��ڐ��萔
	int m_iBigScaleDrawInterval;		// ��ڐ��肠����̒��ڐ��萔
	int m_iTimelineCursorPoint;			// �^�C�����C���J�[�\���̕`��ʒu

	void InitializeTimelineDataOperator(void);
	void DeleteTimelineDataOperator(void);
	BOOL OnLButtonDown(UINT nFlags, CPoint point);	//L�������̏���
	
	BOOL ChangeDisplayScale(void);
	void CalcTimelineDisplayRange(void);

	// �q�b�g�e�X�g
	TrackDataRect* IsPointInAnyTrack(const CPoint& point);
	BOOL IsPointInAnyClipRect(const CPoint& point);
	BOOL IsPointInClipRect(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect);
	//BOOL CheckInTrim(void);
	//BOOL CheckOutTrim(void);
	//BOOL CheckMove(CPoint& point);
	BOOL IsPointInSeekBar(const CPoint& point);
	BOOL IsPointInTimelineControlPanel(const CPoint& point);

	// ���W�v�Z�E�ϊ�
	BOOL CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	BOOL CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	int ChangeTimelineFramePositionToDisplayPoint(const int iFrame);
	int ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame);
	int ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame = 0);



	// Getter
	OpenGLRect* GetTimelineEditPanelRect(void){ return m_pTimelineEditPanelRect; }
	OpenGLRect* GetTimelineEditHeaderRect(void){ return m_pTimelineEditHeaderRect; }
	OpenGLRect* GetTimelineControlPanelRect(void){ return m_pTimelineControlPanelRect; }
	OpenGLRect* GetSeekBarRect(void){ return m_pSeekBarRect; }
	OpenGLRect* GetTrackHeaderRect(void){ return m_pTrackHeaderRect; }
	OpenGLRect* GetTimelineDataRect(void){ return m_pTimelineDataRect; }
	OpenGLRect* GetTimelineCursorHitArea(void){ return m_pTimelineCursorHitArea; }
	//TODO: ����͂���Ȃ������H
	OpenGLRect* GetTransisionRect(void){ return m_pTransisionRect; }

};


