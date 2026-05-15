// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:17178:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fuzz_soft_heap_limit64(const uint8_t *data, size_t size) {
    if (size < sizeof(sqlite3_int64)) return;
    
    sqlite3_int64 limit;
    memcpy(&limit, data, sizeof(sqlite3_int64));
    
    // Test setting the limit
    sqlite3_int64 prev = sqlite3_soft_heap_limit64(limit);
    (void)prev;
    
    // Test querying the current limit
    prev = sqlite3_soft_heap_limit64(-1);
    (void)prev;
    
    // Test disabling the limit
    prev = sqlite3_soft_heap_limit64(0);
    (void)prev;
    
    // Restore previous value
    sqlite3_soft_heap_limit64(prev);
}

static void fuzz_release_memory(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int n;
    memcpy(&n, data, sizeof(int));
    
    // Ensure n is non-negative for the API
    if (n < 0) n = -n;
    
    int freed = sqlite3_release_memory(n);
    (void)freed;
}

static void fuzz_memory_highwater(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    int resetFlag = data[0] & 1;
    
    sqlite3_int64 highwater = sqlite3_memory_highwater(resetFlag);
    (void)highwater;
}

static void fuzz_soft_heap_limit(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int n;
    memcpy(&n, data, sizeof(int));
    
    sqlite3_soft_heap_limit(n);
}

static void fuzz_db_release_memory(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return;
    }
    
    // Execute some SQL to allocate memory
    sqlite3_exec(db, "CREATE TABLE t1(a, b);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2);", NULL, NULL, NULL);
    sqlite3_exec(db, "SELECT * FROM t1;", NULL, NULL, NULL);
    
    // Try to release memory
    int freed = sqlite3_db_release_memory(db);
    (void)freed;
    
    sqlite3_close(db);
}

static void fuzz_status(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    int op = data[0] % 10;  // There are 10 status parameters (SQLITE_STATUS_MEMORY_USED, etc.)
    int resetFlag = data[1] & 1;
    int current = 0;
    int highwater = 0;
    
    int rc = sqlite3_status(op, &current, &highwater, resetFlag);
    (void)rc;
    (void)current;
    (void)highwater;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
    // Ensure we have some minimal data
    if (size < 1) return 0;
    
    // Use the first byte to decide which functions to call
    uint8_t selector = data[0];
    size_t offset = 1;
    
    // Call different combinations of functions based on selector bits
    if (selector & 0x01) {
        if (offset + sizeof(sqlite3_int64) <= size) {
            fuzz_soft_heap_limit64(data + offset, size - offset);
            offset += sizeof(sqlite3_int64);
        }
    }
    
    if (selector & 0x02) {
        if (offset + sizeof(int) <= size) {
            fuzz_release_memory(data + offset, size - offset);
            offset += sizeof(int);
        }
    }
    
    if (selector & 0x04) {
        if (offset + 1 <= size) {
            fuzz_memory_highwater(data + offset, size - offset);
            offset += 1;
        }
    }
    
    if (selector & 0x08) {
        if (offset + sizeof(int) <= size) {
            fuzz_soft_heap_limit(data + offset, size - offset);
            offset += sizeof(int);
        }
    }
    
    if (selector & 0x10) {
        fuzz_db_release_memory(data + offset, size - offset);
        // Don't increment offset as this function doesn't consume input
    }
    
    if (selector & 0x20) {
        if (offset + 2 <= size) {
            fuzz_status(data + offset, size - offset);
            offset += 2;
        }
    }
    
    // Also test some combinations in sequence
    if (size >= sizeof(sqlite3_int64) + sizeof(int) + 3) {
        // Test setting limit, then releasing memory, then checking highwater
        sqlite3_int64 limit;
        memcpy(&limit, data, sizeof(sqlite3_int64));
        sqlite3_soft_heap_limit64(limit);
        
        int n;
        memcpy(&n, data + sizeof(sqlite3_int64), sizeof(int));
        if (n < 0) n = -n;
        sqlite3_release_memory(n);
        
        int resetFlag = data[sizeof(sqlite3_int64) + sizeof(int)] & 1;
        sqlite3_memory_highwater(resetFlag);
    }
    
    return 0;
}