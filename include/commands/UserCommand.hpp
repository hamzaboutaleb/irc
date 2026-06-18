#pragma once
#include "commands/ICommand.hpp"

class UserCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);
};