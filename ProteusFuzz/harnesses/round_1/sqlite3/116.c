// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void setup_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *dummy_file = "./dummy_file";
    setup_dummy_file(dummy_file, Data, Size);

    // Create a valid sqlite3_filename using sqlite3_create_filename.
    // The documentation states that passing raw strings (like dummy_file) to 
    // sqlite3_uri_int64, sqlite3_uri_parameter, etc. is undefined behavior 
    // and can cause crashes. We must use sqlite3_create_filename to construct 
    // a proper sqlite3_filename object.
    
    // Ensure the input string is null-terminated for safety, though create_filename handles it.
    // We pass dummy_file directly. The ASAN error suggests that passing NULL for zJournal or zWal 
    // in sqlite3_create_filename might be causing issues if the internal logic expects valid pointers 
    // or if the specific version behaves differently. 
    // However, the signature is: sqlite3_filename sqlite3_create_filename(const char *zDatabase, 
    // const char *zJournal, const char *zWal, int nDb, const char *zUri);
    // Let's try passing an empty string or ensuring we don't trigger the internal strlen on NULL.
    // Actually, the error trace shows strlen(zJournal) being called where zJournal is NULL.
    // The API documentation for sqlite3_create_filename says zJournal and zWal can be NULL.
    // But if the internal implementation in this specific build crashes on NULL, we should pass empty strings.
    
    sqlite3_filename fn = sqlite3_create_filename(dummy_file, "", "", 0, NULL);
    
    if (fn) {
        // Test sqlite3_uri_int64 with the valid filename object
        sqlite3_int64 val1 = sqlite3_uri_int64(fn, "mode", 100);
        (void)val1;

        val1 = sqlite3_uri_int64(fn, "cache", -1);
        (void)val1;

        // Test sqlite3_uri_parameter
        const char *param = sqlite3_uri_parameter(fn, "mode");
        (void)param;

        param = sqlite3_uri_parameter(fn, "nonexistent");
        (void)param;

        // Test sqlite3_filename_wal
        const char *wal = sqlite3_filename_wal(fn);
        (void)wal;

        // Test sqlite3_filename_journal
        const char *journal = sqlite3_filename_journal(fn);
        (void)journal;

        // Test sqlite3_filename_database
        const char *db = sqlite3_filename_database(fn);
        (void)db;

        // Cleanup the created filename
        sqlite3_free_filename(fn);
    }

    // Test with NULL to ensure robustness (safe according to docs)
    sqlite3_int64 val_null = sqlite3_uri_int64(NULL, "mode", 999);
    (void)val_null;

    const char *param_null = sqlite3_uri_parameter(NULL, "mode");
    (void)param_null;

    return 0;
}