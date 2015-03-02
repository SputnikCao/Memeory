// MemeoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Memeory.h"
#include "MemeoryDlg.h"
#include "Winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMemeoryDlg 对话框


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
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)//自定义消息
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


// CMemeoryDlg 消息处理程序

BOOL CMemeoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//---------------------------必要信息初始化---------------------------------//
	m_pDlg=new CToolDlg;
	m_pDlg->Create(IDD_DIALOG1);
	m_RGB=RGB(5,5,5);
	m_Brush.CreateSolidBrush(m_RGB);
	::GetWindowRect(this->m_hWnd,&m_ClientRc);
	::GetWindowRect(m_pDlg->m_hWnd,&m_ToolRc);
	m_pDlg->m_Pro.SetRange(0,100);
	m_url=_T("http://hi.csdn.net/space.php?do=home");

	//---------------------------托盘显示初始化---------------------------------//

   if(!NotifyIcon())
	  MessageBox(_T("托盘显示初始化失败"));
	
//-----------------------------------定时器设定------------------------------------------------------------------//

	
	SetTimer(m_IDTimer,1000,0);
	SetTimer(m_IDTimer2,m_Speed,0);
	
	
//----------------------------------------内存信息获取-------------------------------------------------------------//

	if(!MemoryUse())
		MessageBox(_T("内存信息获取失败"));

//----------------------------------------附加信息设定-------------------------------------------------------------//
	m_pDlg->m_Pro.SetPos(m_ProPos);
	m_pDlg->m_MemoryUse.SetWindowTextW(m_str);
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_str);

	if(!CPU_USE())
		MessageBox(_T("获取CPU信息失败"));
	if(!CPU_USE())
		MessageBox(_T("获取CPU信息失败"));
	else
	{
		m_pDlg->m_ProCPU.SetPos(m_nCPU);
		m_pDlg->m_CPU.SetWindowTextW(m_strCPU);	    
	}
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_strCPU);
	
//----------------------------------------附加信息设定-------------------------------------------------------------//

	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,WS_EX_LAYERED);//设置扩展形式
	if(!::SetLayeredWindowAttributes(this->m_hWnd,0,(255*80)/55,LWA_ALPHA))//透明背景
		     MessageBox(_T("背景透明失败"));	
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//在任务栏隐藏
	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,m_ClientRc.left,m_ClientRc.top,m_ClientRc.Width(),m_ClientRc.Height(),SWP_NOMOVE);// 置顶显示
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMemeoryDlg::OnPaint()//无用
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMemeoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMemeoryDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::GetWindowRect(this->m_hWnd,&m_ClientRc);
	::GetWindowRect(m_pDlg->m_hWnd,&m_ToolRc);
	int ScreenHeight=GetSystemMetrics(SM_CYSCREEN);
	
	if(m_ClientRc.top<ScreenHeight/2)
	{
	    m_pDlg->MoveWindow(m_ClientRc.left,m_ClientRc.top+23,m_ToolRc.Width(),m_ToolRc.Height());
		m_Judge=1;//窗口在上半区
	}
	else
	{
		m_pDlg->MoveWindow(m_ClientRc.left,m_ClientRc.top-m_ToolRc.Height()-2,m_ToolRc.Width(),m_ToolRc.Height());
		m_Judge=2;//窗口在下半区
	}
	m_pDlg->ShowWindow(SW_SHOW);

	if (m_bMouseTrack)    //若允许追踪，则。
    {
       TRACKMOUSEEVENT csTME;
       csTME.cbSize = sizeof(csTME);
       csTME.dwFlags = TME_LEAVE;
	   csTME.hwndTrack = this->m_hWnd;//指定要追踪的窗口
       csTME.dwHoverTime = 100000;  //鼠标在按钮上停留超过100000ms，才认为状态为HOVER
       ::_TrackMouseEvent(&csTME); //开启Windows的WM_MOUSELEAVE，WM_MOUSEHOVER事件支持
       m_bMouseTrack=FALSE;   //若已经追踪，则停止追踪

	}

	CDialog::OnMouseMove(nFlags, point);
}

void CMemeoryDlg::OnNcMouseMove(UINT nHitTest, CPoint point)//无用
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

//	m_pDlg->ShowWindow(SW_HIDE);
	CDialog::OnNcMouseMove(nHitTest, point);
}

LRESULT CMemeoryDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//--------------------------模拟点击标题消息-------------------------------
//	LRESULT nHitTest = CDialog::OnNcHitTest(point);    
//	if (((nHitTest == HTCLIENT) | (nHitTest == HTTRANSPARENT))&& GetAsyncKeyState(MK_LBUTTON) < 0 )  //如果是客户区//如果鼠标左键按下，GetAsyncKeyState函数的返回值小于0    
//        nHitTest = HTCAPTION;                       //则把它当成标题栏   
//	return nHitTest; 
	return CDialog::OnNcHitTest(point);
}

void CMemeoryDlg::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
    case WM_RBUTTONUP:                                   // 右键起来时弹出菜单
    {     
       ::GetCursorPos(&m_CursorPoint);                            // 得到鼠标位置
	   CMenu menu;
	   menu.LoadMenuW(IDR_MENU1);
	   m_Popup=menu.GetSubMenu(0);
	   m_Popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,m_CursorPoint.x,m_CursorPoint.y,this);
	   HMENU hmenu = menu.Detach();                       //资源回收
       menu.DestroyMenu();
    } 
    break; 
    case WM_LBUTTONDBLCLK:                                 // 双击左键的处理
    { 
       this->ShowWindow(SW_SHOWNORMAL);                     // 显示主窗口
       this->SetForegroundWindow(); 
	   ::SetWindowPos(this->m_hWnd,HWND_TOPMOST,m_ClientRc.left,m_ClientRc.top,m_ClientRc.Width(),m_ClientRc.Height(),SWP_NOMOVE);// 置顶显示

    }
    break;
    }
    return 0;
}
void CMemeoryDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	Shell_NotifyIcon(NIM_DELETE, &m_nid);//清楚系统托盘图标
	m_pDlg->DestroyWindow();
}

void CMemeoryDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	static int t_Mem=0;
	static int t_CPU=0;
	static int i=1;
	if(nIDEvent==1)
	{    
		   if(!MemoryUse())
	           MessageBox(_T("获取内存信息失败"));
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
		   MessageBox(_T("CPU信息获取失败"));
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

	// TODO:  在此更改 DC 的任何属性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC1)//设置文本透明
    {   
         pDC->SetBkMode(TRANSPARENT); 
		 pDC->SetTextColor(RGB(255,255,255));
  //       GetDlgItem(IDC_STATIC1)->SetWindowTextW(m_str);
		 return   m_Brush;
    }   
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_Brush;
}




void CMemeoryDlg::OnLButtonDown(UINT nFlags, CPoint point)//实现点击客户区模拟点击标题栏
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown (nFlags, point );

	//Fool dialog into thinking simeone clicked on its caption bar .
    PostMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARAM(point.x ,point. y ));


	CDialog::OnLButtonDown(nFlags, point);
}

bool CMemeoryDlg::CPU_USE()//CPU信息
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
			
		m_strCPU.Format(_T("CPU使用率: %d %%"),(UINT)dbIdleTime);
		m_nCPU=(UINT)dbIdleTime;
		
	}
		
		// store new CPU's idle and system time 
		liOldIdleTime= SysPerfInfo.liIdleTime; 
		liOldSystemTime= SysTimeInfo.liKeSystemTime;
		
	return true;
}


bool CMemeoryDlg::MemoryUse(void)//内存信息
{
	//初始化内存信息结构体
	MEMORYSTATUS Memory_Info;
	Memory_Info.dwLength=sizeof(MEMORYSTATUS);
	DWORD MemTotal,MemFree,VMemTotal,VMemFree;//存储内存状态信息
	
	
	GlobalMemoryStatus(&Memory_Info); //存储内存状态信息
	MemTotal=Memory_Info.dwTotalPhys/m_Shift;//得到总内存大小（单位b）
	MemFree=Memory_Info.dwAvailPhys/m_Shift;//得到剩余内存大小（单位b）
	VMemTotal=Memory_Info.dwTotalVirtual/m_Shift;//得到总虚拟内存大小（单位b）
	VMemFree=Memory_Info.dwAvailVirtual/m_Shift;//得到剩余虚拟内存内存大小（单位b
	
	m_ProPos=(double)(MemTotal-MemFree)*100/MemTotal;
	m_str.Format(_T("内存使用: %d %%"),m_ProPos);
	
    
	return true;
}

bool CMemeoryDlg::NotifyIcon(void)//系统托盘
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd    = this->m_hWnd;
	m_nid.uID     = IDI_ICON1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	m_nid.uCallbackMessage = WM_SHOWTASK;             // 自定义的消息名称
	m_nid.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1)); //IDR_MAINFRAME可改为自己制作的资源名 
    wcscpy(m_nid.szTip,_T("内存管理器"));              // 信息提示条为"自制闹钟程序"
    if(Shell_NotifyIcon(NIM_ADD, &m_nid))// 在托盘区添加图标
		return true;
	else
		return false;
	
}

void CMemeoryDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	this->ShowWindow(SW_HIDE);
}

LRESULT CMemeoryDlg::OnMinDialog(WPARAM wParam, LPARAM lParam)
{
	this->ShowWindow(SW_HIDE);
	return 0;
}
void CMemeoryDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    
	PopupMenu(point);//弹出菜单
	 
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
	// TODO: 在此添加命令处理程序代码
	SendMessage(WM_CLOSE);	 
}

void CMemeoryDlg::OnLink()
{
	// TODO: 在此添加命令处理程序代码
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
	HMENU hmenu = menu.Detach();                       //资源回收
    menu.DestroyMenu();
	return true;
}
