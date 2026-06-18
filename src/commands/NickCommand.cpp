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

static std::string toLower(const std::string &s)
{
  std::string result = s;
  for (size_t i = 0; i < result.size(); ++i)
    result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
  return result;
}

bool NickCommand::_isNickInUse(const std::string &nick, Context &ctx) const
{
  std::map<int, Client *>::iterator it;
  for (it = ctx.clients.begin(); it != ctx.clients.end(); ++it)
  {
    if (toLower(it->second->info().nickname()) == toLower(nick))
      return true;
  }
  return false;
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
  if (_isNickInUse(newNick, ctx))
  {
    client->send(Replies::nicknameInUse(newNick));
    return;
  }

  client->info().setNickname(newNick);
  if (!client->info().nickReceived())
    client->info().markNickReceived();
  client->tryRegister();
}