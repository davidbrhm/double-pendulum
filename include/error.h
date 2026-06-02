#ifndef DOUBLE_PENDULUM_ERROR_H
#define DOUBLE_PENDULUM_ERROR_H
#include <stdbool.h>

typedef enum ErrorCode {
    ERR_SUCCESS = 0,
    ERR_NULL_POINTER,
    ERR_ALLOCATION_FAILED,
    ERR_OUT_OF_BOUNDS
} ErrorCode;

void log_error(ErrorCode code, const char *context, bool is_fatal);

#endif //DOUBLE_PENDULUM_ERROR_H
