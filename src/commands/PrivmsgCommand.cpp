#include "commands/PrivmsgCommand.hpp"
#include "commands/Context.hpp"
#include "commands/Replies.hpp"
#include "core/Channel.hpp"
#include "core/Config.hpp"

static std::string makePrefix(Client *client)
{
  return ":" + client->info().nickname() + "!" +
         client->info().username() + "@" SERVER_HOST;
}

void PrivmsgCommand::_toChannel(Client *client, const std::string &target,
                                const std::string &text, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  ChannelMap::iterator it = ctx.channels.find(target);
  if (it == ctx.channels.end())
  {
    client->send(Replies::noSuchChannel(nick, target));
    return;
  }
  Channel *channel = it->second;
  if (!channel->hasMember(client))
  {
    client->send(Replies::cannotSendToChan(nick, target));
    return;
  }
  std::string msg = makePrefix(client) + " PRIVMSG " + target + " :" + text + "\r\n";
  channel->broadcast(msg, client);
}

void PrivmsgCommand::_toClient(Client *client, const std::string &target,
                               const std::string &text, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  Client *dest = ctx.findClient(target);
  if (!dest)
  {
    client->send(Replies::noSuchNick(nick, target));
    return;
  }
  std::string msg = makePrefix(client) + " PRIVMSG " + target + " :" + text + "\r\n";
  dest->send(msg);
}

void PrivmsgCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  if (!client->info().isRegistered())
  {
    client->send(Replies::notRegistered(nick));
    return;
  }
  if (msg.params.size() < 2)
  {
    client->send(Replies::needMoreParams(nick, "PRIVMSG"));
    return;
  }
  if (msg.params[1].empty())
  {
    client->send(Replies::noTextToSend(nick));
    return;
  }

  const std::string &target = msg.params[0];
  const std::string &text   = msg.params[1];

  if (target.empty())
  {
    client->send(Replies::noSuchNick(nick, target));
    return;
  }
  if (target[0] == '#')
    _toChannel(client, target, text, ctx);
  else
    _toClient(client, target, text, ctx);
}