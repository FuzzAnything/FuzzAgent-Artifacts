// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
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
// sqlite3_table_column_metadata at sqlite3.c:174883:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

static void cleanup_file(const char *filename) {
    remove(filename);
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    const char *result;
    int int_result;
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    
    // Test sqlite3_open and sqlite3_open_v2
    if (selector & 0x01) {
        // Create dummy file for opening
        dummy_file_write(Data, Size);
        
        // Test sqlite3_open
        rc = sqlite3_open("./dummy_file", &db);
        if (rc == SQLITE_OK) {
            // Test sqlite3_db_name with various N values
            for (int i = -1; i < 5; i++) {
                result = sqlite3_db_name(db, i);
                // Result may be NULL for out of range N
                (void)result;
            }
            
            // Test sqlite3_db_readonly with various database names
            const char *db_names[] = {"main", "temp", "invalid", NULL};
            for (int i = 0; i < 4; i++) {
                int_result = sqlite3_db_readonly(db, db_names[i]);
                (void)int_result;
            }
            
            // Test sqlite3_db_filename with various database names
            for (int i = 0; i < 4; i++) {
                sqlite3_filename filename = sqlite3_db_filename(db, db_names[i]);
                (void)filename;
            }
            
            // Create a simple table for metadata testing
            char *err_msg = NULL;
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);", 
                        NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            
            // Test sqlite3_table_column_metadata
            const char *pzDataType = NULL;
            const char *pzCollSeq = NULL;
            int pNotNull = 0;
            int pPrimaryKey = 0;
            int pAutoinc = 0;
            
            // Test with valid column
            rc = sqlite3_table_column_metadata(db, "main", "test_table", "id", 
                                              &pzDataType, &pzCollSeq, &pNotNull, 
                                              &pPrimaryKey, &pAutoinc);
            
            // Test with NULL column name (checks table existence)
            rc = sqlite3_table_column_metadata(db, "main", "test_table", NULL, 
                                              NULL, NULL, NULL, NULL, NULL);
            
            // Test with invalid column
            rc = sqlite3_table_column_metadata(db, "main", "test_table", "invalid_column", 
                                              NULL, NULL, NULL, NULL, NULL);
            
            // Test with NULL database name
            rc = sqlite3_table_column_metadata(db, NULL, "test_table", "id", 
                                              NULL, NULL, NULL, NULL, NULL);
            
            sqlite3_close(db);
            db = NULL;
        }
        
        // Test sqlite3_open_v2 with various flags
        if (Size > 1) {
            int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
            if (Data[1] & 0x01) flags |= SQLITE_OPEN_MEMORY;
            if (Data[1] & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
            if (Data[1] & 0x04) flags |= SQLITE_OPEN_FULLMUTEX;
            
            rc = sqlite3_open_v2("./dummy_file", &db2, flags, NULL);
            if (rc == SQLITE_OK) {
                // Test functions again with second database
                result = sqlite3_db_name(db2, 0);
                (void)result;
                
                int_result = sqlite3_db_readonly(db2, "main");
                (void)int_result;
                
                sqlite3_filename filename = sqlite3_db_filename(db2, "main");
                (void)filename;
                
                sqlite3_close(db2);
                db2 = NULL;
            }
        }
        
        // Clean up dummy file
        cleanup_file("./dummy_file");
    }
    
    // Test with in-memory database (no file needed)
    if (selector & 0x02) {
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK) {
            // Create table
            char *err_msg = NULL;
            sqlite3_exec(db, "CREATE TABLE mem_table (a INTEGER, b TEXT);", 
                        NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            
            // Test all functions with in-memory DB
            result = sqlite3_db_name(db, 0);
            (void)result;
            
            int_result = sqlite3_db_readonly(db, "main");
            (void)int_result;
            
            sqlite3_filename filename = sqlite3_db_filename(db, "main");
            (void)filename;
            
            // Test metadata on in-memory table
            rc = sqlite3_table_column_metadata(db, "main", "mem_table", "a", 
                                              NULL, NULL, NULL, NULL, NULL);
            
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test with empty string filename (creates temporary on-disk database)
    if (selector & 0x04) {
        rc = sqlite3_open("", &db);
        if (rc == SQLITE_OK) {
            result = sqlite3_db_name(db, 0);
            (void)result;
            
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    // Test edge cases
    if (selector & 0x08) {
        // Test with NULL database handle
        result = sqlite3_db_name(NULL, 0);
        (void)result;
        
        int_result = sqlite3_db_readonly(NULL, "main");
        (void)int_result;
        
        sqlite3_filename filename = sqlite3_db_filename(NULL, "main");
        (void)filename;
        
        // Test sqlite3_open with NULL filename
        rc = sqlite3_open(NULL, &db);
        if (rc == SQLITE_OK) {
            sqlite3_close(db);
            db = NULL;
        }
        
        // Test sqlite3_open_v2 with NULL filename
        rc = sqlite3_open_v2(NULL, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    return 0;
}