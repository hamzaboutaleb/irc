#pragma once
#include <stddef.h>
#include "IoResult.hpp"

class Socket
{
  int _fd;

public:
  Socket(int fd);
  Socket(const Socket &other);
  Socket &operator=(const Socket &other);
  ~Socket();

  static Socket *createTcp();

  void setNonBlocking();
  void setKeepAlive(int idleSecs, int intervalSecs, int probes);
  void bind(int port);
  void listen(int backlog);
  Socket *accept();
  IoResult send(const void *buf, size_t len);
  IoResult recv(void *buf, size_t len);

  int fd() const;
};