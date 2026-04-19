#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} log_level_t;

int log_init(const char *filename, log_level_t min_level);
void log_close(void);
void log_set_level(log_level_t level);
void log_write(log_level_t level, const char *fmt, ...);

#define LOG_DEBUG(...) log_write(LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...)  log_write(LOG_INFO,  __VA_ARGS__)
#define LOG_WARN(...)  log_write(LOG_WARN,  __VA_ARGS__)
#define LOG_ERROR(...) log_write(LOG_ERROR, __VA_ARGS__)

#endif /* LOGGER_H */
