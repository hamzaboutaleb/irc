#include "commands/Context.hpp"

Context::Context(std::map<int, Client *> &clients,
                 std::map<std::string, Channel *> &channels,
                 const std::string &password)
    : clients(clients), channels(channels), password(password) {}