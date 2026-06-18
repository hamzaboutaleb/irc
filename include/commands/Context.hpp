#pragma once
#include "core/Client.hpp"
#include <map>
#include <string>

struct Context
{
  std::map<int, Client *> &clients;
  const std::string &password;

  Context(std::map<int, Client *> &clients,
          const std::string &password)
      : clients(clients), password(password) {}
};