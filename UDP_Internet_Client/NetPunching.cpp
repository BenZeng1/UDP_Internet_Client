#include "stdafx.h"
#include "UDP_Internet_Client.h"
#include "UDP_Internet_ClientDlg.h"
#include "NetPunching.h"


SOCKET PrimaryUDP = -1;
stMessage Connect_msg = { 0 };
sClientStatus client_status = { 0 };

sockaddr_in server;
sockaddr_in p2premoteA;
sockaddr_in p2premoteB;
sockaddr_in temp;

int serverLens, msgLength;
sLoginlUser LogInUserTable[10] = { 0 };

int SocketInit(LPVOID pParam){

	CUDP_Internet_ClientDlg* dlg = (CUDP_Internet_ClientDlg*)pParam;

	//Sock 初始化
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 1), &wsaData)) //调用Windows Sockets DLL
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
	server.sin_addr.S_un.S_addr = inet_addr(s1);

	//Port
	WORD port = dlg->GetDlgItemInt(IDC_PORT);
	server.sin_port = htons(port);//Server的监听端口

	//UserName
	dlg->GetDlgItemText(IDC_NAME, str);
	strcpy_s(Connect_msg.cName, (char*)(LPCTSTR)str);
	Connect_msg.cName[9] = '\0';

	//Password
	dlg->GetDlgItemText(IDC_PW, str);
	strcpy_s(Connect_msg.cPassword, (char*)(LPCTSTR)str);
	Connect_msg.cPassword[9] = '\0';

	str.ReleaseBuffer();
	return 1;
}

//接收数据线程
DWORD WINAPI MsgReceive(LPVOID pParam){

	int ret = -1;
	stMessage msg = { 0 };
	CUDP_Internet_ClientDlg* dlg = (CUDP_Internet_ClientDlg*)pParam;
	sockaddr_in temp;

	client_status.MsgReceiveInit = true;

	while (1)
	{
		memset(&msg, 0, msgLength);

		ret = recvfrom(PrimaryUDP, (char *)&msg, msgLength, 0, (sockaddr*)&temp, &serverLens);
		if (ret < 0)
		{
			continue;
		}

		switch (msg.MessageType)
		{
			case MSG:
			{
						CString str;
						str = "Get Message...";
						dlg->SetDlgItemText(IDC_Showstatus, str);
						str = "";
						dlg->SetDlgItemText(IDC_Receive, str);
						str = msg.MsgContent;
						dlg->SetDlgItemText(IDC_Receive, str);
			}
			case PUNCHING_PASSMSG_S_B:
			{
										 
										 break;
			}
			case LOGONIN_ACK:
			{
								CString str;
								str = "连接服务器成功！";
								dlg->SetDlgItemText(IDC_Showstatus, str);
								client_status.ConUDPServer = true;
								client_status.IsPunched = NOPUNCHING;

								KillTimer(dlg->m_hWnd, TIMER_COUNT1);
								dlg->GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(TRUE);
								dlg->GetDlgItem(IDC_BUTTONGET)->EnableWindow(TRUE);

								break;
			}
			case LOGONOUT_ACK:
			{
								 break;
			}
			case  USRE_FULL:
			{
							   break;
			}
			case HEARTBEAT_ACK:
			{
								  break;
			}
			case HEARTBEAT:
			{
							  break;
			}
			case PUNCHING_PASSMSG_S_A:
			{
										 break;
			}
			case GETUSERONLINE_ACK:
			{
									  CString str;
									  str = "GETUSERONLINE_ACK, 得到登陆用户";
									  dlg->SetDlgItemText(IDC_Showstatus, str);

									  for (int i = 0; i < sizeof(LogInUserTable) / sizeof(sLoginlUser);i++)
									  {
										  strcpy_s(LogInUserTable[i].username, msg.cName);
										  LogInUserTable[i].ip = msg.ip;
										  LogInUserTable[i].port = msg.port;
									  }
									  break;
			}
			default:
				break;
		}
	}
	return 1;
}

//连接服务器
BOOL Connect_proc(){
	Connect_msg.MessageType = LOGONIN;
	serverLens = sizeof(server);
	msgLength = sizeof(stMessage);
	sendto(PrimaryUDP, (char *)&Connect_msg, msgLength, 0, (sockaddr*)&server, serverLens);
	return 1;
}

INT GetUser_Online()
{
	stMessage msg;
	memset(&msg, 0, msgLength);
	msg.MessageType = GETUSERONLINE;
	memset(LogInUserTable, 0x00, sizeof(LogInUserTable));
	sendto(PrimaryUDP, (CHAR *)&msg, msgLength, 0, (sockaddr*)&server, serverLens);
	return 0;
}

/*
VOID Show_Online(LPVOID pParam)
{
	CUDP_Internet_ClientDlg* dlg = (CUDP_Internet_ClientDlg*)pParam;
	CString str;
	str = "";
	dlg->SetDlgItemText(IDC_OnlineUserList, str);
	sockaddr_in temp;
	for (int i = 0; LogInUserTable[i].ip != 0; i++)
	{
		CString str1;
		temp.sin_addr.s_addr = htonl(LogInUserTable[i].ip);
		CHAR *p = inet_ntoa(temp.sin_addr);
		str1 = p;

		CHAR p2[15];
		CString str2;
		itoa(LogInUserTable[i].port, p2, 10);
		str2 = p2;

		CHAR *p4 = LogInUserTable[i].username;
		CString str3;
		str3 = p4;
		str = str + str1 + str2 + str3;
	}
	dlg->SetDlgItemText(IDC_OnlineUserList, str);
}
*/

VOID Punching(LPVOID pParam)
{

}


