#include "commands/QuitCommand.hpp"
#include "commands/Context.hpp"
#include "core/Channel.hpp"
#include "core/Config.hpp"
#include <vector>

void QuitCommand::execute(Client *client, const Message &msg, Context &ctx)
{
  const std::string &nick   = client->info().nickname();
  const std::string &user   = client->info().username();
  const std::string  reason = msg.params.empty() ? "" : msg.params[0];

  std::string quitMsg = ":" + nick + "!" + user + "@" SERVER_HOST
                        " QUIT :" + reason + "\r\n";

  std::vector<Channel *> toLeave;
  ChannelMap::iterator it;
  for (it = ctx.channels.begin(); it != ctx.channels.end(); ++it)
  {
    if (it->second->hasMember(client))
      toLeave.push_back(it->second);
  }

  for (size_t i = 0; i < toLeave.size(); ++i)
  {
    toLeave[i]->broadcast(quitMsg, client);
    ctx.removeFromChannel(client, toLeave[i]);
  }

  client->markDead();
}