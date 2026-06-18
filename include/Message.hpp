#pragma once
#include <string>
#include <vector>

struct Message
{
  std::string              prefix;
  std::string              command;
  std::vector<std::string> params;

  static Message parse(const std::string& raw);
};