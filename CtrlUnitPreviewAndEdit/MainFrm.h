
// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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


