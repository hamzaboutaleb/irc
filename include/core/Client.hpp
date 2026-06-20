#pragma once
#include "network/Socket.hpp"
#include "network/Epoll.hpp"
#include "core/Buffer.hpp"
#include "core/ClientInfo.hpp"

class Client
{
  Socket *_socket;
  Buffer _inBuffer;
  std::string _outBuffer;
  ClientInfo _info;
  bool _dead;

public:
  Client(Socket *socket);
  ~Client();

  int fd() const;
  Socket *socket() const;
  Buffer &inBuffer();
  ClientInfo &info();
  void        send(const std::string &msg);
  void        tryRegister();
  void        flushOutput();
  bool        hasPendingOutput() const;
  void        markDead();
  bool        isDead() const;
};