#include <SDL.h>
#include <SDL_net.h>
#include "SNServer.h"
#include <string.h>
#include "SNWorld.h"
#include "SNAnimator.h"

void SNServer::Setup()
{
	SDLNet_Init();

	SDLNet_ResolveHost(&ip, NULL, 6969);

	server = SDLNet_TCP_Open(&ip);
	if (!server && printErrors)
	{
		printf("Server Open: %s\n", SDLNet_GetError());
	}

	printf("Server Setup!\n");

	socketSet = SDLNet_AllocSocketSet(MAX_NUM_SOCKETS);
}

bool SNServer::AcceptConnection()
{
	if (!client)
	{
		if (!(client = SDLNet_TCP_Accept(server)) && printErrors)
		{
			printf("Server Accept: %s\n", SDLNet_GetError());
			return false;
		}

		if (client)
		{
			remoteIp = SDLNet_TCP_GetPeerAddress(client);
			if (!remoteIp && printErrors)
			{
				printf("Server Peer Address: %s\n", SDLNet_GetError());
			}

			if (printDebug)
			{
				printf("Accepted connection from: %d : %d\n", remoteIp->host, remoteIp->port);
			}
			SDLNet_TCP_AddSocket(socketSet, client);
			return true;
		}
	}
	return false;

}

bool SNServer::RecvData()
{
	if (client == nullptr)
		return false;

	int numReady;
	numReady = SDLNet_CheckSockets(socketSet, RECV_TIMEOUT_MS);
	if (numReady == -1 && printErrors)
	{
		printf("Server Check Sockets: %s\n", SDLNet_GetError());
	}
	else if (numReady && printDebug)
	{
		printf("There are %d sockets ready!\n", numReady);
	}

	if (numReady <= 0)
		return false;

	for (int i = 0; i < MAX_NUM_SOCKETS; ++i)
	{
		if (SDLNet_SocketReady(client))
		{
			Uint8* dataBuffer = InternalRecvData();
			if (dataBuffer == nullptr)
			{
				return false;
			}

			Uint8 flags;
			memcpy(&flags, dataBuffer, sizeof(Uint8));

			switch (flags)
			{
			case TRANSFORM_FLAG: {
				// Set Transform of Simulated Proxy
				int16_t posX;
				int16_t posY;
				int8_t flip;

				memcpy(&flip, dataBuffer + sizeof(Uint8), sizeof(int8_t));
				memcpy(&posX, dataBuffer + sizeof(Uint8) + sizeof(int8_t), sizeof(int16_t));
				memcpy(&posY, dataBuffer + sizeof(Uint8) + sizeof(int8_t) + sizeof(int16_t), sizeof(int16_t));

				world->simulatedProxy.SetPosition({ (float)posX, (float)posY });
				world->simulatedProxy.transform.SetFacingRight(flip < 0 ? true : false);
				return true;
			} break;

			case SP_STATE_FLAG: {
				// Set Simulated Proxy State
				Uint8 state;
				memcpy(&state, dataBuffer + sizeof(flags), sizeof(Uint8));
				world->simulatedProxy.SetState(state);
				return true;
			} break;

			case EVENT_FLAG: {
				// Call Event
				Uint8 event;
				memcpy(&event, dataBuffer + sizeof(flags), sizeof(Uint8));
				// CALL EVENTS USING EVENT CLASS
				world->eventHandler.InvokeEvent(event);
				return true;
			} break;

			default:
				return false;
				break;
			}
		}
	}
}

Uint8* SNServer::InternalRecvData()
{
	Uint8 recvData[20];
	int len = SDLNet_TCP_Recv(client, recvData, 20);

	if (!len && printErrors)
	{
		printf("Server Recieve: %s\n", SDLNet_GetError());
	}
	else if (len != -1)
	{
		Uint8 flags;
		memcpy(&flags, recvData, sizeof(Uint8));
		switch (flags)
		{
		case TRANSFORM_FLAG: {
			Uint8 retData[20];
			memcpy(retData, recvData, 20 * sizeof(Uint8));
			return retData;
		}

		case SP_STATE_FLAG: {
			Uint8 retData[4];
			memcpy(retData, recvData, 4 * sizeof(Uint8));
			return retData;
		}

		case EVENT_FLAG: {
			Uint8 retData[4];
			memcpy(retData, recvData, 4 * sizeof(Uint8));
			return retData;
		}

		default:
			return nullptr;
		}
	}
	return nullptr;
}

void SNServer::SendData(SNTransformPacket* data)
{
	if (client == nullptr)
		return;

	Uint8 buffer[20];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, &data->flip, sizeof(int8_t));
	offset += sizeof(int8_t);

	memcpy(buffer + offset, &data->posX, sizeof(int16_t));
	offset += sizeof(int16_t);

	memcpy(buffer + offset, &data->posY, sizeof(int16_t));

	SDLNet_TCP_Send(client, buffer, 20);
}

void SNServer::SendData(SNStatePacket* data)
{
	if (client == nullptr)
		return;

	Uint8 buffer[4];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, &data->state, sizeof(Uint8));

	SDLNet_TCP_Send(client, buffer, 4);
}

void SNServer::SendData(SNEventPacket* data)
{
	if (client == nullptr)
		return;

	Uint8 buffer[4];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, &data->eventFlag, sizeof(Uint8));

	SDLNet_TCP_Send(client, buffer, 4);
}

void SNServer::Close()
{
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(client);
}
