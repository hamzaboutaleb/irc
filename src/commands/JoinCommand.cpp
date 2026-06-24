#include "commands/JoinCommand.hpp"
#include "commands/Replies.hpp"
#include "core/Channel.hpp"
#include "core/Config.hpp"

bool JoinCommand::_isValidChannelName(const std::string &name) const
{
  if (name.size() < 2 || name.size() > 50)
    return false;
  if (name[0] != '#')
    return false;
  for (size_t i = 1; i < name.size(); ++i)
  {
    unsigned char c = static_cast<unsigned char>(name[i]);
    if (c == '\0' || c < 32 || c == ' ' || c == ',' || c == '\007')
      return false;
  }
  return true;
}

void JoinCommand::_sendJoinReplies(Client *client, Channel *channel) const
{
  const std::string &nick = client->info().nickname();
  const std::string &user = client->info().username();

  // JOIN confirmation broadcast to all including the joiner
  std::string joinMsg = ":" + nick + "!" + user + "@" SERVER_HOST " JOIN " + channel->name() + "\r\n";
  channel->broadcast(joinMsg, NULL);

  // topic
  if (channel->topic().empty())
    client->send(Replies::noTopic(nick, channel->name()));
  else
    client->send(Replies::topic(nick, channel->name(), channel->topic()));

  // TODO: split names into chunks of ~400 chars to handle large channels (multiple 353 replies)
  // build names list
  std::string names;
  const std::map<Client*, ChannelRole> &members = channel->members();
  std::map<Client*, ChannelRole>::const_iterator it;
  for (it = members.begin(); it != members.end(); ++it)
  {
    if (!names.empty())
      names += " ";
    if (it->second == ROLE_OPERATOR)
      names += "@";
    names += it->first->info().nickname();
  }

  client->send(Replies::namesList(nick, channel->name(), names));
  client->send(Replies::endOfNames(nick, channel->name()));
}

void JoinCommand::_joinChannel(Client *client, const std::string &name, const std::string &key, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  std::map<std::string, Channel *>::iterator it = ctx.channels.find(name);

  if (it == ctx.channels.end())
  {
    // channel doesn't exist — create it, client is operator
    Channel *channel = new Channel(name);
    ctx.channels[name] = channel;
    channel->addMember(client, ROLE_OPERATOR);
  }
  else
  {
    Channel *channel = it->second;

    if (channel->hasMember(client))
      return;
    if (!channel->key().empty() && channel->key() != key)
    {
      client->send(Replies::badChannelKey(nick, name));
      return;
    }
    if (channel->limit() > 0 && channel->memberCount() >= channel->limit())
    {
      client->send(Replies::channelIsFull(nick, name));
      return;
    }
    if (channel->isInviteOnly() && !channel->isInvited(nick))
    {
      client->send(Replies::inviteOnlyChan(nick, name));
      return;
    }
    channel->addMember(client, ROLE_MEMBER);
    channel->removeInvite(nick);   // transient invite is single-use

  }

  _sendJoinReplies(client, ctx.channels[name]);
}

void JoinCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname();

  if (!client->info().isRegistered())
  {
    client->send(Replies::notRegistered(nick));
    return;
  }
  if (msg.params.empty())
  {
    client->send(Replies::needMoreParams(nick, "JOIN"));
    return;
  }

  const std::string &name = msg.params[0];
  const std::string  key  = msg.params.size() > 1 ? msg.params[1] : "";

  if (!_isValidChannelName(name))
  {
    client->send(Replies::noSuchChannel(nick, name));
    return;
  }

  _joinChannel(client, name, key, ctx);
}