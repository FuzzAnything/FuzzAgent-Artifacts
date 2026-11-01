// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_memory_alarm_callback(void *pArg, sqlite3_int64 nBytes, int isHard) {
    (void)pArg;
    (void)nBytes;
    (void)isHard;
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse input data to generate diverse arguments
    int idx = 0;
    
    // Extract values for sqlite3_memory_alarm
    void *alarm_arg = NULL;
    sqlite3_int64 alarm_threshold = 0;
    if (idx < Size) alarm_threshold = (sqlite3_int64)Data[idx++];
    if (idx < Size) alarm_threshold += (sqlite3_int64)Data[idx++];
    
    // Call sqlite3_memory_alarm with diverse callbacks (dummy and NULL)
    sqlite3_memory_alarm(dummy_memory_alarm_callback, alarm_arg, alarm_threshold);
    sqlite3_memory_alarm(NULL, alarm_arg, alarm_threshold);
    sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, 0);

    // Extract values for sqlite3_status64 and sqlite3_status
    int op_code = 0;
    int reset_flag = 0;
    if (idx < Size) op_code = Data[idx++];
    if (idx < Size) reset_flag = Data[idx++];

    sqlite3_int64 pCurrent64 = 0;
    sqlite3_int64 pHighwater64 = 0;
    int pCurrent32 = 0;
    int pHighwater32 = 0;

    // Call sqlite3_status64 with valid arguments only to avoid NULL pointer dereference
    // The library may not check for NULL pointers if SQLITE_ENABLE_API_ARMOR is not defined
    sqlite3_status64(op_code, &pCurrent64, &pHighwater64, reset_flag);
    
    // Skip calls that pass NULL to pCurrent or pHighwater to prevent crashes
    // sqlite3_status64(op_code, NULL, &pHighwater64, reset_flag);
    // sqlite3_status64(op_code, &pCurrent64, NULL, reset_flag);
    // sqlite3_status64(op_code, NULL, NULL, reset_flag);

    // Call sqlite3_status with valid arguments only
    sqlite3_status(op_code, &pCurrent32, &pHighwater32, reset_flag);
    
    // Skip calls that pass NULL to pCurrent or pHighwater
    // sqlite3_status(op_code, NULL, &pHighwater32, reset_flag);
    // sqlite3_status(op_code, &pCurrent32, NULL, reset_flag);
    // sqlite3_status(op_code, NULL, NULL, reset_flag);

    // Call sqlite3_memory_used (no arguments)
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;

    // Extract values for sqlite3_soft_heap_limit64
    sqlite3_int64 heap_limit = 0;
    if (idx < Size) heap_limit = (sqlite3_int64)Data[idx++];
    if (idx < Size) heap_limit += (sqlite3_int64)Data[idx++];
    
    // Call sqlite3_soft_heap_limit64 with diverse limits
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(heap_limit);
    (void)prev_limit;
    
    // Test with negative value to query current limit
    sqlite3_soft_heap_limit64(-1);
    
    // Test with zero
    sqlite3_soft_heap_limit64(0);

    // Call sqlite3_memory_highwater with diverse reset flags
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    (void)highwater;
    sqlite3_memory_highwater(0);
    sqlite3_memory_highwater(1);

    return 0;
}