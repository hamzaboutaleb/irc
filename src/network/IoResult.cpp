#include "network/IoResult.hpp"

IoResult::IoResult(IoStatus status, size_t bytes, int err)
    : status(status), bytes(bytes), err(err) {}