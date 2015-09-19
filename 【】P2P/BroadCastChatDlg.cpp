// BroadCastChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BroadCastChat.h"
#include "BroadCastChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBroadCastChatDlg dialog

CBroadCastChatDlg::CBroadCastChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBroadCastChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBroadCastChatDlg)
	m_mymsg = _T("");
	m_msgrecord = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBroadCastChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBroadCastChatDlg)
	DDX_Control(pDX, IDC_COMBO_SENDTO, m_combo_sendto);
	DDX_Control(pDX, IDC_LIST_FRIEND, m_List_friend);
	DDX_Text(pDX, IDC_EDITMSG, m_mymsg);
	DDX_Text(pDX, IDC_EDITRECORD, m_msgrecord);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBroadCastChatDlg, CDialog)
	//{{AFX_MSG_MAP(CBroadCastChatDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONBC, OnButtonbc)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNSEND, OnBtnsend)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST_FRIEND, OnDblclkListFriend)
	ON_CBN_SELCHANGE(IDC_COMBO_SENDTO, OnSelchangeComboSendto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBroadCastChatDlg message handlers

BOOL CBroadCastChatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	cliPortBC=5050;

	sockCliBC = ::socket(AF_INET, SOCK_DGRAM, 0);

	// 有效SO_BROADCAST选项
	BOOL bBroadcast = TRUE;
	::setsockopt(sockCliBC, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL));
	int iMode = 1; //0：阻塞
	ioctlsocket(sockCliBC,FIONBIO, (u_long FAR*) &iMode);//非阻塞设置


	// 设置广播地址
	addrSrvInfo.sin_family = AF_INET;
	addrSrvInfo.sin_addr.s_addr = INADDR_BROADCAST;
	addrSrvInfo.sin_port = htons(cliPortBC);

	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = INADDR_ANY;	
	sin.sin_port = ::ntohs(cliPortBC);
	if(::bind(sockCliBC, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		CString t;
		int a=WSAGetLastError();
		t.Format("ERROR:%d",a);
		MessageBox(t);
	}



	GetDlgItem(IDC_BTNSEND)->EnableWindow(FALSE);
	friend_nums=0;
	
	struct hostent *host;
	struct in_addr *ptr;	// 获得IP地址
	char szHostName[200];
	//获得主机名
	gethostname( szHostName, strlen( szHostName ) );
	host = gethostbyname("");/////获取本机IP
	ptr = (struct in_addr *) host->h_addr_list[0];

	m_combo_sendto.AddString("所有人");/////

	// 提取IP地址信息，地址形式如下： 211.40.35.76 				
	int a = ptr->S_un.S_un_b.s_b1;  // 211				
	int b = ptr->S_un.S_un_b.s_b2;  // 40
	int c = ptr->S_un.S_un_b.s_b3;  // 35
	int d = ptr->S_un.S_un_b.s_b4;  // 76
	strLocalInfo.Format("#REQUEST#:%s;%d.%d.%d.%d",szHostName,a,b,c,d);//////////////广播消息开头为INFO		

	GetDlgItem(IDC_STATIC_LOCALIP)->SetWindowText("本机："+strLocalInfo.Mid(10));


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBroadCastChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBroadCastChatDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBroadCastChatDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBroadCastChatDlg::OnButtonbc()
{
	SetTimer(1,500,NULL);//第一个参数可以自己设置是定时器ID 第二个参数是间隔时(毫秒为单位的）
	SetTimer(2,500,NULL);
	T1state=true;
	GetDlgItem(IDC_BUTTONBC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONBC)->SetWindowText("请等待...");
}

void CBroadCastChatDlg::OnTimer(UINT nIDEvent) 
{
	CString temp_string;
	SOCKADDR_IN sender;
	
	int n;
	switch(nIDEvent)
	{
	case 1:
		// 发送广播	
		n=SendMsg(255,strLocalInfo);///////广播地址
		temp_string.Format("BC:%d:",n);
		m_msgrecord+=temp_string;
		m_msgrecord+=strLocalInfo;
		m_msgrecord+="\r\n";
		UpdateData(FALSE);
		break;
	case 2:
		if (Listen(temp_string,sender)>0)
		{
			CString t= inet_ntoa(sender.sin_addr);
			if (t==strLocalInfo.Right(13))///////////接收到自己发送的消息
			{break;}

			if(temp_string.Left(9)=="#REQUEST#")
			{
				if(LB_ERR!=m_List_friend.FindString(-1,t))////////说明找到了这个人  这个人已经加到好友列表了
					break;/////不要再添加到好友列表   也不要介绍给别人
				
				////////如果接受到了别人
				connectstate=true;	
				//给自己的好友介绍新朋友			
				temp_string=inet_ntoa(sender.sin_addr);
				SendMsg(0,"#NEWCOMMER#"+temp_string);
			//	t=temp_string.Right(10);
			//	int n=t.Find(';',-1);
			//	t=t.Left(n);//////////提取出PC名称、以后可以输入昵称

				newMember(sender);//////加到好友列表   
			
			}
			else if (temp_string.Left(11)=="#NEWCOMMER#")///////////朋友发过来的新的朋友  的信息
			{
			//	gethostname( szHostName, strlen( szHostName ) );
				newMember(sender);//////加到好友列表   
				//给自己的好友介绍新朋友
				temp_string=inet_ntoa(sender.sin_addr);
				SendMsg(0,"#NEWCOMMER#"+temp_string);
				

			}
			else if (temp_string.Left(10)=="#OFF_LINE#")
			{
				m_List_friend.DeleteString(m_List_friend.FindString(-1,t));
				m_combo_sendto.DeleteString(m_combo_sendto.FindString(-1,t));

				friend_nums--;

				t+="已下线！";
				m_msgrecord+=t;
				m_msgrecord+="\r\n";
				UpdateData(FALSE);

				

		//		GetDlgItem(IDC_BTNSEND)->EnableWindow(FALSE);

		//		GetDlgItem(IDC_BUTTONBC)->SetWindowText("连接");
		//		GetDlgItem(IDC_BUTTONBC)->EnableWindow(TRUE);
			
			}
			else
			{	
				PlaySound("msg.wav",0,SND_SYNC | SND_FILENAME);

				m_msgrecord+="--对方：";
				m_msgrecord+=temp_string;
				m_msgrecord+="\r\n";
				UpdateData(FALSE);
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

bool CBroadCastChatDlg::newMember(SOCKADDR_IN sen)
{
	CString t= inet_ntoa(addrSrvInfo.sin_addr);
	if (t==strLocalInfo.Right(13))///////////接收到自己发送的消息
	{
		return false;
	}

	t= inet_ntoa(sen.sin_addr);
	if(LB_ERR!=m_List_friend.FindString(-1,t))////////说明找到了这个人  这个人已经加到好友列表了
		return false;


	int n;
	char *sendbuf="#REQUEST#CONNECTED#";
	for (int i=0;i<5;i++)
	{
		n=::sendto(sockCliBC,sendbuf,strlen(sendbuf)+1,0,(sockaddr*)&sen,sizeof(sen));
//		CString t,t2;
//		t.Format("%d:",n);
//		t2= inet_ntoa(sen.sin_addr);
//		t+=t2;
	//	MessageBox(t);
	}
	

	if (T1state==true)
	{
		PlaySound("Global.wav",0,SND_SYNC | SND_FILENAME);
		
		KillTimer(1);/////广播关闭
		T1state=false;
		
		m_combo_sendto.SetCurSel(0);
	}

	friend_nums++;
	
	m_List_friend.AddString(t);
	
	m_combo_sendto.AddString(t);
//	m_combo_sendto.AddString(name);
	m_msgrecord+=t;
	m_msgrecord+="已连接网络\r\n";





	GetDlgItem(IDC_BUTTONBC)->SetWindowText("已连接");
	GetDlgItem(IDC_BTNSEND)->EnableWindow(TRUE);
	UpdateData(FALSE);
	return true;
}

void CBroadCastChatDlg::OnBtnsend() 
{	
	//发送消息 
	UpdateData(TRUE);
	
	m_msgrecord+="--本机：";
	m_msgrecord+=m_mymsg;
	m_msgrecord+="\r\n";

	int n=m_combo_sendto.GetCurSel();

//	CString to;
//	to.Format("index=%d,nums=%d",n,friend_nums);
//	MessageBox(to);

	int a=SendMsg(n,m_mymsg)>0;
	
	m_mymsg="";
	
	UpdateData(FALSE);
}
int CBroadCastChatDlg::Listen(CString& t,SOCKADDR_IN& sender)
{
	int len=sizeof(SOCKADDR);
	char recvbuf[1024];
	memset(recvbuf, 0,1024);
	int n= recvfrom(sockCliBC,recvbuf,1024,0,(SOCKADDR*)&sender,&len);
	t=recvbuf;

//	if (n>0)
//	{
//		CString t= inet_ntoa(sender.sin_addr);
//		MessageBox(t);
//	}
	return n;
}

int CBroadCastChatDlg::SendMsg(int index,CString msg)
{
	CString temp_string;
	SOCKADDR_IN temp;
	if (index==255)
	{
		temp.sin_family = AF_INET;
		temp.sin_addr.s_addr = INADDR_BROADCAST;
		temp.sin_port = htons(cliPortBC);
		
		
		char *sendbuf;
		sendbuf=msg.GetBuffer(msg.GetLength());
		
		int n=::sendto(sockCliBC,sendbuf,msg.GetLength()+1,0,(sockaddr*)&temp,sizeof(temp));
		return n;

	}

	
	if (index==friend_nums)///////选中最后一个选项  即  所有人
	{
		for (int i=0;i<friend_nums;i++)/////////注意  没有=   不能讲最后一个  所有人也包含进去
		{
			m_List_friend.GetText(i,temp_string);
			temp.sin_addr.s_addr = ::inet_addr(temp_string.GetBuffer(temp_string.GetLength()));
			temp.sin_family = AF_INET;
			temp.sin_port = htons(cliPortBC);

			char *sendbuf;
			sendbuf=msg.GetBuffer(msg.GetLength());
			
			int n=::sendto(sockCliBC,sendbuf,msg.GetLength()+1,0,(sockaddr*)&temp,sizeof(temp));
		}
	}
	else
	{
		m_List_friend.GetText(index,temp_string);///////list 中相对于combo中少一个“所有人”

		temp.sin_addr.s_addr = ::inet_addr(temp_string.GetBuffer(temp_string.GetLength()));
		temp.sin_family = AF_INET;
		temp.sin_port = htons(cliPortBC);
		
		char *sendbuf;
		sendbuf=msg.GetBuffer(msg.GetLength());
		
		int n=::sendto(sockCliBC,sendbuf,msg.GetLength()+1,0,(sockaddr*)&temp,sizeof(temp));


//		CString t231;
//		t231.Format("%d:",n);
//		t231+= inet_ntoa(temp.sin_addr);
	//	MessageBox(t231);


		return n;
	}
	return -1;
}

void CBroadCastChatDlg::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_msgrecord="";
	UpdateData(FALSE);
}

void CBroadCastChatDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	SendMsg(0,"#OFF_LINE#");//////////////////0  代表列表中的所有人
}



void CBroadCastChatDlg::OnDblclkListFriend() 
{
	// TODO: Add your control notification handler code here
	/////双击列表人   选中对话发送人


}

void CBroadCastChatDlg::OnSelchangeComboSendto() 
{
	// TODO: Add your control notification handler code here
	
}
