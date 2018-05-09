 
#include "tcp.h"
CTCP::CTCP()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd))return;
	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//ULONG ul;
	//ioctlsocket(m_skt, FIONBIO, &ul);
	ZeroMemory(&m_saLocal, sizeof(m_saLocal));
	ZeroMemory(&m_saRemote, sizeof(m_saRemote));
}
CTCP::~CTCP()
{

}

BOOL CTCP::Bind(LPSTR lpszIPAddress, INT nPort)
{
	m_saLocal.sin_family = AF_INET;
	m_saLocal.sin_addr.S_un.S_addr = inet_addr(lpszIPAddress);
	m_saLocal.sin_port = htons(nPort);
	ZeroMemory(m_saLocal.sin_zero, 8);
	return (SOCKET_ERROR != bind(m_skt, (sockaddr*)&m_saLocal, sizeof(sockaddr_in)));
}
BOOL CTCP::Connect(LPSTR lpszIPAddress, INT nPort)
{
	m_saRemote.sin_family = AF_INET;
	m_saRemote.sin_addr.S_un.S_addr = inet_addr(lpszIPAddress);
	m_saRemote.sin_port = htons(nPort);
	ZeroMemory(m_saRemote.sin_zero, 8);
	return (SOCKET_ERROR != connect(m_skt, (sockaddr*)&m_saRemote, sizeof(sockaddr_in)));
}

INT CTCP::Send(LPVOID lpData, INT nLength)
{
	return send(m_skt, (char*)lpData, nLength, 0);
}
INT CTCP::Recv(LPVOID lpData, INT nLength)
{
	int nFromLen = sizeof(sockaddr_in);
	return recv(m_skt, (char*)lpData, nLength, 0);
}

BOOL CTCP::SetRecvBufferSize(INT nSize)
{
	return (SOCKET_ERROR != setsockopt(m_skt, SOL_SOCKET, SO_RCVBUF, (const char*)&nSize, sizeof(INT)));
}
BOOL CTCP::SetSendBufferSize(INT nSize)
{
	return (SOCKET_ERROR != setsockopt(m_skt, SOL_SOCKET, SO_SNDBUF, (const char*)&nSize, sizeof(INT)));
}
BOOL CTCP::GetRecvBufferSize(INT *pnSize)
{
	INT len = sizeof(INT);
	return (SOCKET_ERROR != getsockopt(m_skt, SOL_SOCKET, SO_RCVBUF, (char*)pnSize, &len));
}
BOOL CTCP::GetSendBufferSize(INT *pnSize)
{
	INT len = sizeof(INT);
	return (SOCKET_ERROR != getsockopt(m_skt, SOL_SOCKET, SO_SNDBUF, (char*)pnSize, &len));
}
