#pragma once
#include <stddef.h>
#include "IoStatus.hpp"

class IoResult
{
public:
  IoStatus status;
  size_t bytes;
  int err;

  IoResult(IoStatus status, size_t bytes, int err);
};