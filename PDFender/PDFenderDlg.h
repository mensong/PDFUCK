
// PDFenderDlg.h: 头文件
//

#pragma once
#include "../PDFUCK/PDFuck.h"

// CPDFenderDlg 对话框
class CPDFenderDlg : public CDialogEx
{
// 构造
public:
	CPDFenderDlg(CWnd* pParent = nullptr);	// 标准构造函数

	enum { IDD = IDD_PDFENDER_DIALOG };

	PDF_DOCUMENT* m_doc;
	PDF_PAGE* m_curPage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	CStatic m_renderStatic;
};
