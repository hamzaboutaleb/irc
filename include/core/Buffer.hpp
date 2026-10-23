#pragma once
#include <string>

class Buffer
{
  std::string _data;

public:
  void append(const char *data, size_t len);
  bool hasMessage() const;
  std::string extractMessage();
  std::string extractRemaining();
  size_t size() const;
};