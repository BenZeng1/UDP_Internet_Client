#include <WinSock2.h>


// ���͵� Msg ��ʽ
struct stMessage
{
	int MessageType;
	char cName[10];
	char cPassword[10];
	unsigned int ip;
	unsigned short port;
	char MsgContent[100];

};
