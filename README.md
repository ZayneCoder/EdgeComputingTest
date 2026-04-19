# Edge Computing Test Application

A simple edge computing application written in C that demonstrates logging, scheduled tasks, and email notifications.

## Features

- **Logging System**: Initializes a logging system that writes to `output/logs/app.log` with configurable log levels (DEBUG, INFO, WARN, ERROR).
- **Scheduled Greeting Task**: Runs a daily greeting task that sends an email at 10:00 AM if the current time matches.
- **Email Notification**: Uses sendmail to send greeting emails to a predefined recipient.

## Project Structure

```
EdgeComputingTest/
├── build.sh          # Build script
├── Makefile          # Build configuration
├── build/            # Build artifacts (object files)
├── include/          # Header files
│   ├── logger.h      # Logging system header
│   └── main.h        # Main application header
├── output/           # Output directory
│   ├── edge_app      # Compiled executable
│   └── logs/         # Log files
│       └── app.log   # Application log file
└── src/              # Source files
    ├── logger.c      # Logging system implementation
    └── main.c        # Main application logic
```

## Dependencies

- GCC (GNU Compiler Collection)
- Make
- Sendmail (for email functionality)

## Building the Project

1. Clone or navigate to the project directory.
2. Run the following commands:

   ```bash
   make clean  # Optional: Clean previous builds
   make        # Build the application
   ```

   This will compile the source files and generate the executable at `output/edge_app`.

## Running the Application

Execute the application from the project root:

```bash
./output/edge_app
```

- On successful initialization, it will print "Application initialization complete."
- It will then run the scheduled greeting task.
- If the current time is 10:00 AM, it will attempt to send a greeting email.
- Logs are written to `output/logs/app.log`.

## Configuration

- **Log Path**: The log file is automatically set to `output/logs/app.log` relative to the executable's directory.
- **Email Recipient**: Hardcoded to `zaynejiang@163.com` in `src/main.c`. Modify the `recipient` variable in `send_daily_greeting()` to change it.
- **Greeting Time**: The greeting is sent only at 10:00 AM. This is checked in `send_daily_greeting()`.

## Troubleshooting

- **Logging Initialization Failure**: Ensure the `output/logs/` directory exists and is writable. The application will print an error if logging fails but will continue running the task.
- **Email Sending Issues**: Verify that sendmail is installed and configured. Check the log file for email send status.
- **Build Errors**: Ensure GCC and Make are installed. Check for missing headers or libraries.

## License

This project is for educational purposes. No specific license is applied.