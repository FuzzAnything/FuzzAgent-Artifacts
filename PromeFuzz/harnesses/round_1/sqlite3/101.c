// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:70486:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:70472:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:69987:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:70161:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:70418:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for potential database operations
    create_dummy_file(Data, Size);
    
    // Initialize database connections
    sqlite3 *db1 = NULL;
    sqlite3 *db2 = NULL;
    int rc1 = sqlite3_open(":memory:", &db1);
    int rc2 = sqlite3_open("./dummy_file", &db2);
    
    if (rc1 != SQLITE_OK || rc2 != SQLITE_OK || !db1 || !db2) {
        if (db1) sqlite3_close(db1);
        if (db2) sqlite3_close(db2);
        cleanup_dummy_file();
        return 0;
    }
    
    // Test sqlite3_deserialize with various configurations
    unsigned char *data_copy = malloc(Size);
    if (data_copy) {
        memcpy(data_copy, Data, Size);
        
        // Try different schema names
        const char *schemas[] = {"main", "temp", NULL};
        for (int i = 0; schemas[i] != NULL; i++) {
            sqlite3_deserialize(db1, schemas[i], data_copy, 
                               (sqlite3_int64)Size, 
                               (sqlite3_int64)Size * 2,
                               0);
        }
        
    }
    
    // Create a backup operation
    sqlite3_backup *pBackup = sqlite3_backup_init(db2, "main", db1, "main");
    
    if (pBackup) {
        // Test backup_pagecount and backup_remaining
        int pagecount = sqlite3_backup_pagecount(pBackup);
        int remaining = sqlite3_backup_remaining(pBackup);
        
        // Perform backup steps with different page counts
        // Use fuzz data to determine step size
        int step_size = (Data[0] % 100) + 1;  // 1-100 pages per step
        
        int step_result;
        do {
            step_result = sqlite3_backup_step(pBackup, step_size);
            
            // Update progress monitoring
            pagecount = sqlite3_backup_pagecount(pBackup);
            remaining = sqlite3_backup_remaining(pBackup);
            
            // Try different step sizes occasionally
            if (Size > 1 && (step_result == SQLITE_OK)) {
                step_size = (Data[step_result % Size] % 50) + 1;
            }
            
        } while (step_result == SQLITE_OK);
        
        // Test with negative step size (copy all remaining pages)
        if (Size > 2) {
            sqlite3_backup *pBackup2 = sqlite3_backup_init(db1, "main", db2, "main");
            if (pBackup2) {
                sqlite3_backup_step(pBackup2, -1);
                sqlite3_backup_finish(pBackup2);
            }
        }
        
        // Finish the backup
        int finish_result = sqlite3_backup_finish(pBackup);
        
        // Test with NULL backup handle
        sqlite3_backup_pagecount(NULL);
        sqlite3_backup_remaining(NULL);
        sqlite3_backup_finish(NULL);
    }
    
    // Test backup operations with different database names
    if (Size > 10) {
        sqlite3_backup *pBackup3 = sqlite3_backup_init(db1, "temp", db2, "main");
        if (pBackup3) {
            sqlite3_backup_step(pBackup3, 5);
            sqlite3_backup_finish(pBackup3);
        }
    }
    
    // Cleanup
    sqlite3_close(db1);
    if (data_copy) {
        free(data_copy);
    }
    sqlite3_close(db2);
    cleanup_dummy_file();
    
    return 0;
}