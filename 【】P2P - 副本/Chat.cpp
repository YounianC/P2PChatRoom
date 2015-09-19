// Chat.cpp : implementation file
//
#include "stdafx.h"
#include "BroadCastChat.h"
#include "Chat.h"
#include "BroadCastChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChat

IMPLEMENT_DYNCREATE(CChat, CMDIChildWnd)

CString CChat::Msg = "";
CChat::CChat()
{
	//请求socket 1.1
	WORD wVersionRequested = MAKEWORD( 1, 1 );
	WSADATA wsaData;
	WSAStartup( wVersionRequested, &wsaData );
}

CChat::~CChat()
{
	closesocket(m_sockListen);
	WSACleanup();
}

BOOL CChat::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
// 	return m_wndSplitter.Create(this,
// 		2, 2,       // TODO: adjust the number of rows, columns
// 		CSize(10, 10),  // TODO: adjust the minimum pane size
// 		pContext);
	return TRUE;
}

void CChat::EnableBroadcast( BOOL bEnable )
{
	setsockopt(m_sockListen,SOL_SOCKET,SO_BROADCAST,       
		(CHAR *)&bEnable,sizeof(BOOL));       
}


// 发送消息
BOOL CChat::SendMsg( CString szMsg, int nLen, unsigned long szHost, short lPort )
{
	int nSend;
	//unsigned int addr=0;
	
	SOCKADDR_IN dest; //发送目标地址
	dest.sin_addr.s_addr = szHost; 
	dest.sin_family = AF_INET; 
	dest.sin_port = htons( lPort );
	char * tembuffer=szMsg.GetBuffer(nLen);
    nSend = sendto( m_sockListen, tembuffer, nLen, 0, ( SOCKADDR* )&dest, sizeof( SOCKADDR ));

	


	return TRUE;
}

BOOL CChat::Listen(unsigned short sListeningPort)
{
	//创建Socket
	m_sockListen = socket( AF_INET, SOCK_DGRAM, 0 );
	//填充地址信息
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = htons(sListeningPort);
	m_sockAddr.sin_addr.s_addr = htonl(0);
    bind(m_sockListen, (sockaddr*)&m_sockAddr, sizeof(SOCKADDR));
	
	HANDLE hThreadID;   //线程ID
	DWORD thread;
	//创建接收线程
	hThreadID = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(CChat::ListenThread),
		(void*)this, 0, &thread );
	return TRUE;
}
//接收线程
void* CChat::ListenThread( void *data)
{
	const int MAXBUFFER = 4096;   //设置最大缓存区
	char szBuf[MAXBUFFER];
	CChat *pChat = ( CChat* )data;
	int len = sizeof( pChat->m_sockRemoteAddr );
	
	int result = recvfrom( pChat->m_sockListen, szBuf, sizeof( szBuf ) - 1, 
		0, ( sockaddr* )&pChat->m_sockRemoteAddr, &len );
	if( result >0 )
	{
		szBuf[ result ] = 0;
		Msg=szBuf;
	}
	return NULL;
}


BEGIN_MESSAGE_MAP(CChat, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChat)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChat message handlers
