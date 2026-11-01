// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. sqlite3_sourceid
    const char *source_id = sqlite3_sourceid();
    (void)source_id;

    // 2. sqlite3_compileoption_get
    int index = 0;
    if (Size >= 4) {
        index = *(const int32_t *)Data;
    } else if (Size >= 1) {
        index = Data[0];
    }
    const char *compile_option = sqlite3_compileoption_get(index);
    (void)compile_option;

    // 3. sqlite3_sleep
    int sleep_ms = 0;
    if (Size >= 4) {
        sleep_ms = *(const int32_t *)(Data + (Size >= 4 ? 4 : 0));
    } else if (Size >= 1) {
        sleep_ms = Data[0];
    }
    if (sleep_ms > 1000) sleep_ms = 1000;
    if (sleep_ms < 0) sleep_ms = 0;
    int actual_sleep = sqlite3_sleep(sleep_ms);
    (void)actual_sleep;

    // 4. sqlite3_free
    void *ptr = sqlite3_malloc(100);
    if (ptr != NULL) {
        memset(ptr, 0, 100);
        sqlite3_free(ptr);
    }
    sqlite3_free(NULL);

    return 0;
}