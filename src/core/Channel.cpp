#include "core/Channel.hpp"

Channel::Channel(const std::string &name)
  : _name(name), _limit(0), _inviteOnly(false) {}

void Channel::addMember(Client *client, ChannelRole role)
{
  _members[client] = role;
}

void Channel::removeMember(Client *client)
{
  _members.erase(client);
}

bool Channel::hasMember(Client *client) const
{
  return _members.find(client) != _members.end();
}

bool Channel::hasPermission(Client *client, Permission perm) const
{
  std::map<Client*, ChannelRole>::const_iterator it = _members.find(client);
  if (it == _members.end())
    return false;
  return RoleManager::hasPermission(it->second, perm);
}

void Channel::setRole(Client *client, ChannelRole role)
{
  if (hasMember(client))
    _members[client] = role;
}

ChannelRole Channel::getRole(Client *client) const
{
  std::map<Client*, ChannelRole>::const_iterator it = _members.find(client);
  if (it == _members.end())
    return ROLE_MEMBER;
  return it->second;
}

void Channel::broadcast(const std::string &msg, Client *exclude)
{
  std::map<Client*, ChannelRole>::iterator it;
  for (it = _members.begin(); it != _members.end(); ++it)
  {
    if (it->first != exclude)
      it->first->send(msg);
  }
}

const std::string &Channel::name() const        { return _name; }
const std::string &Channel::topic() const       { return _topic; }
const std::string &Channel::key() const         { return _key; }
int                Channel::limit() const       { return _limit; }
bool               Channel::isInviteOnly() const { return _inviteOnly; }
int Channel::memberCount() const                { return static_cast<int>(_members.size()); }
const std::map<Client*, ChannelRole> &Channel::members() const { return _members; }

void Channel::setTopic(const std::string &topic)  { _topic = topic; }
void Channel::setKey(const std::string &key)       { _key = key; }
void Channel::setLimit(int limit)                  { _limit = limit; }
void Channel::setInviteOnly(bool val)              { _inviteOnly = val; }