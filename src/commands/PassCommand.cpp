#include "commands/PassCommand.hpp"
#include "commands/Replies.hpp"

void PassCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string nick = client->info().nickname().empty() ? "*" : client->info().nickname();

  if (client->info().isRegistered() || client->info().passReceived())
  {
    client->send(Replies::alreadyRegistered(nick));
    return;
  }
  if (msg.params.empty())
  {
    client->send(Replies::needMoreParams(nick, "PASS"));
    return;
  }
  if (msg.params[0] != ctx.password)
  {
    client->send(Replies::passwdMismatch(nick));
    return;
  }
  client->info().markPassReceived();
}