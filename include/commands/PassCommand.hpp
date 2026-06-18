#pragma once
#include "commands/ICommand.hpp"

class PassCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);
};