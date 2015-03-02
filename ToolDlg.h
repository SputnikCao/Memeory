#pragma once
#include "afxcmn.h"
#include "afxwin.h"

//#include "MemeoryDlg.h"
// CToolDlg 对话框

class CToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolDlg)

public:
	CToolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CProgressCtrl m_Pro;
	CStatic m_MemoryUse;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	CBrush m_Brush;
	COLORREF m_BkColor;
public:
	CStatic m_CPU;
	CProgressCtrl m_ProCPU;
	afx_msg void OnNMCustomdrawProgress2(NMHDR *pNMHDR, LRESULT *pResult);
};
