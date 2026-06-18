#pragma once
#include "Client.hpp"
#include "Message.hpp"
#include "Context.hpp"

class ICommand
{
public:
  virtual ~ICommand() {}
  virtual void execute(Client *client, const Message &msg, Context &ctx) = 0;
};