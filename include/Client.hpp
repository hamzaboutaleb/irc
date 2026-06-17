#pragma once
#include "network/Socket.hpp"

class Client
{
  Socket* _socket;

public:
  Client(Socket* socket);
  ~Client();

  int fd() const;
};