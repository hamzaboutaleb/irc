#include "commands/TopicCommand.hpp"
#include "commands/Replies.hpp"
#include "commands/Context.hpp"
#include "core/Channel.hpp"

void TopicCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  if (!client->info().isRegistered())
  {
    client->send(Replies::notRegistered(nick));
    return;
  }
  if (msg.params.empty())
  {
    client->send(Replies::needMoreParams(nick, "TOPIC"));
    return;
  }

  const std::string &chanName = msg.params[0];

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

  // get topic
  if (msg.params.size() == 1)
  {
    if (channel->topic().empty())
      client->send(Replies::noTopic(nick, chanName));
    else
      client->send(Replies::topic(nick, chanName, channel->topic()));
    return;
  }

  // set topic
  if (channel->isTopicLocked() && !channel->hasPermission(client, PERM_TOPIC))
  {
    client->send(Replies::chanOpPrivsNeeded(nick, chanName));
    return;
  }

  channel->setTopic(msg.params[1]);
  channel->broadcast(
    Replies::topicChange(nick, client->info().username(), chanName, msg.params[1]),
    NULL
  );
}