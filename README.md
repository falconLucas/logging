This logging module is designed to work with or without FreeRTOS in blocking or non-blocking mode. It handles the formatting of log messages and writes to a desired output stream.

## Usage
Create a source file `logging-conf.c` that implements `logging_init()` and `logging_write()` with the desired output stream, e.g. UART. If non-blocking writes are used, you must also implement `logging_register_callback()` and initialize a mutex in the `logging_init()` function implementation. If FreeRTOS is not used, you must also implement `logging_get_time()` to get the current time in milliseconds. This is used to timestamp the log messages.

## Note
* Logging in bare-metal applications requires either blocking writes on the output stream, or busy wait in `logging_lock()` for the mutex to be realeased.

* If any RTOS is used in preemptive mode, `LOG_WITH_MUTEX` must be set even in blocking mode to protect the buffer. This requires implementation of `logging_lock()` and `logging_unlock()` functions.

* If you wish to use non-blocking writes to output stream, add `-DLOG_MODE_NONE_BLOCKING=1` to `CFLAGS` in your build system.
