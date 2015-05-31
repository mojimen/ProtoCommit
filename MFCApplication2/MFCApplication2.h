
// MFCApplication2.h : MFCApplication2 アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CMFCApplication2App:
// このクラスの実装については、MFCApplication2.cpp を参照してください。
//

class CMFCApplication2App : public CWinApp
{
public:
	CMFCApplication2App();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication2App theApp;
