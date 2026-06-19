#pragma once
#include "commands/ICommand.hpp"

class PrivmsgCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);

private:
  void _toChannel(Client *client, const std::string &target,
                  const std::string &text, Context &ctx);
  void _toClient(Client *client, const std::string &target,
                 const std::string &text, Context &ctx);
};