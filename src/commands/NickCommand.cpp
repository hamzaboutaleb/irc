#include "commands/NickCommand.hpp"
#include "commands/Replies.hpp"
#include "commands/Context.hpp"
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

  client->info().setNickname(newNick);
  if (!client->info().nickReceived())
    client->info().markNickReceived();
  client->tryRegister();
}