#include "Client.hpp"
#include "Buffer.hpp"

Client::Client(Socket* socket) : _socket(socket) {}

Client::~Client()
{
  delete _socket;
}

int Client::fd() const
{
  return _socket->fd();
}

Socket* Client::socket() const
{
  return _socket;
}

Buffer& Client::inBuffer()
{
  return _inBuffer;
}

ClientInfo& Client::info()
{
  return _info;
}