#pragma once
#include <SDL_net.h>
#include "SNDataPacket.h"
#include "SNDataPackets.h"

class SNWorld;

class SNClient
{
public:
	static const int MAX_NUM_SOCKETS = 1;
	static const int RECV_TIMEOUT_MS = 0;

	void Setup();
	void Close();

	void SendData(SNTransformPacket* data);
	void SendData(SNStatePacket* data);
	
	bool RecvData();
	Uint8* InternalRecvData();

	TCPsocket tcpsock;
	IPaddress ip;

	SDLNet_SocketSet socketSet;

	bool printDebug;
	bool printErrors;

	SNWorld* world;
};