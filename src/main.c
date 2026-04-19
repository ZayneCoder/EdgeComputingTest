#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "main.h"
#include "logger.h"

static int get_executable_dir(char *buf, size_t size) {
    ssize_t len = readlink("/proc/self/exe", buf, size - 1);
    if (len <= 0 || (size_t)len >= size - 1) {
        return -1;
    }

    buf[len] = '\0';
    char *dir = dirname(buf);
    if (dir == NULL) {
        return -1;
    }

    size_t dir_len = strlen(dir);
    if (dir_len >= size) {
        return -1;
    }
    memmove(buf, dir, dir_len + 1);
    return 0;
}

static int get_log_path(char *path, size_t size) {
    char dir[PATH_MAX];
    if (get_executable_dir(dir, sizeof(dir)) != 0) {
        return -1;
    }

    int written = snprintf(path, size, "%s/output/logs/app.log", dir);
    return written < 0 || (size_t)written >= size ? -1 : 0;
}

static int send_email(const char *recipient, const char *subject, const char *body) {
    if (recipient == NULL || recipient[0] == '\0') {
        return -1;
    }

    FILE *mail = popen("/usr/sbin/sendmail -t -oi", "w");
    if (mail == NULL) {
        return -1;
    }

    fprintf(mail, "To: %s\nSubject: %s\nMIME-Version: 1.0\nContent-Type: text/plain; charset=UTF-8\n\n%s\n", recipient, subject, body);
    int status = pclose(mail);
    if (status == -1) {
        return -1;
    }
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return -1;
}

static void send_daily_greeting(void) {
    time_t now = time(NULL);
    struct tm local_time;
    localtime_r(&now, &local_time);
    
    // Only send greeting at 10:00
    if (local_time.tm_hour != 10 || local_time.tm_min != 0) {
        return;
    }
    
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &local_time);

    const char *message = "Good morning! A new day has begun, wishing you a great day!";
    LOG_INFO("[%s] Daily 10 AM greeting: %s", timestamp, message);
    printf("%s %s\n", timestamp, message);

    const char *recipient = "zaynejiang@163.com";

    int email_result = send_email(recipient, "Daily 10 AM Greeting", message);
    if (email_result == 0) {
        LOG_INFO("Greeting email sent to %s", recipient);
    } else {
        LOG_WARN("Greeting email failed to send, recipient=%s, error=%d", recipient, email_result);
    }
}

void app_init(void) {
    char log_path[PATH_MAX];
    if (get_log_path(log_path, sizeof(log_path)) != 0 || log_init(log_path, LOG_DEBUG) != 0) {
        fprintf(stderr, "Failed to initialize logging system.\n");
        return;
    }

    LOG_INFO("Application initialization complete.");
    printf("Application initialization complete.\n");
}

int app_run(void) {
    LOG_INFO("Application is running scheduled greeting task...");
    printf("Application is running scheduled greeting task...\n");
    send_daily_greeting();
    return 0;
}

int main(void) {
    app_init();
    int result = app_run();
    log_close();
    return result;
}
