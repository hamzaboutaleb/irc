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
          const std::string &password)
      : clients(clients), channels(channels), password(password) {}
};