#include "tcp.h"
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795028841971693993751058209

typedef struct _UR5_POSE {
	double x;
	double y;
	double z;
	double rx;
	double ry;
	double rz;
}UR5_POSE, *PUR5_POSE, *LPUR5_POSE;

UR5_POSE ur5_pose;

void Move(double x, double y, double z, double rx, double ry, double rz, CTCP* pTcp)                                                                                                                                                                                                                                                                                                                 
{
	CHAR szCmd[0x100];
	sprintf(szCmd, "movel(p[%f, %f, %f, %f, %f, %f], a=1.2 ,v=0.05)\n",x,y,z,rx,ry,rz);
	pTcp->Send(szCmd, strlen(szCmd));
}

void NormalizePose(PUR5_POSE pPose)
{
	if (pPose->rx < 0)
	{
		double theta = sqrt(pPose->rx * pPose->rx + pPose->ry * pPose->ry + pPose->rz * pPose->rz);
		double new_theta = 2 * PI - theta;
		pPose->rx = -pPose->rx / theta * new_theta;
		pPose->ry = -pPose->ry / theta * new_theta;
		pPose->rz = -pPose->rz / theta * new_theta;
	}
}

double ntohd(double d)
{
	double ret;
	unsigned char *p = (unsigned char *)&d;
	unsigned char *r = (unsigned char *)&ret;
	for (int i = 0; i < sizeof(double); i++)
	{
		r[i] = p[sizeof(double) - 1 - i];
	}
	return ret;
}

void UpdateState(LPVOID lpBuffer, ULONG ulLength)
{
	memcpy(&ur5_pose, (PUCHAR)lpBuffer + 440, sizeof(UR5_POSE));
	ur5_pose.x = ntohd(ur5_pose.x);
	ur5_pose.y = ntohd(ur5_pose.y);
	ur5_pose.z = ntohd(ur5_pose.z);
	ur5_pose.rx = ntohd(ur5_pose.rx);
	ur5_pose.ry = ntohd(ur5_pose.ry);
	ur5_pose.rz = ntohd(ur5_pose.rz);
	NormalizePose(&ur5_pose);
}

void RecvThread(CTCP* pTcp)
{
	while (true)
	{
		ULONG ulLength;
		pTcp->Recv(&ulLength, sizeof(ULONG));
		ulLength = ntohl(ulLength);
		LPVOID lpBuffer[0x1000];
		pTcp->Recv(lpBuffer, ulLength - sizeof(ULONG));
		UpdateState(lpBuffer, ulLength - sizeof(ULONG));
	}
}

int main()
{
	CTCP tcp;
	tcp.Connect("192.168.1.102", 30003);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvThread, &tcp, NULL, NULL);
	Sleep(1000);
	//m
	Move(ur5_pose.x  , ur5_pose.y , ur5_pose.z , ur5_pose.rx , ur5_pose.ry, ur5_pose.rz, &tcp);
	getchar();
	return 0;
}
