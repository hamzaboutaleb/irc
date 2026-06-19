#pragma once
#include "core/Client.hpp"
#include "core/Channel.hpp"
#include <map>
#include <string>

struct Context
{
  std::map<int, Client *> &clients;
  std::map<std::string, Channel *> &channels;
  const std::string &password;

  Context(std::map<int, Client *> &clients,
          std::map<std::string, Channel *> &channels,
          const std::string &password);

  Client *findClient(const std::string &nick) const;
  void    removeFromChannel(Client *client, Channel *channel);
};