// ToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Memeory.h"
#include "ToolDlg.h"


// CToolDlg �Ի���

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


// CToolDlg ��Ϣ�������
BOOL CToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_BkColor=RGB(0,0,0);
	m_Brush.CreateSolidBrush(m_BkColor);
    

	return TRUE; 
}

void CToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	

	CDialog::OnMouseMove(nFlags, point);
}

int CToolDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,WS_EX_LAYERED);//������չ��ʽ
	if(!::SetLayeredWindowAttributes(this->m_hWnd,0,(255*100)/55,LWA_ALPHA))//͸������
		     MessageBox(_T("����͸��ʧ��"));

	return 0;
}

HBRUSH CToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(pWnd->GetDlgCtrlID()==IDC_STATIC2)//�����ı�͸��
    {   
         pDC->SetBkMode(TRANSPARENT); 
		 pDC->SetTextColor(RGB(255,255,255));
 
//		 return   HBRUSH(GetStockObject(HOLLOW_BRUSH));;
		 return m_Brush;
    }   
	
	if(pWnd->GetDlgCtrlID()==IDC_CPU)//�����ı�͸��
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

	// TODO:  �ڴ˸��� DC ���κ�����


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return m_Brush;
}

BOOL CToolDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	pDC->SelectObject(&m_Brush);
	pDC->SetBkColor(m_BkColor);

	return CDialog::OnEraseBkgnd(pDC);
}

void CToolDlg::OnNMCustomdrawProgress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
