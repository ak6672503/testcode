﻿
// TestView.h: CTestView 类的接口
//
#include "Resource.h"
#pragma once


class CTestView : public CView
{
protected: // 仅从序列化创建
	CTestView() noexcept;
	DECLARE_DYNCREATE(CTestView)

// 特性
public:
	CTestDoc* GetDocument() const;

// 操作
public:

private:
	CBitmap bitmap;
	int m_nWidth;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
private:
	CString m_strLine;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	CPoint m_ptOrign; //保留鼠标左键单击的坐标位置，以便在打字的时候显示
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // TestView.cpp 中的调试版本
inline CTestDoc* CTestView::GetDocument() const
   { return reinterpret_cast<CTestDoc*>(m_pDocument); }
#endif

