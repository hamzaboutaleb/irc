#include "network/Socket.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>

Socket::Socket(int fd) : _fd(fd) {}

Socket::Socket(const Socket &other) : _fd(dup(other._fd)) {}

Socket &Socket::operator=(const Socket &other)
{
  if (this != &other)
  {
    if (_fd != -1)
      close(_fd);
    _fd = dup(other._fd);
  }
  return *this;
}

Socket::~Socket()
{
  if (_fd != -1)
    close(_fd);
}

int Socket::fd() const
{
  return _fd;
}

Socket *Socket::createTcp()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1)
    throw std::runtime_error(std::strerror(errno));

  int opt = 1; // for setting resuaddr. when program shutdown we can resue port immediatly
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
  {
    close(fd);
    throw std::runtime_error(std::strerror(errno));
  }

  return new Socket(fd);
}
/*
  This option controls the time to remove a zombie socket.
  It works at the TCP level. After X seconds of idleness, it starts sending
  a probe every X seconds for N times. If there's no response, it fires
  EPOLLHUP so that we can handle it.
  we dont need it becuase we use PING/PONG
*/
void Socket::setKeepAlive(int idleSecs, int intervalSecs, int probes)
{
  int opt = 1;
  if (setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1)
    throw std::runtime_error(std::strerror(errno));
  if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPIDLE, &idleSecs, sizeof(idleSecs)) == -1)
    throw std::runtime_error(std::strerror(errno));
  if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPINTVL, &intervalSecs, sizeof(intervalSecs)) == -1)
    throw std::runtime_error(std::strerror(errno));
  if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPCNT, &probes, sizeof(probes)) == -1)
    throw std::runtime_error(std::strerror(errno));
}

void Socket::setNonBlocking()
{
  int flags = fcntl(_fd, F_GETFL, 0); // we get old flags so that we will not lose it when we set nonblock flag
  if (flags == -1 || fcntl(_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    throw std::runtime_error(std::strerror(errno));
}

void Socket::bind(int port)
{
  sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (::bind(_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
    throw std::runtime_error(std::strerror(errno));
}

void Socket::listen(int backlog) // queue size
{
  if (::listen(_fd, backlog) == -1)
    throw std::runtime_error(std::strerror(errno));
}

Socket *Socket::accept()
{
  int clientFd = ::accept(_fd, NULL, NULL);
  if (clientFd == -1)
    throw std::runtime_error(std::strerror(errno));
  return new Socket(clientFd);
}

IoResult Socket::send(const void *buf, size_t len)
{
  ssize_t n = ::send(_fd, buf, len, 0);
  if (n > 0)
    return IoResult(IO_OK, n, 0);
  if (n == 0)
    return IoResult(IO_WOULDBLOCK, 0, 0);
  if (errno == EAGAIN || errno == EWOULDBLOCK)
    return IoResult(IO_WOULDBLOCK, 0, errno);
  if (errno == EPIPE || errno == ECONNRESET)
    return IoResult(IO_CLOSED, 0, errno);
  return IoResult(IO_ERROR, 0, errno);
}

IoResult Socket::recv(void *buf, size_t len)
{
  ssize_t n = ::recv(_fd, buf, len, 0);
  if (n > 0)
    return IoResult(IO_OK, n, 0);
  if (n == 0)
    return IoResult(IO_CLOSED, 0, 0);
  if (errno == EAGAIN || errno == EWOULDBLOCK)
    return IoResult(IO_WOULDBLOCK, 0, errno);
  return IoResult(IO_ERROR, 0, errno);
}
