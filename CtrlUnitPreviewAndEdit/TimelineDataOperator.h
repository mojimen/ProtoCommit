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


	UUID m_uiTimelineDataOperatorId;

	// ���쒆�N���b�v�ւ̃|�C���^
	ClipDataRect* m_pOperatingClipData;

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
	BOOL m_fLButtonDown;			// �}�E�X�{�^����������Ă��邩�ǂ������L�^
	BOOL m_fMove;						// Move���쒆
	BOOL m_fSingleInTrim;			// In��SingleTrim���쒆
	BOOL m_fSingleOutTrim;			// Out��SingleTrim���쒆
	BOOL m_fScrub;					// Scrub���쒆
	BOOL m_fDragShuttle;				// DragShuttle���쒆

	// �^�C�����C���f�[�^�Ǘ�
	TimelineDataManager* m_pTimelineDataManager;
	UUID m_uiTimelineDataManagerId;
	TrackDataManager* m_pTrackDataVideoManager;
	UUID m_uiTrackDataVideoManagerId;
	TrackDataManager* m_pTrackDataAudioManager;
	UUID m_uiTrackDataAudioManagerId;
	TrackDataManager* m_pTrackDataInfoManager;
	UUID m_uiTrackDataInfoManagerId;
	TrackDataManager* m_pTrackDataMasterManager;
	UUID m_uiTrackDataMasterManagerId;
	ClipDataManager* m_pClipDataManager;
	UUID m_uiClipDataManagerId;

	int m_iTimelineCursorFramePosition;	// �^�C�����C���J�[�\���ʒu�̃t���[���ԍ�
	int m_iLeftFrameNumber;				// �^�C�����C���f�[�^�\���͈͂̐擪�t���[��
	int m_iRightFrameNumber;			// �^�C�����C���f�[�^�\���͈͂̍ŏI�t���[��
	int m_iOperatingFrameCount;			// ���쒆�̈ړ��t���[����
	int m_iOperatingTimelineCursorFramePosition;	// �^�C�����C���J�[�\���ʒu�̃t���[���ԍ�
	int m_iOperatingLeftFrameNumber;	// �^�C�����C���f�[�^�\���͈͂̐擪�t���[��
	int m_iOperatingRightFrameNumber;	// �^�C�����C���f�[�^�\���͈͂̍ŏI�t���[��
	int m_iOperatingClipFrameCount;		// ���쒆�N���b�v�̈ړ��^�L�k�t���[����
	float m_fSuttleSpeed;				// �V���g�����쒆�̈ړ��X�s�[�h�i�{�j
	int m_iEnableMovingFrameCount;		// Move���쒆�Ɉړ����\�ł��������߂̈ړ��t���[����
	TrackDataRect* m_pEnableMovingTrack;// Move���쒆�Ɉړ����\�ł��������߂̃g���b�N�ʒu

	int m_iFramePerPoint;				// �P�|�C���g������̃t���[����
	int m_iPointPerFrame;				// �P�t���[��������̃|�C���g��
	int m_iSmallScaleDrawInterval;		// ���ڐ��肠����̃t���[����
	int m_iMiddleScaleDrawInterval;		// ���ڐ��肠����̏��ڐ��萔
	int m_iBigScaleDrawInterval;		// ��ڐ��肠����̒��ڐ��萔
	int m_iTimelineCursorPoint;			// �^�C�����C���J�[�\���̕`��ʒu


public:
	BOOL InitializeTimelineDataOperator(UUID& uiTimelineDataOperatorId);
	void DeleteTimelineDataOperator(void);
	BOOL OnLButtonDown(UINT nFlags, CPoint point);	//L�{�^���̏���
	BOOL OnLButtonUp(UINT nFlags, CPoint point);	//L�{�^���̏���
	BOOL OnRButtonUp(UINT nFlags, CPoint point);	//L�{�^���̏���
	BOOL OnMouseMove(UINT nFlags, CPoint point);	//�}�E�X�ړ����̏���

	BOOL ChangeDisplayScale(void);
	void CalcTimelineDisplayRange(void);

	// �q�b�g�e�X�g
	TrackDataRect* IsPointInAnyTrack(const CPoint& point);
	BOOL IsPointInAnyClipRect(const CPoint& point);
	BOOL IsPointInClipRect(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInSeekBar(const CPoint& point);
	BOOL IsPointInTimelineControlPanel(const CPoint& point);

	// ����`�F�b�N
	BOOL CheckInTrim(void);
	BOOL CheckOutTrim(void);
	BOOL CheckMove(CPoint& point);

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

	TimelineDataManager* GetTimelineDataManager(void){ return m_pTimelineDataManager; }
	UUID GetTimelineDataManagerId(void){ return m_uiTimelineDataManagerId; }
	BOOL IsLButtonDown(void){ return m_fLButtonDown; }
	BOOL IsSingleInTrim(void){ return m_fSingleInTrim; }
	BOOL IsSingleOutTrim(void){ return m_fSingleOutTrim; }
	BOOL IsMove(void){ return m_fMove; }
	BOOL IsScrub(void){ return m_fScrub; }
	BOOL IsDragShuttle(void){ return m_fDragShuttle; }

	int GetTimelineCursorFramePosition(void){ return m_iTimelineCursorFramePosition; }
	int GetLeftFrameNumber(void){ return m_iLeftFrameNumber; }
	int GetRightFrameNumber(void){ return m_iRightFrameNumber; }
	int GetOperatingFrameCount(void){ return m_iOperatingFrameCount; }
	int GetOperatingTimelineCursorFramePosition(void){ return m_iOperatingTimelineCursorFramePosition; }
	int GetOperatingLeftFrameNumber(void){ return m_iOperatingLeftFrameNumber; }
	int GetOperatingRightFrameNumber(void){ return m_iOperatingRightFrameNumber; }
	int GetOperatingClipFrameCount(void){ return m_iOperatingClipFrameCount; }
	float GetSuttleSpeed(void){ return m_fSuttleSpeed; }
	int GetEnableMovingFrameCount(void){ return m_iEnableMovingFrameCount; }
	TrackDataRect* GetEnableMovingTrack(void){ return m_pEnableMovingTrack; }

	int GetFramePerPoint(void){ return m_iFramePerPoint; }
	int GetPointPerFrame(void){ return m_iPointPerFrame; }
	int GetSmallScaleDrawInterval(void){ return m_iSmallScaleDrawInterval; }
	int GetMiddleScaleDrawInterval(void){ return m_iMiddleScaleDrawInterval; }
	int GetBigScaleDrawInterval(void){ return m_iBigScaleDrawInterval; }
	int GetTimelineCursorPoint(void){ return m_iTimelineCursorPoint; }

	ClipDataRect* GetOperatingClipData(void){ return m_pOperatingClipData; }
	CRect GetMousePointRect(void){ return m_rcMousePointRect; }
	CPoint GetMousePointerLocation(void){ return m_poMousePointerLocation; }

	TrackDataRect* GetSelectedTrack(void){ return m_pSelectedTrack; }
	TrackDataInfo* GetSelectedTrackInfo(void){ return m_pSelectedTrackInfo; }
	TrackDataRect* GetOperateToTrack(void){ return m_pOperateToTrack; }
	TrackDataInfo* GetOperateToTrackInfo(void){ return m_pOperateToTrackInfo; }
};


