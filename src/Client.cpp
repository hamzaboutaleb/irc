#include "Client.hpp"

Client::Client(Socket* socket) : _socket(socket) {}

Client::~Client()
{
  delete _socket;
}

int Client::fd() const
{
  return _socket->fd();
}