#include "core/Server.hpp"
#include "core/Message.hpp"
#include <sys/epoll.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <iostream>

#define MAX_EVENTS 64
#define MAX_MSG_LENGTH 512

Server::Server(int port, const std::string &password)
    : _listener(NULL), _port(port), _password(password), _handler(_clients, _channels, _password) {}

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
  Epoll::instance().add(_listener->fd(), EPOLLIN);
  _loop();
}

void Server::stop()
{
  std::map<int, Client *>::iterator it;
  for (it = _clients.begin(); it != _clients.end(); ++it)
    delete it->second;
  _clients.clear();

  std::map<std::string, Channel *>::iterator ch;
  for (ch = _channels.begin(); ch != _channels.end(); ++ch)
    delete ch->second;
  _channels.clear();

  delete _listener;
  _listener = NULL;
}

void Server::_scheduleRemoval(int fd)
{
  _pendingRemoval.insert(fd);
}

void Server::_flushRemovals()
{
  std::set<int>::iterator it;
  for (it = _pendingRemoval.begin(); it != _pendingRemoval.end(); ++it)
    _removeClient(*it);
  _pendingRemoval.clear();
}

void Server::_loop()
{
  epoll_event events[MAX_EVENTS];

  while (true)
  {
    int n = Epoll::instance().wait(events, MAX_EVENTS);
    for (int i = 0; i < n; ++i)
    {
      int fd = events[i].data.fd;
      uint32_t ev = events[i].events;

      if (_pendingRemoval.count(fd))
        continue;
      if (fd == _listener->fd())
        _acceptClient();
      else if (ev & (EPOLLHUP | EPOLLERR))
        _scheduleRemoval(fd);
      else if (ev & EPOLLIN)
        _handleRead(fd);
      else if (ev & EPOLLOUT)
        _handleWrite(fd);
    }
    _flushRemovals();
  }
}

void Server::_acceptClient()
{
  Socket *clientSocket = _listener->accept();
  try
  {
    clientSocket->setNonBlocking();
  }
  catch (...)
  {
    delete clientSocket;
    throw;
  }
  int fd = clientSocket->fd();
  _clients[fd] = new Client(clientSocket);
  Epoll::instance().add(fd, EPOLLIN);
  std::cout << "client connected: fd=" << fd << std::endl;
}

void Server::_handleRead(int fd)
{
  char buf[MAX_MSG_LENGTH];
  IoResult result = _clients[fd]->socket()->recv(buf, sizeof(buf));

  if (result.status == IO_OK)
    _processClient(fd, buf, result.bytes);
  else if (result.status == IO_CLOSED || result.status == IO_ERROR)
    _scheduleRemoval(fd);
}

void Server::_processClient(int fd, const char *data, size_t len)
{
  Client *client = _clients[fd];
  client->inBuffer().append(data, len);
  if (!client->inBuffer().hasMessage() && client->inBuffer().size() > MAX_MSG_LENGTH)
  {
    _scheduleRemoval(fd);
    return;
  }
  while (client->inBuffer().hasMessage())
  {
    if (_pendingRemoval.count(fd))
      break;
    std::string raw = client->inBuffer().extractMessage();
    Message msg = Message::parse(raw);
    std::cout << "fd=" << fd << " " << msg << std::endl;
    _handler.handle(client, msg);
    if (client->isDead())
    {
      _scheduleRemoval(fd);
      break;
    }
  }
}

void Server::_handleWrite(int fd)
{
  _clients[fd]->flushOutput();
}

void Server::_removeClient(int fd)
{
  Client *client = _clients[fd];

  std::map<std::string, Channel *>::iterator it = _channels.begin();
  while (it != _channels.end())
  {
    Channel *channel = it->second;
    if (channel->hasMember(client))
    {
      channel->removeMember(client);
      if (channel->memberCount() == 0)
      {
        std::map<std::string, Channel *>::iterator next = it;
        ++next;
        delete channel;
        _channels.erase(it);
        it = next;
        continue;
      }
    }
    ++it;
  }

  Epoll::instance().del(fd);
  delete client;
  _clients.erase(fd);
  std::cout << "client disconnected: fd=" << fd << std::endl;
}