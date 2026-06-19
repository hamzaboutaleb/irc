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

  std::string notOnChannel(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 442 " + nick + " " + channel + " :You're not on that channel\r\n";
  }

  std::string noSuchNick(const std::string &nick, const std::string &target)
  {
    return ":" SERVER " 401 " + nick + " " + target + " :No such nick\r\n";
  }

  std::string noTextToSend(const std::string &nick)
  {
    return ":" SERVER " 412 " + nick + " :No text to send\r\n";
  }

  std::string cannotSendToChan(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 404 " + nick + " " + channel + " :Cannot send to channel\r\n";
  }

  std::string noSuchChannel(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 403 " + nick + " " + channel + " :No such channel\r\n";
  }

  std::string badChannelKey(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";
  }

  std::string channelIsFull(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";
  }

  std::string inviteOnlyChan(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";
  }

  std::string chanOpPrivsNeeded(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 482 " + nick + " " + channel + " :You're not channel operator\r\n";
  }

  std::string noTopic(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 331 " + nick + " " + channel + " :No topic is set\r\n";
  }

  std::string topic(const std::string &nick, const std::string &channel, const std::string &t)
  {
    return ":" SERVER " 332 " + nick + " " + channel + " :" + t + "\r\n";
  }

  std::string namesList(const std::string &nick, const std::string &channel, const std::string &names)
  {
    return ":" SERVER " 353 " + nick + " = " + channel + " :" + names + "\r\n";
  }

  std::string endOfNames(const std::string &nick, const std::string &channel)
  {
    return ":" SERVER " 366 " + nick + " " + channel + " :End of /NAMES list\r\n";
  }
}