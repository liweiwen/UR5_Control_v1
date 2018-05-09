#pragma once

#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#define CTCP_IPV4_ADDRESS_MAX_LENGTH	0x10

typedef struct _CTCP_PAKCET {
	LPVOID lpData;
	DWORD  dwLength;
}CTCP_PAKCET, *PCTCP_PAKCET;

class CTCP
{
public:
	CTCP();
	~CTCP();
public:
	BOOL Bind(LPSTR lpszIPAddress, INT nPort);
	BOOL Connect(LPSTR lpszIPAddress, INT nPort);
	BOOL SetRecvBufferSize(INT nSize);
	BOOL SetSendBufferSize(INT nSize);
	BOOL GetRecvBufferSize(INT *pnSize);
	BOOL GetSendBufferSize(INT *pnSize);

	INT Send(LPVOID lpData, INT nLength);
	INT Recv(LPVOID lpData, INT nLegnth);
private:
	SOCKET	m_skt;
	sockaddr_in m_saLocal;
	sockaddr_in m_saRemote;
};