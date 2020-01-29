#include <SDL.h>
#include <SDL_net.h>
#include "SNClient.h"
#include <string.h>
#include <stdio.h>
#include "SNDataPackets.h"
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
	if (tcpsock == nullptr)
		return false;

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
				Uint16 posX;
				Uint16 posY;
				int8_t flip;

				memcpy(&flip, dataBuffer + sizeof(Uint8), sizeof(int8_t));
				memcpy(&posX, dataBuffer + sizeof(Uint8) + sizeof(int8_t), sizeof(Uint16));
				memcpy(&posY, dataBuffer + sizeof(Uint8) + sizeof(int8_t) + sizeof(Uint16), sizeof(Uint16));

				world->simulatedProxy.SetPosition({ (float)posX, (float)posY });
				world->simulatedProxy.flip = flip > 0 ? true : false;
			} break;

			case SP_STATE_FLAG: {
				// Set Simulated Proxy State
				Uint8 state;
				memcpy(&state, dataBuffer + sizeof(flags), sizeof(Uint8));
				//world->simulatedProxy.SetState(state);
				return true;
			} break;

			case AP_STATE_FLAG: {
				// Set Autonomous Proxy State
				Uint8 state;
				memcpy(&state, dataBuffer + sizeof(flags), sizeof(Uint8));
				//world->autonomousProxy.SetState(state);
				return true;
			} break;

			default:
				return false;
				break;
			}
		}
	}
}

Uint8* SNClient::InternalRecvData()
{
	Uint8 recvData[20]; // sizeof largest packet
	int len = SDLNet_TCP_Recv(tcpsock, recvData, 20);
	printf("Client Recieved data!\n");

	if (!len && printErrors)
	{
		printf("Client Recieve: %s\n", SDLNet_GetError());
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
			Uint8 retData[20];
			memcpy(retData, recvData, sizeof(SNStatePacket));
			return retData;
		}

		case AP_STATE_FLAG: {
			Uint8 retData[20];
			memcpy(retData, recvData, sizeof(SNStatePacket));
			return retData;
		}

		default:
			return nullptr;
		}
	}
	return nullptr;
}

void SNClient::SendData(SNTransformPacket* data)
{
	if (tcpsock == nullptr)
		return;

	Uint8 buffer[20];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, &data->flip, sizeof(int8_t));
	offset += sizeof(int8_t);

	memcpy(buffer + offset, &data->posX, sizeof(Uint16));
	offset += sizeof(Uint16);

	memcpy(buffer + offset, &data->posY, sizeof(Uint16));

	SDLNet_TCP_Send(tcpsock, buffer, 20);
}

void SNClient::SendData(SNStatePacket* data)
{
	char buffer[4];
	//sprintf_s(buffer, "%hu %c", data->state, data->flag);
	//InternalSendData(buffer, sizeof(buffer));
}

void SNClient::Close()
{
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(tcpsock);
}
