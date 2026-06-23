#include "core/Client.hpp"
#include "core/Buffer.hpp"
#include "core/Config.hpp"
#include "network/IoResult.hpp"
#include "commands/Replies.hpp"

Client::Client(Socket* socket) : _socket(socket), _dead(false) {}

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
  flushOutput();
}

// TODO: add _watchingWrite bool to Client to avoid redundant epoll_ctl calls in flushOutput
void Client::flushOutput()
{
  while (!_outBuffer.empty())
  {
    IoResult result = _socket->send(_outBuffer.c_str(), _outBuffer.size());
    if (result.status == IO_OK)
      _outBuffer.erase(0, result.bytes);
    else
      break;
  }
  if (_outBuffer.empty())
    Epoll::instance().mod(_socket->fd(), EPOLLIN);
  else
    Epoll::instance().mod(_socket->fd(), EPOLLIN | EPOLLOUT);
}

bool Client::hasPendingOutput() const
{
  return !_outBuffer.empty();
}

void Client::markDead() { _dead = true; }
bool Client::isDead() const { return _dead; }

void Client::tryRegister()
{
  if (_info.isRegistered() || !_info.canRegister())
    return;
  _info.markRegistered();
  send(Replies::welcome(_info.nickname(), _info.username(), SERVER_HOST));
}