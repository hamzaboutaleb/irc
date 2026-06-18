#include "Message.hpp"
#include <cctype>
#include <ostream>

static std::string toUpper(const std::string& s)
{
  std::string result = s;
  for (size_t i = 0; i < result.size(); ++i)
    result[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[i])));
  return result;
}

static size_t skipSpaces(const std::string& s, size_t i)
{
  while (i < s.size() && s[i] == ' ')
    ++i;
  return i;
}

static std::string readUntilSpace(const std::string& s, size_t& i)
{
  size_t start = i;
  while (i < s.size() && s[i] != ' ')
    ++i;
  return s.substr(start, i - start);
}

Message Message::parse(const std::string& raw)
{
  Message msg;
  size_t  i = 0;

  if (raw.empty())
    return msg;

  // step 1: optional prefix
  if (raw[i] == ':')
  {
    ++i;
    msg.prefix = readUntilSpace(raw, i);
    i = skipSpaces(raw, i);
  }

  // step 2: command
  msg.command = toUpper(readUntilSpace(raw, i));

  // step 3: params
  while (true)
  {
    i = skipSpaces(raw, i);
    if (i >= raw.size())
      break;
    if (raw[i] == ':')
    {
      msg.params.push_back(raw.substr(i + 1));
      break;
    }
    msg.params.push_back(readUntilSpace(raw, i));
  }

  return msg;
}

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
  os << "prefix=[" << msg.prefix << "] "
     << "cmd=[" << msg.command << "] "
     << "params(" << msg.params.size() << "):";
  for (size_t i = 0; i < msg.params.size(); ++i)
    os << " [" << msg.params[i] << "]";
  return os;
}