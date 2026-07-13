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
  else
    Context::assignOp(channel);
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
//human made aw9
void  Context::assignOp(Channel *channel)
{
    int has_op = 0;
    std::map<int, Client *>::const_iterator it;
    std::map<int, Client *>::const_iterator howa;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
     if (channel->hasMember(it->second) == true)
     {
        howa = it;
        if(channel->getRole(it->second) == ROLE_OPERATOR)
          has_op++;

     }
    }
    if (!has_op)
      channel->setRole(howa->second, ROLE_OPERATOR);
}