// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void write_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file("./dummy_file");

    // The sqlite3_filename functions (e.g., databaseName) scan backwards 
    // from the input pointer to find a separator or the start of the string.
    // If the input pointer is at the very beginning of the allocated buffer,
    // reading backwards causes a heap-buffer-overflow.
    // We must ensure there is a safe "guard" region before the string data
    // that the library can read without crashing.
    
    // Allocate extra space: 4 bytes for the fake header + 4 bytes guard + input size + null terminator
    size_t alloc_size = Size + 16; 
    char *raw_buf = (char *)malloc(alloc_size);
    if (!raw_buf) {
        return 0;
    }

    // Initialize the entire buffer to zero.
    // This ensures that any backward scan by the library will hit a null byte 
    // within the allocated memory, stopping the scan safely.
    memset(raw_buf, 0, alloc_size);

    // We place the actual data at offset 8.
    // Offset 0-3: Fake header (as expected by some internal logic if it exists)
    // Offset 4-7: Guard zone (zeros) to prevent out-of-bounds reads when scanning backwards.
    // Offset 8+: Input data.
    size_t offset = 8;
    size_t space = alloc_size - offset;
    size_t len = (Size < space) ? Size : space;
    
    memcpy(raw_buf + offset, Data, len);
    raw_buf[offset + len] = '\0';

    // Set up the fake header at the beginning.
    raw_buf[0] = 0x01; 
    raw_buf[1] = 0x00;
    raw_buf[2] = 0x00;
    raw_buf[3] = 0x00;

    // Point to the start of the string (after the guard zone).
    sqlite3_filename z = (sqlite3_filename)(raw_buf + offset);

    // Test sqlite3_filename_database
    const char *db_name = sqlite3_filename_database(z);
    (void)db_name;

    // Test sqlite3_filename_journal
    const char *journal_name = sqlite3_filename_journal(z);
    (void)journal_name;

    // Test sqlite3_filename_wal
    const char *wal_name = sqlite3_filename_wal(z);
    (void)wal_name;

    // Test sqlite3_uri_parameter
    const char *param_val = sqlite3_uri_parameter(z, "mode");
    (void)param_val;
    
    param_val = sqlite3_uri_parameter(z, "immutable");
    (void)param_val;

    // Test sqlite3_uri_int64
    sqlite3_int64 int_val = sqlite3_uri_int64(z, "cache_size", -1);
    (void)int_val;

    // Test with NULL input
    const char *null_db = sqlite3_filename_database(NULL);
    (void)null_db;
    const char *null_journal = sqlite3_filename_journal(NULL);
    (void)null_journal;
    const char *null_wal = sqlite3_filename_wal(NULL);
    (void)null_wal;
    const char *null_param = sqlite3_uri_parameter(NULL, "test");
    (void)null_param;
    sqlite3_int64 null_int = sqlite3_uri_int64(NULL, "test", 0);
    (void)null_int;

    // Note: sqlite3_free_filename expects a pointer returned by sqlite3_create_filename.
    // Since we manually allocated this buffer, calling sqlite3_free_filename on 'z'
    // is undefined behavior and may crash or corrupt memory. 
    // For safe fuzzing of the parsing logic, we skip freeing via the library 
    // and free it manually.
    // sqlite3_free_filename(z); // Removed to prevent double-free or invalid free

    free(raw_buf);
    return 0;
}