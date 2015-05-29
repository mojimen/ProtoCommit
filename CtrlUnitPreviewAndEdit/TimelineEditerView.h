#pragma once

#include <map>
#include "OpenGLView.h"

class OpenGLRect;
class ClipDataRect;
class TrackDataInfo;
class TrackDataRect;

typedef std::map<int, int> DisplayScaleMap;

// TimelineEditerView �r���[

class TimelineEditerView : public OpenGLView
{
	DECLARE_DYNCREATE(TimelineEditerView)

protected:
	TimelineEditerView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~TimelineEditerView();

public:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);




private:
	// ���샂�[�h
#define PROTOTYPEMODE
#define SEEKBAR_MIDDLESCALELINE_DRAW
#define SEEKBAR_SMALLSCALELINE_DRAW

	// ���ʃJ���[
#define BLACKCOLOR_BRUSH_FLOAT 0.0f, 0.0f, 0.0f, 1.0f
#define WHITECOLOR_BRUSH_FLOAT 1.0f, 1.0f, 1.0f, 1.0f
#define REDCOLOR_BRUSH_FLOAT 1.0f, 0.0f, 0.0f, 1.0f
#define BLUECOLOR_BRUSH_FLOAT 0.0f, 0.0f, 1.0f, 1.0f
#define GREENCOLOR_BRUSH_FLOAT 0.0f, 1.0f, 0.0f, 1.0f
#define DARKGRAYCOLOR_BRUSH_FLOAT 0.3f, 0.3f, 0.3f, 1.0f
#define DARKGRAYCOLOR2_BRUSH_FLOAT 0.2f, 0.2f, 0.2f, 1.0f
#define DARKGRAYCOLOR3_BRUSH_FLOAT 0.1f, 0.1f, 0.1f, 1.0f
#define GRAYCOLOR_BRUSH_FLOAT 0.5f, 0.5f, 0.5f, 1.0f
#define LIGHTGRAYCOLOR_BRUSH_FLOAT 0.7f, 0.7f, 0.7f, 1.0f
#define LIGHTGRAYCOLOR2_BRUSH_FLOAT 0.8f, 0.8f, 0.8f, 1.0f
#define LIGHTGRAYCOLOR3_BRUSH_FLOAT 0.9f, 0.9f, 0.9f, 1.0f

#define ACCENTCOLOR_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 1.0f
#define ACCENTCOLOR_HALFALPHA_BRUSH_FLOAT 0.3f, 0.7f, 1.0f, 0.5f
#define ACCENTCOLOR2_BRUSH_FLOAT 0.3f,0.4f,1.0f,1.0f
#define ACCENTCOLOR2_HALFALPHA_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.5f
#define ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT 0.3f,0.4f,1.0f,0.7f
#define ACCENTCOLOR3_BRUSH_FLOAT 0.8f,0.9f,1.0f,1.0f
#define ACCENTCOLOR4_BRUSH_FLOAT 0.5f,0.8f,1.0f,1.0f

#define BASECOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define CAUTIONCOLOR_BRUSH_FLOAT REDCOLOR_BRUSH_FLOAT

	// �ŗL�J���[
#define PREVIEWPANELBORDER_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define TIMELINEBASECOLOR_BRUSH_FLOAT BASECOLOR_BRUSH_FLOAT

#define TIMELINECONTROLPANELBACKGROUNDCOLOR_BRUSH_FLOAT ACCENTCOLOR2_BRUSH_FLOAT
#define SEEKBARBACKGROUNDCOLOR_BRUSH_FLOAT ACCENTCOLOR2_BRUSH_FLOAT
#define TIMELINETRACKHEADERBORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH_FLOAT WHITECOLOR_BRUSH_FLOAT
#define TIMELINETRACKCOLOR_BRUSH2_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINETRACKBORDERCOLOR_BRUSH2_FLOAT ACCENTCOLOR2_ALPHA1_BRUSH_FLOAT
#define TIMELINEDATAAREABORDERCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define SEEKBARBIGSCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define SEEKBARMIDDLESCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR2_BRUSH_FLOAT
#define SEEKBARSMALLSCALECOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR_BRUSH_FLOAT

#define SEEKBARTIMECODETEXTCOLOR_BRUSH_FLOAT LIGHTGRAYCOLOR3_BRUSH_FLOAT

#define SEEKBARBIGSCALELINECOLOR_BRUSH_FLOAT SEEKBARBIGSCALECOLOR_BRUSH_FLOAT
#define SEEKBARMIDDLESCALELINECOLOR_BRUSH_FLOAT 1.0f, 0.8f, 1.0f, 1.0f
#define SEEKBARSMALLSCALELINECOLOR_BRUSH_FLOAT 1.0f, 1.0f, 0.9f, 1.0f

#define TIMELINECURSORCOLOR_BRUSH_FLOAT 1.0f, 0.0f, 1.0f, 0.6f
#define TIMELINECURSORDRAGGUIDELINECOLOR_BRUSH ACCENTCOLOR_BRUSH



	// ������ʁ^�T�C�Y
#define FONTFACE_MSMINCHOU _T("�l�r ����")
#define FONTFACE_MSGOTHIC _T("�l�r �S�V�b�N")
#define FONTFACE_MEIRIO _T("���C���I")
#define DEFAULT_FONTFACE FONTFACE_MSGOTHIC
#define SEEKBARTIMECODE_FONTSIZE 15
#define SEEKBARTIMECODE_MIDDLE_FONTSIZE 12

	// ��
#define PREVIEWPANELBORDER_LINEWIDTH 1.0f
#define SEEKBARBIGSCALE_LINEWIDTH 1.0f
#define SEEKBARMIDDLESCALE_LINEWIDTH 1.0f
#define SEEKBARSMALLSCALE_LINEWIDTH 1.0f
#define SEEKBARBIGSCALELINE_LINEWIDTH SEEKBARBIGSCALE_LINEWIDTH
#define SEEKBARMIDDLESCALELINE_LINEWIDTH SEEKBARMIDDLESCALE_LINEWIDTH
#define SEEKBARSMALLSCALELINE_LINEWIDTH SEEKBARSMALLSCALE_LINEWIDTH
#define TIMELINECURSOR_LINEWIDTH 1.0f


	//�}�[�W���A�ʒu�␳
#define SEEKBARTIMECODE_MARGINX 2
#define SEEKBARTIMECODE_MARGINY 16
#define SEEKBARTIMECODE_MIDDLE_MARGINY 17
#define SEEKBARBIGSCALE_TOPMARGIN 10
#define SEEKBARMIDDLESCALE_TOPMARGIN 20
#define SEEKBARSMALLSCALE_TOPMARGIN 25
#define TIMELINE_DEFAULTZ 0.0f
#define CLIPHIGHT 0.9f


	const int kSplitterWidth = 5;					// �X�v���b�^�̕�
	const int kSplitterHeight = 5;					// �X�v���b�^�̕�
	const double kSlideTrimAreaRate = 0.1;			// �X���C�h�g�����G���A�Ƃ��Ďg�p����䗦
	const double kTrimAreaRate = 0.2;				// �g�����G���A�Ƃ��Ďg�p����䗦
	const int kClipHitCheckMinWidth = 15;			// �N���b�v�����蔻��̍ŏ��ۏؕ��i������N���b�v���Z���ꍇ�Ɏg�p���镝�j
	const int kTrimHitCheckMinWidth = 5;			// �g�����`�F�b�N�Ŏg�p����ŏ����i������g�����`�F�b�N�͈͂��Z���ꍇ�Ɏg�p���镝�j
	const int kTrimHitCheckMaxWidth = 30;			// �g�����`�F�b�N�Ŏg�p����ő啝�i������g�����`�F�b�N�͈͂������ꍇ�Ɏg�p���镝�j
	const int kPreviewPanelDefaltHeight = 300;		// ���g�p�ɂȂ�
	const int kPreviewPanelMinHeight = 100;			// ���g�p�ɂȂ�

	const int kTimelineEditDefaltHeight = 300;		// �^�C�����C���f�[�^�\���p�l���̏�������
	const int kTimelineEditPanelMinHeight = 100;	// �^�C�����C���f�[�^�\���p�l���̍ŏ�����
	const int kTimelineEditHeaderDefaltHeight = 30;	// �^�C�����C���f�[�^�\���p�l���w�b�_�[�s�̏�������
	const int kTimelineControlPanelDefaultWidth = 300;	// �^�C�����C���f�[�^�\���p�l�����o���̏�����

	const int kTrackDefaultHeight = 100;			// �g���b�N�̏��������iTODO: �g���b�N�}�l�[�W���[�Ɉړ����ׂ��j
	const int kTrackMinHeight = 10;					// �g���b�N�̍ŏ�����
	const int kTrackMaxHeight = 1000;				// �g���b�N�̍ő卂��


	const int kTimelineCursorDragArea = 5;					// �^�C�����C���J�[�\���h���b�O���̃q�b�g�̈�i���E�ɂ��̒l���v���X���Ĕ���j
	const int kTimelineCursorDragGuideLineThickness = 1;	// �^�C�����C���J�[�\���h���b�O���̃K�C�h���C���̑���
	const float kTimelineCursorDragOneSpeed = 0.1f;			// �^�C�����C���J�[�\���h���b�O���́~�P�{�ړ��͈�
	const float kTimelineCursorDragTowSpeed = 0.2f;			// �^�C�����C���J�[�\���h���b�O���́~�Q�{�ړ��͈�
	const float kTimelineCursorDragFourSpeed = 0.28f;		// �^�C�����C���J�[�\���h���b�O���́~�S�{�ړ��͈�
	const float kTimelineCursorDragEightSpeed = 0.36f;		// �^�C�����C���J�[�\���h���b�O���́~�W�{�ړ��͈�
	const float kTimelineCursorDragSixteenSpeed = 0.42f;	// �^�C�����C���J�[�\���h���b�O���́~�P�U�{�ړ��͈�
	const float kTimelineCursorDragThirtyTwoSpeed = 0.47f;	// �^�C�����C���J�[�\���h���b�O���́~�R�Q�{�ړ��͈�
	const float kTimelineCursorDragSixtyFourSpeed = 0.5f;	// �^�C�����C���J�[�\���h���b�O���́~�U�S�{�ړ��͈�


	//�\�����
private:
	OpenGLRect* m_prcPreviewPanelRect;
	OpenGLRect* m_prcTimelineEditPanelRect;
	OpenGLRect* m_prcTimelineEditHeaderRect;
	OpenGLRect* m_prcTimelineControlPanelRect;
	OpenGLRect* m_prcSeekBarRect;
	OpenGLRect* m_prcTrackHeaderRect;
	OpenGLRect* m_prcTimelineDataRect;
	OpenGLRect* m_prcTimelineCursorHitArea;
	OpenGLRect* m_prcTransisionRect;

	int m_iTimelineCursorFramePosition;	// �^�C�����C���J�[�\���ʒu�̃t���[���ԍ�
	int m_iLeftFrameNumber;				// �^�C�����C���f�[�^�\���͈͂̐擪�t���[��
	int m_iRightFrameNumber;			// �^�C�����C���f�[�^�\���͈͂̍ŏI�t���[��
	int m_iOperatingFrameCount;			// ���쒆�̈ړ��t���[����
	int m_iOperatingTimelineCursorFramePosition;	// �^�C�����C���J�[�\���ʒu�̃t���[���ԍ�
	int m_iOperatingLeftFrameNumber;	// �^�C�����C���f�[�^�\���͈͂̐擪�t���[��
	int m_iOperatingRightFrameNumber;	// �^�C�����C���f�[�^�\���͈͂̍ŏI�t���[��
	int m_iOperatingClipFrameCount;		// ���쒆�N���b�v�̈ړ��^�L�k�t���[����
	int m_fSuttleSpeed;					// �V���g�����쒆�̈ړ��X�s�[�h�i�{�j
	int m_iFramePerPoint;				// �P�|�C���g������̃t���[����
	int m_iPointPerFrame;				// �P�t���[��������̃|�C���g��
	int m_iSmallScaleDrawInterval;		// ���ڐ��肠����̃t���[����
	int m_iMiddleScaleDrawInterval;		// ���ڐ��肠����̏��ڐ��萔
	int m_iBigScaleDrawInterval;		// ��ڐ��肠����̒��ڐ��萔
	DisplayScaleMap m_mpScalePoint;	// �\���{���ύX��񌟍��p
	DisplayScaleMap m_mpScaleFrame;	// �\���{���ύX��񌟍��p
	int m_iDisplayScaleArray[2][30][4];		// �\���{���ύX���
	//0: m_iSmallScaleDrawInterval�����Z�l
	//1: m_iMiddleScaleDrawInterval�����Z�l
	//2: m_iBigScaleDrawInterval�����Z�l
	//3: m_iPointPerFrame/m_iFramePerPoint�����Z�l
	int m_iTimelineCursorPoint;			// �^�C�����C���J�[�\���̕`��ʒu

	void SetPanelRect(void);
	BOOL CalcClipRectDisplayPoint(CRect& rcClipRect, const ClipDataRect* clClipData, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	BOOL CalcClipRect(CRect& rcClipRect, const int& iInPoint, const int& iDuration, const CRect& rcTrackRect,
		const int& iMoveFrame = 0, const int& iIntrimFrames = 0, const int& iOuttrimFrames = 0);
	BOOL ChangeDisplayScale(void);
	int ChangeTimelineFramePositionToDisplayPoint(const int iFrame);
	int ChangeDisplayPointToTimelineFramePosition(const CPoint& point, int& iActualFrame);
	int ChangeOperatingDistanceToTimelineFrames(const CSize& szMoveSize, const int iStartFrame = 0);

	// ���z�N���b�v
	ClipDataRect* m_clClipData1;
	ClipDataRect* m_clClipData2;
	ClipDataRect* m_clMovingClipData;
	ClipDataRect* m_clStaticClipData;
	// ���z�g���b�N
	TrackDataRect* m_clTrack1;
	TrackDataRect* m_clTrack2;
	TrackDataInfo* m_clTrackInfo1;
	TrackDataInfo* m_clTrackInfo2;
	TrackDataRect* m_clSelectedTrack;
	TrackDataRect* m_clOperateToTrack;
	TrackDataInfo* m_clSelectedTrackInfo;
	TrackDataInfo* m_clOperateToTrackInfo;

	// �}�`�h���b�O�p
private:
	CRect m_rcMousePointRect;		// �ړ����̃C���[�W�i�d�Ȃ蒆�̃C���[�W�j
	CPoint m_poMousePointerLocation; // �}�E�X�{�^���������ꂽ�Ƃ��̈ʒu
	BOOL m_fLButtonClicking;	// �}�E�X�{�^����������Ă��邩�ǂ������L�^
	BOOL m_fMoving;				// Move���쒆
	BOOL m_fSingleInTriming;	// In��SingleTrim���쒆
	BOOL m_fSingleOutTriming;	// Out��SingleTrim���쒆
	BOOL m_fScrubing;			// Scrub���쒆
	BOOL m_fDragShuttling;		// DragShuttle���쒆

	TrackDataRect* IsPointInAnyTrack(const CPoint& point);
	BOOL IsPointInAnyClipRect(const CPoint& point);
	BOOL IsPointInClipRect(const CPoint& point, const CRect& rcClipRect);
	BOOL IsPointInTrimRange(const CPoint& point, const CRect& rcClipRect);
	BOOL CheckInTrim(void);
	BOOL CheckOutTrim(void);
	BOOL CheckMove(CPoint& point);

	BOOL IsPointInSeekBar(const CPoint& point);
	BOOL IsPointInTimelineControlPanel(const CPoint& point);
	//float SetShuttleSpeed(const CPoint& point, CSize& szMoveSize);
	//float SetShuttleSpeedByMoveLength(int iMoveLength);

	// �`��

	void DrawTimelineControlPanel(void);
	void DrawSeekBar(const CDC& dcPaintDC, const int iHeight);
	void DrawBigScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawMiddleScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawSmallScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawTrackHeader(void);
	void DrawTrack(void);
	BOOL DrawClip(const int iHeight);
	void DrawClipInTrack(TrackDataRect* pTrackDataRect, const int iHeight);
	void DrawTimelineDataRect(void);
	BOOL DrawOperatingClip(const CDC& dcPaintDC, const int iHeight);
	BOOL DrawTimelineCursor(const CDC& dcPaintDC, const int iHeight);
	//void DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcShuttleLineRect, float fGuideAreaWidth);
	//void DrawAnimation(const int iFrame);


public:

private:
	// �����ݒ�
	void InitAreaRect(void);
	void InitTestObject(void);
	void CreateZoomMap(void);

	// �\���\�ȃ^�C�����C���͈̔́i���[�A�E�[�j���v�Z����
	void CalcTimelineDisplayRange();
};
