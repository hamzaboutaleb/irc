#include "commands/ModeCommand.hpp"
#include "commands/Replies.hpp"
#include "commands/Context.hpp"
#include "core/Config.hpp"
#include <cstdlib>

void ModeCommand::_applyMode(Client *client, Channel *channel, char sign, char flag,
                             const std::vector<std::string> &params, size_t &paramIdx, Context &ctx)
{
  const std::string &nick     = client->info().nickname();
  const std::string &chanName = channel->name();

  switch (flag)
  {
    case 'i':
      channel->setInviteOnly(sign == '+');
      break;

    case 't':
      channel->setTopicLocked(sign == '+');
      break;

    case 'k':
      if (sign == '+')
      {
        if (paramIdx >= params.size())
        {
          client->send(Replies::needMoreParams(nick, "MODE"));
          return;
        }
        channel->setKey(params[paramIdx++]);
      }
      else
        channel->setKey("");
      break;

    case 'l':
      if (sign == '+')
      {
        if (paramIdx >= params.size())
        {
          client->send(Replies::needMoreParams(nick, "MODE"));
          return;
        }
        int limit = std::atoi(params[paramIdx++].c_str());
        if (limit > 0)
          channel->setLimit(limit);
      }
      else
        channel->setLimit(0);
      break;

    case 'o':
    {
      if (paramIdx >= params.size())
      {
        client->send(Replies::needMoreParams(nick, "MODE"));
        return;
      }
      const std::string &targetNick = params[paramIdx++];
      Client            *target     = ctx.findClient(targetNick);
      if (!target)
      {
        client->send(Replies::noSuchNick(nick, targetNick));
        return;
      }
      if (!channel->hasMember(target))
      {
        client->send(Replies::notOnChannel(nick, chanName));
        return;
      }
      channel->setRole(target, sign == '+' ? ROLE_OPERATOR : ROLE_MEMBER);
      break;
    }

    default:
      break;
  }
}

void ModeCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  if (!client->info().isRegistered())
  {
    client->send(Replies::notRegistered(nick));
    return;
  }
  if (msg.params.empty())
  {
    client->send(Replies::needMoreParams(nick, "MODE"));
    return;
  }

  const std::string &chanName = msg.params[0];

  std::map<std::string, Channel *>::iterator it = ctx.channels.find(chanName);
  if (it == ctx.channels.end())
  {
    client->send(Replies::noSuchChannel(nick, chanName));
    return;
  }

  Channel *channel = it->second;

  if (!channel->hasPermission(client, PERM_OPERATOR))
  {
    client->send(Replies::chanOpPrivsNeeded(nick, chanName));
    return;
  }

  if (msg.params.size() < 2)
    return;

  const std::string &modeStr = msg.params[1];
  char               sign    = '+';
  size_t             paramIdx = 2;

  for (size_t i = 0; i < modeStr.size(); ++i)
  {
    char c = modeStr[i];
    if (c == '+' || c == '-')
      sign = c;
    else
      _applyMode(client, channel, sign, c, msg.params, paramIdx, ctx);
  }
}