#include "commands/PassCommand.hpp"
#include "commands/Replies.hpp"

void PassCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string nick = client->info().nickname().empty() ? "*" : client->info().nickname();

  if (client->info().isRegistered() || client->info().passReceived())
  {
    std::string reply = Replies::alreadyRegistered(nick);
    client->socket()->send(reply.c_str(), reply.size());
    return;
  }
  if (msg.params.empty())
  {
    std::string reply = Replies::needMoreParams(nick, "PASS");
    client->socket()->send(reply.c_str(), reply.size());
    return;
  }
  if (msg.params[0] != ctx.password)
  {
    std::string reply = Replies::passwdMismatch(nick);
    client->socket()->send(reply.c_str(), reply.size());
    return;
  }
  client->info().markPassReceived();
}