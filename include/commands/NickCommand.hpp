#pragma once
#include "commands/ICommand.hpp"

class NickCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);

private:
  bool _isValidNick(const std::string &nick) const;
  void _broadcastNickChange(Client *client, const std::string &oldNick, Context &ctx);
};