#pragma once
#include "commands/ICommand.hpp"

class InviteCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);
};