#include "commands/Context.hpp"
#include "core/IrcString.hpp"

Context::Context(std::map<int, Client *> &clients,
                 ChannelMap &channels,
                 const std::string &password)
    : clients(clients), channels(channels), password(password) {}

void Context::removeFromChannel(Client *client, Channel *channel)
{
  channel->removeMember(client);
  if (channel->memberCount() == 0)
  {
    channels.erase(channel->name());
    delete channel;
  }
}

Client *Context::findClient(const std::string &nick) const
{
  std::map<int, Client *>::const_iterator it;
  for (it = clients.begin(); it != clients.end(); ++it)
  {
    if (rfcCaseFold(it->second->info().nickname()) == rfcCaseFold(nick))
      return it->second;
  }
  return NULL;
}