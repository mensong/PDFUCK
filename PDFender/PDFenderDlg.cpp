
// PDFenderDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PDFender.h"
#include "PDFenderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPDFenderDlg 对话框



CPDFenderDlg::CPDFenderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PDFENDER_DIALOG, pParent)
	, m_doc(NULL)
	, m_curPage(NULL)
	, m_curPageIndex(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPDFenderDlg::switchToPage(int pageIndex)
{
	if (!m_doc)
		return;
	if (pageIndex == m_curPageIndex || pageIndex < 0 || 
		pageIndex >= m_doc->CountPages())
		return;

	if (m_curPage)
		m_doc->ClosePage(&m_curPage);

	m_curPage = m_doc->OpenPage(pageIndex);
	m_curPageIndex = pageIndex;
	m_renderStatic.SetPage(m_curPage);
	m_renderStatic.Invalidate();

	m_cmbPageIndexs.SetCurSel(pageIndex);
}

void CPDFenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RENDER, m_renderStatic);
	DDX_Control(pDX, IDC_CMB_PAGES, m_cmbPageIndexs);
	DDX_Control(pDX, IDC_CHK_Transparency_CANVAS, m_chkTransparencyCanvas);
	DDX_Control(pDX, IDC_CHK_DOUBLEBUFFER, m_chkDoubleBuffer);
}

BEGIN_MESSAGE_MAP(CPDFenderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CPDFenderDlg::OnBnClickedBtnOpen)
	ON_CBN_SELCHANGE(IDC_CMB_PAGES, &CPDFenderDlg::OnCbnSelchangeCmbPages)
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CPDFenderDlg::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CPDFenderDlg::OnBnClickedBtnNextPage)
	ON_BN_CLICKED(IDC_CHK_Transparency_CANVAS, &CPDFenderDlg::OnBnClickedChkTransparencyCanvas)
	ON_BN_CLICKED(IDC_CHK_DOUBLEBUFFER, &CPDFenderDlg::OnBnClickedChkDoublebuffer)
END_MESSAGE_MAP()


// CPDFenderDlg 消息处理程序

BOOL CPDFenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_scale.AddExclude(GetDlgItem(IDC_BTN_OPEN)->m_hWnd);
	m_scale.AddExclude(m_cmbPageIndexs.m_hWnd);
	m_scale.AddExclude(GetDlgItem(IDC_STATIC_PAGE)->m_hWnd);
	m_scale.AddExclude(GetDlgItem(IDC_BTN_PRE_PAGE)->m_hWnd);
	m_scale.AddExclude(GetDlgItem(IDC_BTN_NEXT_PAGE)->m_hWnd);
	m_scale.AddExclude(m_chkTransparencyCanvas.m_hWnd);
	m_scale.AddExclude(m_chkDoubleBuffer.m_hWnd);
	m_scale.SetAnchor(m_renderStatic.m_hWnd, 
		CCtrlScale::AnchorLeftToWinLeft |
		CCtrlScale::AnchorRightToWinRight |
		CCtrlScale::AnchorTopToWinTop |
		CCtrlScale::AnchorBottomToWinBottom
	);
	m_scale.Init(m_hWnd);

	m_chkDoubleBuffer.SetCheck(BST_CHECKED);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPDFenderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPDFenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool OpenFileDialog(TCHAR* filePath, int size, HWND hParent = NULL) 
{
	OPENFILENAME ofn;       // 公共对话框结构
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hParent;   // 父窗口句柄，可以为NULL
	ofn.lpstrFilter = _T("All Files\0*.*\0PDF Files\0*.pdf\0"); // 文件过滤器
	ofn.lpstrInitialDir = _T("C:\\"); // 初始目录
	ofn.lpstrFile = filePath;  // 保存选择的文件路径
	ofn.nMaxFile = size;   // 文件路径最大长度
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER; // 标志

	return GetOpenFileName(&ofn) == TRUE; // 显示对话框并返回是否成功
}

void CPDFenderDlg::OnBnClickedBtnOpen()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	if (!OpenFileDialog(szBuffer, MAX_PATH, m_hWnd))
		return;

	std::string pdfFile = (LPSTR)CW2A(szBuffer);

	m_cmbPageIndexs.ResetContent();
	if (m_doc)
	{
		if (m_curPage)
			m_doc->ClosePage(&m_curPage);
		m_curPageIndex = -1;
		PDFuck::Ins().CloseDocument(&m_doc);
	}
	
	m_doc = PDFuck::Ins().LoadDocumentFromFile(pdfFile.c_str(), NULL);
	if (!m_doc)
		return;
		
	int countPages = m_doc->CountPages();
	for (int i = 0; i < countPages; i++)
	{
		m_cmbPageIndexs.AddString(std::to_wstring(i + 1).c_str());
	}

	if (countPages > 0)
	{
		switchToPage(0);
	}
}


void CPDFenderDlg::OnCbnSelchangeCmbPages()
{
	CString sPage;
	m_cmbPageIndexs.GetWindowText(sPage);
	
	int pageIndex = _wtoi(sPage.GetString());
	if (pageIndex > 0)
	{
		switchToPage(pageIndex - 1);
	}
}


void CPDFenderDlg::OnBnClickedBtnPrePage()
{
	switchToPage(m_curPageIndex - 1);
}


void CPDFenderDlg::OnBnClickedBtnNextPage()
{
	switchToPage(m_curPageIndex + 1);
}


void CPDFenderDlg::OnBnClickedChkTransparencyCanvas()
{
	m_renderStatic.SetDrawPageBackgroup(
		m_chkTransparencyCanvas.GetCheck() != BST_CHECKED);
	m_renderStatic.Invalidate();
}


void CPDFenderDlg::OnBnClickedChkDoublebuffer()
{
	m_renderStatic.SetEnableDoubleBuffer(
		m_chkDoubleBuffer.GetCheck() == BST_CHECKED);
	m_renderStatic.Invalidate();
}
