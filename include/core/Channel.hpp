#pragma once
#include "core/Client.hpp"
#include "core/Permission.hpp"
#include "core/RoleManager.hpp"
#include <map>
#include <string>

class Channel
{
  std::string                    _name;
  std::map<Client*, ChannelRole> _members;

public:
  Channel(const std::string &name);

  void        addMember(Client *client, ChannelRole role);
  void        removeMember(Client *client);
  bool        hasMember(Client *client) const;
  bool        hasPermission(Client *client, Permission perm) const;
  void        setRole(Client *client, ChannelRole role);
  ChannelRole getRole(Client *client) const;

  void        broadcast(const std::string &msg, Client *exclude);

  const std::string &name() const;
  int                memberCount() const;
  const std::map<Client*, ChannelRole> &members() const;
};