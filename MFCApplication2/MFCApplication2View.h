
// MFCApplication2View.h : CMFCApplication2View クラスのインターフェイス
//

#pragma once


class CMFCApplication2View : public CView
{
protected: // シリアル化からのみ作成します。
	CMFCApplication2View();
	DECLARE_DYNCREATE(CMFCApplication2View)

// 属性
public:
	CMFCApplication2Doc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CMFCApplication2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFCApplication2View.cpp のデバッグ バージョン
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

