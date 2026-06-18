#pragma once
#include "commands/ICommand.hpp"

class NickCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);

private:
  bool _isValidNick(const std::string &nick) const;
  bool _isNickInUse(const std::string &nick, Context &ctx) const;
};