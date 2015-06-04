#pragma once

class TimelineEditorView;

// TimelineEditorDialog ダイアログ

class TimelineEditorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(TimelineEditorDialog)

public:
	TimelineEditorDialog(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~TimelineEditorDialog();

// ダイアログ データ
	enum { IDD = IDD_TIMELINEEditor_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();

	void SetPosition(CRect& rcParentRect);
	virtual BOOL OnInitDialog();

private:
		TimelineEditorView* m_pEditorView;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
