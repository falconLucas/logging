#include "logging.h"
#include <stdio.h>
#include <windows.h>

#define LOG_MODULE "test-module"
#define LOG_LEVEL LOG_LEVEL_DBG

int main()
{
    printf("Test started...\n");
    log_init();
    LOG_DBG("This is a debug message\n");
    Sleep(10);
    LOG_INFO("Pi = %lf fyi\n", (double)3.141592);
    Sleep(10);
    LOG_WARN("Dragons are dangerous!\n");
    Sleep(10);
    LOG_ERR("Test failed succesfully.\n");
    printf("Test finished!\n");
    return 0;
}
