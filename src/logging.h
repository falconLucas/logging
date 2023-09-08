/* Copyright (c) 2022 Nida Tech AB. All rights reserved. */

#ifndef LOGGING_H_
#define LOGGING_H_

#define LOG_LEVEL_ERR          0
#define LOG_LEVEL_WARN         1
#define LOG_LEVEL_INFO         2
#define LOG_LEVEL_DBG          3

#define SYS_LOG(level, ...) do { \
    if(level <= LOG_LEVEL) { \
      log_output(level, LOG_MODULE, __VA_ARGS__); \
    } \
} while(0)

#define SYS_LOG_ASSERT(condition, ...) if((condition) == 0) log_assert(__FILE__, __LINE__, __VA_ARGS__)

#define LOG_ERR(...)   SYS_LOG(LOG_LEVEL_ERR, __VA_ARGS__)
#define LOG_WARN(...)  SYS_LOG(LOG_LEVEL_WARN, __VA_ARGS__)
#define LOG_INFO(...)  SYS_LOG(LOG_LEVEL_INFO, __VA_ARGS__)
#define LOG_DBG(...)   SYS_LOG(LOG_LEVEL_DBG, __VA_ARGS__)

void log_init(void);
void log_output(int level, const char *module, const char *fmt, ...)
__attribute__((__format__(__printf__, 3, 4)));
void log_assert(const char *filename, int line, const char *fmt, ...)
__attribute__((__format__(__printf__, 3, 4)));

#endif /* LOGGING_H_ */
