#include <SDL.h>
#include <SDL_net.h>
#include "SNClient.h"
#include <string.h>
#include <stdio.h>
#include "SNDataPacket.h"
#include "SNWorld.h"

void SNClient::Setup()
{
	SDLNet_Init();

	int res;
	res = SDLNet_ResolveHost(&ip, "127.0.0.1", 6969);
	if (!res && printErrors)
	{
		printf("Client Connect: %s\n", SDLNet_GetError());
	}

	tcpsock = SDLNet_TCP_Open(&ip);
	if (!tcpsock && printErrors)
	{
		printf("Client Open: %s\n", SDLNet_GetError());
	}

	printf("Client Setup!\n");

	socketSet = SDLNet_AllocSocketSet(MAX_NUM_SOCKETS);
	SDLNet_TCP_AddSocket(socketSet, tcpsock);
}

bool SNClient::RecvData()
{
	int numReady;
	numReady = SDLNet_CheckSockets(socketSet, RECV_TIMEOUT_MS);
	if (numReady == -1 && printErrors)
	{
		printf("Client Check Sockets: %s\n", SDLNet_GetError());
	}
	else if (numReady && printDebug)
	{
		printf("There are %d sockets ready!\n", numReady);
	}

	if (numReady <= 0)
		return false;

	for (int i = 0; i < MAX_NUM_SOCKETS; ++i)
	{
		if (SDLNet_SocketReady(tcpsock))
		{

			char recvData[1024];
			int len = SDLNet_TCP_Recv(tcpsock, recvData, 1024);

			if (!len)
			{
				printf("Client Recieve: %s\n", SDLNet_GetError());
				return false;
			}
			else if (len != -1)
			{
				sscanf_s(recvData, "%i %hu %hu %hu %hu %hu", &recievedData.flags, &recievedData.id, &recievedData.posX, &recievedData.posY, &recievedData.health, &recievedData.animState);
				return true;
			}
		}
	}
	return false;
}

void SNClient::SendData()
{
	if (tcpsock == nullptr)
		return;

	char buffer[1024];

	sprintf_s(buffer, "%i %hu %hu %hu %hu %hu", statePack.flags, statePack.id, statePack.posX, statePack.posY, statePack.health, statePack.animState);

	int len = strlen(buffer);
	if (len)
	{
		int result;

		if (printDebug)
		{
			printf("Client Sending Message: %.*s\n", len, buffer);
		}

 		result = SDLNet_TCP_Send(tcpsock, buffer, len);
		if (result < len && printErrors)
			printf("Client Message Sent: %s\n", SDLNet_GetError());
	}
}

void SNClient::Close()
{
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(tcpsock);
}
