// MemeoryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Memeory.h"
#include "MemeoryDlg.h"
#include "Winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMemeoryDlg �Ի���


typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);

PROCNTQSI NtQuerySystemInformation;



CMemeoryDlg::CMemeoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemeoryDlg::IDD, pParent)
	, m_bMouseTrack(true)
	, m_IDTimer(1)
	, m_Shift(1024*1024)
	, m_ProPos(0)
	, m_str(_T(""))
	, m_strT(_T(""))
	, m_strCPU(_T(""))
	, m_nCPU(0)
	, m_IDTimer2(2)
	, m_Speed(3000)
	, m_CursorPoint(0)
	, m_url(_T(""))
	, m_Judge(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	
}

void CMemeoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMemeoryDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_MOUSELEAVE()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)//�Զ�����Ϣ
	ON_MESSAGE(WM_MINDIALOG,OnMinDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_32774, &CMemeoryDlg::On32774)
	
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_32771, &CMemeoryDlg::On32771)
	ON_COMMAND(ID_32772, &CMemeoryDlg::OnLink)
END_MESSAGE_MAP()


// CMemeoryDlg ��Ϣ�������

BOOL CMemeoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//---------------------------��Ҫ��Ϣ��ʼ��---------------------------------//
	m_pDlg=new CToolDlg;
	m_pDlg->Create(IDD_DIALOG1);
	m_RGB=RGB(5,5,5);
	m_Brush.CreateSolidBrush(m_RGB);
	::GetWindowRect(this->m_hWnd,&m_ClientRc);
	::GetWindowRect(m_pDlg->m_hWnd,&m_ToolRc);
	m_pDlg->m_Pro.SetRange(0,100);
	m_url=_T("http://hi.csdn.net/space.php?do=home");

	//---------------------------������ʾ��ʼ��---------------------------------//

   if(!NotifyIcon())
	  MessageBox(_T("������ʾ��ʼ��ʧ��"));
	
//-----------------------------------��ʱ���趨------------------------------------------------------------------//

	
	SetTimer(m_IDTimer,1000,0);
	SetTimer(m_IDTimer2,m_Speed,0);
	
	
//----------------------------------------�ڴ���Ϣ��ȡ-------------------------------------------------------------//

	if(!MemoryUse())
		MessageBox(_T("�ڴ���Ϣ��ȡʧ��"));

//----------------------------------------������Ϣ�趨-------------------------------------------------------------//
	m_pDlg->m_Pro.SetPos(m_ProPos);
	m_pDlg->m_MemoryUse.SetWindowTextW(m_str);
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_str);

	if(!CPU_USE())
		MessageBox(_T("��ȡCPU��Ϣʧ��"));
	if(!CPU_USE())
		MessageBox(_T("��ȡCPU��Ϣʧ��"));
	else
	{
		m_pDlg->m_ProCPU.SetPos(m_nCPU);
		m_pDlg->m_CPU.SetWindowTextW(m_strCPU);	    
	}
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_strCPU);
	
//----------------------------------------������Ϣ�趨-------------------------------------------------------------//

	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,WS_EX_LAYERED);//������չ��ʽ
	if(!::SetLayeredWindowAttributes(this->m_hWnd,0,(255*80)/55,LWA_ALPHA))//͸������
		     MessageBox(_T("����͸��ʧ��"));	
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//������������
	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,m_ClientRc.left,m_ClientRc.top,m_ClientRc.Width(),m_ClientRc.Height(),SWP_NOMOVE);// �ö���ʾ
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMemeoryDlg::OnPaint()//����
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMemeoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMemeoryDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::GetWindowRect(this->m_hWnd,&m_ClientRc);
	::GetWindowRect(m_pDlg->m_hWnd,&m_ToolRc);
	int ScreenHeight=GetSystemMetrics(SM_CYSCREEN);
	
	if(m_ClientRc.top<ScreenHeight/2)
	{
	    m_pDlg->MoveWindow(m_ClientRc.left,m_ClientRc.top+23,m_ToolRc.Width(),m_ToolRc.Height());
		m_Judge=1;//�������ϰ���
	}
	else
	{
		m_pDlg->MoveWindow(m_ClientRc.left,m_ClientRc.top-m_ToolRc.Height()-2,m_ToolRc.Width(),m_ToolRc.Height());
		m_Judge=2;//�������°���
	}
	m_pDlg->ShowWindow(SW_SHOW);

	if (m_bMouseTrack)    //������׷�٣���
    {
       TRACKMOUSEEVENT csTME;
       csTME.cbSize = sizeof(csTME);
       csTME.dwFlags = TME_LEAVE;
	   csTME.hwndTrack = this->m_hWnd;//ָ��Ҫ׷�ٵĴ���
       csTME.dwHoverTime = 100000;  //����ڰ�ť��ͣ������100000ms������Ϊ״̬ΪHOVER
       ::_TrackMouseEvent(&csTME); //����Windows��WM_MOUSELEAVE��WM_MOUSEHOVER�¼�֧��
       m_bMouseTrack=FALSE;   //���Ѿ�׷�٣���ֹͣ׷��

	}

	CDialog::OnMouseMove(nFlags, point);
}

void CMemeoryDlg::OnNcMouseMove(UINT nHitTest, CPoint point)//����
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

//	m_pDlg->ShowWindow(SW_HIDE);
	CDialog::OnNcMouseMove(nHitTest, point);
}

LRESULT CMemeoryDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//--------------------------ģ����������Ϣ-------------------------------
//	LRESULT nHitTest = CDialog::OnNcHitTest(point);    
//	if (((nHitTest == HTCLIENT) | (nHitTest == HTTRANSPARENT))&& GetAsyncKeyState(MK_LBUTTON) < 0 )  //����ǿͻ���//������������£�GetAsyncKeyState�����ķ���ֵС��0    
//        nHitTest = HTCAPTION;                       //��������ɱ�����   
//	return nHitTest; 
	return CDialog::OnNcHitTest(point);
}

void CMemeoryDlg::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

   m_pDlg->ShowWindow(SW_HIDE);
   m_bMouseTrack=true;
   

	CDialog::OnMouseLeave();
}

LRESULT CMemeoryDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
    if(wParam != IDI_ICON1) 
          return 1;
    switch(lParam) 
    { 
    case WM_RBUTTONUP:                                   // �Ҽ�����ʱ�����˵�
    {     
       ::GetCursorPos(&m_CursorPoint);                            // �õ����λ��
	   CMenu menu;
	   menu.LoadMenuW(IDR_MENU1);
	   m_Popup=menu.GetSubMenu(0);
	   m_Popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,m_CursorPoint.x,m_CursorPoint.y,this);
	   HMENU hmenu = menu.Detach();                       //��Դ����
       menu.DestroyMenu();
    } 
    break; 
    case WM_LBUTTONDBLCLK:                                 // ˫������Ĵ���
    { 
       this->ShowWindow(SW_SHOWNORMAL);                     // ��ʾ������
       this->SetForegroundWindow(); 
	   ::SetWindowPos(this->m_hWnd,HWND_TOPMOST,m_ClientRc.left,m_ClientRc.top,m_ClientRc.Width(),m_ClientRc.Height(),SWP_NOMOVE);// �ö���ʾ

    }
    break;
    }
    return 0;
}
void CMemeoryDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	Shell_NotifyIcon(NIM_DELETE, &m_nid);//���ϵͳ����ͼ��
	m_pDlg->DestroyWindow();
}

void CMemeoryDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	static int t_Mem=0;
	static int t_CPU=0;
	static int i=1;
	if(nIDEvent==1)
	{    
		   if(!MemoryUse())
	           MessageBox(_T("��ȡ�ڴ���Ϣʧ��"));
		   else
		   {
                if(t_Mem!=m_ProPos)
		       {
			        if(i==2)
				        GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_str);
			        m_pDlg->m_Pro.SetPos(m_ProPos); 
	                m_pDlg->m_MemoryUse.SetWindowTextW(m_str);		       
		            t_Mem=m_ProPos;
		       }
		   }
		   
	 
	   if(!CPU_USE())
		   MessageBox(_T("CPU��Ϣ��ȡʧ��"));
	   else
	   {
		   if(m_nCPU!=t_CPU)
		   {
			  if(i==1)
			       GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_strCPU);
		      m_pDlg->m_CPU.SetWindowTextW(m_strCPU);
		      m_pDlg->m_ProCPU.SetPos(m_nCPU);
			  t_CPU=m_nCPU;
		   }
	   }
	   
	}
	if(nIDEvent==2)
	{
		if(i==1)
			i=2;
		else
			i=1;
		if(i==2)
		{
			GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_str);
            			
		}
		if(i==1)
		{
            GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_strCPU);
		}
        
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CMemeoryDlg::OnEraseBkgnd(CDC* pDC)
{
	pDC->SelectObject(m_Brush);
	pDC->SetBkColor(m_RGB);
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CMemeoryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_STATIC1)//�����ı�͸��
    {   
         pDC->SetBkMode(TRANSPARENT); 
		 pDC->SetTextColor(RGB(255,255,255));
  //       GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_str);
		 return   m_Brush;
    }   
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return m_Brush;
}




void CMemeoryDlg::OnLButtonDown(UINT nFlags, CPoint point)//ʵ�ֵ���ͻ���ģ����������
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnLButtonDown (nFlags, point );

	//Fool dialog into thinking simeone clicked on its caption bar .
    PostMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARAM(point.x ,point. y ));


	CDialog::OnLButtonDown(nFlags, point);
}

bool CMemeoryDlg::CPU_USE()//CPU��Ϣ
{
	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo; 
	SYSTEM_TIME_INFORMATION SysTimeInfo; 
	SYSTEM_BASIC_INFORMATION SysBaseInfo; 
	double dbIdleTime; 
	double dbSystemTime; 
	LONG status; 
	static LARGE_INTEGER liOldIdleTime = {0,0}; 
	static LARGE_INTEGER liOldSystemTime = {0,0};
	
	NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle(_T("ntdll")),(LPCSTR)"NtQuerySystemInformation");
	
	if (!NtQuerySystemInformation) 
		return false;
	
	// get number of processors in the system 
	status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
	if (status != NO_ERROR) 
		return false;
	
	// get new system time 
    status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0); 
	if (status!=NO_ERROR) 
		return false;
		
	// get new CPU's idle time 
	status =NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if (status != NO_ERROR) 
		return false;
		
	// if it's a first call - skip it 
	if (liOldIdleTime.QuadPart != 0) 
	{ 
		// CurrentValue = NewValue - OldValue 
		dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime); 
		dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) -Li2Double(liOldSystemTime);
			
		// CurrentCpuIdle = IdleTime / SystemTime 
		dbIdleTime = dbIdleTime / dbSystemTime;
			
		// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors 
		dbIdleTime = 100.0 - dbIdleTime * 100.0 /
				
		(double)SysBaseInfo.bKeNumberProcessors + 0.5;
			
		m_strCPU.Format(_T("CPUʹ����: %d %%"),(UINT)dbIdleTime);
		m_nCPU=(UINT)dbIdleTime;
		
	}
		
		// store new CPU's idle and system time 
		liOldIdleTime= SysPerfInfo.liIdleTime; 
		liOldSystemTime= SysTimeInfo.liKeSystemTime;
		
	return true;
}


bool CMemeoryDlg::MemoryUse(void)//�ڴ���Ϣ
{
	//��ʼ���ڴ���Ϣ�ṹ��
	MEMORYSTATUS Memory_Info;
	Memory_Info.dwLength=sizeof(MEMORYSTATUS);
	DWORD MemTotal,MemFree,VMemTotal,VMemFree;//�洢�ڴ�״̬��Ϣ
	
	
	GlobalMemoryStatus(&Memory_Info); //�洢�ڴ�״̬��Ϣ
	MemTotal=Memory_Info.dwTotalPhys/m_Shift;//�õ����ڴ��С����λb��
	MemFree=Memory_Info.dwAvailPhys/m_Shift;//�õ�ʣ���ڴ��С����λb��
	VMemTotal=Memory_Info.dwTotalVirtual/m_Shift;//�õ��������ڴ��С����λb��
	VMemFree=Memory_Info.dwAvailVirtual/m_Shift;//�õ�ʣ�������ڴ��ڴ��С����λb
	
	m_ProPos=(double)(MemTotal-MemFree)*100/MemTotal;
	m_str.Format(_T("�ڴ�ʹ��: %d %%"),m_ProPos);
	
    
	return true;
}

bool CMemeoryDlg::NotifyIcon(void)//ϵͳ����
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd    = this->m_hWnd;
	m_nid.uID     = IDI_ICON1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	m_nid.uCallbackMessage = WM_SHOWTASK;             // �Զ������Ϣ����
	m_nid.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1)); //IDR_MAINFRAME�ɸ�Ϊ�Լ���������Դ�� 
    wcscpy(m_nid.szTip,_T("�ڴ������"));              // ��Ϣ��ʾ��Ϊ"�������ӳ���"
    if(Shell_NotifyIcon(NIM_ADD, &m_nid))// �����������ͼ��
		return true;
	else
		return false;
	
}

void CMemeoryDlg::On32774()
{
	// TODO: �ڴ���������������
	this->ShowWindow(SW_HIDE);
}

LRESULT CMemeoryDlg::OnMinDialog(WPARAM wParam, LPARAM lParam)
{
	this->ShowWindow(SW_HIDE);
	return 0;
}
void CMemeoryDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    
	PopupMenu(point);//�����˵�
	 
	 int ScreenHeight=GetSystemMetrics(SM_CYSCREEN);
	
	  if(m_ClientRc.top<ScreenHeight/2)
	    m_pDlg->MoveWindow(m_ClientRc.left,m_ClientRc.top+23,m_ToolRc.Width(),m_ToolRc.Height());
	  else
		m_pDlg->MoveWindow(m_ClientRc.left,m_ClientRc.top-m_ToolRc.Height()-2,m_ToolRc.Width(),m_ToolRc.Height());
	m_pDlg->ShowWindow(SW_SHOW);
	CDialog::OnRButtonDown(nFlags, point);
}

void CMemeoryDlg::On32771()
{
	// TODO: �ڴ���������������
	SendMessage(WM_CLOSE);	 
}

void CMemeoryDlg::OnLink()
{
	// TODO: �ڴ���������������
	ShellExecute(this->m_hWnd, _T("open"), m_url,
                   NULL,NULL,SW_SHOWNORMAL);

}

bool CMemeoryDlg::PopupMenu(CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	m_Popup=menu.GetSubMenu(0);
	ClientToScreen(&point);
	m_Popup->TrackPopupMenu( TPM_TOPALIGN,point.x,point.y,this);
	HMENU hmenu = menu.Detach();                       //��Դ����
    menu.DestroyMenu();
	return true;
}
