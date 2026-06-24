#pragma once
#include <string>

// RFC 2812 §2.2 case folding: ASCII tolower plus the Scandinavian pairs
// {≡[  }≡]  |≡\  ^≡~  — used for nick/channel equivalence.
std::string rfcCaseFold(const std::string &s);