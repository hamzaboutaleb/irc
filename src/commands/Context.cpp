#include "commands/Context.hpp"
#include <cctype>

static std::string toLower(const std::string &s)
{
  std::string r = s;
  for (size_t i = 0; i < r.size(); ++i)
    r[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(r[i])));
  return r;
}

Context::Context(std::map<int, Client *> &clients,
                 std::map<std::string, Channel *> &channels,
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

// TODO: replace with a nick→Client* index in Server for O(log n) lookup
Client *Context::findClient(const std::string &nick) const
{
  std::map<int, Client *>::const_iterator it;
  for (it = clients.begin(); it != clients.end(); ++it)
  {
    if (toLower(it->second->info().nickname()) == toLower(nick))
      return it->second;
  }
  return NULL;
}