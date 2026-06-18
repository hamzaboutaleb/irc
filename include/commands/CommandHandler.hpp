#pragma once
#include "ICommand.hpp"
#include "Context.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include <map>
#include <string>

class CommandHandler
{
  Context _ctx;
  std::map<std::string, ICommand *> _commands;

public:
  CommandHandler(std::map<int, Client *> &clients,
                 const std::string &password);
  ~CommandHandler();

  void handle(Client *client, const Message &msg);

private:
  void _registerCommands();
};