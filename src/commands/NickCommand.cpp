#include "commands/NickCommand.hpp"
#include "commands/Replies.hpp"
#include "commands/Context.hpp"
#include "core/Channel.hpp"
#include "core/Config.hpp"
#include <cctype>
#include <set>

static bool isSpecial(char c)
{
  return c == '[' || c == ']' || c == '\\' || c == '^' ||
         c == '{' || c == '}' || c == '|';
}

bool NickCommand::_isValidNick(const std::string &nick) const
{
  if (nick.empty() || nick.size() > 9)
    return false;
  if (!std::isalpha(static_cast<unsigned char>(nick[0])) && !isSpecial(nick[0]))
    return false;
  for (size_t i = 1; i < nick.size(); ++i)
  {
    char c = nick[i];
    if (!std::isalnum(static_cast<unsigned char>(c)) && !isSpecial(c) && c != '-')
      return false;
  }
  return true;
}

void NickCommand::_broadcastNickChange(Client *client, const std::string &oldNick, Context &ctx)
{
  std::string nickMsg = ":" + oldNick + "!" + client->info().username() +
                        "@" SERVER_HOST " NICK " + client->info().nickname() + "\r\n";

  std::set<Client *> recipients;
  recipients.insert(client);

  std::map<std::string, Channel *>::iterator it;
  for (it = ctx.channels.begin(); it != ctx.channels.end(); ++it)
  {
    Channel *channel = it->second;
    if (!channel->hasMember(client))
      continue;
    const std::map<Client *, ChannelRole> &members = channel->members();
    std::map<Client *, ChannelRole>::const_iterator mit;
    for (mit = members.begin(); mit != members.end(); ++mit)
      recipients.insert(mit->first);
  }

  std::set<Client *>::iterator rit;
  for (rit = recipients.begin(); rit != recipients.end(); ++rit)
    (*rit)->send(nickMsg);
}

void NickCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string nick = client->info().nickname().empty() ? "*" : client->info().nickname();

  if (msg.params.empty())
  {
    client->send(Replies::noNicknameGiven());
    return;
  }

  const std::string &newNick = msg.params[0];

  if (!_isValidNick(newNick))
  {
    client->send(Replies::erroneusNickname(nick));
    return;
  }
  if (ctx.findClient(newNick))
  {
    client->send(Replies::nicknameInUse(newNick));
    return;
  }

  const std::string oldNick = client->info().nickname();
  client->info().setNickname(newNick);
  if (!client->info().nickReceived())
  {
    client->info().markNickReceived();
    client->tryRegister();
  }
  else
    _broadcastNickChange(client, oldNick, ctx);
}