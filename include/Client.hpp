#pragma once
#include "network/Socket.hpp"
#include "Buffer.hpp"

class Client
{
  Socket* _socket;
  Buffer  _inBuffer;

public:
  Client(Socket* socket);
  ~Client();

  int     fd()     const;
  Socket* socket() const;

  Buffer& inBuffer();
};