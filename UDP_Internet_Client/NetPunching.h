#include <WinSock2.h>


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
