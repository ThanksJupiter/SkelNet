#pragma once
#include <SDL_net.h>
#include "SNDataPackets.h"

class SNWorld;

class SNServer
{
public:
	static const int MAX_NUM_SOCKETS = 1;
	static const int RECV_TIMEOUT_MS = 0;

	void Setup();
	void Close();
	bool AcceptConnection();
	bool allreadySetUp = false;

	void SendData(SNTransformPacket* data);
	void SendData(SNStatePacket* data);
	void SendData(SNEventPacket* data);

	bool RecvData();
	Uint8* InternalRecvData();

	TCPsocket server;
	TCPsocket client;
	IPaddress ip;
	IPaddress* remoteIp;

	SDLNet_SocketSet socketSet;

	bool printDebug;
	bool printErrors;

	SNWorld* world;
};