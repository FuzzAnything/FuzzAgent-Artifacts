// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3 filename manipulation APIs.
 * 
 * Target APIs:
 * - sqlite3_filename_journal
 * - sqlite3_uri_parameter
 * - sqlite3_filename_wal
 * - sqlite3_free_filename
 * - sqlite3_filename_database
 * 
 * Note: sqlite3_database_file_object is excluded as per instructions.
 */

static int create_test_filename_from_input(const uint8_t *data, size_t size, char *out_buf, size_t out_size) {
    if (size == 0 || out_size == 0) {
        return 0;
    }

    size_t copy_size = (size < out_size - 1) ? size : out_size - 1;
    memcpy(out_buf, data, copy_size);
    out_buf[copy_size] = '\0';
    return 1;
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char input_buf[1024];
    sqlite3_filename filename_obj = NULL;

    if (!create_test_filename_from_input(Data, Size, input_buf, sizeof(input_buf))) {
        return 0;
    }

    /* 
     * sqlite3_create_filename signature:
     * sqlite3_filename sqlite3_create_filename(
     *   const char *zDatabase,
     *   const char *zJournal,
     *   const char *zWal,
     *   int nParam,
     *   const char **azParam
     * );
     * 
     * We pass the constructed input as the database name.
     * Journal and WAL are NULL (let SQLite derive them).
     * No extra parameters are passed.
     * 
     * Fix: Ensure input_buf is not NULL before passing to sqlite3_create_filename.
     * The function internally calls strlen on zDatabase, zJournal, and zWal.
     * If any of these are NULL, it will crash.
     * We already ensure input_buf is valid, but we must pass NULL explicitly for zJournal and zWal.
     * However, the crash log shows strlen on zJournal or zWal being NULL.
     * Wait, the log shows: nByte = strlen(zDatabase) + strlen(zJournal) + strlen(zWal) + 10;
     * This implies zJournal or zWal is NULL.
     * But we passed NULL. Let's check the sqlite3_create_filename implementation.
     * Actually, looking at the error, it seems sqlite3_create_filename does not handle NULL for zJournal/zWal gracefully in some versions,
     * OR the issue is that we are passing NULL and the function expects non-NULL.
     * Let's re-read the documentation or common usage.
     * Usually, if you want SQLite to derive them, you pass NULL.
     * But the crash happens inside sqlite3_create_filename at strlen(zJournal).
     * This suggests that in this specific build/version, passing NULL is not allowed, or there's a bug in the library.
     * However, the standard usage allows NULL.
     * Let's try passing empty strings instead of NULL for zJournal and zWal to be safe.
     */
    
    const char *zDatabase = input_buf;
    const char *zJournal = ""; 
    const char *zWal = "";
    int nParam = 0;
    const char **azParam = NULL;

    filename_obj = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    if (filename_obj == NULL) {
        return 0;
    }

    /* Test sqlite3_filename_database */
    const char *db_name = sqlite3_filename_database(filename_obj);
    (void)db_name;

    /* Test sqlite3_filename_journal */
    const char *journal_name = sqlite3_filename_journal(filename_obj);
    (void)journal_name;

    /* Test sqlite3_filename_wal */
    const char *wal_name = sqlite3_filename_wal(filename_obj);
    (void)wal_name;

    /* Test sqlite3_uri_parameter with various parameter names */
    const char *params[] = {"mode", "immutable", "cache", "temp_store", "locking_mode", "journal_mode", "uri", "null"};
    int num_params = sizeof(params) / sizeof(params[0]);
    
    for (int i = 0; i < num_params; i++) {
        const char *val = sqlite3_uri_parameter(filename_obj, params[i]);
        (void)val;
    }

    /* Try extracting parameters from the fuzzed input itself */
    char param_name[64];
    size_t param_len = (Size < 63) ? Size : 63;
    memcpy(param_name, Data, param_len);
    param_name[param_len] = '\0';
    
    const char *val_from_input = sqlite3_uri_parameter(filename_obj, param_name);
    (void)val_from_input;

    /* Cleanup */
    sqlite3_free_filename(filename_obj);

    return 0;
}