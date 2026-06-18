#include "core/Client.hpp"
#include "core/Buffer.hpp"
#include "network/IoResult.hpp"

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

void Client::send(const std::string &msg)
{
  _outBuffer += msg;
}

void Client::flushOutput()
{
  while (!_outBuffer.empty())
  {
    IoResult result = _socket->send(_outBuffer.c_str(), _outBuffer.size());
    if (result.status == IO_OK)
      _outBuffer.erase(0, result.bytes);
    else if (result.status == IO_WOULDBLOCK)
      break;
    else
      break;
  }
}

bool Client::hasPendingOutput() const
{
  return !_outBuffer.empty();
}