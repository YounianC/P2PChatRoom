// BroadCastChat.h : main header file for the BROADCASTCHAT application
//

#if !defined(AFX_BROADCASTCHAT_H__F4CE270B_980A_4049_ACAB_26874A3B6B60__INCLUDED_)
#define AFX_BROADCASTCHAT_H__F4CE270B_980A_4049_ACAB_26874A3B6B60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBroadCastChatApp:
// See BroadCastChat.cpp for the implementation of this class
//

class CBroadCastChatApp : public CWinApp
{
public:
	CBroadCastChatApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBroadCastChatApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBroadCastChatApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROADCASTCHAT_H__F4CE270B_980A_4049_ACAB_26874A3B6B60__INCLUDED_)
