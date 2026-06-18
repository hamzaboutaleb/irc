#include "commands/PingCommand.hpp"
#include "commands/Replies.hpp"

void PingCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  (void)ctx;
  if (msg.params.empty())
  {
    client->send(Replies::needMoreParams(client->info().nickname(), "PING"));
    return;
  }
  client->send(":localhost PONG localhost :" + msg.params[0] + "\r\n");
}