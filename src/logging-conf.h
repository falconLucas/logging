
#ifndef _LOGGING_CONF_H_
#define _LOGGING_CONF_H_

#include <stddef.h>
#include <stdint.h>

#ifdef LOG_MODE_NON_BLOCKING
typedef void (*logging_callback_t)(void);
void logging_register_callback(logging_callback_t cb);
#endif

#ifdef LOG_WITH_MUTEX
int logging_lock(void);
int logging_unlock(void);
#endif

void logging_init(void);
void logging_write(char *buf, size_t size);
uint32_t logging_get_time();

#endif // _LOGGING_CONF_H_
