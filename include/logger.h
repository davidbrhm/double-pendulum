#ifndef DOUBLE_PENDULUM_ERROR_H
#define DOUBLE_PENDULUM_ERROR_H

typedef enum LogLevel {
    LOG_LVL_DEBUG = 0,
    LOG_LVL_INFO,
    LOG_LVL_WARN,
    LOG_LVL_ERROR,
    LOG_LVL_FATAL
} LogLevel;


void init_logger(void);

void close_logger(void);

void core_log(LogLevel level, const char *file, int line, const char *format, ...);

#ifndef NDEBUG
#define LOG_DEBUG(...) core_log(LOG_LVL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  core_log(LOG_LVL_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  core_log(LOG_LVL_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) core_log(LOG_LVL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#else

#define LOG_DEBUG(...) (void)0
#define LOG_INFO(...)  (void)0
#define LOG_WARN(...)  (void)0
#define LOG_ERROR(...) (void)0

#endif

#define LOG_FATAL(...) core_log(LOG_LVL_FATAL, __FILE__, __LINE__, __VA_ARGS__)


#endif //DOUBLE_PENDULUM_ERROR_H
