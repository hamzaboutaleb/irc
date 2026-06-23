#pragma once
#include "commands/ICommand.hpp"

class TopicCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);
};