#include <WinSock2.h>

//port
#define SERVER_PORT 8000;


//msg type
#define HEARTBEAT 1
#define HEARTBEAT_ACK 101
#define MSG 2
#define LOGONIN 4
#define LOGONIN_ACK 41
#define USRE_FULL 6
#define USRE_EMPTY 5
#define LOGONOUT 7
#define LOGONOUT_ACK 701
#define GETUSERONLINE 8
#define GETUSERONLINE_ACK 801

#define PUNCHING_PASSMSG_A_S 100
#define PUNCHING_PASSMSG_S_B 102
#define PUNCHING_PASSMSG_B_S 103
#define PUNCHING_PASSMSG_B_A 104
#define PUNCHING_PASSMSG_S_A 105

//Timer
#define  TIMER_HEARTBEAT  1
#define  TIMER_DRAW_STATUS  2
#define  TIMER_COUNT1  3
#define  TIMER_COUNT2  4
#define  TIMER_5  5






// 发送的 Msg 格式
struct stMessage
{
	int MessageType;
	char cName[10];
	char cPassword[10];
	unsigned int ip;
	unsigned short port;
	char MsgContent[100];
};

//Cient状态
struct sClientStatus
{
	bool P2Pisalive;
	int  P2Palive_count;
	int  IsPunched;
	bool MsgReceiveInit;
	bool ConUDPServer;
	bool GerUserReady;

};

//是否穿透的状态
enum PUNCHING_STATUS
{
	NOPUNCHING = 0,		//  
	ISPUNCHINGED = 1,	//  
	PUNCHINGED = 2,	//  
};

//登陆用户信息
struct sLoginlUser
{
	char username[10];
	unsigned int ip;
	unsigned short port;
};


int SocketInit(LPVOID pParam);
DWORD WINAPI MsgReceive(LPVOID pParam);
BOOL Connect_proc();
int GetUser_Online();
VOID Show_Online(LPVOID pParam);