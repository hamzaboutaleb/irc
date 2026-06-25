#include "core/Args.hpp"
#include "core/Server.hpp"
#include <iostream>
#include <signal.h>

static bool g_running = true;

static void signalHandler(int)
{
  g_running = false;
}

int main(int argc, char **argv)
{
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);
  try
  {
    Args args = parseArgs(argc, argv);
    Server server(args.port, args.password);
    server.start(g_running);
  }
  catch (const std::exception &e)
  {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}