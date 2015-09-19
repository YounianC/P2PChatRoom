#if !defined(AFX_CHAT_H__93E83C60_92E4_441C_9821_5A25785F0F46__INCLUDED_)
#define AFX_CHAT_H__93E83C60_92E4_441C_9821_5A25785F0F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChat frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CChat : public CMDIChildWnd
{
private:
	SOCKADDR_IN m_sockAddr;  // 服务端绑定地址
	SOCKADDR_IN m_sockRemoteAddr; // 发送数据方地址	
	SOCKET m_sockListen;     // 等待接受数据的Socket

	DECLARE_DYNCREATE(CChat)
//protected:
//	CSplitterWnd    m_wndSplitter;
public:
	static CString Msg;
	
	CChat();
	// 等待接收数据
	BOOL Listen(unsigned short sListeningPort);
	static void* ListenThread( void *data);
	
	//发送UDP数据包
	BOOL SendMsg( CString szMsg, int nLen, unsigned long szHost, short lPort );
	
	// 设置广播
	void EnableBroadcast( BOOL bEnable );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChat)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChat();

	// Generated message map functions
	//{{AFX_MSG(CChat)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAT_H__93E83C60_92E4_441C_9821_5A25785F0F46__INCLUDED_)
