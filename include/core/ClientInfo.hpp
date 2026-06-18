#pragma once
#include <string>

enum ClientState
{
  CONNECTING,
  REGISTERED
};

class ClientInfo
{
  ClientState _state;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  bool _isOper;
  bool _passReceived;
  bool _nickReceived;
  bool _userReceived;

public:
  ClientInfo();

  ClientState state() const;
  const std::string &nickname() const;
  const std::string &username() const;
  const std::string &realname() const;
  bool isOper() const;
  bool passReceived() const;
  bool nickReceived() const;
  bool userReceived() const;
  bool isRegistered() const;

  void setNickname(const std::string &nickname);
  void setUsername(const std::string &username);
  void setRealname(const std::string &realname);
  void markPassReceived();
  void markNickReceived();
  void markUserReceived();
  void markRegistered();
  void promoteToOper();
};