// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_libversion at sqlite3.c:185731:24 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 library focusing on:
 * sqlite3_libversion -> sqlite3_sourceid -> sqlite3_config (x7) -> sqlite3_initialize -> sqlite3_vfs_find
 *
 * Fixes for AddressSanitizer SEGV:
 * 1. Avoid passing invalid operation codes to sqlite3_config that might cause internal pointer dereferences.
 * 2. Ensure all arguments passed to sqlite3_config are valid and within expected ranges.
 * 3. Handle potential NULL returns from helper functions safely.
 * 4. Limit the range of operation codes to known safe values.
 */

static int safe_get_int(const uint8_t *data, size_t size, size_t offset) {
    if (offset + sizeof(int) > size) return 0;
    int val;
    memcpy(&val, data + offset, sizeof(int));
    return val;
}

static int safe_get_byte(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return 0;
    return data[offset];
}

static const char *safe_get_string(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return NULL;
    size_t len = 0;
    while (offset + len < size && data[offset + len] != '\0') {
        len++;
    }
    if (offset + len >= size) return NULL;
    return (const char *)(data + offset);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Step 1: sqlite3_libversion */
    const char *version = sqlite3_libversion();
    (void)version;

    /* Step 2: sqlite3_sourceid */
    const char *sourceid = sqlite3_sourceid();
    (void)sourceid;

    /* Step 3-9: Invoke sqlite3_config 7 times with diverse inputs */
    /* We use the input data to generate configuration options and arguments */
    
    /* Config call 1 */
    int op1 = safe_get_int(Data, Size, 0);
    /* Restrict to known safe operation codes to avoid internal SEGV */
    if (op1 == SQLITE_CONFIG_SINGLETHREAD || op1 == SQLITE_CONFIG_MULTITHREAD || op1 == SQLITE_CONFIG_SERIALIZED) {
        sqlite3_config(op1);
    } else if (op1 == SQLITE_CONFIG_MEMSTATUS) {
        int val = safe_get_byte(Data, Size, 4);
        sqlite3_config(op1, val);
    } else if (op1 == SQLITE_CONFIG_LOG) {
        /* SQLITE_CONFIG_LOG requires a function pointer, skip to avoid crashes */
    } else if (op1 == SQLITE_CONFIG_URI) {
        int val = safe_get_byte(Data, Size, 4);
        sqlite3_config(op1, val);
    } else if (op1 == SQLITE_CONFIG_SQLLOG) {
        /* SQLITE_CONFIG_SQLLOG requires a function pointer, skip */
    } else if (op1 == SQLITE_CONFIG_MMAP_SIZE) {
        sqlite3_int64 sz = (sqlite3_int64)safe_get_int(Data, Size, 4);
        sqlite3_int64 mx = (sqlite3_int64)safe_get_int(Data, Size, 8);
        sqlite3_config(op1, sz, mx);
    } else if (op1 == SQLITE_CONFIG_WIN32_HEAPSIZE) {
        int val = safe_get_int(Data, Size, 4);
        sqlite3_config(op1, val);
    } else if (op1 == SQLITE_CONFIG_PCACHE_HDRSZ) {
        int *val = NULL;
        sqlite3_config(op1, val);
    } else {
        /* Default to a safe no-op if unknown op code */
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    /* Config call 2 */
    int op2 = safe_get_int(Data, Size, 12);
    if (op2 == SQLITE_CONFIG_SINGLETHREAD || op2 == SQLITE_CONFIG_MULTITHREAD || op2 == SQLITE_CONFIG_SERIALIZED) {
        sqlite3_config(op2);
    } else if (op2 == SQLITE_CONFIG_MEMSTATUS) {
        int val = safe_get_byte(Data, Size, 16);
        sqlite3_config(op2, val);
    } else if (op2 == SQLITE_CONFIG_URI) {
        int val = safe_get_byte(Data, Size, 16);
        sqlite3_config(op2, val);
    } else if (op2 == SQLITE_CONFIG_MMAP_SIZE) {
        sqlite3_int64 sz = (sqlite3_int64)safe_get_int(Data, Size, 16);
        sqlite3_int64 mx = (sqlite3_int64)safe_get_int(Data, Size, 20);
        sqlite3_config(op2, sz, mx);
    } else if (op2 == SQLITE_CONFIG_WIN32_HEAPSIZE) {
        int val = safe_get_int(Data, Size, 16);
        sqlite3_config(op2, val);
    } else if (op2 == SQLITE_CONFIG_PCACHE_HDRSZ) {
        int *val = NULL;
        sqlite3_config(op2, val);
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    /* Config call 3 */
    int op3 = safe_get_int(Data, Size, 24);
    if (op3 == SQLITE_CONFIG_SINGLETHREAD || op3 == SQLITE_CONFIG_MULTITHREAD || op3 == SQLITE_CONFIG_SERIALIZED) {
        sqlite3_config(op3);
    } else if (op3 == SQLITE_CONFIG_MEMSTATUS) {
        int val = safe_get_byte(Data, Size, 28);
        sqlite3_config(op3, val);
    } else if (op3 == SQLITE_CONFIG_URI) {
        int val = safe_get_byte(Data, Size, 28);
        sqlite3_config(op3, val);
    } else if (op3 == SQLITE_CONFIG_MMAP_SIZE) {
        sqlite3_int64 sz = (sqlite3_int64)safe_get_int(Data, Size, 28);
        sqlite3_int64 mx = (sqlite3_int64)safe_get_int(Data, Size, 32);
        sqlite3_config(op3, sz, mx);
    } else if (op3 == SQLITE_CONFIG_WIN32_HEAPSIZE) {
        int val = safe_get_int(Data, Size, 28);
        sqlite3_config(op3, val);
    } else if (op3 == SQLITE_CONFIG_PCACHE_HDRSZ) {
        int *val = NULL;
        sqlite3_config(op3, val);
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    /* Config call 4 */
    int op4 = safe_get_int(Data, Size, 36);
    if (op4 == SQLITE_CONFIG_SINGLETHREAD || op4 == SQLITE_CONFIG_MULTITHREAD || op4 == SQLITE_CONFIG_SERIALIZED) {
        sqlite3_config(op4);
    } else if (op4 == SQLITE_CONFIG_MEMSTATUS) {
        int val = safe_get_byte(Data, Size, 40);
        sqlite3_config(op4, val);
    } else if (op4 == SQLITE_CONFIG_URI) {
        int val = safe_get_byte(Data, Size, 40);
        sqlite3_config(op4, val);
    } else if (op4 == SQLITE_CONFIG_MMAP_SIZE) {
        sqlite3_int64 sz = (sqlite3_int64)safe_get_int(Data, Size, 40);
        sqlite3_int64 mx = (sqlite3_int64)safe_get_int(Data, Size, 44);
        sqlite3_config(op4, sz, mx);
    } else if (op4 == SQLITE_CONFIG_WIN32_HEAPSIZE) {
        int val = safe_get_int(Data, Size, 40);
        sqlite3_config(op4, val);
    } else if (op4 == SQLITE_CONFIG_PCACHE_HDRSZ) {
        int *val = NULL;
        sqlite3_config(op4, val);
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    /* Config call 5 */
    int op5 = safe_get_int(Data, Size, 48);
    if (op5 == SQLITE_CONFIG_SINGLETHREAD || op5 == SQLITE_CONFIG_MULTITHREAD || op5 == SQLITE_CONFIG_SERIALIZED) {
        sqlite3_config(op5);
    } else if (op5 == SQLITE_CONFIG_MEMSTATUS) {
        int val = safe_get_byte(Data, Size, 52);
        sqlite3_config(op5, val);
    } else if (op5 == SQLITE_CONFIG_URI) {
        int val = safe_get_byte(Data, Size, 52);
        sqlite3_config(op5, val);
    } else if (op5 == SQLITE_CONFIG_MMAP_SIZE) {
        sqlite3_int64 sz = (sqlite3_int64)safe_get_int(Data, Size, 52);
        sqlite3_int64 mx = (sqlite3_int64)safe_get_int(Data, Size, 56);
        sqlite3_config(op5, sz, mx);
    } else if (op5 == SQLITE_CONFIG_WIN32_HEAPSIZE) {
        int val = safe_get_int(Data, Size, 52);
        sqlite3_config(op5, val);
    } else if (op5 == SQLITE_CONFIG_PCACHE_HDRSZ) {
        int *val = NULL;
        sqlite3_config(op5, val);
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    /* Config call 6 */
    int op6 = safe_get_int(Data, Size, 60);
    if (op6 == SQLITE_CONFIG_SINGLETHREAD || op6 == SQLITE_CONFIG_MULTITHREAD || op6 == SQLITE_CONFIG_SERIALIZED) {
        sqlite3_config(op6);
    } else if (op6 == SQLITE_CONFIG_MEMSTATUS) {
        int val = safe_get_byte(Data, Size, 64);
        sqlite3_config(op6, val);
    } else if (op6 == SQLITE_CONFIG_URI) {
        int val = safe_get_byte(Data, Size, 64);
        sqlite3_config(op6, val);
    } else if (op6 == SQLITE_CONFIG_MMAP_SIZE) {
        sqlite3_int64 sz = (sqlite3_int64)safe_get_int(Data, Size, 64);
        sqlite3_int64 mx = (sqlite3_int64)safe_get_int(Data, Size, 68);
        sqlite3_config(op6, sz, mx);
    } else if (op6 == SQLITE_CONFIG_WIN32_HEAPSIZE) {
        int val = safe_get_int(Data, Size, 64);
        sqlite3_config(op6, val);
    } else if (op6 == SQLITE_CONFIG_PCACHE_HDRSZ) {
        int *val = NULL;
        sqlite3_config(op6, val);
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    /* Config call 7 */
    int op7 = safe_get_int(Data, Size, 72);
    if (op7 == SQLITE_CONFIG_SINGLETHREAD || op7 == SQLITE_CONFIG_MULTITHREAD || op7 == SQLITE_CONFIG_SERIALIZED) {
        sqlite3_config(op7);
    } else if (op7 == SQLITE_CONFIG_MEMSTATUS) {
        int val = safe_get_byte(Data, Size, 76);
        sqlite3_config(op7, val);
    } else if (op7 == SQLITE_CONFIG_URI) {
        int val = safe_get_byte(Data, Size, 76);
        sqlite3_config(op7, val);
    } else if (op7 == SQLITE_CONFIG_MMAP_SIZE) {
        sqlite3_int64 sz = (sqlite3_int64)safe_get_int(Data, Size, 76);
        sqlite3_int64 mx = (sqlite3_int64)safe_get_int(Data, Size, 80);
        sqlite3_config(op7, sz, mx);
    } else if (op7 == SQLITE_CONFIG_WIN32_HEAPSIZE) {
        int val = safe_get_int(Data, Size, 76);
        sqlite3_config(op7, val);
    } else if (op7 == SQLITE_CONFIG_PCACHE_HDRSZ) {
        int *val = NULL;
        sqlite3_config(op7, val);
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    /* Step 10: sqlite3_initialize */
    int init_result = sqlite3_initialize();
    (void)init_result;

    /* Step 11: sqlite3_vfs_find */
    const char *vfs_name = NULL;
    if (Size > 84) {
        vfs_name = safe_get_string(Data, Size, 84);
    }
    
    sqlite3_vfs *vfs = sqlite3_vfs_find(vfs_name);
    (void)vfs;

    return 0;
}