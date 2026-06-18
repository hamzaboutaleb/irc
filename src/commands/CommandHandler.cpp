#include "commands/CommandHandler.hpp"
#include "commands/PassCommand.hpp"
#include "commands/NickCommand.hpp"
#include "commands/UserCommand.hpp"
#include <iostream>

CommandHandler::CommandHandler(std::map<int, Client *> &clients,
                               const std::string &password)
    : _ctx(clients, password)
{
  _registerCommands();
}

CommandHandler::~CommandHandler()
{
  std::map<std::string, ICommand *>::iterator it;
  for (it = _commands.begin(); it != _commands.end(); ++it)
    delete it->second;
}

void CommandHandler::_registerCommands()
{
  _commands["PASS"] = new PassCommand();
  _commands["NICK"] = new NickCommand();
  _commands["USER"] = new UserCommand();
}

void CommandHandler::handle(Client *client, const Message &msg)
{
  if (msg.command.empty())
    return;

  std::map<std::string, ICommand *>::iterator it = _commands.find(msg.command);
  if (it == _commands.end())
  {
    std::cout << "unknown command: " << msg.command << std::endl;
    return;
  }
  it->second->execute(client, msg, _ctx);
}