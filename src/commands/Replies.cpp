#include "commands/Replies.hpp"

#define SERVER "localhost"

namespace Replies
{
  std::string needMoreParams(const std::string &nick, const std::string &cmd)
  {
    return ":" SERVER " 461 " + nick + " " + cmd + " :Not enough parameters\r\n";
  }

  std::string alreadyRegistered(const std::string &nick)
  {
    return ":" SERVER " 462 " + nick + " :You may not reregister\r\n";
  }

  std::string passwdMismatch(const std::string &nick)
  {
    return ":" SERVER " 464 " + nick + " :Password incorrect\r\n";
  }

  std::string welcome(const std::string &nick, const std::string &user, const std::string &host)
  {
    return ":" SERVER " 001 " + nick + " :Welcome to the IRC server " + nick + "!" + user + "@" + host + "\r\n";
  }

  std::string notRegistered(const std::string &nick)
  {
    return ":" SERVER " 451 " + nick + " :You have not registered\r\n";
  }

  std::string erroneusNickname(const std::string &nick)
  {
    return ":" SERVER " 432 " + nick + " :Erroneous nickname\r\n";
  }

  std::string nicknameInUse(const std::string &nick)
  {
    return ":" SERVER " 433 " + nick + " :Nickname is already in use\r\n";
  }

  std::string noNicknameGiven()
  {
    return ":" SERVER " 431 * :No nickname given\r\n";
  }
}