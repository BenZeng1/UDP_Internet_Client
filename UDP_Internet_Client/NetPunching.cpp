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

char user1[10] = { 0 };
char user2[10] = { 0 };

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
						str = msg.MsgContent;
						dlg->SetRevBoxText("对方说：" + str);
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
								 CString str;
								 str = "退出UDP服务器成功!";
								 dlg->SetDlgItemText(IDC_Showstatus, str);
								 CloseSession();
								 client_status.ConUDPServer = FALSE;
								 KillTimer(dlg->m_hWnd, TIMER_COUNT2);
								 dlg->GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(TRUE);
								 dlg->GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(FALSE);
								 dlg->GetDlgItem(IDC_BUTTONGET)->EnableWindow(FALSE);
								 break;
			}
			case  USRE_FULL:
			{
							   break;
			}
			case GETUSERONLINE_ACK:
			{
									  CString str;
									  str = "GETUSERONLINE_ACK, 得到登陆用户";
									  dlg->SetDlgItemText(IDC_Showstatus, str);

									  for (int i = 0; i < sizeof(LogInUserTable)/sizeof(sLoginlUser); i++)
									  {
										  if (LogInUserTable[i].username[0] == 0)
										  {
										  strcpy_s(LogInUserTable[i].username, msg.cName);
										  LogInUserTable[i].ip = msg.ip;
										  LogInUserTable[i].port = msg.port;
										  break;
										  }
									  }
									  break;
			}
			case PUNCHING_PASSMSG_S_B:
			{
										 p2premoteA.sin_addr.S_un.S_addr = htonl(msg.ip);
										 p2premoteA.sin_family = AF_INET;
										 p2premoteA.sin_port = htons(msg.port);

										 stMessage msg1 = { 0 };
										 memcpy(&msg1, &msg, sizeof(stMessage));

										 CString str;
										 str = "Get S_B,Sent B_A,Sent B_S \n";
										 dlg->SetDlgItemText(IDC_Showstatus, str);

										 memset(&msg, 0, msgLength);
										 msg.MessageType = PUNCHING_PASSMSG_B_A;
										 ret = sendto(PrimaryUDP, (CHAR*)&msg, msgLength, 0, (struct sockaddr*)&p2premoteA, serverLens);

										 msg1.MessageType = PUNCHING_PASSMSG_B_S;
										 temp.sin_family = AF_INET;
										 ret = sendto(PrimaryUDP, (CHAR*)&msg1, msgLength, 0, (struct sockaddr*)&temp, serverLens);
										 client_status.IsPunched = ISPUNCHINGED;

										 for (int i = 0; LogInUserTable[i].ip != 0; i++)
										 {
											 if (strcmp(LogInUserTable[i].username, Connect_msg.cName) == 0)
											 {
												 p2premoteB.sin_addr.S_un.S_addr = htonl(LogInUserTable[i].ip);
												 p2premoteB.sin_family = AF_INET;
												 p2premoteB.sin_port = htons(LogInUserTable[i].port);
												 break;
											 }
										 }
										 dlg->GetDlgItem(IDC_BUTTONSEND)->EnableWindow(TRUE);


				

										/* strcpy_s(user2, msg1.cName);
										 strcpy_s(user1, Connect_msg.cName);
										 
										 CString strUser1, strUser2;
										 strUser1 = "User1:";
										 strUser2 = "User2:";
										 str = strUser1 + user1;
										 dlg->SetDlgItemText()*/
										
										 break;
			}
			case PUNCHING_PASSMSG_S_A:
			{
										 CString str;
										 str = "Get PUNCHING_ACK, P2P start...";
										 dlg->SetDlgItemText(IDC_Showstatus, str);

										 SetTimer(dlg->m_hWnd, TIMER_HEARTBEAT, 10000, NULL);

										 stMessage msg = { 0 };
										 memset(&msg, 0, msgLength);
										 msg.MessageType = HEARTBEAT;
										 sendto(PrimaryUDP, (CHAR*)&msg, msgLength, 0, (sockaddr*)&p2premoteB, serverLens);

										 client_status.P2Pisalive = TRUE;
										 client_status.P2Palive_count = 0;
										 client_status.IsPunched = PUNCHINGED;

										 dlg->GetDlgItem(IDC_BUTTONSEND)->EnableWindow(TRUE);

										 break;
			}
			case HEARTBEAT_ACK:
			{
								  CString str;
								  str = "Get HEARTBEAT_ACK... ";
								  dlg->SetDlgItemText(IDC_Showstatus, str);
								  client_status.P2Palive_count = 0;
								  client_status.P2Pisalive = true;
								  break;
			}
			case HEARTBEAT:
			{
							  CString str;
							  str.Format(_T("Get HEARTBEAT, Sent HEARTBEAT_ACK "));
							  dlg->SetDlgItemText(IDC_Showstatus, str);

							  //
							  memset(&msg, 0, msgLength);
							  msg.MessageType = HEARTBEAT_ACK;
							  sendto(PrimaryUDP, (char *)&msg, msgLength, 0, (sockaddr*)&p2premoteA, serverLens);
							  client_status.P2Palive_count = 0;
							  client_status.P2Pisalive = true;
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

VOID Show_Online(LPVOID pParam)
{
	CUDP_Internet_ClientDlg* dlg = (CUDP_Internet_ClientDlg*)pParam;
	CString str;
	str = "";
	//dlg->SetDlgItemText(IDC_OnlineUserList, str);
	sockaddr_in temp;
	for (int i = 0; LogInUserTable[i].ip != 0; i++)
	{
		CString str1, strTab, strEnt;
		strTab = "\t";
		strEnt = "\n\r";
		temp.sin_addr.s_addr = htonl(LogInUserTable[i].ip);
		CHAR *p = inet_ntoa(temp.sin_addr);
		str1 = p;

		CHAR p2[15];
		CString str2;
		_itoa_s(LogInUserTable[i].port, p2, 10);
		str2 = p2;

		CHAR *p4 = LogInUserTable[i].username;
		CString str3;
		str3 = p4;
		str = str + str1 +strTab + str2 + strTab + str3 + strEnt;
	}
	dlg->SetDlgItemText(IDC_OnlineUserList, str);
}


VOID Punching(LPVOID pParam)
{
	stMessage msg = { 0 };
	CUDP_Internet_ClientDlg* dlg = (CUDP_Internet_ClientDlg*)pParam;

	CString str;
	str = "Punching...";
	dlg->SetDlgItemText(IDC_Showstatus, str);

	CString strGetName;
	dlg->GetDlgItemText(IDC_PunchingName, strGetName);
	LPTSTR sGetName = strGetName.GetBuffer();
	for (int i = 0; LogInUserTable[i].ip != 0; i++)
	{
		if (strcmp(LogInUserTable[i].username, sGetName) == 0 )
		{
			p2premoteB.sin_addr.S_un.S_addr = htonl(LogInUserTable[i].ip);
			p2premoteB.sin_family = AF_INET;
			p2premoteB.sin_port = htons(LogInUserTable[i].port);
			strcpy_s(user2, LogInUserTable[i].username);
			strcpy_s(user1, Connect_msg.cName);
			msg.MessageType = PUNCHING_PASSMSG_A_S;
			strcpy_s(msg.cName, Connect_msg.cName);
			msg.ip = LogInUserTable[i].ip;
			msg.port = LogInUserTable[i].port;
			break;
		}
	}
	for (int i = 0; LogInUserTable[i].ip != 0; i++)
	{
		if (strcmp(LogInUserTable[i].username, Connect_msg.cName) == 0)
		{
			p2premoteA.sin_addr.S_un.S_addr = htonl(LogInUserTable[i].ip);
			p2premoteA.sin_family = AF_INET;
			p2premoteA.sin_port = htons(LogInUserTable[i].port);
			break;
		}
	}

	client_status.IsPunched = NOPUNCHING;
	sendto(PrimaryUDP, (CHAR*)&msg, msgLength, 0, (sockaddr*)&server, serverLens);
	return;
}

BOOL DisConnectServer()
{
	if (client_status.ConUDPServer == true)
	{
		Connect_msg.MessageType = LOGONOUT;
		sendto(PrimaryUDP, (CHAR *)&Connect_msg, msgLength, 0, (sockaddr*)&server, serverLens);
	}
	return FALSE;
}

VOID CloseSession()
{
	if (PrimaryUDP > 0)
	{
		closesocket(PrimaryUDP);
		WSACleanup();
	}
}

VOID SendMsg(LPVOID pParam)
{
	CUDP_Internet_ClientDlg* dlg = (CUDP_Internet_ClientDlg*)pParam;
	CString str;
	stMessage msg = { 0 };
	dlg->GetDlgItemText(IDC_EDIT_SEND, str);
	LPTSTR s1 = str.GetBuffer();

	memset(&msg, 0, msgLength);
	msg.MessageType = MSG;
	strcpy_s(msg.MsgContent, s1);
	
	if (client_status.IsPunched == ISPUNCHINGED)
	{
		sendto(PrimaryUDP, (CHAR*)&msg, msgLength, 0, (sockaddr*)&p2premoteA, serverLens);
	}
	else if ( client_status.IsPunched == PUNCHINGED)
	{
		sendto(PrimaryUDP, (CHAR*)&msg, msgLength, 0, (sockaddr*)&p2premoteB, serverLens);
	}

	dlg->SetRevBoxText(_T("我自己：") + str);
	dlg->SetDlgItemText(IDC_EDIT_SEND, "");
}

