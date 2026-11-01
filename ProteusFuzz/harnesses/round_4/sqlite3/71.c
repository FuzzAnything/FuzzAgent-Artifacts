// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper function to safely read a 64-bit integer from fuzz data */
static sqlite3_int64 read_int64(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(sqlite3_int64) > Size) {
        return 0;
    }
    sqlite3_int64 val = 0;
    memcpy(&val, Data + *offset, sizeof(sqlite3_int64));
    *offset += sizeof(sqlite3_int64);
    return val;
}

/* Helper function to safely read a 32-bit integer from fuzz data */
static int read_int32(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int) > Size) {
        return 0;
    }
    int val = 0;
    memcpy(&val, Data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/* Helper function to safely read an 8-bit integer from fuzz data */
static int read_int8(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int8_t) > Size) {
        return 0;
    }
    int8_t val = Data[*offset];
    *offset += sizeof(int8_t);
    return (int)val;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_int64 current64, highwater64;
    int current32, highwater32;
    int op, resetFlag;
    size_t offset = 0;

    /* Initialize SQLite library if needed (usually done once at startup) */
    sqlite3_initialize();

    /* Create a dummy file for database operations */
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        fclose(f);
    }

    /* Open database connection */
    if (sqlite3_open(dummy_file, &db) != SQLITE_OK) {
        /* If we can't open the database, we can still test global status functions */
    }

    /* Test sqlite3_status64 */
    if (Size >= 3 * sizeof(int)) {
        op = read_int32(Data, Size, &offset);
        resetFlag = read_int32(Data, Size, &offset);
        /* Use valid pointers for output */
        current64 = 0;
        highwater64 = 0;
        sqlite3_status64(op, &current64, &highwater64, resetFlag);
    }

    /* Test sqlite3_db_status64 */
    if (db && Size >= 3 * sizeof(int)) {
        op = read_int32(Data, Size, &offset);
        resetFlag = read_int32(Data, Size, &offset);
        current64 = 0;
        highwater64 = 0;
        sqlite3_db_status64(db, op, &current64, &highwater64, resetFlag);
    }

    /* Test sqlite3_memory_used */
    (void)sqlite3_memory_used();

    /* Test sqlite3_memory_highwater */
    if (Size > offset) {
        resetFlag = read_int8(Data, Size, &offset);
        (void)sqlite3_memory_highwater(resetFlag);
    }

    /* Test sqlite3_soft_heap_limit64 */
    if (Size >= sizeof(sqlite3_int64)) {
        sqlite3_int64 limit = read_int64(Data, Size, &offset);
        (void)sqlite3_soft_heap_limit64(limit);
    }

    /* Test sqlite3_status */
    if (Size >= 3 * sizeof(int)) {
        op = read_int32(Data, Size, &offset);
        resetFlag = read_int32(Data, Size, &offset);
        current32 = 0;
        highwater32 = 0;
        sqlite3_status(op, &current32, &highwater32, resetFlag);
    }

    /* Cleanup */
    if (db) {
        sqlite3_close(db);
    }

    /* Remove dummy file */
    remove(dummy_file);

    return 0;
}