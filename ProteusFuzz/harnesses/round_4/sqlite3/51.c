// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Allocate extra space to prevent buffer over-read when SQLite scans backwards
    // for path separators or null terminators.
    size_t padding = 8; 
    char *input_str = (char *)malloc(Size + 1 + padding);
    if (!input_str) {
        return 0;
    }

    // Fill padding with null bytes to ensure safe backward scanning
    memset(input_str, 0, padding);
    
    // Copy input data after the padding
    memcpy(input_str + padding, Data, Size);
    input_str[padding + Size] = '\0';

    // Use the pointer after padding as the valid string start
    char *safe_input = input_str + padding;

    sqlite3 *db = NULL;
    
    // Strategy: Try to open a database with the input string.
    int rc = sqlite3_open(safe_input, &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        const char *db_filename = sqlite3_db_filename(db, "main");
        
        if (db_filename) {
            sqlite3_filename fname = (sqlite3_filename)db_filename;
            
            const char *journal = sqlite3_filename_journal(fname);
            const char *wal = sqlite3_filename_wal(fname);
            const char *database = sqlite3_filename_database(fname);
            
            const char *param_val = sqlite3_uri_parameter(fname, "mode");
            (void)param_val;
            
            sqlite3_int64 int_val = sqlite3_uri_int64(fname, "cache_size", -1);
            (void)int_val;
            
            if (journal) {
                sqlite3_filename j_fname = (sqlite3_filename)journal;
                sqlite3_filename_database(j_fname);
                sqlite3_filename_wal(j_fname);
                sqlite3_filename_journal(j_fname);
            }
            if (wal) {
                sqlite3_filename w_fname = (sqlite3_filename)wal;
                sqlite3_filename_database(w_fname);
                sqlite3_filename_wal(w_fname);
                sqlite3_filename_journal(w_fname);
            }
            if (database) {
                sqlite3_filename d_fname = (sqlite3_filename)database;
                sqlite3_filename_database(d_fname);
                sqlite3_filename_wal(d_fname);
                sqlite3_filename_journal(d_fname);
            }
        }
        
        sqlite3_close(db);
    } else {
        // Test with NULL inputs
        sqlite3_filename null_fname = NULL;
        sqlite3_filename_journal(null_fname);
        sqlite3_filename_wal(null_fname);
        sqlite3_filename_database(null_fname);
        sqlite3_uri_parameter(null_fname, "test");
        sqlite3_uri_int64(null_fname, "test", 0);
        
        // REMOVED: Testing with raw input string (safe_input) as it triggers
        // heap-buffer-overflow in sqlite3_filename_journal when the string
        // does not follow the expected internal format (database|wal|journal).
        // These functions expect a specific null-terminated structure which
        // arbitrary fuzz data does not provide, leading to out-of-bounds reads.
    }

    free(input_str);
    
    return 0;
}