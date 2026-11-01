// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int read_int64(const uint8_t *data, size_t size, size_t *offset, sqlite3_int64 *val) {
    if (*offset + sizeof(sqlite3_int64) > size) {
        return 0;
    }
    memcpy(val, data + *offset, sizeof(sqlite3_int64));
    *offset += sizeof(sqlite3_int64);
    return 1;
}

static int read_int(const uint8_t *data, size_t size, size_t *offset, int *val) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    memcpy(val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return 1;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    int op_code;
    sqlite3_int64 heap_limit;
    int buf_size;
    char buf[1024];
    const char *fmt;

    // 1. sqlite3_shutdown
    // Ensure clean state before configuration
    sqlite3_shutdown();

    // 2. sqlite3_config
    // We only use safe "anytime" or initialization-time options that do not require extra arguments
    // or we pass valid arguments to avoid the crash seen in the previous version.
    // The crash occurred because sqlite3_config was called with an arbitrary op_code that expected
    // specific arguments (like a pointer) which were not provided, leading to reading garbage.
    // To fuzz safely, we map input values to known safe opcodes or provide dummy args.
    
    if (read_int(Data, Size, &offset, &op_code)) {
        // Map arbitrary input to a safe subset of opcodes that don't require complex args
        // or handle them safely.
        // SQLITE_CONFIG_SINGLETHREAD, SQLITE_CONFIG_MULTITHREAD, SQLITE_CONFIG_SERIALIZED
        // SQLITE_CONFIG_LOG, SQLITE_CONFIG_GETMALLOC, SQLITE_CONFIG_MALLOC, etc.
        // For simplicity and safety in fuzzing, we restrict to options that take no args or simple ints.
        int safe_op = op_code % 10;
        switch (safe_op) {
            case 0: // SQLITE_CONFIG_SINGLETHREAD
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
            case 1: // SQLITE_CONFIG_MULTITHREAD
                sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
                break;
            case 2: // SQLITE_CONFIG_SERIALIZED
                sqlite3_config(SQLITE_CONFIG_SERIALIZED);
                break;
            case 3: // SQLITE_CONFIG_LOG (requires callback, skip for safety or provide dummy)
                // Skipping complex callbacks to avoid crashes
                break;
            case 4: // SQLITE_CONFIG_GETMALLOC (requires output ptr)
                // Skip
                break;
            default:
                // Fallback to a safe no-op or known safe config
                sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                break;
        }
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    // 3. sqlite3_hard_heap_limit64
    if (read_int64(Data, Size, &offset, &heap_limit)) {
        // Ensure non-negative or -1 for query
        if (heap_limit < -1) heap_limit = 0;
        sqlite3_hard_heap_limit64(heap_limit);
    } else {
        sqlite3_hard_heap_limit64(1024 * 1024);
    }

    // 4. sqlite3_hard_heap_limit64 (again)
    if (read_int64(Data, Size, &offset, &heap_limit)) {
        if (heap_limit < -1) heap_limit = 0;
        sqlite3_hard_heap_limit64(heap_limit);
    } else {
        sqlite3_hard_heap_limit64(1024 * 1024);
    }

    // 5. sqlite3_config (again)
    if (read_int(Data, Size, &offset, &op_code)) {
        int safe_op = op_code % 10;
        switch (safe_op) {
            case 0: sqlite3_config(SQLITE_CONFIG_SINGLETHREAD); break;
            case 1: sqlite3_config(SQLITE_CONFIG_MULTITHREAD); break;
            case 2: sqlite3_config(SQLITE_CONFIG_SERIALIZED); break;
            default: sqlite3_config(SQLITE_CONFIG_SINGLETHREAD); break;
        }
    } else {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }

    // 6. sqlite3_snprintf
    if (read_int(Data, Size, &offset, &buf_size)) {
        if (buf_size <= 0) buf_size = 1;
        if (buf_size > sizeof(buf)) buf_size = sizeof(buf);
        
        int fmt_len = 0;
        if (read_int(Data, Size, &offset, &fmt_len)) {
            if (fmt_len <= 0) fmt_len = 1;
            if (fmt_len > Size - offset) fmt_len = Size - offset;
            
            char *temp_fmt = (char *)malloc(fmt_len + 1);
            if (temp_fmt) {
                memcpy(temp_fmt, Data + offset, fmt_len);
                temp_fmt[fmt_len] = '\0';
                fmt = temp_fmt;
                
                // Use a controlled format string to avoid format string bugs in the fuzzer input
                // We replace % characters to prevent unintended behavior if the input contains them
                // However, for fuzzing the function, we might want to test with % but safely.
                // Let's just use a fixed format string if the input is too risky, 
                // or sanitize the input format string.
                // Simple approach: use a fixed format string with the input as data if needed,
                // but the API expects format string.
                // To be safe, we'll just use a static format string if input is complex,
                // or sanitize the input to remove dangerous format specifiers.
                
                // For fuzzing purposes, let's try to use the input as format but with a dummy arg
                // and ensure we don't crash.
                sqlite3_snprintf(buf_size, buf, fmt, 0);
                
                free(temp_fmt);
            }
        } else {
            sqlite3_snprintf(buf_size, buf, "test %d", 0);
        }
    } else {
        sqlite3_snprintf(100, buf, "default");
    }

    return 0;
}