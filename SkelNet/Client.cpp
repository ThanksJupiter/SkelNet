#include <SDL.h>
#include <SDL_net.h>
#include "Client.h"
#include <string.h>
#include <stdio.h>
#include "DataPacket.h"
#include "World.h"

void Client::Setup()
{
	SDLNet_Init();

	int res;
	res = SDLNet_ResolveHost(&ip, "127.0.0.1", 6969);
	if (!res)
	{
		printf("Client Connect: %s\n", SDLNet_GetError());
	}

	tcpsock = SDLNet_TCP_Open(&ip);
	if (!tcpsock)
	{
		printf("Client Open: %s\n", SDLNet_GetError());
	}

	socketSet = SDLNet_AllocSocketSet(MAX_NUM_SOCKETS);
	SDLNet_TCP_AddSocket(socketSet, tcpsock);
}

bool Client::RecvData()
{
	int numReady;
	numReady = SDLNet_CheckSockets(socketSet, RECV_TIMEOUT_MS);
	if (numReady == -1)
	{
		printf("Client Check Sockets: %s\n", SDLNet_GetError());
	}
	else if (numReady)
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
				sscanf_s(recvData, "%hu %hu %hu %hu", &recievedData.id, &recievedData.posX, &recievedData.posY, &recievedData.health);
				return true;
			}
		}
	}
	return false;
}

void Client::SendData()
{
	char buffer[1024];

	sprintf_s(buffer, "%hu %hu %hu %hu", transformPack.id, transformPack.posX, transformPack.posY, transformPack.health);
	int len = strlen(buffer);

	if (len)
	{
		int result;

		printf("Client Sending Message: %.*s\n", len, buffer);

		result = SDLNet_TCP_Send(tcpsock, buffer, len);
		if (result < len)
			printf("Client Message Sent: %s\n", SDLNet_GetError());
	}
}

void Client::Close()
{
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(tcpsock);
}
