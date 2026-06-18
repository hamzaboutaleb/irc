#include "core/Args.hpp"
#include "core/Server.hpp"
#include <iostream>

int main(int argc, char** argv)
{
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