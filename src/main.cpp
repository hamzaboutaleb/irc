#include "core/Args.hpp"
#include "core/Server.hpp"
#include <iostream>
#include <signal.h>

// TODO: flood protection — two complementary guards (see Server::_processClient):
//   1. RecvQ cap: if a client's accumulated unprocessed input exceeds a max
//      buffer size, disconnect them ("Excess Flood") — bounds memory.
//   2. Per-client rate limit (token bucket, lazy refill): throttle how many
//      commands are processed per client over time so one flooder can't starve
//      the single epoll loop. Make capacity/refill/recvq cap configurable in Config.hpp.

static bool g_running = true;

static void signalHandler(int)
{
  g_running = false;
}

int main(int argc, char** argv)
{
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);
  try
  {
    Args   args = parseArgs(argc, argv);
    Server server(args.port, args.password);
    server.start(g_running);
  }
  catch (const std::exception& e)
  {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}