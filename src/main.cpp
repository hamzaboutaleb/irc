#include "core/Args.hpp"
#include "core/Server.hpp"
#include <iostream>
#include <signal.h>

// TODO: handle SIGINT/SIGTERM to call server.stop() gracefully before exit
int main(int argc, char** argv)
{
  signal(SIGPIPE, SIG_IGN);
  try
  {
    Args   args = parseArgs(argc, argv);
    Server server(args.port, args.password);
    server.start();
  }
  catch (const std::exception& e)
  {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}