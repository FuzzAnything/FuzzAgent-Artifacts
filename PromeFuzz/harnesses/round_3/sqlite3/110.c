// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "sqlite3.h"

/* Busy handler callback for sqlite3_busy_handler */
static int busy_handler_callback(void* data, int count) {
    /* Return non-zero to retry, zero to abort */
    return (count < 5) ? 1 : 0;  /* Retry up to 5 times */
}

/* Busy handler callback that uses fuzz data */
static int busy_handler_callback_fuzz(void* data, int count) {
    int* fuzz_data = (int*)data;
    /* Use fuzz data to decide whether to retry */
    return (count < (*fuzz_data & 0xF)) ? 1 : 0;
}

/* Busy handler callback that always returns 0 (abort) */
static int busy_handler_callback_abort(void* data, int count) {
    return 0;
}

/* Busy handler callback that always returns 1 (retry) */
static int busy_handler_callback_retry(void* data, int count) {
    return 1;
}

/* Fuzz driver entry point */
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    /* Ensure we have at least some data */
    if (Size < 1) {
        return 0;
    }

    /* Use the first byte to determine which functions to test */
    uint8_t selector = Data[0];
    
    /* Test sqlite3_sleep */
    if (selector & 0x01) {
        /* Use up to 4 bytes for sleep time, interpret as signed int */
        int sleep_ms = 0;
        if (Size >= 5) {
            memcpy(&sleep_ms, Data + 1, sizeof(int));
        } else if (Size >= 2) {
            sleep_ms = (int)Data[1];
        }
        
        /* Call sqlite3_sleep with the derived value */
        int actual_sleep = sqlite3_sleep(sleep_ms);
        (void)actual_sleep; /* Ignore return value */
    }
    
    /* Test sqlite3_open and related functions */
    if (selector & 0x02) {
        sqlite3* db = NULL;
        const char* filename = "./dummy_file";
        int rc;
        
        /* Create a dummy file for database operations */
        FILE* fp = fopen(filename, "wb");
        if (fp) {
            /* Write some initial data if available */
            if (Size > 0) {
                size_t write_size = Size > 1024 ? 1024 : Size;
                fwrite(Data, 1, write_size, fp);
            }
            fclose(fp);
        }
        
        /* Test sqlite3_open */
        rc = sqlite3_open(filename, &db);
        
        if (rc == SQLITE_OK && db != NULL) {
            /* Test sqlite3_busy_handler with different callbacks */
            if (Size >= 2) {
                int fuzz_value = (int)Data[1];
                sqlite3_busy_handler(db, busy_handler_callback_fuzz, &fuzz_value);
                sqlite3_busy_handler(db, busy_handler_callback, NULL);
                sqlite3_busy_handler(db, busy_handler_callback_abort, NULL);
                sqlite3_busy_handler(db, busy_handler_callback_retry, NULL);
            } else {
                sqlite3_busy_handler(db, busy_handler_callback, NULL);
            }
            
            /* Test sqlite3_busy_timeout */
            if (Size >= 3) {
                int timeout_ms = (int)Data[2] * 10; /* Scale to get meaningful values */
                sqlite3_busy_timeout(db, timeout_ms);
                
                /* Also test with negative and zero values */
                sqlite3_busy_timeout(db, -1);
                sqlite3_busy_timeout(db, 0);
            }
            
            /* Test sqlite3_setlk_timeout if supported */
            if (Size >= 4) {
                int setlk_timeout = (int)Data[3] * 10;
                int flags = (Size >= 5) ? (int)Data[4] & 0x01 : 0; /* Use bit 0 for flags */
                sqlite3_setlk_timeout(db, setlk_timeout, flags);
                
                /* Test edge cases */
                sqlite3_setlk_timeout(db, 0, 0);
                sqlite3_setlk_timeout(db, -1, 0);
            }
            
            /* Test sqlite3_is_interrupted */
            int interrupted = sqlite3_is_interrupted(db);
            (void)interrupted; /* Ignore return value */
            
            /* Close the database */
            sqlite3_close(db);
        } else {
            /* If open failed, still try to close if db was partially created */
            if (db) {
                sqlite3_close(db);
            }
        }
        
        /* Remove the dummy file */
        remove(filename);
    }
    
    /* Additional tests with in-memory database */
    if (selector & 0x04) {
        sqlite3* db = NULL;
        int rc;
        
        /* Open an in-memory database */
        rc = sqlite3_open(":memory:", &db);
        
        if (rc == SQLITE_OK && db != NULL) {
            /* Test all the busy/timeout functions on in-memory db */
            sqlite3_busy_handler(db, busy_handler_callback, NULL);
            
            if (Size >= 2) {
                int timeout = (int)Data[1] * 10;
                sqlite3_busy_timeout(db, timeout);
            }
            
            if (Size >= 3) {
                int setlk_timeout = (int)Data[2] * 10;
                sqlite3_setlk_timeout(db, setlk_timeout, 0);
            }
            
            /* Test interruption check */
            sqlite3_is_interrupted(db);
            
            sqlite3_close(db);
        }
    }
    
    /* Test edge cases for sqlite3_sleep */
    if (selector & 0x08) {
        /* Test with various edge values */
        sqlite3_sleep(0);
        sqlite3_sleep(1);
        sqlite3_sleep(100);
        
        /* Test with negative value (should be treated as 0 in recent versions) */
        sqlite3_sleep(-1);
        sqlite3_sleep(-100);
        
        /* Test with large value */
        sqlite3_sleep(10000);
    }
    
    /* Test sqlite3_open with various filenames */
    if (selector & 0x10) {
        sqlite3* db = NULL;
        
        /* Test with empty string (temporary database) */
        sqlite3_open("", &db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
        
        /* Test with NULL filename (should fail) */
        sqlite3_open(NULL, &db);
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
        
        /* Test with a filename derived from fuzz data */
        if (Size >= 2) {
            char temp_filename[256];
            snprintf(temp_filename, sizeof(temp_filename), "./temp_db_%d", (int)Data[1]);
            
            sqlite3_open(temp_filename, &db);
            if (db) {
                sqlite3_close(db);
                db = NULL;
            }
            
            remove(temp_filename);
        }
    }
    
    return 0;
}