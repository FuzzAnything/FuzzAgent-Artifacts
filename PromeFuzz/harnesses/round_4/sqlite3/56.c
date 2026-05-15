// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

static sqlite3 *open_temp_db(void) {
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

static sqlite3 *open_file_db(void) {
    sqlite3 *db = NULL;
    if (sqlite3_open("./dummy_file", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

static unsigned char *allocate_sqlite_buffer(size_t size) {
    return (unsigned char *)sqlite3_malloc64(size);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    create_dummy_file(Data, Size);
    
    // Test sqlite3_deserialize
    sqlite3 *db1 = open_temp_db();
    if (db1) {
        // Prepare a buffer with sqlite3_malloc for SQLITE_DESERIALIZE_FREEONCLOSE
        size_t alloc_size = Size > 4096 ? Size : 4096;
        unsigned char *buf = allocate_sqlite_buffer(alloc_size);
        if (buf) {
            // Copy fuzzer data into buffer
            size_t copy_size = Size < alloc_size ? Size : alloc_size;
            memcpy(buf, Data, copy_size);
            
            // Ensure buffer size is multiple of page size (512)
            sqlite3_int64 szBuf = (copy_size / 512) * 512;
            if (szBuf < 512) szBuf = 512;
            sqlite3_int64 szDb = szBuf;
            
            // Try deserialize with different flags
            unsigned int flags[] = {0, SQLITE_DESERIALIZE_FREEONCLOSE, 
                                   SQLITE_DESERIALIZE_RESIZEABLE,
                                   SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE};
            
            for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
                // Use a copy of the buffer for each attempt
                unsigned char *buf_copy = allocate_sqlite_buffer(szBuf);
                if (buf_copy) {
                    memcpy(buf_copy, buf, szBuf < copy_size ? szBuf : copy_size);
                    sqlite3_deserialize(db1, "main", buf_copy, szDb, szBuf, flags[i]);
                    // If FREEONCLOSE flag wasn't set, we need to free the buffer
                    if (!(flags[i] & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                        sqlite3_free(buf_copy);
                    }
                }
            }
            
            sqlite3_free(buf);
        }
        sqlite3_close(db1);
    }
    
    // Test backup APIs
    sqlite3 *src_db = open_temp_db();
    sqlite3 *dest_db = open_file_db();
    
    if (src_db && dest_db) {
        // Create a simple table in source database
        char *err_msg = NULL;
        sqlite3_exec(src_db, "CREATE TABLE IF NOT EXISTS t1(a,b);", NULL, NULL, &err_msg);
        if (err_msg) sqlite3_free(err_msg);
        
        // Initialize backup
        sqlite3_backup *backup = sqlite3_backup_init(dest_db, "main", src_db, "main");
        
        if (backup) {
            // Test backup_pagecount and backup_remaining before step
            int pagecount = sqlite3_backup_pagecount(backup);
            int remaining = sqlite3_backup_remaining(backup);
            
            // Perform backup steps with different page counts
            int step_sizes[] = {1, 5, -1, (int)(Data[0] % 100)};
            
            for (size_t i = 0; i < sizeof(step_sizes)/sizeof(step_sizes[0]); i++) {
                int result = sqlite3_backup_step(backup, step_sizes[i]);
                
                // Test monitoring functions after each step
                pagecount = sqlite3_backup_pagecount(backup);
                remaining = sqlite3_backup_remaining(backup);
                
                if (result == SQLITE_DONE || result == SQLITE_OK) {
                    // Continue
                } else if (result != SQLITE_BUSY && result != SQLITE_LOCKED) {
                    break;
                }
            }
            
            // Finish backup
            sqlite3_backup_finish(backup);
        }
        
        // Test with NULL backup handle
        sqlite3_backup_pagecount(NULL);
        sqlite3_backup_remaining(NULL);
        
        sqlite3_close(src_db);
        sqlite3_close(dest_db);
    } else {
        if (src_db) sqlite3_close(src_db);
        if (dest_db) sqlite3_close(dest_db);
    }
    
    // Additional test: try backup between two in-memory databases
    sqlite3 *mem1 = open_temp_db();
    sqlite3 *mem2 = open_temp_db();
    
    if (mem1 && mem2) {
        sqlite3_backup *backup2 = sqlite3_backup_init(mem2, "main", mem1, "main");
        if (backup2) {
            sqlite3_backup_step(backup2, -1);
            sqlite3_backup_finish(backup2);
        }
        sqlite3_close(mem1);
        sqlite3_close(mem2);
    }
    
    remove_dummy_file();
    return 0;
}