#include "stdafx.h"
#include "UDP_Internet_Client.h"
#include "UDP_Internet_ClientDlg.h"
#include "NetPunching.h"


SOCKET PrimaryUDP = -1;
stMessage Connect_msg = { 0 };
sockaddr_in server;

int SocketInit(LPVOID pParam){

	CUDP_Internet_ClientDlg* dlg = (CUDP_Internet_ClientDlg*)pParam;

	//Sock 初始化
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 1), &wsaData)); //调用Windows Sockets DLL
	{
		//AfxMessageBox(_T("WinSock Init failed！"));
		return 0;
	}

	//创建 sock
	PrimaryUDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (PrimaryUDP < 0)
	{
		return 0;
	}

	//server
	server.sin_family = AF_INET;

	//IP
	CString str;
	dlg->GetDlgItemText(IDC_IP, str);
	LPTSTR s1 = str.GetBuffer();

	//Port
	WORD port = dlg->GetDlgItemInt(IDC_PORT);
	server.sin_port = htons(port);//Server的监听端口

	//UserName
	dlg->GetDlgItemText(IDC_NAME, str);
	strcpy(Connect_msg.cName, (char*)(LPCTSTR)str);

	//Password




}