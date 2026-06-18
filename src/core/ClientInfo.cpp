#include "core/ClientInfo.hpp"

ClientInfo::ClientInfo()
    : _state(CONNECTING), _isOper(false), _passReceived(false) {}

ClientState ClientInfo::state() const { return _state; }
const std::string &ClientInfo::nickname() const { return _nickname; }
const std::string &ClientInfo::username() const { return _username; }
const std::string &ClientInfo::realname() const { return _realname; }
bool ClientInfo::isOper() const { return _isOper; }
bool ClientInfo::passReceived() const { return _passReceived; }

bool ClientInfo::isRegistered() const
{
  return _state == REGISTERED;
}

void ClientInfo::setNickname(const std::string &nickname) { _nickname = nickname; }
void ClientInfo::setUsername(const std::string &username) { _username = username; }
void ClientInfo::setRealname(const std::string &realname) { _realname = realname; }

void ClientInfo::markPassReceived() { _passReceived = true; }

void ClientInfo::markRegistered() { _state = REGISTERED; }

void ClientInfo::promoteToOper() { _isOper = true; }