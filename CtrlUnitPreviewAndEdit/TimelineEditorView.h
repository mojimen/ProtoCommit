#pragma once

#include <map>
#include "OpenGLView.h"
#include "ConstantDefine.h"
#include "TimelineDataManager.h"

class OpenGLRect;
class TimelineDataOperator;
class TimelineDataManager;
class TrackDataManager;
class TrackDataRect;
class ClipDataRect;
class TimelineEditorDialog;

class TimecodeOperator;

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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

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
	CWnd* m_pMainWnd;
	TimelineEditorDialog* m_pParentDialog;

	//�\�����
	OpenGLRect* m_pDebugInfoPanelRect;
	OpenGLRect* m_pTimelineEditPanelRect;
	OpenGLRect* m_pTimelineEditHeaderRect;
	OpenGLRect* m_pTimelineControlPanelRect;
	OpenGLRect* m_pSeekBarRect;
	OpenGLRect* m_pTrackHeaderRect;
	OpenGLRect* m_pTimelineDataRect;
	OpenGLRect* m_pTimelineCursorHitArea;
	OpenGLRect* m_pTransitionRect;

	int m_iTimelineCursorPoint;			// �^�C�����C���J�[�\���̕`��ʒu

	ClipDataPositionMap m_mpClipMap;

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

	TimecodeOperator* m_pTimecodeOperator;
	CString m_strTimecodeOperator;


	// �`��
	void DrawTimelineEditorView(const CDC& dcDc);
	void DrawTimelineControlPanel(void);
	void DrawSeekBar(const CDC& dcDC, const int& iHeight);
	void DrawBigScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine);
	void DrawMiddleScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine);
	void DrawSmallScale(const CDC& dcDC, const int& iDrawFrame, const int& iHeight, POINT& pScaleLine);
	void DrawTrackHeader(void);
	void DrawTrack(const CDC& dcDC, const int& iHeight);
	BOOL DrawClip(const CDC& dcDC, const int& iHeight);
	int DrawClipInTrack(const CDC& dcDC, TrackDataRect* pTrackDataRect, const int& iHeight, int iClipTotalCount = 0);
	void DrawTimelineDataRect(void);
	BOOL DrawOperatingClip(const CDC& dcDC, const int& iHeight);
	BOOL DrawDragAndDropClip(const CDC& dcDC, const int& iHeight);
	BOOL DrawTimelineCursor(const CDC& dcDC, const int& iHeight);
	void TimelineEditorView::DrawJumpFrame(int iMoveFrame, int iMoveDitance);
	//void DrawShuttleGuideLine(CDC& dcMemDc, CDC& dcMovingMemDc, BLENDFUNCTION& blAlphaBlend, CRect& rcShuttleLineRect, float fGuideAreaWidth);

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
	virtual void OnDragLeave(void);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	//virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnClipDelete(void);
	afx_msg void OnTransitionSetIn(void);
	afx_msg void OnTransitionSetOut(void);
	afx_msg void OnTransitionResetInCenter(void);
	afx_msg void OnTransitionResetInStart(void);
	afx_msg void OnTransitionResetInEnd(void);
	afx_msg void OnTransitionResetOutCenter(void);
	afx_msg void OnTransitionResetOutStart(void);
	afx_msg void OnTransitionResetOutEnd(void);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	void OnPlay(int iSpeed);
	void OnStop(void);
	int GetTimelineCursorFrame(void);

	void SetMainWnd(CWnd* pWnd) { m_pMainWnd = pWnd; }
	void SetParentDialog(TimelineEditorDialog* pDlg) { m_pParentDialog = pDlg; }

	afx_msg LRESULT OnNcHitTest(CPoint point);
};
