#pragma once
#include <string>
#include <vector>
#include <ostream>

struct Message
{
  std::string prefix;
  std::string command;
  std::vector<std::string> params;

  static Message parse(const std::string &raw);
};

std::ostream &operator<<(std::ostream &os, const Message &msg);