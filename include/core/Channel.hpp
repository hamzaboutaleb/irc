#pragma once
#include "core/Client.hpp"
#include "core/Permission.hpp"
#include "core/RoleManager.hpp"
#include <map>
#include <set>
#include <string>

class Channel
{
  std::string                    _name;
  std::string                    _topic;
  std::string                    _key;
  int                            _limit;
  bool                           _inviteOnly;
  bool                           _topicLocked;
  std::set<std::string>          _inviteList;
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
  const std::string &topic() const;
  const std::string &key() const;
  int                limit() const;
  bool               isInviteOnly() const;
  bool               isTopicLocked() const;
  bool               isInvited(const std::string &nick) const;
  void               addInvite(const std::string &nick);
  int                memberCount() const;
  const std::map<Client*, ChannelRole> &members() const;

  void setTopic(const std::string &topic);
  void setKey(const std::string &key);
  void setLimit(int limit);
  void setInviteOnly(bool val);
  void setTopicLocked(bool val);
};