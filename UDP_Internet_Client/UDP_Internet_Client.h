
// UDP_Internet_Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUDP_Internet_ClientApp: 
// �йش����ʵ�֣������ UDP_Internet_Client.cpp
//

class CUDP_Internet_ClientApp : public CWinApp
{
public:
	CUDP_Internet_ClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUDP_Internet_ClientApp theApp;