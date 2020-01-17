#include <SDL.h>
#include <SDL_net.h>
#include "SNServer.h"
#include <string.h>
#include "SNWorld.h"
#include "DataPacket.h"

//https://gist.github.com/psqq/b92243f2149fcf4dd46370d4c0b5fef9

void SNServer::Setup()
{
	SDLNet_Init();

	SDLNet_ResolveHost(&ip, NULL, 6969);

	server = SDLNet_TCP_Open(&ip);
	if (!server)
	{
		printf("Server Open: %s\n", SDLNet_GetError());
	}

	socketSet = SDLNet_AllocSocketSet(MAX_NUM_SOCKETS);
}

void SNServer::AcceptConnection()
{
	client = SDLNet_TCP_Accept(server);
	if (!client)
	{
		printf("Server Accept: %s\n", SDLNet_GetError());
	}

	remoteIp = SDLNet_TCP_GetPeerAddress(client);
	if (!remoteIp)
	{
		printf("Server Peer Address: %s\n", SDLNet_GetError());
	}

	printf("Accepted connection from: %d : %d\n", remoteIp->host, remoteIp->port);

	SDLNet_TCP_AddSocket(socketSet, client);
}

bool SNServer::RecvData()
{
	int numReady;
	numReady = SDLNet_CheckSockets(socketSet, RECV_TIMEOUT_MS);
	if (numReady == -1)
	{
		printf("Server Check Sockets: %s\n", SDLNet_GetError());
	}
	else if (numReady)
	{
		printf("There are %d sockets ready!\n", numReady);
	}

	if (numReady <= 0)
		return false;

	for (int i = 0; i < MAX_NUM_SOCKETS; ++i)
	{
		if (SDLNet_SocketReady(client))
		{

			char recvData[1024];
			int len = SDLNet_TCP_Recv(client, recvData, 1024);

			if (!len)
			{
				printf("Server Recieve: %s\n", SDLNet_GetError());
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

// TODO: Decouple data from server and client
void SNServer::SendData(/* DataPacket dataToSend */)
{
	char buffer[1024];

	sprintf_s(buffer, "%hu %hu %hu %hu", transformPack.id, transformPack.posX, transformPack.posY, transformPack.health);
	int len = strlen(buffer);

	if (client)
	{
		if (len)
		{
			int result;

			printf("Server Sending Message: %.*s\n", len, buffer);

			result = SDLNet_TCP_Send(client, buffer, len);
			if (result < len)
				printf("Server Message Sent: %s\n", SDLNet_GetError());
		}
	}
}

void SNServer::Close()
{
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(client);
}
