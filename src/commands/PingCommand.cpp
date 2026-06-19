#include "commands/PingCommand.hpp"
#include "commands/Replies.hpp"
#include "core/Config.hpp"

void PingCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  (void)ctx;
  if (msg.params.empty())
  {
    client->send(Replies::needMoreParams(client->info().nickname(), "PING"));
    return;
  }
  client->send(":" SERVER_HOST " PONG " SERVER_HOST " :" + msg.params[0] + "\r\n");
}