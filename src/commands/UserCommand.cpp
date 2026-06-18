#include "commands/UserCommand.hpp"
#include "commands/Replies.hpp"

void UserCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  (void)ctx;
  const std::string nick = client->info().nickname().empty() ? "*" : client->info().nickname();

  if (client->info().isRegistered() || client->info().userReceived())
  {
    client->send(Replies::alreadyRegistered(nick));
    return;
  }
  if (msg.params.size() < 4)
  {
    client->send(Replies::needMoreParams(nick, "USER"));
    return;
  }
  const std::string &username = msg.params[0];
  const std::string &realname = msg.params[3];

  if (username.empty() || username.size() > 10)
  {
    client->send(Replies::needMoreParams(nick, "USER"));
    return;
  }
  if (realname.empty() || realname.size() > 50)
  {
    client->send(Replies::needMoreParams(nick, "USER"));
    return;
  }

  client->info().setUsername(username);
  client->info().setRealname(realname);
  client->info().markUserReceived();
  client->tryRegister();
}