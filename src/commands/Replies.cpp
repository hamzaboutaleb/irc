#include "commands/Replies.hpp"

namespace Replies
{
  std::string needMoreParams(const std::string &nick, const std::string &cmd)
  {
    return ":" + nick + " 461 " + nick + " " + cmd + " :Not enough parameters\r\n";
  }

  std::string alreadyRegistered(const std::string &nick)
  {
    return ":" + nick + " 462 " + nick + " :You may not reregister\r\n";
  }

  std::string passwdMismatch(const std::string &nick)
  {
    return ":" + nick + " 464 " + nick + " :Password incorrect\r\n";
  }

  std::string welcome(const std::string &nick, const std::string &user, const std::string &host)
  {
    return ":server 001 " + nick + " :Welcome to the IRC server " + nick + "!" + user + "@" + host + "\r\n";
  }

  std::string notRegistered(const std::string &nick)
  {
    return ":" + nick + " 451 " + nick + " :You have not registered\r\n";
  }

  std::string erroneusNickname(const std::string &nick)
  {
    return ":" + nick + " 432 " + nick + " :Erroneous nickname\r\n";
  }

  std::string nicknameInUse(const std::string &nick)
  {
    return ":" + nick + " 433 * " + nick + " :Nickname is already in use\r\n";
  }

  std::string noNicknameGiven()
  {
    return ":* 431 * :No nickname given\r\n";
  }
}