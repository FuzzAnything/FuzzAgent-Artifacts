// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <fcntl.h>
#include <unistd.h>

static int create_dummy_db(const char *path) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open_v2(path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        return -1;
    }
    
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY);";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }
    
    sqlite3_close(db);
    return 0;
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input is null-terminated for string operations
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    const char *dummy_path = "./dummy_file";
    
    if (create_dummy_db(dummy_path) != 0) {
        free(input_str);
        return 0;
    }

    sqlite3 *db = NULL;
    int rc = sqlite3_open(dummy_path, &db);
    if (rc != SQLITE_OK || db == NULL) {
        free(input_str);
        return 0;
    }

    // Get the filename from the connection. This returns a valid sqlite3_filename.
    const char *valid_filename = sqlite3_db_filename(db, "main");
    
    // Fuzzing the filename translation APIs with the valid filename from the DB
    if (valid_filename) {
        const char *wal_name = sqlite3_filename_wal(valid_filename);
        const char *journal_name = sqlite3_filename_journal(valid_filename);
        const char *db_name = sqlite3_filename_database(valid_filename);
        
        (void)wal_name;
        (void)journal_name;
        (void)db_name;

        // Test URI parameter extraction
        char param_name[64];
        size_t param_len = Size < 63 ? Size : 63;
        memcpy(param_name, input_str, param_len);
        param_name[param_len] = '\0';
        
        const char *param_val = sqlite3_uri_parameter(valid_filename, param_name);
        (void)param_val;
    }

    // Strategy 2: REMOVED.
    // The original code cast raw input to sqlite3_filename and passed it to 
    // sqlite3_filename_wal/journal/database. These functions assume the input
    // points to a specific internal structure (containing multiple null-terminated
    // strings) and may read backwards (e.g., zName[-1]) to find boundaries.
    // Passing a raw malloc'd buffer causes a heap-buffer-overflow (underflow).
    // Since we cannot safely construct a fake internal filename structure without
    // internal API access, we skip this unsafe strategy to prevent crashes.

    // Cleanup
    sqlite3_close(db);
    free(input_str);
    
    return 0;
}