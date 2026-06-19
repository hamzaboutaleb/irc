#include "core/RoleManager.hpp"

bool RoleManager::hasPermission(ChannelRole role, Permission perm)
{
  static std::map<ChannelRole, std::set<Permission> > permissions;

  if (permissions.empty())
  {
    permissions[ROLE_OPERATOR].insert(PERM_KICK);
    permissions[ROLE_OPERATOR].insert(PERM_BAN);
    permissions[ROLE_OPERATOR].insert(PERM_MODE);
    permissions[ROLE_OPERATOR].insert(PERM_TOPIC);
    permissions[ROLE_OPERATOR].insert(PERM_INVITE);
  }

  std::map<ChannelRole, std::set<Permission> >::iterator it = permissions.find(role);
  if (it == permissions.end())
    return false;
  return it->second.count(perm) > 0;
}