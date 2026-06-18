#pragma once
#include <string>

struct Args
{
  int         port;
  std::string password;
};

Args parseArgs(int argc, char** argv);