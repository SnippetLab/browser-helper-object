// EditZCB.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CEditZCBApp:
// �йش����ʵ�֣������ EditZCB.cpp
//

class CEditZCBApp : public CWinApp
{
public:
	CEditZCBApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEditZCBApp theApp;
