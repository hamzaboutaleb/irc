#pragma once
#include "Socket.hpp"
#include "Client.hpp"
#include <map>

class Server
{
  Socket *listener;
  std::map<int, Client *> clients; // fd, client
};