#pragma once
#include <string>

namespace Replies
{
  std::string needMoreParams(const std::string &nick, const std::string &cmd);
  std::string alreadyRegistered(const std::string &nick);
  std::string passwdMismatch(const std::string &nick);
  std::string welcome(const std::string &nick, const std::string &user, const std::string &host);
  std::string notRegistered(const std::string &nick);
  std::string erroneusNickname(const std::string &nick);
  std::string nicknameInUse(const std::string &nick);
  std::string noNicknameGiven();
}