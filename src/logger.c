#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_GRAY    "\x1b[90m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_FATAL   "\x1b[1;31m"

static FILE *log_file = NULL;

static const char *level_strings[] = {
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

static const char *level_colors[] = {
    COLOR_GRAY, COLOR_CYAN, COLOR_YELLOW, COLOR_RED, COLOR_FATAL
};

void init_logger(void) {
    log_file = fopen("latest.log", "w");

    if (!log_file) {
        fprintf(stderr, COLOR_RED "Failed to initialize log file!\n" COLOR_RESET);
    } else {
        LOG_INFO("Logger initialized successfully. File target: latest.log");
    }
}

void close_logger(void) {
    if (!log_file) return;

    LOG_INFO("Closing logger cleanly...");
    fclose(log_file);
    log_file = NULL;
}

void core_log(LogLevel level, const char *file, int line, const char *format, ...) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_buf[16];
    strftime(time_buf, sizeof(time_buf), "%H:%M:%S", t);

    va_list args;

    va_start(args, format);
    fprintf(stdout, "%s[%s] [%s] %s:%d - ", level_colors[level], time_buf, level_strings[level], file, line);
    vfprintf(stdout, format, args);

    fprintf(stdout, COLOR_RESET "\n");
    va_end(args);


    if (log_file) {
        va_start(args, format);
        fprintf(log_file, "[%s] [%s] %s:%d - ", time_buf, level_strings[level], file, line);
        vfprintf(log_file, format, args);
        fprintf(log_file, "\n");
        fflush(log_file);
        va_end(args);
    }

    if (level == LOG_LVL_FATAL) {
        // TODO: save app state/config before force exit
        close_logger();
        exit(EXIT_FAILURE);
    }
}
