#include "core/Args.hpp"
#include <cstdlib>
#include <cctype>
#include <stdexcept>
#include <string>

Args parseArgs(int argc, char **argv)
{
  if (argc != 3)
    throw std::runtime_error("usage: ./irc <port> <password>");

  char *end;
  long port = std::strtol(argv[1], &end, 10);

  if (*end != '\0')
    throw std::runtime_error("port must be a number");
  if (port < 1024 || port > 65535)
    throw std::runtime_error("port must be between 1024 and 65535");
  std::string password = argv[2];
  if (password.empty())
    throw std::runtime_error("password cannot be empty");
  for (size_t i = 0; i < password.size(); ++i)
  {
    unsigned char c = static_cast<unsigned char>(password[i]);
    if (!std::isalnum(c) && !std::ispunct(c))
      throw std::runtime_error("password must contain only letters, digits and symbols");
  }

  Args args;
  args.port = static_cast<int>(port);
  args.password = password;
  return args;
}