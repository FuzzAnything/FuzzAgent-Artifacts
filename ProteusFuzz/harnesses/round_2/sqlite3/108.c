// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static void fuzz_filename_construct(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    // Create a buffer with at least Size + 1 bytes to ensure null termination
    char *buf = (char *)malloc(Size + 1);
    if (!buf) return;

    memcpy(buf, Data, Size);
    buf[Size] = '\0'; // Ensure null termination

    // Fix: Pass empty strings "" instead of NULL for zJournal and zWal.
    // The internal sqlite3_create_filename implementation calls strlen() on these
    // arguments without null checks, causing a crash if NULL is passed.
    sqlite3_filename zFilename = sqlite3_create_filename(buf, "", "", 0, NULL);
    
    if (zFilename) {
        // 1. sqlite3_filename_journal
        const char *journal = sqlite3_filename_journal(zFilename);
        (void)journal; // Use result to suppress warnings

        // 2. sqlite3_filename_wal
        const char *wal = sqlite3_filename_wal(zFilename);
        (void)wal;

        // 3. sqlite3_filename_database
        const char *db = sqlite3_filename_database(zFilename);
        (void)db;

        // 4. sqlite3_uri_parameter
        // Try to parse a parameter from the fuzzed data
        const char *paramName = "mode"; 
        const char *val = sqlite3_uri_parameter(zFilename, paramName);
        (void)val;

        // Try another parameter name
        val = sqlite3_uri_parameter(zFilename, "cache");
        (void)val;

        // 5. sqlite3_uri_int64
        sqlite3_int64 intVal = sqlite3_uri_int64(zFilename, "mode", -1);
        (void)intVal;
        
        intVal = sqlite3_uri_int64(zFilename, "cache", 0);
        (void)intVal;

        // Cleanup
        sqlite3_free_filename(zFilename);
    }

    free(buf);
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Strategy 1: Direct filename fuzzing (with null termination)
    fuzz_filename_construct(Data, Size);

    // Strategy 2: Construct specific URI patterns to ensure coverage of query params
    // We prepend "file:" to ensure it's treated as a URI
    if (Size > 10) {
        char *uriBuf = (char *)malloc(Size + 10);
        if (uriBuf) {
            memcpy(uriBuf, "file:", 5);
            memcpy(uriBuf + 5, Data, Size);
            uriBuf[5 + Size] = '\0';
            
            // Fix: Pass empty strings "" instead of NULL for zJournal and zWal.
            sqlite3_filename zFilename = sqlite3_create_filename(uriBuf, "", "", 0, NULL);
            if (zFilename) {
                sqlite3_filename_journal(zFilename);
                sqlite3_filename_wal(zFilename);
                sqlite3_filename_database(zFilename);
                sqlite3_uri_parameter(zFilename, "immutable");
                sqlite3_uri_parameter(zFilename, "journal_mode");
                sqlite3_uri_int64(zFilename, "timeout", 100);
                sqlite3_free_filename(zFilename);
            }
            free(uriBuf);
        }
    }

    // Strategy 3: Test with NULL input for sqlite3_uri_parameter (safe per docs)
    // sqlite3_uri_parameter(NULL, "test") should return NULL
    const char *nullRes = sqlite3_uri_parameter(NULL, "test");
    (void)nullRes;

    // Strategy 4: Test with NULL input for sqlite3_uri_int64
    // sqlite3_uri_int64(NULL, "test", 42) should return 42
    sqlite3_int64 nullIntRes = sqlite3_uri_int64(NULL, "test", 42);
    (void)nullIntRes;

    return 0;
}