#include "commands/InviteCommand.hpp"
#include "commands/Replies.hpp"
#include "commands/Context.hpp"
#include "core/Channel.hpp"

void InviteCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  if (!client->info().isRegistered())
  {
    client->send(Replies::notRegistered(nick));
    return;
  }
  if (msg.params.size() < 2)
  {
    client->send(Replies::needMoreParams(nick, "INVITE"));
    return;
  }

  const std::string &targetNick = msg.params[0];
  const std::string &chanName   = msg.params[1];

  std::map<std::string, Channel *>::iterator it = ctx.channels.find(chanName);
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

  if (!channel->hasPermission(client, PERM_INVITE))
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

  if (channel->hasMember(target))
  {
    client->send(Replies::userOnChannel(nick, targetNick, chanName));
    return;
  }

  channel->addInvite(targetNick);
  client->send(Replies::inviting(nick, targetNick, chanName));
  target->send(Replies::inviteMsg(nick, client->info().username(), targetNick, chanName));
}