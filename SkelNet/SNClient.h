#pragma once
#include <SDL_net.h>
#include "SNDataPacket.h"

class SNClient
{
public:
	static const int MAX_NUM_SOCKETS = 1;
	static const int RECV_TIMEOUT_MS = 0;

	void Setup();
	bool RecvData();
	void SendData();
	void Close();

	SNDataPacket statePack;
	SNDataPacket recievedData;

	TCPsocket tcpsock;
	IPaddress ip;

	SDLNet_SocketSet socketSet;

	bool printDebug;
	bool printErrors;
};