#pragma once
#include <sys/types.h>
#include "IoStatus.hpp"

class IoResult
{
public:
  IoStatus status;
  ssize_t bytes;
  int err;

  IoResult(IoStatus status, ssize_t bytes, int err);
};