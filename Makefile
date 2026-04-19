CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/logger.c
BUILD_DIR = build
OUT_DIR = output
OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))
TARGET = $(OUT_DIR)/edge_app
EMAIL ?= root@localhost
CRON_LOG = $(CURDIR)/output/logs/cron.log
CRON_JOB = 0 9 * * * cd $(CURDIR) && GREETING_EMAIL=$(EMAIL) $(CURDIR)/$(TARGET) >> $(CRON_LOG) 2>&1

.PHONY: all clean run install-cron uninstall-cron

all: $(TARGET)

$(TARGET): $(OBJ) | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR) $(OUT_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(OUT_DIR)

run: $(TARGET)
	./$(TARGET)

install-cron: $(TARGET)
	@echo "Installing daily 9:00 cron job..."
	@(crontab -l 2>/dev/null | grep -v -F '$(CURDIR)/$(TARGET)' ; echo "$(CRON_JOB)") | crontab -
	@echo "Cron job installed."

uninstall-cron:
	@echo "Removing daily 9:00 cron job..."
	@(crontab -l 2>/dev/null | grep -v -F '$(CURDIR)/$(TARGET)') | crontab -
	@echo "Cron job removed."
