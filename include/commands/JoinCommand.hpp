#pragma once
#include "commands/ICommand.hpp"

class JoinCommand : public ICommand
{
public:
  void execute(Client *client, const Message &msg, Context &ctx);

private:
  bool _isValidChannelName(const std::string &name) const;
  void _joinChannel(Client *client, const std::string &name, const std::string &key, Context &ctx);
  void _sendJoinReplies(Client *client, Channel *channel) const;
};