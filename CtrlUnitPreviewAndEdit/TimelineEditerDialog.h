#pragma once

class TimelineEditerView;

// TimelineEditerDialog ダイアログ

class TimelineEditerDialog : public CDialogEx
{
	DECLARE_DYNAMIC(TimelineEditerDialog)

public:
	TimelineEditerDialog(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~TimelineEditerDialog();

// ダイアログ データ
	enum { IDD = IDD_TIMELINEEDITER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();

	void SetPosition(CRect& rcParentRect);
	virtual BOOL OnInitDialog();

private:
		TimelineEditerView* m_pEditerView;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
