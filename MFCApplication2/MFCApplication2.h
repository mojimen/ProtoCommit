
// MFCApplication2.h : MFCApplication2 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CMFCApplication2App:
// ���̃N���X�̎����ɂ��ẮAMFCApplication2.cpp ���Q�Ƃ��Ă��������B
//

class CMFCApplication2App : public CWinApp
{
public:
	CMFCApplication2App();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication2App theApp;
