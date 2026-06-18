#include "Args.hpp"
#include <iostream>

int main(int argc, char** argv)
{
  try
  {
    Args args = parseArgs(argc, argv);
    (void)args;
  }
  catch (const std::exception& e)
  {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}