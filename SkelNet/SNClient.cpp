#include <SDL.h>
#include <SDL_net.h>
#include "SNClient.h"
#include <string.h>
#include <stdio.h>
#include "SNDataPackets.h"
#include "SNWorld.h"
#include "SNSimulatedProxy.h"
#include "SNAnimator.h"
#include "SNServer.h"

bool SNClient::Setup(const char* ipAddress)
{
	SDLNet_Init();

	int res;
	res = SDLNet_ResolveHost(&ip, ipAddress, 6969);
	if (!res && printErrors)
	{
		printf("Client Connect: %s\n", SDLNet_GetError());
		return false;
	}

	tcpsock = SDLNet_TCP_Open(&ip);
	if (!tcpsock && printErrors)
	{
		printf("Client Open: %s\n", SDLNet_GetError());
		return false;
	}
	else if (!tcpsock)
	{
		return false;
	}

	printf("Client Setup!\n");

	socketSet = SDLNet_AllocSocketSet(MAX_NUM_SOCKETS);
	SDLNet_TCP_AddSocket(socketSet, tcpsock);

	return true;
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
				int16_t posX;
				int16_t posY;
				int8_t flip;

				memcpy(&flip, dataBuffer + sizeof(Uint8), sizeof(int8_t));
				memcpy(&posX, dataBuffer + sizeof(Uint8) + sizeof(int8_t), sizeof(int16_t));
				memcpy(&posY, dataBuffer + sizeof(Uint8) + sizeof(int8_t) + sizeof(int16_t), sizeof(int16_t));

				world->simulatedProxy.SetPosition({ (float)posX, (float)posY });
				world->simulatedProxy.transform.SetFacingRight(flip > 0 ? true : false);
			} break;

			case SP_STATE_FLAG: {
				// Set Simulated Proxy State
				Uint8 state;
				memcpy(&state, dataBuffer + sizeof(flags), sizeof(Uint8));
				world->simulatedProxy.SetState(state);
				return true;
			} break;

			case AP_STATE_FLAG: {
				// Set Autonomous Proxy State
				Uint8 state;
				memcpy(&state, dataBuffer + sizeof(flags), sizeof(Uint8));
				world->autonomousProxy.SetState(state);
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

			case STRING_FLAG: {
				char string[15];
				memcpy(string, dataBuffer + sizeof(flags), (sizeof(char) * 15) - sizeof(Uint8));
				
				world->simProxyNameText->UpdateText(string);
				//printf(string);
				return true;
			} break;

			case HEALTH_FLAG: {
				Uint8 serverHealth;
				Uint8 serverStocks;
				Uint8 clientHealth;
				Uint8 clientStocks;
				memcpy(&serverHealth, dataBuffer + sizeof(flags), sizeof(Uint8));
				memcpy(&serverStocks, dataBuffer + sizeof(flags) + sizeof(Uint8), sizeof(Uint8));
				memcpy(&clientHealth, dataBuffer + sizeof(flags) + (sizeof(Uint8) * 2), sizeof(Uint8));
				memcpy(&clientStocks, dataBuffer + sizeof(flags) + (sizeof(Uint8) * 3), sizeof(Uint8));

				// Autonomous proxy is client
				world->autonomousProxy.SetHealthAndStocks(clientHealth, clientStocks);
				world->simulatedProxy.SetHealthAndStocks(serverHealth, serverStocks);
				return true;
			} break;

			case DOOT_FLAG: {
				Uint8 dootFlag;
				memcpy(&dootFlag, dataBuffer + sizeof(flags), sizeof(Uint8));
				world->simulatedProxy.PlayDoot(dootFlag);
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
			Uint8 retData[4];
			memcpy(retData, recvData, 4 * sizeof(Uint8));
			return retData;
		}

		case AP_STATE_FLAG: {
			Uint8 retData[4];
			memcpy(retData, recvData, 4 * sizeof(Uint8));
			return retData;
		}

		case EVENT_FLAG: {
			Uint8 retData[4];
			memcpy(retData, recvData, 4 * sizeof(Uint8));
			return retData;
		}

		case STRING_FLAG: {
			Uint8 retData[15];
			memcpy(retData, recvData, 15 * sizeof(Uint8));
			return retData;
		}

		case HEALTH_FLAG: {
			Uint8 retData[10];
			memcpy(retData, recvData, 10 * sizeof(Uint8));
			return retData;
		}

		case DOOT_FLAG: {
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

	memcpy(buffer + offset, &data->posX, sizeof(int16_t));
	offset += sizeof(int16_t);

	memcpy(buffer + offset, &data->posY, sizeof(int16_t));

	SDLNet_TCP_Send(tcpsock, buffer, 20);
}

void SNClient::SendData(SNStatePacket* data)
{
	if (tcpsock == nullptr)
		return;

	Uint8 buffer[4];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, &data->state, sizeof(Uint8));

	SDLNet_TCP_Send(tcpsock, buffer, 4);
}

void SNClient::SendData(SNEventPacket* data)
{
	if (tcpsock == nullptr)
		return;

	Uint8 buffer[4];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, &data->eventFlag, sizeof(Uint8));

	SDL_Delay(10);
	SDLNet_TCP_Send(tcpsock, buffer, 4);
}

void SNClient::SendData(SNStringPacket* data)
{
	if (tcpsock == nullptr)
		return;

	char buffer[15];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, data->string, (sizeof(char) * 15) - sizeof(Uint8));

	SDL_Delay(10);
	SDLNet_TCP_Send(tcpsock, buffer, 15);
}

void SNClient::SendData(SNDootPacket* data)
{
	if (tcpsock == nullptr)
		return;

	Uint8 buffer[4];
	int offset = 0;
	memcpy(buffer, &data->flag, sizeof(Uint8));
	offset += sizeof(Uint8);

	memcpy(buffer + offset, &data->dootFlag, sizeof(Uint8));

	SDL_Delay(10);
	SDLNet_TCP_Send(tcpsock, buffer, 4);
}

void SNClient::Close()
{
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(tcpsock);
}
