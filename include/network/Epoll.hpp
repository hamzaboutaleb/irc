#pragma once
#include <sys/epoll.h>

class Epoll
{
  int _fd;

public:
  Epoll();
  ~Epoll();

  void add(int fd, uint32_t events);
  void mod(int fd, uint32_t events);
  void del(int fd);
  int  wait(epoll_event* events, int maxEvents);
};