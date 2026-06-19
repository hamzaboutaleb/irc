#include "network/Epoll.hpp"
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>

Epoll &Epoll::instance()
{
  static Epoll epoll;
  return epoll;
}

Epoll::Epoll()
{
  _fd = epoll_create1(0);
  if (_fd == -1)
    throw std::runtime_error(std::strerror(errno));
}

Epoll::~Epoll()
{
  close(_fd);
}

void Epoll::add(int fd, uint32_t events)
{
  epoll_event ev;
  ev.events  = events;
  ev.data.fd = fd;
  if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
    throw std::runtime_error(std::strerror(errno));
}

void Epoll::mod(int fd, uint32_t events)
{
  epoll_event ev;
  ev.events  = events;
  ev.data.fd = fd;
  if (epoll_ctl(_fd, EPOLL_CTL_MOD, fd, &ev) == -1)
    throw std::runtime_error(std::strerror(errno));
}

void Epoll::del(int fd)
{
  if (epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
    throw std::runtime_error(std::strerror(errno));
}

int Epoll::wait(epoll_event* events, int maxEvents)
{
  int n = epoll_wait(_fd, events, maxEvents, -1);
  if (n == -1)
    throw std::runtime_error(std::strerror(errno));
  return n;
}