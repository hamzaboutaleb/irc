#pragma once
#include "network/Socket.hpp"
#include "network/Epoll.hpp"
#include "core/Client.hpp"
#include "core/Channel.hpp"
#include "commands/CommandHandler.hpp"
#include <map>
#include <set>
#include <string>

class Server
{
  Socket *_listener;
  std::map<int, Client *> _clients;
  std::map<std::string, Channel *> _channels;
  std::set<int> _pendingRemoval;
  int _port;
  std::string _password;
  CommandHandler _handler;

public:
  Server(int port, const std::string &password);
  ~Server();

  void start(bool &running);
  void stop();

private:
  void _loop(bool &running);
  void _acceptClient();
  void _handleRead(int fd);
  void _processClient(int fd, const char *data, size_t len);
  void _handleWrite(int fd);
  void _scheduleRemoval(int fd);
  void _flushRemovals();
  void _removeClient(int fd);
};