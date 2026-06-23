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

  std::string notOnChannel(const std::string &nick, const std::string &channel);
  std::string noSuchNick(const std::string &nick, const std::string &target);
  std::string noTextToSend(const std::string &nick);
  std::string cannotSendToChan(const std::string &nick, const std::string &channel);

  // channel replies
  std::string noSuchChannel(const std::string &nick, const std::string &channel);
  std::string badChannelKey(const std::string &nick, const std::string &channel);
  std::string channelIsFull(const std::string &nick, const std::string &channel);
  std::string inviteOnlyChan(const std::string &nick, const std::string &channel);
  std::string chanOpPrivsNeeded(const std::string &nick, const std::string &channel);
  std::string noTopic(const std::string &nick, const std::string &channel);
  std::string topic(const std::string &nick, const std::string &channel, const std::string &topic);
  std::string namesList(const std::string &nick, const std::string &channel, const std::string &names);
  std::string endOfNames(const std::string &nick, const std::string &channel);
  std::string modeChange(const std::string &nick, const std::string &user,
                         const std::string &channel, const std::string &modeStr,
                         const std::string &params);
  std::string topicChange(const std::string &nick, const std::string &user,
                          const std::string &channel, const std::string &topic);
  std::string inviting(const std::string &nick, const std::string &target,
                       const std::string &channel);
  std::string userOnChannel(const std::string &nick, const std::string &target,
                            const std::string &channel);
  std::string inviteMsg(const std::string &nick, const std::string &user,
                        const std::string &target, const std::string &channel);
  std::string userNotInChannel(const std::string &nick, const std::string &target,
                               const std::string &channel);
}