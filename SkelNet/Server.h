#pragma once
#include <SDL_net.h>
#include "DataPacket.h"

class Server
{
public:
	static const int MAX_NUM_SOCKETS = 1;
	static const int RECV_TIMEOUT_MS = 0;

	void Setup();
	void AcceptConnection();
	bool RecvData();
	void SendData();
	void Close();

	DataPacket transformPack;
	DataPacket recievedData;

	TCPsocket server;
	TCPsocket client;
	IPaddress ip;
	IPaddress* remoteIp;

	SDLNet_SocketSet socketSet;
};