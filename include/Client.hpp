#pragma once
#include "network/Socket.hpp"
#include "Buffer.hpp"
#include "ClientInfo.hpp"

class Client
{
  Socket *_socket;
  Buffer _inBuffer;
  ClientInfo _info;

public:
  Client(Socket *socket);
  ~Client();

  int fd() const;
  Socket *socket() const;
  Buffer &inBuffer();
  ClientInfo &info();
};