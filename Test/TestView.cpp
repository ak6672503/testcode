
// TestView.cpp: CTestView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Test.h"
#endif

#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//纯粹测试一下
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTestView 构造/析构

CTestView::CTestView() noexcept
{
	m_strLine = "";
	m_ptOrign = 0;
	m_nWidth = 0;
}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestView 绘图

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CString str;
	str = _T("VC++ 深入编程");
	pDC->TextOut(50, 50, str);

	CSize sz = pDC->GetTextExtent(str);
	str.LoadString(IDS_STRING101);
	pDC->TextOut(0, 200, str);

	pDC->BeginPath();
	pDC->Rectangle(50, 50, 50 + sz.cx, 50 + sz.cy);
	pDC->EndPath();
	pDC->SelectClipPath(RGN_AND);

	for (int i = 0; i < 300; i += 10) {
		pDC->MoveTo(0,i);
		pDC->LineTo(300,i);
		pDC->MoveTo( i,0);
		pDC->LineTo( i,300);
	}

}


// CTestView 打印

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTestView 诊断

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestView 消息处理程序


void CTestView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CClientDC dc(this); //创建设备描述表
	TEXTMETRIC tm; //定义文本信息结构体变量
	dc.GetTextMetrics(&tm); //获取设备描述表中的文本信息
	CFont font;
	font.CreatePointFont(300, _T("楷体"), NULL);
	CFont* pOldFont = dc.SelectObject(&font);
	

	if (0x0d == nChar) {
		m_strLine.Empty();
		m_ptOrign.y += tm.tmHeight;
	}
	else if (0x08 == nChar) {
		COLORREF clr = dc.SetTextColor(dc.GetBkColor());
		dc.TextOut(m_ptOrign.x, m_ptOrign.y, m_strLine);

		m_strLine = m_strLine.Left(m_strLine.GetLength() - 1);

		dc.SetTextColor(clr);
	}
	else {
		m_strLine += (TCHAR)nChar;

	}
	CSize sz = dc.GetTextExtent(m_strLine);
	CPoint pt;
	pt.x = m_ptOrign.x + sz.cx;
	pt.y = m_ptOrign.y;
	SetCaretPos(pt);
	dc.TextOut(m_ptOrign.x, m_ptOrign.y, m_strLine);
	dc.SelectObject(pOldFont);


	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCaretPos(point);
	m_strLine.Empty();
	m_ptOrign = point;


	CView::OnLButtonDown(nFlags, point);
}


int CTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	bitmap.LoadBitmapW(IDB_BITMAP1);
	CreateCaret(&bitmap);
	ShowCaret();

	SetTimer(1, 100, NULL);
	return 0;
}


void CTestView::OnTimer(UINT_PTR nIDEvent)
{

	m_nWidth += 5;
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	CRect rect;
	rect.left = 0;
	rect.top = 200;
	rect.right = m_nWidth;
	rect.bottom = rect.top + tm.tmHeight;
	dc.SetTextColor(RGB(255, 0, 0));
	CString str;
	str.LoadStringW(IDS_STRING101);
	dc.DrawText(str, rect, DT_LEFT);


	rect.top = 150;

	rect.bottom = rect.top + tm.tmHeight;

	dc.DrawText(str, rect, DT_RIGHT);



	CSize sz = dc.GetTextExtent(str);
	if (m_nWidth > sz.cx) {
		m_nWidth = 0;
		dc.SetTextColor(RGB(0, 255, 0));
		dc.TextOutW(0, 200, str);
	}

	CView::OnTimer(nIDEvent);
}
