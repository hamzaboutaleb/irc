#pragma once
#include <sys/epoll.h>

class Epoll
{
  int _fd;

  Epoll();
  ~Epoll();
  Epoll(const Epoll &);
  Epoll &operator=(const Epoll &);

public:
  static Epoll &instance();

  void add(int fd, uint32_t events);
  void mod(int fd, uint32_t events);
  void del(int fd);
  int  wait(epoll_event *events, int maxEvents);
};