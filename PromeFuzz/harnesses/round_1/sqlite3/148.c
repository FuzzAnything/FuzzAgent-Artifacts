// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
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

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    const char *result = NULL;
    int rc;
    sqlite3_int64 serial_size = 0;
    unsigned char *serial_buf = NULL;

    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    // Test sqlite3_open with dummy file
    dummy_file_write(fuzz_data, fuzz_size % 1024); // Limit size for file
    rc = sqlite3_open("./dummy_file", &db);

    // Test sqlite3_errmsg with various states
    if (db) {
        result = sqlite3_errmsg(db);
        // Also test with NULL db (should be safe)
        (void)sqlite3_errmsg(NULL);
    } else {
        (void)sqlite3_errmsg(NULL);
    }

    // Test sqlite3_db_name with various N values
    if (db) {
        // Test with valid indices (0=main, 1=temp)
        result = sqlite3_db_name(db, 0);
        result = sqlite3_db_name(db, 1);
        
        // Test with out-of-range indices
        result = sqlite3_db_name(db, -1);
        result = sqlite3_db_name(db, 100);
        
        // Test with NULL db
        result = sqlite3_db_name(NULL, 0);
    }

    // Test sqlite3_open_v2 with various flags
    sqlite3 *db2 = NULL;
    int flags = 0;
    if (fuzz_size > 0) {
        // Use fuzz data to determine flags
        flags = fuzz_data[0] % 8;
        if (flags & 1) flags |= SQLITE_OPEN_READWRITE;
        if (flags & 2) flags |= SQLITE_OPEN_CREATE;
        if (flags & 4) flags |= SQLITE_OPEN_MEMORY;
        
        rc = sqlite3_open_v2("./dummy_file", &db2, flags, NULL);
        if (db2) {
            (void)sqlite3_errmsg(db2);
            sqlite3_close(db2);
        }
    }

    // Test sqlite3_serialize if db is open
    if (db) {
        // Test with main schema
        serial_buf = sqlite3_serialize(db, "main", &serial_size, 0);
        
        // Test with temp schema
        serial_buf = sqlite3_serialize(db, "temp", NULL, 0);
        
        // Test with NULL schema (should use main)
        serial_buf = sqlite3_serialize(db, NULL, &serial_size, 0);
        
        // Test with invalid schema name
        serial_buf = sqlite3_serialize(db, "invalid_schema", NULL, 0);
        
        // Test with NOCOPY flag if we got a buffer
        if (serial_buf) {
            unsigned char *nocopy_buf = sqlite3_serialize(db, "main", &serial_size, 
                                                         SQLITE_SERIALIZE_NOCOPY);
            // Don't free NOCOPY buffer - SQLite manages it
        }
        
        // Test with NULL db
        serial_buf = sqlite3_serialize(NULL, "main", &serial_size, 0);
    }

    // Test sqlite3_close
    if (db) {
        rc = sqlite3_close(db);
        // Test closing NULL (should be safe)
        rc = sqlite3_close(NULL);
    } else {
        rc = sqlite3_close(NULL);
    }

    cleanup_file();
    return 0;
}