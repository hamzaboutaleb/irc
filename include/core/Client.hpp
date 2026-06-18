#pragma once
#include "network/Socket.hpp"
#include "core/Buffer.hpp"
#include "core/ClientInfo.hpp"

class Client
{
  Socket *_socket;
  Buffer _inBuffer;
  std::string _outBuffer;
  ClientInfo _info;

public:
  Client(Socket *socket);
  ~Client();

  int fd() const;
  Socket *socket() const;
  Buffer &inBuffer();
  ClientInfo &info();
  void        send(const std::string &msg);
  void        flushOutput();
  bool        hasPendingOutput() const;
};