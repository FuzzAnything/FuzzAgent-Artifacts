// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_last_insert_rowid at sqlite3.c:186738:25 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:189458:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for SQLite collation and context APIs.
 * Target APIs:
 * 1. sqlite3_collation_needed
 * 2. sqlite3_collation_needed16
 * 3. sqlite3_create_collation16
 * 4. sqlite3_create_collation
 * 5. sqlite3_context_db_handle
 * 6. sqlite3_create_collation_v2
 */

static int compare_utf8(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    (void)pArg;
    if (nLeft == 0 || nRight == 0) return nLeft - nRight;
    return memcmp(pLeft, pRight, (nLeft < nRight) ? nLeft : nRight);
}

static int compare_utf16(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    (void)pArg;
    if (nLeft == 0 || nRight == 0) return nLeft - nRight;
    return memcmp(pLeft, pRight, (nLeft < nRight) ? nLeft : nRight);
}

static void coll_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pArg;
    (void)eTextRep;
    if (zName == NULL) return;
    sqlite3_create_collation(db, zName, SQLITE_UTF8, NULL, compare_utf8);
}

static void coll_needed_callback16(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    (void)pArg;
    (void)eTextRep;
    if (zName == NULL) return;
    sqlite3_create_collation16(db, zName, SQLITE_UTF16LE, NULL, compare_utf16);
}

/* 
 * A safe destructor that does nothing.
 * We avoid passing malloc'd memory as pArg to sqlite3_create_collation_v2
 * to prevent double-free issues when SQLite closes the connection.
 */
static void safe_destroy_arg(void *pArg) {
    (void)pArg;
}

/* Fixed signature: must return void, not int */
static void context_test_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3 *db_handle = sqlite3_context_db_handle(ctx);
    if (db_handle) {
        sqlite3_last_insert_rowid(db_handle);
    }
    sqlite3_result_int(ctx, 0);
}

static sqlite3 *create_db_from_input(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    db = create_db_from_input(Data, Size);
    if (db == NULL) {
        return 0;
    }
    
    /* 1. sqlite3_collation_needed */
    rc = sqlite3_collation_needed(db, (void *)Data, coll_needed_callback);
    (void)rc;
    
    /* 2. sqlite3_collation_needed16 */
    rc = sqlite3_collation_needed16(db, (void *)Data, coll_needed_callback16);
    (void)rc;
    
    /* 3. sqlite3_create_collation */
    if (Size > 0) {
        char name[256];
        size_t name_len = (Size < 255) ? Size : 255;
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        
        rc = sqlite3_create_collation(db, name, SQLITE_UTF8, (void *)Data, compare_utf8);
        (void)rc;
        
        rc = sqlite3_create_collation(db, name, SQLITE_UTF16LE, (void *)Data, compare_utf16);
        (void)rc;
        
        rc = sqlite3_create_collation(db, name, SQLITE_UTF16BE, (void *)Data, compare_utf16);
        (void)rc;
        
        rc = sqlite3_create_collation(db, name, SQLITE_UTF8, (void *)Data, NULL);
        (void)rc;
    }
    
    /* 4. sqlite3_create_collation16 */
    if (Size > 0) {
        size_t utf16_len = (Size < 254) ? Size : 254;
        size_t alloc_size = utf16_len * 2 + 2;
        void *utf16_name = malloc(alloc_size);
        
        if (utf16_name) {
            for (size_t i = 0; i < utf16_len; i++) {
                ((uint16_t *)utf16_name)[i] = Data[i];
            }
            ((uint16_t *)utf16_name)[utf16_len] = 0;
            
            rc = sqlite3_create_collation16(db, utf16_name, SQLITE_UTF16LE, (void *)Data, compare_utf16);
            (void)rc;
            
            rc = sqlite3_create_collation16(db, utf16_name, SQLITE_UTF16BE, (void *)Data, compare_utf16);
            (void)rc;
            
            rc = sqlite3_create_collation16(db, utf16_name, SQLITE_UTF16LE, (void *)Data, NULL);
            (void)rc;
            
            free(utf16_name);
        }
    }
    
    /* 5. sqlite3_create_collation_v2 */
    /* 
     * To avoid double-free, we pass a pointer to static data or a pointer 
     * that SQLite will not free (using a no-op destructor).
     * We do NOT malloc memory here and pass it with a real destructor.
     */
    if (Size > 0) {
        char name[256];
        size_t name_len = (Size < 255) ? Size : 255;
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        
        /* Use the input data pointer directly, but with a no-op destructor 
         * so SQLite doesn't try to free it. */
        rc = sqlite3_create_collation_v2(db, name, SQLITE_UTF8, (void *)Data, compare_utf8, safe_destroy_arg);
        (void)rc;
        
        rc = sqlite3_create_collation_v2(db, name, SQLITE_UTF16LE, (void *)Data, compare_utf16, safe_destroy_arg);
        (void)rc;
    }
    
    /* 6. sqlite3_context_db_handle via function registration */
    rc = sqlite3_create_function(db, "context_test", 0, SQLITE_UTF8, NULL, context_test_func, NULL, NULL);
    (void)rc;
    
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT context_test();", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        (void)rc;
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return 0;
}