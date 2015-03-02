// MemeoryDlg.h : 头文件
//

#pragma once
#include "tooldlg.h"
#include "atltypes.h"
#include "afxwin.h"

#define WM_SHOWTASK WM_USER+100
#define WM_MINDIALOG WM_USER+101

#define SystemBasicInformation 0 
#define SystemPerformanceInformation 2 
#define SystemTimeInformation 3

#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))
// CMemeoryDlg 对话框
typedef struct 
{ 
	DWORD dwUnknown1; 
	ULONG uKeMaximumIncrement; 
	ULONG uPageSize; 
	ULONG uMmNumberOfPhysicalPages; 
	ULONG uMmLowestPhysicalPage; 
	ULONG uMmHighestPhysicalPage; 
	ULONG uAllocationGranularity; 
	PVOID pLowestUserAddress; 
	PVOID pMmHighestUserAddress; 
	ULONG uKeActiveProcessors; 
	BYTE bKeNumberProcessors; 
	BYTE bUnknown2; 
	WORD wUnknown3; 
} SYSTEM_BASIC_INFORMATION;
typedef struct 
{ 
	LARGE_INTEGER liIdleTime; 
	DWORD dwSpare[76]; 
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct 
{ 
	LARGE_INTEGER liKeBootTime; 
	LARGE_INTEGER liKeSystemTime; 
	LARGE_INTEGER liExpTimeZoneBias; 
	ULONG uCurrentTimeZoneId; 
	DWORD dwReserved; 
} SYSTEM_TIME_INFORMATION;

class CMemeoryDlg : public CDialog
{
// 构造
public:
	CMemeoryDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MEMEORY_DIALOG };

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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	
	CRect m_ClientRc;
	CRect m_ToolRc;
public:
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMinDialog(WPARAM wParam, LPARAM lParam);
private:
	bool m_bMouseTrack;
	NOTIFYICONDATA m_nid;
public:
	afx_msg void OnDestroy();
	CToolDlg *m_pDlg;
private:
	int m_IDTimer;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	int m_Shift;
	int m_ProPos;
	CString m_str;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CString m_strT;
	CBrush m_Brush;
	COLORREF m_RGB;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool CPU_USE();
private:
	CFont m_Font;
	LARGE_INTEGER m_liOldIdleTime; 
	LARGE_INTEGER m_liOldSystemTime;
	CString m_strCPU;
public:
	bool CPUUse(void);
private:
	int m_nCPU;
public:
	bool MemoryUse(void);
	bool NotifyIcon(void);
private:
	int m_IDTimer2;
	long m_Speed;
public:
	afx_msg void On32774();
private:
	CPoint m_CursorPoint;
	CMenu *m_Popup;
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void On32771();
	afx_msg void OnLink();
private:
	CString m_url;
public:
	bool PopupMenu(CPoint);
private:
	int m_Judge;
};
