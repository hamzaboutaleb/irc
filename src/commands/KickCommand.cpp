#include "commands/KickCommand.hpp"
#include "commands/Replies.hpp"
#include "commands/Context.hpp"
#include "core/Channel.hpp"
#include "core/Config.hpp"

void KickCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  if (!client->info().isRegistered())
  {
    client->send(Replies::notRegistered(nick));
    return;
  }
  if (msg.params.size() < 2)
  {
    client->send(Replies::needMoreParams(nick, "KICK"));
    return;
  }

  const std::string &chanName   = msg.params[0];
  const std::string &targetNick = msg.params[1];
  const std::string  reason     = msg.params.size() > 2 ? msg.params[2] : nick;

  ChannelMap::iterator it = ctx.channels.find(chanName);
  if (it == ctx.channels.end())
  {
    client->send(Replies::noSuchChannel(nick, chanName));
    return;
  }

  Channel *channel = it->second;

  if (!channel->hasMember(client))
  {
    client->send(Replies::notOnChannel(nick, chanName));
    return;
  }

  if (!channel->hasPermission(client, PERM_KICK))
  {
    client->send(Replies::chanOpPrivsNeeded(nick, chanName));
    return;
  }

  Client *target = ctx.findClient(targetNick);
  if (!target)
  {
    client->send(Replies::noSuchNick(nick, targetNick));
    return;
  }

  if (!channel->hasMember(target))
  {
    client->send(Replies::userNotInChannel(nick, targetNick, chanName));
    return;
  }

  std::string kickMsg = ":" + nick + "!" + client->info().username() +
                        "@" SERVER_HOST " KICK " + chanName + " " + targetNick +
                        " :" + reason + "\r\n";

  channel->broadcast(kickMsg, NULL);
  ctx.removeFromChannel(target, channel);
}