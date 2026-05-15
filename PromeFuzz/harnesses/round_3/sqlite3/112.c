// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_112(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    // Create dummy file with fuzzer data
    create_dummy_file(data, size);
    
    // Use first byte to determine which API to test more extensively
    uint8_t selector = data[0];
    
    // Try sqlite3_open with various flags
    const char *filename = (selector & 0x01) ? ":memory:" : "./dummy_file";
    
    // Test sqlite3_open
    rc = sqlite3_open(filename, &db);
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_db_name with various N values
        for (int n = 0; n < 3; n++) {
            const char *db_name = sqlite3_db_name(db, n);
            (void)db_name; // Use result to avoid optimization
        }
        
        // Test sqlite3_db_readonly with various database names
        const char *db_names[] = {"main", "temp", NULL};
        for (int i = 0; i < 3; i++) {
            int readonly = sqlite3_db_readonly(db, db_names[i]);
            (void)readonly; // Use result
        }
        
        // Test sqlite3_db_filename
        for (int i = 0; i < 3; i++) {
            sqlite3_filename filename_ptr = sqlite3_db_filename(db, db_names[i]);
            (void)filename_ptr; // Use result
        }
        
        // Create a simple table for metadata testing if we have enough data
        if (size > 10) {
            char *err_msg = NULL;
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER, name TEXT)", 
                        NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            
            // Test sqlite3_table_column_metadata
            const char *pzDataType = NULL;
            const char *pzCollSeq = NULL;
            int pNotNull = 0, pPrimaryKey = 0, pAutoinc = 0;
            
            // Test with various parameter combinations
            sqlite3_table_column_metadata(db, "main", "test_table", "id",
                                         &pzDataType, &pzCollSeq, &pNotNull, 
                                         &pPrimaryKey, &pAutoinc);
            
            sqlite3_table_column_metadata(db, "main", "test_table", NULL,
                                         NULL, NULL, NULL, NULL, NULL);
            
            sqlite3_table_column_metadata(db, NULL, "test_table", "id",
                                         &pzDataType, NULL, &pNotNull, 
                                         NULL, &pAutoinc);
        }
        
        // Test sqlite3_open_v2 with various flags
        sqlite3 *db2 = NULL;
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (selector & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (selector & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (selector & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        
        rc = sqlite3_open_v2(filename, &db2, flags, NULL);
        if (rc == SQLITE_OK && db2) {
            // Test the other APIs on the second database connection
            for (int n = 0; n < 2; n++) {
                const char *name = sqlite3_db_name(db2, n);
                (void)name;
            }
            
            sqlite3_close(db2);
        }
        
        sqlite3_close(db);
    }
    
    // Clean up
    cleanup_dummy_file();
    
    return 0;
}