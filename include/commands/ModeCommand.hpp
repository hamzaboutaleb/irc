#pragma once
#include "commands/ICommand.hpp"
#include "core/Channel.hpp"
#include <string>

class ModeCommand : public ICommand
{
  bool _applyMode(Client *client, Channel *channel, char sign, char flag,
                  const std::vector<std::string> &params, size_t &paramIdx, Context &ctx);

public:
  void execute(Client *client, const Message &msg, Context &ctx);
};