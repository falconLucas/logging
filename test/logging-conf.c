#include "logging-conf.h"
#include <stdio.h>
#include <sys\timeb.h> 

typedef struct timeb timeb_t;

static timeb_t init_time;
void logging_init(void)
{
    ftime(&init_time);
    return;
}
void logging_write(char *buf, size_t size)
{
    printf("%s", buf);
}
uint32_t logging_get_time()
{
    timeb_t t;
    ftime(&t);
    return (1000 * (t.time - init_time.time) + (t.millitm - init_time.millitm)) & 0x7FFFFF;
}
