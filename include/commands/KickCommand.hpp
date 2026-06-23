#pragma once
#include "commands/ICommand.hpp"

class KickCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);
};