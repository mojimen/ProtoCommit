
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFullScreen();

private:
	DWORD m_dwStyle;
	DWORD m_dwExStyle;
	CRect m_rcWindowSize;
	BOOL m_fFullScreen;

public:
	BOOL GetScreenMode(void){ return m_fFullScreen; }

};


