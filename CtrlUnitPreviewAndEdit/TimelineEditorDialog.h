#pragma once

class TimelineEditorView;
class CCtrlUnitPreviewView;

// TimelineEditorDialog �_�C�A���O

class TimelineEditorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(TimelineEditorDialog)

public:
	TimelineEditorDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~TimelineEditorDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TIMELINEEditor_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();

	void SetPosition(CRect& rcParentRect);
	virtual BOOL OnInitDialog();

private:
		TimelineEditorView* m_pEditorView;
		CMDIFrameWnd* m_pMainWnd;
		CCtrlUnitPreviewView* m_pMainView;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void OnPlay(int iSpeed = 1 );
	void OnStop(void);
	int GetTimelineCursorFrame(void);
	CWnd* GetMainWnd(void) { return m_pParentWnd; }
	void SetMainView(CCtrlUnitPreviewView* pView) { m_pMainView = pView; }
	void SetTimelineCursorFrame(int iFrame);
	void ChangePlay(void);

	virtual void OnCancel();
	virtual void OnOK();
};
