#pragma once
#include <map>
#include <string>
#include "core/IrcString.hpp"

class Channel;

// RFC 2812 §1.3: channel names are case insensitive, folded per §2.2.
struct ChannelNameLess
{
  bool operator()(const std::string &a, const std::string &b) const
  {
    return rfcCaseFold(a) < rfcCaseFold(b);
  }
};

typedef std::map<std::string, Channel *, ChannelNameLess> ChannelMap;