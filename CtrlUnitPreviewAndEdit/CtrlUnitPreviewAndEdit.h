
// CtrlUnitPreviewAndEdit.h : CtrlUnitPreviewAndEdit アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CCtrlUnitPreviewAndEditApp:
// このクラスの実装については、CtrlUnitPreviewAndEdit.cpp を参照してください。
//

class CCtrlUnitPreviewAndEditApp : public CWinApp
{
public:
	CCtrlUnitPreviewAndEditApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCtrlUnitPreviewAndEditApp theApp;
