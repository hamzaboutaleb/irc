#include "commands/PartCommand.hpp"
#include "commands/Context.hpp"
#include "commands/Replies.hpp"
#include "core/Channel.hpp"
#include "core/Config.hpp"

void PartCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  if (!client->info().isRegistered())
  {
    client->send(Replies::notRegistered(nick));
    return;
  }
  if (msg.params.empty())
  {
    client->send(Replies::needMoreParams(nick, "PART"));
    return;
  }

  const std::string &name   = msg.params[0];
  const std::string  reason = msg.params.size() > 1 ? msg.params[1] : "";

  std::map<std::string, Channel *>::iterator it = ctx.channels.find(name);
  if (it == ctx.channels.end())
  {
    client->send(Replies::noSuchChannel(nick, name));
    return;
  }

  Channel *channel = it->second;
  if (!channel->hasMember(client))
  {
    client->send(Replies::notOnChannel(nick, name));
    return;
  }

  std::string partMsg = ":" + nick + "!" + client->info().username() +
                        "@" SERVER_HOST " PART " + name;
  if (!reason.empty())
    partMsg += " :" + reason;
  partMsg += "\r\n";

  channel->broadcast(partMsg, NULL);
  ctx.removeFromChannel(client, channel);
}