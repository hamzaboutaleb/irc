#include "core/IrcString.hpp"
#include <cctype>

std::string rfcCaseFold(const std::string &s)
{
  std::string r = s;
  for (size_t i = 0; i < r.size(); ++i)
  {
    char c = r[i];
    switch (c)
    {
      case '[':  c = '{'; break;
      case ']':  c = '}'; break;
      case '\\': c = '|'; break;
      case '~':  c = '^'; break;
      default:   c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    r[i] = c;
  }
  return r;
}