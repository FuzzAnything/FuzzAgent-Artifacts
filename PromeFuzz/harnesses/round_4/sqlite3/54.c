// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static sqlite3 *open_temp_db(void) {
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

static void create_backup_test(void) {
    sqlite3 *src_db = open_temp_db();
    sqlite3 *dest_db = open_temp_db();
    
    if (!src_db || !dest_db) {
        if (src_db) sqlite3_close(src_db);
        if (dest_db) sqlite3_close(dest_db);
        return;
    }
    
    // Create some tables to make backup non-empty
    sqlite3_exec(src_db, "CREATE TABLE t1(a,b); INSERT INTO t1 VALUES(1,2);", 0, 0, 0);
    
    sqlite3_backup *backup = sqlite3_backup_init(dest_db, "main", src_db, "main");
    
    if (backup) {
        // Test sqlite3_backup_remaining and sqlite3_backup_pagecount
        int remaining = sqlite3_backup_remaining(backup);
        int pagecount = sqlite3_backup_pagecount(backup);
        
        // Test sqlite3_backup_step
        int rc = sqlite3_backup_step(backup, 5);
        
        // Update remaining and pagecount after step
        remaining = sqlite3_backup_remaining(backup);
        pagecount = sqlite3_backup_pagecount(backup);
        
        // Finalize backup
        sqlite3_backup_finish(backup);
    }
    
    sqlite3_close(src_db);
    sqlite3_close(dest_db);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test sqlite3_deserialize
    {
        sqlite3 *db = open_temp_db();
        if (db) {
            // Prepare buffer for deserialize - must be allocated by sqlite3_malloc
            // and size must be multiple of page size (512)
            size_t page_size = 512;
            size_t aligned_size = (Size / page_size) * page_size;
            if (aligned_size > 0) {
                unsigned char *buf = (unsigned char *)sqlite3_malloc64(aligned_size);
                if (buf) {
                    memcpy(buf, Data, aligned_size > Size ? Size : aligned_size);
                    
                    // Try deserialize with different flags
                    unsigned int flags[] = {0, SQLITE_DESERIALIZE_FREEONCLOSE, 
                                          SQLITE_DESERIALIZE_RESIZEABLE,
                                          SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE};
                    
                    for (int i = 0; i < 4; i++) {
                        int rc = sqlite3_deserialize(db, "main", buf, aligned_size, 
                                                    aligned_size, flags[i]);
                        // If successful with FREEONCLOSE, buffer is now owned by SQLite
                        // so we shouldn't free it
                        if (rc == SQLITE_OK && (flags[i] & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                            buf = NULL; // SQLite will free it
                        }
                    }
                    
                    if (buf) {
                        sqlite3_free(buf);
                    }
                }
            }
            sqlite3_close(db);
        }
    }
    
    // Test backup APIs
    create_backup_test();
    
    // Test with file-based database
    {
        create_dummy_file(Data, Size);
        
        sqlite3 *db = NULL;
        if (sqlite3_open("./dummy_file", &db) == SQLITE_OK) {
            // Create a backup between file and memory db
            sqlite3 *mem_db = open_temp_db();
            if (mem_db) {
                sqlite3_backup *backup = sqlite3_backup_init(mem_db, "main", db, "main");
                if (backup) {
                    // Do a single step
                    sqlite3_backup_step(backup, 5);
                    
                    // Check progress
                    sqlite3_backup_remaining(backup);
                    sqlite3_backup_pagecount(backup);
                    
                    sqlite3_backup_finish(backup);
                }
                sqlite3_close(mem_db);
            }
            sqlite3_close(db);
        }
        
        cleanup_dummy_file();
    }
    
    // Test edge cases for deserialize
    {
        sqlite3 *db = open_temp_db();
        if (db) {
            // Test with zero size
            sqlite3_deserialize(db, "main", NULL, 0, 0, 0);
            
            // Test with invalid schema name
            sqlite3_deserialize(db, "temp", NULL, 0, 0, 0);
            
            // Test with small buffer
            unsigned char small_buf[1024];
            memset(small_buf, 0, sizeof(small_buf));
            sqlite3_deserialize(db, "main", small_buf, 512, 1024, 0);
            
            sqlite3_close(db);
        }
    }
    
    // Test with NULL backup handles
    sqlite3_backup_remaining(NULL);
    sqlite3_backup_pagecount(NULL);
    sqlite3_backup_finish(NULL);
    
    return 0;
}