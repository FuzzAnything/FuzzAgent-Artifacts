// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 memory and status related APIs.
 * Targets:
 *  - sqlite3_status64
 *  - sqlite3_memory_alarm
 *  - sqlite3_soft_heap_limit64
 *  - sqlite3_memory_used
 *  - sqlite3_status
 *  - sqlite3_memory_highwater
 */

static int invoke_status64(const uint8_t *Data, size_t Size) {
    int op = 0;
    sqlite3_int64 pCurrent = 0;
    sqlite3_int64 pHighwater = 0;
    int resetFlag = 0;

    if (Size >= 4) {
        op = *(const int *)(Data);
    }
    if (Size >= 12) {
        pCurrent = *(const sqlite3_int64 *)(Data + 4);
    }
    if (Size >= 20) {
        pHighwater = *(const sqlite3_int64 *)(Data + 12);
    }
    if (Size >= 21) {
        resetFlag = (Data[20] % 2) ? 1 : 0;
    }

    /* 
     * Fix: Do not pass NULL pointers to sqlite3_status64.
     * The function writes to these pointers. Passing NULL causes a crash.
     * We always pass valid stack addresses.
     */
    return sqlite3_status64(op, &pCurrent, &pHighwater, resetFlag);
}

static int invoke_memory_alarm(const uint8_t *Data, size_t Size) {
    /* Callback pointer - we pass NULL or a dummy function pointer based on data */
    void (*callback)(void*, sqlite3_int64, int) = NULL;
    void *arg = NULL;
    sqlite3_int64 threshold = 0;

    if (Size >= 8) {
        threshold = *(const sqlite3_int64 *)(Data);
    }
    if (Size >= 16) {
        arg = (void *)(uintptr_t)(*(const uint64_t *)(Data + 8));
    }

    /* We cannot safely construct a real callback from fuzz data, so we pass NULL */
    /* The API is a stub and ignores arguments anyway */
    return sqlite3_memory_alarm(callback, arg, threshold);
}

static int invoke_soft_heap_limit64(const uint8_t *Data, size_t Size) {
    sqlite3_int64 N = 0;

    if (Size >= 8) {
        N = *(const sqlite3_int64 *)(Data);
    } else if (Size > 0) {
        N = (sqlite3_int64)Data[0];
    }

    sqlite3_int64 prev = sqlite3_soft_heap_limit64(N);
    (void)prev; /* Use return value to suppress unused warning, logic is side-effect free for fuzzing */
    return 0;
}

static int invoke_memory_used(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    sqlite3_int64 used = sqlite3_memory_used();
    (void)used;
    return 0;
}

static int invoke_status(const uint8_t *Data, size_t Size) {
    int op = 0;
    int pCurrent = 0;
    int pHighwater = 0;
    int resetFlag = 0;

    if (Size >= 4) {
        op = *(const int *)(Data);
    }
    if (Size >= 8) {
        pCurrent = *(const int *)(Data + 4);
    }
    if (Size >= 12) {
        pHighwater = *(const int *)(Data + 8);
    }
    if (Size >= 13) {
        resetFlag = (Data[12] % 2) ? 1 : 0;
    }

    /* 
     * Fix: Do not pass NULL pointers to sqlite3_status.
     * The function writes to these pointers. Passing NULL causes a crash.
     * We always pass valid stack addresses.
     */
    return sqlite3_status(op, &pCurrent, &pHighwater, resetFlag);
}

static int invoke_memory_highwater(const uint8_t *Data, size_t Size) {
    int resetFlag = 0;

    if (Size >= 1) {
        resetFlag = (Data[0] % 2) ? 1 : 0;
    }

    sqlite3_int64 hw = sqlite3_memory_highwater(resetFlag);
    (void)hw;
    return 0;
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Use first byte to select which API to fuzz */
    int selector = Data[0] % 6;

    switch (selector) {
        case 0:
            invoke_status64(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 1:
            invoke_memory_alarm(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 2:
            invoke_soft_heap_limit64(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 3:
            invoke_memory_used(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 4:
            invoke_status(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 5:
            invoke_memory_highwater(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
    }

    return 0;
}