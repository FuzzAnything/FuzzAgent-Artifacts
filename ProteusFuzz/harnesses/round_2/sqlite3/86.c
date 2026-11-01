// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * Helper function to create a dummy file with fuzzed data if needed.
 */
static void create_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        }
        close(fd);
    }
}

/*
 * Helper function to create a dummy UTF-16 file content.
 * We construct a simple UTF-16 string from the input data.
 */
static int create_dummy_utf16_file(const uint8_t *Data, size_t Size, wchar_t **out_wstr) {
    if (Size == 0) {
        *out_wstr = NULL;
        return 0;
    }
    
    // Limit size to avoid allocation issues
    if (Size > 4096) Size = 4096;
    
    // Allocate buffer for UTF-16 string (2 bytes per char + null terminator)
    size_t wlen = Size + 1;
    wchar_t *wstr = (wchar_t *)malloc(wlen * sizeof(wchar_t));
    if (!wstr) return -1;
    
    // Convert bytes to wchar_t (simple mapping, not strict UTF-16 validation)
    for (size_t i = 0; i < Size; i++) {
        wstr[i] = (wchar_t)Data[i];
    }
    wstr[Size] = L'\0';
    
    *out_wstr = wstr;
    return 0;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Guard against empty input
    if (Size == 0) {
        return 0;
    }
    
    // Initialize SQLite if needed (optional, but good practice)
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Create dummy file for testing
    create_dummy_file(Data, Size);
    
    // Test Case 1: sqlite3_open_v2 with various flags
    // We use a subset of valid flags to trigger different code paths
    int flags_options[] = {
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_URI,
        SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_NOMUTEX,
        SQLITE_OPEN_FULLMUTEX,
        SQLITE_OPEN_SHAREDCACHE,
        SQLITE_OPEN_PRIVATECACHE,
        SQLITE_OPEN_EXCLUSIVE,
        SQLITE_OPEN_NOFOLLOW
    };
    int num_flags = sizeof(flags_options) / sizeof(flags_options[0]);
    
    // Use input data to select flags
    int flag_idx = (Data[0] % num_flags);
    int flags = flags_options[flag_idx];
    
    // Try opening with UTF-8 filename
    rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    
    // Test error retrieval functions if db is not NULL
    if (db != NULL) {
        // Call error retrieval functions
        int errcode = sqlite3_errcode(db);
        (void)errcode; // Suppress unused variable warning
        
        int ext_errcode = sqlite3_extended_errcode(db);
        (void)ext_errcode;
        
        int sys_errno = sqlite3_system_errno(db);
        (void)sys_errno;
        
        // Try to execute a simple query to potentially trigger errors
        const char *sql = "SELECT 1;";
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        // Test error functions again after potential operation
        errcode = sqlite3_errcode(db);
        (void)errcode;
        ext_errcode = sqlite3_extended_errcode(db);
        (void)ext_errcode;
        sys_errno = sqlite3_system_errno(db);
        (void)sys_errno;
        
        // Close the database
        sqlite3_close_v2(db);
        db = NULL;
    } else {
        // If open failed, still try to get error info (though db is NULL)
        // This tests the NULL pointer handling in error functions
        int sys_errno = sqlite3_system_errno(NULL);
        (void)sys_errno;
        
        // Try to get error code from NULL (should return SQLITE_MISUSE)
        int errcode = sqlite3_errcode(NULL);
        (void)errcode;
        
        int ext_errcode = sqlite3_extended_errcode(NULL);
        (void)ext_errcode;
    }
    
    // Test Case 2: sqlite3_open16 with UTF-16 filename
    wchar_t *utf16_filename = NULL;
    if (create_dummy_utf16_file(Data, Size, &utf16_filename) == 0 && utf16_filename != NULL) {
        rc = sqlite3_open16(utf16_filename, &db);
        
        if (db != NULL) {
            // Test error retrieval functions
            int errcode = sqlite3_errcode(db);
            (void)errcode;
            
            int ext_errcode = sqlite3_extended_errcode(db);
            (void)ext_errcode;
            
            int sys_errno = sqlite3_system_errno(db);
            (void)sys_errno;
            
            sqlite3_close_v2(db);
            db = NULL;
        }
        
        free(utf16_filename);
    }
    
    // Test Case 3: Try to open with invalid VFS name to trigger errors
    rc = sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, "invalid_vfs_name");
    if (db != NULL) {
        // Even if open fails, we might have a partially initialized db
        int errcode = sqlite3_errcode(db);
        (void)errcode;
        
        int ext_errcode = sqlite3_extended_errcode(db);
        (void)ext_errcode;
        
        int sys_errno = sqlite3_system_errno(db);
        (void)sys_errno;
        
        sqlite3_close_v2(db);
        db = NULL;
    }
    
    // Test Case 4: Test with NULL database handle for all error functions
    int sys_errno_null = sqlite3_system_errno(NULL);
    (void)sys_errno_null;
    
    int errcode_null = sqlite3_errcode(NULL);
    (void)errcode_null;
    
    int ext_errcode_null = sqlite3_extended_errcode(NULL);
    (void)ext_errcode_null;
    
    // Test Case 5: Try multiple open/close cycles with different flags
    // Fixed loop count to avoid dependency on Size in modulo
    int loop_count = 3;
    if (Size > 0 && Data[0] > 0) {
        loop_count = Data[0] % 5 + 1; // 1 to 5 iterations
    }
    
    for (int i = 0; i < loop_count; i++) {
        // Safe modulo operation
        int val = (i < Size) ? Data[i] : Data[0];
        int test_flags = (val % 3) * SQLITE_OPEN_READWRITE;
        if (test_flags == 0) test_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        
        rc = sqlite3_open_v2("./dummy_file", &db, test_flags, NULL);
        if (db != NULL) {
            // Trigger some operations
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER)", NULL, NULL, NULL);
            sqlite3_exec(db, "INSERT INTO test VALUES (1)", NULL, NULL, NULL);
            
            // Check errors
            int ec = sqlite3_errcode(db);
            (void)ec;
            int eec = sqlite3_extended_errcode(db);
            (void)eec;
            int se = sqlite3_system_errno(db);
            (void)se;
            
            sqlite3_close_v2(db);
            db = NULL;
        }
    }
    
    // Cleanup: Ensure no resources are leaked
    if (db != NULL) {
        sqlite3_close_v2(db);
    }
    
    return 0;
}