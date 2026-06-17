#include "network/Socket.hpp"
#include <unistd.h>

Socket::Socket(int fd) : _fd(fd) {}

Socket::Socket(const Socket &other) : _fd(dup(other._fd)) {}

Socket &Socket::operator=(const Socket &other)
{
  if (this != &other)
  {
    if (_fd != -1)
      ::close(_fd);
    _fd = dup(other._fd);
  }
  return *this;
}

Socket::~Socket()
{
  if (_fd != -1)
    ::close(_fd);
}
