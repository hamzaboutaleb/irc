#include "commands/NickCommand.hpp"
#include "commands/Replies.hpp"
#include <cctype>

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

bool NickCommand::_isNickInUse(const std::string &nick, Context &ctx) const
{
  std::map<int, Client *>::iterator it;
  for (it = ctx.clients.begin(); it != ctx.clients.end(); ++it)
  {
    if (it->second->info().nickname() == nick)
      return true;
  }
  return false;
}

void NickCommand::_tryRegister(Client *client) const
{
  if (client->info().passReceived() &&
      client->info().nickReceived() &&
      client->info().userReceived())
  {
    client->info().markRegistered();
    client->send(Replies::welcome(
        client->info().nickname(),
        client->info().username(),
        "localhost"));
  }
}

void NickCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick = client->info().nickname().empty() ? "*" : client->info().nickname();

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
  if (_isNickInUse(newNick, ctx))
  {
    client->send(Replies::nicknameInUse(newNick));
    return;
  }

  client->info().setNickname(newNick);
  if (!client->info().nickReceived())
  {
    client->info().markNickReceived();
    _tryRegister(client);
  }
}