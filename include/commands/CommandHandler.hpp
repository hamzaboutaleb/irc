#pragma once
#include "commands/ICommand.hpp"
#include "commands/Context.hpp"
#include "core/Channel.hpp"
#include "core/Message.hpp"
#include "core/Client.hpp"
#include <map>
#include <string>

class CommandHandler
{
  Context _ctx;
  std::map<std::string, ICommand *> _commands;

public:
  CommandHandler(std::map<int, Client *> &clients,
                 std::map<std::string, Channel *> &channels,
                 const std::string &password);
  ~CommandHandler();

  void handle(Client *client, const Message &msg);

private:
  void _registerCommands();
};