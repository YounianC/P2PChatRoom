// BroadCastChatDlg.h : header file
//

#if !defined(AFX_BROADCASTCHATDLG_H__B4A95F94_C627_49E4_8BBC_F50195BD7092__INCLUDED_)
#define AFX_BROADCASTCHATDLG_H__B4A95F94_C627_49E4_8BBC_F50195BD7092__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")




/////////////////////////////////////////////////////////////////////////////
// CBroadCastChatDlg dialog

class CBroadCastChatDlg : public CDialog
{
// Construction
public:
	int SendMsg(int index,CString msg);
	int Listen(CString& t,SOCKADDR_IN& sender);
	bool newMember(SOCKADDR_IN sen);
	CBroadCastChatDlg(CWnd* pParent = NULL);	// standard constructor


	SOCKET sock2;

	SOCKET sockCliBC;
	SOCKADDR_IN addrSrvInfo;

	SOCKADDR_IN sin;//////接受   绑定到socket上
	unsigned short cliPortBC;//发出端口
	bool connectstate;

	bool T1state;//Timer存在状态
//	bool T2state;

	int friend_nums;

//	CString SRVinfo;
//	CString SRVIP;


	CString strLocalInfo;

	CString Msgin;

	
// Dialog Data
	//{{AFX_DATA(CBroadCastChatDlg)
	enum { IDD = IDD_BROADCASTCHAT_DIALOG };
	CComboBox	m_combo_sendto;
	CListBox	m_List_friend;
	CString	m_mymsg;
	CString	m_msgrecord;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBroadCastChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBroadCastChatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonbc();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnsend();
	afx_msg void OnBtnClear();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListFriend();
	afx_msg void OnSelchangeComboSendto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROADCASTCHATDLG_H__B4A95F94_C627_49E4_8BBC_F50195BD7092__INCLUDED_)
