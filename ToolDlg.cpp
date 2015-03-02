// ToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Memeory.h"
#include "ToolDlg.h"


// CToolDlg 对话框

IMPLEMENT_DYNAMIC(CToolDlg, CDialog)

CToolDlg::CToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolDlg::IDD, pParent)
{

}

CToolDlg::~CToolDlg()
{
}

void CToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Pro);
	DDX_Control(pDX, IDC_STATIC2, m_MemoryUse);
	DDX_Control(pDX, IDC_CPU, m_CPU);
	DDX_Control(pDX, IDC_PROGRESS2, m_ProCPU);
}


BEGIN_MESSAGE_MAP(CToolDlg, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS2, &CToolDlg::OnNMCustomdrawProgress2)
END_MESSAGE_MAP()


// CToolDlg 消息处理程序
BOOL CToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_BkColor=RGB(0,0,0);
	m_Brush.CreateSolidBrush(m_BkColor);
    

	return TRUE; 
}

void CToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	

	CDialog::OnMouseMove(nFlags, point);
}

int CToolDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,WS_EX_LAYERED);//设置扩展形式
	if(!::SetLayeredWindowAttributes(this->m_hWnd,0,(255*100)/55,LWA_ALPHA))//透明背景
		     MessageBox(_T("背景透明失败"));

	return 0;
}

HBRUSH CToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(pWnd->GetDlgCtrlID()==IDC_STATIC2)//设置文本透明
    {   
         pDC->SetBkMode(TRANSPARENT); 
		 pDC->SetTextColor(RGB(255,255,255));
 
//		 return   HBRUSH(GetStockObject(HOLLOW_BRUSH));;
		 return m_Brush;
    }   
	
	if(pWnd->GetDlgCtrlID()==IDC_CPU)//设置文本透明
    {   
         pDC->SetBkMode(TRANSPARENT); 
		 pDC->SetTextColor(RGB(255,255,255));
//		 return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
		 return m_Brush;
    }  
	if(pWnd->GetDlgCtrlID()==IDC_PROGRESS1)
	{
		m_Pro.SetBarColor(RGB(0,255,0));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	if(pWnd->GetDlgCtrlID()==IDC_PROGRESS2)
	{
		m_Pro.SetBarColor(RGB(0,255,0));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}

	// TODO:  在此更改 DC 的任何属性


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_Brush;
}

BOOL CToolDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pDC->SelectObject(&m_Brush);
	pDC->SetBkColor(m_BkColor);

	return CDialog::OnEraseBkgnd(pDC);
}

void CToolDlg::OnNMCustomdrawProgress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
