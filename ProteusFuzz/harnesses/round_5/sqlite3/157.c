// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int create_dummy_file(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    const char *dummy_data = "SQLite dummy database file for fuzzing";
    write(fd, dummy_data, strlen(dummy_data));
    close(fd);
    return 0;
}

static void cleanup_dummy_file(const char *filename) {
    unlink(filename);
}

static void fuzz_open_and_error_codes(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    // Create dummy file
    if (create_dummy_file(filename) != 0) {
        return;
    }
    
    // Convert input data to null-terminated string for filename
    char *filename_buf = malloc(Size + 1);
    if (!filename_buf) {
        cleanup_dummy_file(filename);
        return;
    }
    memcpy(filename_buf, Data, Size);
    filename_buf[Size] = '\0';
    
    // Try to open with various flags
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        flags |= (Data[0] % 4) << 2; // Add some random flags
    }
    
    int rc = sqlite3_open_v2(filename_buf, &db, flags, NULL);
    
    // Test error code retrieval functions
    if (db) {
        // Test sqlite3_errcode
        int errcode = sqlite3_errcode(db);
        (void)errcode; // Suppress unused warning
        
        // Test sqlite3_extended_errcode
        int ext_errcode = sqlite3_extended_errcode(db);
        (void)ext_errcode; // Suppress unused warning
        
        // Test sqlite3_system_errno
        int sys_errno = sqlite3_system_errno(db);
        (void)sys_errno; // Suppress unused warning
        
        // Try to execute a simple query to potentially trigger errors
        const char *sql = "SELECT * FROM non_existent_table;";
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            // Error occurred, check error codes again
            errcode = sqlite3_errcode(db);
            ext_errcode = sqlite3_extended_errcode(db);
            sys_errno = sqlite3_system_errno(db);
            (void)errcode;
            (void)ext_errcode;
            (void)sys_errno;
        }
        
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        
        // Close the database
        sqlite3_close_v2(db);
    } else {
        // Test error functions with NULL db
        int errcode = sqlite3_errcode(NULL);
        int ext_errcode = sqlite3_extended_errcode(NULL);
        int sys_errno = sqlite3_system_errno(NULL);
        (void)errcode;
        (void)ext_errcode;
        (void)sys_errno;
    }
    
    free(filename_buf);
    cleanup_dummy_file(filename);
}

static void fuzz_open16(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    
    // Convert input to UTF-16
    if (Size == 0) return;
    
    wchar_t *utf16_filename = malloc((Size + 1) * sizeof(wchar_t));
    if (!utf16_filename) return;
    
    // Simple conversion: treat each byte as a character
    for (size_t i = 0; i < Size && i < 100; i++) {
        utf16_filename[i] = (wchar_t)Data[i];
    }
    utf16_filename[Size] = L'\0';
    
    // Try to open with UTF-16 filename
    int rc = sqlite3_open16((const void *)utf16_filename, &db);
    
    if (db) {
        // Test error code retrieval
        int errcode = sqlite3_errcode(db);
        int ext_errcode = sqlite3_extended_errcode(db);
        int sys_errno = sqlite3_system_errno(db);
        (void)errcode;
        (void)ext_errcode;
        (void)sys_errno;
        
        sqlite3_close_v2(db);
    } else {
        // Test with NULL db
        int errcode = sqlite3_errcode(NULL);
        int ext_errcode = sqlite3_extended_errcode(NULL);
        int sys_errno = sqlite3_system_errno(NULL);
        (void)errcode;
        (void)ext_errcode;
        (void)sys_errno;
    }
    
    free(utf16_filename);
}

int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz different code paths based on first byte
    switch (Data[0] % 3) {
        case 0:
            fuzz_open_and_error_codes(Data, Size);
            break;
        case 1:
            fuzz_open16(Data, Size);
            break;
        case 2:
            // Test with NULL database handle directly
            {
                int errcode = sqlite3_errcode(NULL);
                int ext_errcode = sqlite3_extended_errcode(NULL);
                int sys_errno = sqlite3_system_errno(NULL);
                (void)errcode;
                (void)ext_errcode;
                (void)sys_errno;
            }
            break;
    }
    
    return 0;
}