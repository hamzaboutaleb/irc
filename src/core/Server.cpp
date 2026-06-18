#include "core/Server.hpp"
#include "core/Message.hpp"
#include <sys/epoll.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>

#define MAX_EVENTS      64
#define MAX_MSG_LENGTH  512

Server::Server(int port, const std::string& password)
  : _listener(NULL), _port(port), _password(password), _handler(_clients, _password) {}

Server::~Server()
{
  stop();
}

void Server::start()
{
  _listener = Socket::createTcp();
  _listener->setNonBlocking();
  _listener->bind(_port);
  _listener->listen(128);
  _epoll.add(_listener->fd(), EPOLLIN);
  _loop();
}

void Server::stop()
{
  std::map<int, Client*>::iterator it;
  for (it = _clients.begin(); it != _clients.end(); ++it)
    delete it->second;
  _clients.clear();

  delete _listener;
  _listener = NULL;
}

void Server::_loop()
{
  epoll_event events[MAX_EVENTS];

  while (true)
  {
    int n = _epoll.wait(events, MAX_EVENTS);
    for (int i = 0; i < n; ++i)
    {
      int fd     = events[i].data.fd;
      uint32_t ev = events[i].events;

      if (fd == _listener->fd())
        _acceptClient();
      else if (ev & (EPOLLHUP | EPOLLERR))
        _removeClient(fd);
      else if (ev & EPOLLIN)
        _handleRead(fd);
    }
  }
}

void Server::_acceptClient()
{
  Socket* clientSocket = _listener->accept();
  try { clientSocket->setNonBlocking(); }
  catch (...) { delete clientSocket; throw; }
  int fd = clientSocket->fd();
  _clients[fd] = new Client(clientSocket);
  _epoll.add(fd, EPOLLIN);
  std::cout << "client connected: fd=" << fd << std::endl;
}

void Server::_handleRead(int fd)
{
  char     buf[MAX_MSG_LENGTH];
  IoResult result = _clients[fd]->socket()->recv(buf, sizeof(buf));

  if (result.status == IO_OK)
    _processClient(fd, buf, result.bytes);
  else if (result.status == IO_CLOSED || result.status == IO_ERROR)
    _removeClient(fd);
}

void Server::_processClient(int fd, const char* data, size_t len)
{
  Client* client = _clients[fd];
  client->inBuffer().append(data, len);
  if (!client->inBuffer().hasMessage() && client->inBuffer().size() > MAX_MSG_LENGTH)
  {
    _removeClient(fd);
    return;
  }
  while (client->inBuffer().hasMessage())
  {
    std::string raw = client->inBuffer().extractMessage();
    Message     msg = Message::parse(raw);
    std::cout << "fd=" << fd << " " << msg << std::endl;
    _handler.handle(client, msg);
  }
}

void Server::_removeClient(int fd)
{
  _epoll.del(fd);
  delete _clients[fd];
  _clients.erase(fd);
  std::cout << "client disconnected: fd=" << fd << std::endl;
}