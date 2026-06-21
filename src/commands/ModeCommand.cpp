#include "commands/ModeCommand.hpp"
#include "commands/Replies.hpp"
#include "commands/Context.hpp"
#include "core/Config.hpp"

void ModeCommand::_applyMode(Client *client, Channel *channel, char sign, char flag,
                             const std::vector<std::string> &params, size_t &paramIdx, Context &ctx)
{
  (void)client;
  (void)channel;
  (void)sign;
  (void)flag;
  (void)params;
  (void)paramIdx;
  (void)ctx;
}

void ModeCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  (void)client;
  (void)msg;
  (void)ctx;
}