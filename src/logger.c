#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include "logger.h"

static FILE *log_file = NULL;
static log_level_t current_level = LOG_DEBUG;
static const char *const level_names[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
};

static int ensure_directory(const char *dir_path) {
    if (mkdir(dir_path, 0755) == 0) {
        return 0;
    }
    if (errno == EEXIST) {
        return 0;
    }
    return -1;
}

int log_init(const char *filename, log_level_t min_level) {
    if (filename == NULL) {
        return -1;
    }

    if (ensure_directory("output/logs") != 0) {
        return -1;
    }

    log_file = fopen(filename, "a");
    if (log_file == NULL) {
        return -1;
    }

    current_level = min_level;
    return 0;
}

void log_close(void) {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_set_level(log_level_t level) {
    if (level >= LOG_DEBUG && level <= LOG_ERROR) {
        current_level = level;
    }
}

void log_write(log_level_t level, const char *fmt, ...) {
    if (log_file == NULL || level < current_level || level < LOG_DEBUG || level > LOG_ERROR) {
        return;
    }

    time_t now = time(NULL);
    struct tm local_time;
    localtime_r(&now, &local_time);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &local_time);

    fprintf(log_file, "%s [%s] ", timestamp, level_names[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);
}
