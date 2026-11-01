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
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int read_int32(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) return 0;
    int32_t val = (data[*offset] << 24) | (data[*offset + 1] << 16) | 
                  (data[*offset + 2] << 8) | data[*offset + 3];
    *offset += 4;
    return val;
}

static int read_int16(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 2 > size) return 0;
    int16_t val = (data[*offset] << 8) | data[*offset + 1];
    *offset += 2;
    return val;
}

static int read_int8(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 1 > size) return 0;
    int8_t val = data[*offset];
    *offset += 1;
    return val;
}

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;
    int op_code = read_int8(Data, &offset, Size);
    
    // Initialize SQLite library if not already done
    sqlite3_initialize();

    // Create a dummy file for testing
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Open database connection
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }

    // Fuzz sqlite3_enable_load_extension
    if (offset < Size) {
        int onoff = read_int8(Data, &offset, Size);
        sqlite3_enable_load_extension(db, onoff);
    }

    // Fuzz sqlite3_db_config
    if (offset < Size) {
        int db_op = read_int32(Data, &offset, Size);
        if (db_op == SQLITE_DBCONFIG_LOOKASIDE) {
            // Allocate a valid memory buffer for lookaside
            int sz = read_int32(Data, &offset, Size);
            int cnt = read_int32(Data, &offset, Size);
            
            // Ensure sz is a multiple of sizeof(void*) and greater than 0
            if (sz <= 0 || sz % sizeof(void*) != 0) {
                sz = sizeof(void*) * 16; // Default fallback
            }
            if (cnt < 0) cnt = 0;

            void *buffer = malloc(sz * cnt);
            if (buffer) {
                sqlite3_db_config(db, db_op, buffer, sz, cnt);
                free(buffer);
            }
        } else {
            // For other operations, pass dummy arguments
            sqlite3_db_config(db, db_op, NULL, 0, 0);
        }
    }

    // Fuzz sqlite3_config
    if (offset < Size) {
        int config_op = read_int32(Data, &offset, Size);
        // Pass dummy arguments based on operation type
        if (config_op == SQLITE_CONFIG_SINGLETHREAD || 
            config_op == SQLITE_CONFIG_MULTITHREAD || 
            config_op == SQLITE_CONFIG_SERIALIZED) {
            sqlite3_config(config_op);
        } else if (config_op == SQLITE_CONFIG_MALLOC) {
            sqlite3_config(config_op, NULL);
        } else if (config_op == SQLITE_CONFIG_GETMALLOC) {
            sqlite3_config(config_op, NULL);
        } else if (config_op == SQLITE_CONFIG_PAGECACHE) {
            void *buf = malloc(1024);
            sqlite3_config(config_op, buf, 1024, 10);
            free(buf);
        } else if (config_op == SQLITE_CONFIG_HEAP) {
            void *heap = malloc(1024);
            sqlite3_config(config_op, heap, 1024, 128);
            free(heap);
        } else if (config_op == SQLITE_CONFIG_MEMSTATUS) {
            int onoff = read_int8(Data, &offset, Size);
            sqlite3_config(config_op, onoff);
        } else if (config_op == SQLITE_CONFIG_MUTEX) {
            sqlite3_config(config_op, NULL);
        } else if (config_op == SQLITE_CONFIG_GETMUTEX) {
            sqlite3_config(config_op, NULL);
        } else if (config_op == SQLITE_CONFIG_LOOKASIDE) {
            int sz = read_int32(Data, &offset, Size);
            int cnt = read_int32(Data, &offset, Size);
            sqlite3_config(config_op, sz, cnt);
        } else if (config_op == SQLITE_CONFIG_LOG) {
            sqlite3_config(config_op, NULL, NULL);
        } else if (config_op == SQLITE_CONFIG_URI) {
            int onoff = read_int8(Data, &offset, Size);
            sqlite3_config(config_op, onoff);
        } else {
            // Default case with dummy arguments
            sqlite3_config(config_op, NULL, 0, 0);
        }
    }

    // Fuzz sqlite3_reset_auto_extension
    sqlite3_reset_auto_extension();

    // Fuzz sqlite3_total_changes
    int changes = sqlite3_total_changes(db);
    (void)changes; // Suppress unused variable warning

    // Execute some SQL to trigger changes
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    
    sql = "INSERT INTO test (name) VALUES ('fuzz');";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    
    sql = "UPDATE test SET name = 'updated' WHERE id = 1;";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    
    sql = "DELETE FROM test WHERE id = 1;";
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    // Fuzz sqlite3_close_v2
    rc = sqlite3_close_v2(db);
    (void)rc; // Suppress unused variable warning

    // Cleanup
    sqlite3_shutdown();
    remove("./dummy_file");

    return 0;
}