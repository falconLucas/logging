/* Copyright (c) 2023 Nida Tech AB. All rights reserved. */
#include "logging.h"
#include "logging-conf.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef LOG_RTOS_DETAILS
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef LOG_MODE_NON_BLOCKING
#ifndef LOG_WITH_MUTEX
#define LOG_WITH_MUTEX
#endif
#endif

/* Buffer to store the log message */
static char s_buf[128];
/* Current position in buffer */
static size_t s_buf_len = 0;

static bool s_initialized = false;

static const char *
log_level_as_str(int level)
{
  switch (level)
  {
  case LOG_LEVEL_ERR:
    return "ERR";
  case LOG_LEVEL_WARN:
    return "WARN";
  case LOG_LEVEL_INFO:
    return "info";
  case LOG_LEVEL_DBG:
    return "dbg";
  default:
    return "";
  }
}

#ifdef LOG_MODE_NON_BLOCKING
static void
transmit_complete_cb(void)
{
  if (logging_unlock() != 0)
  {
    log_assert(__FILE__, __LINE__, "Failed to unlock logging\n");
  }
}
#endif

static size_t
lvprintf(const char *fmt, va_list va)
{
  const size_t len = vsnprintf(&s_buf[s_buf_len], sizeof(s_buf) - s_buf_len, fmt, va);
  return len;
}

static size_t
lprintf(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  const size_t len = lvprintf(fmt, va);
  va_end(va);

  return len;
}

void log_init(void)
{
  logging_init();
#ifdef LOG_MODE_NON_BLOCKING
  logging_register_callback(transmit_complete_cb);
#endif
  s_initialized = true;
}

void log_output(int level, const char *module, const char *fmt, ...)
{
  if (!s_initialized)
  {
    return;
  }

#ifdef LOG_WITH_MUTEX
  if (logging_lock() != 0)
  {
    log_assert(__FILE__, __LINE__, "Failed to lock logging\n");
  }
#endif

  uint32_t t = logging_get_time();
#ifdef LOG_RTOS_DETAILS
  TaskHandle_t handle = xTaskGetCurrentTaskHandle();
  TaskStatus_t xTaskDetails;
  vTaskGetInfo(handle, &xTaskDetails, pdFALSE, eInvalid);

  s_buf_len = 0;
  s_buf_len += lprintf("[%07d %-4s %-13s %10s] ", t, log_level_as_str(level), module, xTaskDetails.pcTaskName);
#else
  s_buf_len = 0;
  s_buf_len += lprintf("[%07d %4s %-13s] ", t, log_level_as_str(level), module);
#endif
  va_list va;
  va_start(va, fmt);
  s_buf_len += lvprintf(fmt, va);
  va_end(va);

  /* Now we can start writing the buffer to the uart. We will return straight
   * away but the semaphore won't be released until the transmit_complete_cb
   * is called */
  logging_write(s_buf, s_buf_len);
#if !defined(LOG_MODE_NON_BLOCKING) && defined(LOG_WITH_MUTEX)
  if (logging_unlock() != 0)
  {
    log_assert(__FILE__, __LINE__, "Failed to unlock logging\n");
  }
#endif
}

void log_assert(const char *filename, int line, const char *fmt, ...)
{
#ifdef LOG_WITH_MUTEX
  if (s_initialized)
  {
    logging_lock();
  }
#endif
  s_buf_len = 0;
  s_buf_len += lprintf("----ASSERT----\n*\n* in file %s, line %d\n", filename, line);

  va_list va;
  va_start(va, fmt);
  s_buf_len += lvprintf(fmt, va);
  va_end(va);

  /* Now we can start writing the buffer to the uart. We will return straight
   * away but the semaphore won't be released until the transmit_complete_cb
   * is called */
  logging_write(s_buf, s_buf_len);

  while (1)
    ;
}

#ifdef USE_FREERTOS
uint32_t
logging_get_time()
{
  return xTaskGetTickCount();
}
#endif