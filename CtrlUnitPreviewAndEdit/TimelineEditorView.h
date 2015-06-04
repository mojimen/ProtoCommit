#pragma once

#include <map>
#include "OpenGLView.h"
#include "ConstantDefine.h"

class OpenGLRect;
class TimelineDataOperator;
class TimelineDataManager;
class TrackDataManager;
class TrackDataRect;
class ClipDataRect;

typedef std::map<int, int> DisplayScaleMap;

// TimelineEditorView �r���[

class TimelineEditorView : public OpenGLView
{
	DECLARE_DYNCREATE(TimelineEditorView)

protected:
	TimelineEditorView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~TimelineEditorView();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnDropFiles(HDROP hDropInfo);

private:
	// �b��Œ�l
	const int kSplitterWidth = 5;					// �X�v���b�^�̕�
	const int kSplitterHeight = 5;					// �X�v���b�^�̕�
	const int kDebugInfoPanelDefaltHeight = 300;		// ���g�p�ɂȂ�
	const int kDebugInfoPanelMinHeight = 100;			// ���g�p�ɂȂ�

	const int kTimelineEditDefaltHeight = 300;		// �^�C�����C���f�[�^�\���p�l���̏�������
	const int kTimelineEditPanelMinHeight = 100;	// �^�C�����C���f�[�^�\���p�l���̍ŏ�����
	const int kTimelineEditHeaderDefaltHeight = 30;	// �^�C�����C���f�[�^�\���p�l���w�b�_�[�s�̏�������
	const int kTimelineControlPanelDefaultWidth = 300;	// �^�C�����C���f�[�^�\���p�l�����o���̏�����

	const int kTimelineCursorDragArea = 5;					// �^�C�����C���J�[�\���h���b�O���̃q�b�g�̈�i���E�ɂ��̒l���v���X���Ĕ���j
	const int kTimelineCursorDragGuideLineThickness = 1;	// �^�C�����C���J�[�\���h���b�O���̃K�C�h���C���̑���
	//const float kTimelineCursorDragOneSpeed = 0.1f;			// �^�C�����C���J�[�\���h���b�O���́~�P�{�ړ��͈�
	//const float kTimelineCursorDragTowSpeed = 0.2f;			// �^�C�����C���J�[�\���h���b�O���́~�Q�{�ړ��͈�
	//const float kTimelineCursorDragFourSpeed = 0.28f;		// �^�C�����C���J�[�\���h���b�O���́~�S�{�ړ��͈�
	//const float kTimelineCursorDragEightSpeed = 0.36f;		// �^�C�����C���J�[�\���h���b�O���́~�W�{�ړ��͈�
	//const float kTimelineCursorDragSixteenSpeed = 0.42f;	// �^�C�����C���J�[�\���h���b�O���́~�P�U�{�ړ��͈�
	//const float kTimelineCursorDragThirtyTwoSpeed = 0.47f;	// �^�C�����C���J�[�\���h���b�O���́~�R�Q�{�ړ��͈�
	//const float kTimelineCursorDragSixtyFourSpeed = 0.5f;	// �^�C�����C���J�[�\���h���b�O���́~�U�S�{�ړ��͈�

	BOOL m_fInitialize;

	//�\�����
	OpenGLRect* m_pDebugInfoPanelRect;
	OpenGLRect* m_pTimelineEditPanelRect;
	OpenGLRect* m_pTimelineEditHeaderRect;
	OpenGLRect* m_pTimelineControlPanelRect;
	OpenGLRect* m_pSeekBarRect;
	OpenGLRect* m_pTrackHeaderRect;
	OpenGLRect* m_pTimelineDataRect;
	OpenGLRect* m_pTimelineCursorHitArea;
	OpenGLRect* m_pTransisionRect;

	int m_iTimelineCursorPoint;			// �^�C�����C���J�[�\���̕`��ʒu

	// ���쒆�N���b�v�ւ̃|�C���^
	ClipDataRect* m_pOperatingClipData;

	// �^�C�����C���f�[�^�Ǘ�
	TimelineDataOperator* m_pTimelineDataOperator;
	UUID m_uiTimelineDataOperatorId;
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

	// �`��
	void DrawTimelineEditorView(CPaintDC& dcPaintDc);
	void DrawTimelineControlPanel(void);
	void DrawSeekBar(const CDC& dcPaintDC, const int iHeight);
	void DrawBigScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawMiddleScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawSmallScale(const CDC& dcPaintDC, const int iDrawFrame, const int iHeight, POINT& pScaleLine);
	void DrawTrackHeader(void);
	void DrawTrack(const int iHeight, const CPaintDC& dcPaintDC);
	BOOL DrawClip(const int iHeight, CPaintDC& dcPaintDC);
	int DrawClipInTrack(TrackDataRect* pTrackDataRect, const int iHeight, CPaintDC& dcPaintDC, int iClipTotalCount = 0);
	void DrawTimelineDataRect(void);
	BOOL DrawOperatingClip(const CDC& dcPaintDC, const int iHeight);
	BOOL DrawDragAndDropClip(const CDC& dcPaintDC, const int iHeight);
	BOOL DrawTimelineCursor(const CDC& dcPaintDC, const int iHeight);
	//void DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcShuttleLineRect, float fGuideAreaWidth);
	//void DrawAnimation(const int iFrame);

	// �\���ʒu�v�Z
	void SetPanelRect(void);
	// �����ݒ�
	void InitAreaRect(void);
	void InitTestObject(void);
	void CreateZoomMap(void);

	// �h���b�O���h���b�v�p
	COleDropTarget m_DropTarget;

public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	//virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
};
