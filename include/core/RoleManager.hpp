#pragma once
#include "core/Permission.hpp"
#include <map>
#include <set>

class RoleManager
{
public:
  static bool hasPermission(ChannelRole role, Permission perm);
};