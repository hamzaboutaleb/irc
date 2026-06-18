#pragma once
#include "core/Client.hpp"
#include "core/Message.hpp"
#include "commands/Context.hpp"

class ICommand
{
public:
  virtual ~ICommand() {}
  virtual void execute(Client *client, const Message &msg, Context &ctx) = 0;
};