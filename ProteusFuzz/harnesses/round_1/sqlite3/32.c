// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. sqlite3_sourceid
    // No arguments required, returns a static string.
    const char *sourceid = sqlite3_sourceid();
    (void)sourceid; 

    // 2. sqlite3_compileoption_get
    // Use fuzzing data to determine the index N.
    int index1 = 0;
    if (Size >= 1) {
        index1 = Data[0];
    }
    
    int index2 = 0;
    if (Size >= 2) {
        index2 = (int)(Data[1] - 128); 
    }
    
    const char *opt1 = sqlite3_compileoption_get(index1);
    (void)opt1;
    
    const char *opt2 = sqlite3_compileoption_get(index2);
    (void)opt2;

    // 3. sqlite3_sleep
    int sleep_ms = 0;
    if (Size >= 3) {
        sleep_ms = Data[2];
    }
    if (sleep_ms > 100) {
        sleep_ms = 100;
    }
    
    int sleep_ms_neg = 0;
    if (Size >= 4) {
        sleep_ms_neg = (int)(Data[3] - 128);
    }

    sqlite3_sleep(sleep_ms);
    sqlite3_sleep(sleep_ms_neg);

    // 4. sqlite3_free
    size_t alloc_size = (Size > 1024) ? 1024 : Size;
    void *ptr = sqlite3_malloc(alloc_size);
    if (ptr != NULL) {
        memcpy(ptr, Data, alloc_size);
        sqlite3_free(ptr);
    }

    sqlite3_free(NULL);

    return 0;
}