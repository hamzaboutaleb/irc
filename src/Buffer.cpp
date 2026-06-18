#include "Buffer.hpp"

void Buffer::append(const char* data, size_t len)
{
  _data.append(data, len);
}

bool Buffer::hasMessage() const
{
  return _data.find("\r\n") != std::string::npos;
}

std::string Buffer::extractMessage()
{
  size_t pos = _data.find("\r\n");
  std::string message = _data.substr(0, pos);
  _data.erase(0, pos + 2);
  return message;
}

size_t Buffer::size() const
{
  return _data.size();
}