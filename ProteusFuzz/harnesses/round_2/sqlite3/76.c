// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Helper function to read an integer from the fuzzing input safely */
static int read_int_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int) > Size) {
        return 0;
    }
    int val;
    memcpy(&val, &Data[*offset], sizeof(int));
    *offset += sizeof(int);
    return val;
}

/* Helper function to read a null-terminated string from input safely */
static const char *read_string_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) {
        return NULL;
    }
    /* Find the end of the string or the end of buffer */
    size_t start = *offset;
    while (*offset < Size && Data[*offset] != '\0') {
        (*offset)++;
    }
    /* Ensure we don't return a string longer than a reasonable limit to prevent huge allocations */
    if (*offset - start > 256) {
        *offset = start + 256;
    }
    /* Create a temporary buffer for the string */
    char *str = (char *)malloc(*offset - start + 1);
    if (!str) return NULL;
    memcpy(str, &Data[start], *offset - start);
    str[*offset - start] = '\0';
    (*offset)++; /* Skip the null terminator if it exists */
    return str;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t offset = 0;
    int rc;
    sqlite3 *db = NULL;
    sqlite3_vfs *vfs = NULL;
    sqlite3_vfs *customVfs = NULL;
    char random_buf[1024];
    int sleep_ms;
    int flags;
    const char *filename = NULL;
    const char *vfs_name = NULL;

    /* Initialize SQLite library explicitly to ensure state is ready */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* 1. Test sqlite3_sleep */
    /* Read sleep duration from input */
    sleep_ms = read_int_from_input(Data, Size, &offset);
    /* Ensure sleep_ms is within a reasonable range to avoid hanging the fuzzer */
    if (sleep_ms < 0) sleep_ms = 0;
    if (sleep_ms > 1000) sleep_ms = 1000;
    sqlite3_sleep(sleep_ms);

    /* 2. Test sqlite3_randomness */
    /* Read number of bytes for randomness */
    int rand_bytes = read_int_from_input(Data, Size, &offset);
    if (rand_bytes < 0) rand_bytes = 0;
    if (rand_bytes > (int)sizeof(random_buf)) rand_bytes = sizeof(random_buf);
    
    /* Call with valid buffer */
    if (rand_bytes > 0) {
        sqlite3_randomness(rand_bytes, random_buf);
    } else {
        /* Call with NULL to seed */
        sqlite3_randomness(0, NULL);
    }

    /* 3. Test sqlite3_vfs_find and sqlite3_vfs_register */
    /* Try to find a VFS by name from input */
    vfs_name = read_string_from_input(Data, Size, &offset);
    vfs = sqlite3_vfs_find(vfs_name ? vfs_name : NULL);
    
    /* Try to find default VFS */
    vfs = sqlite3_vfs_find(NULL);

    /* Create a custom VFS structure for registration testing */
    /* We need to allocate memory for the VFS structure */
    customVfs = (sqlite3_vfs *)malloc(sizeof(sqlite3_vfs));
    if (customVfs) {
        memset(customVfs, 0, sizeof(sqlite3_vfs));
        customVfs->iVersion = 3;
        customVfs->szOsFile = 0;
        customVfs->mxPathname = 256;
        customVfs->zName = "fuzz_vfs";
        /* Pointers to functions are NULL, which is valid for testing registration logic */
        
        /* Register the custom VFS (makeDflt = 0) */
        rc = sqlite3_vfs_register(customVfs, 0);
        (void)rc; /* Ignore return for fuzzing, just trigger the code path */
        
        /* Try to find it */
        vfs = sqlite3_vfs_find("fuzz_vfs");
        
        /* Unregister it */
        sqlite3_vfs_unregister(customVfs);
        free(customVfs);
        customVfs = NULL;
    }

    /* 4. Test sqlite3_open_v2 */
    /* Read filename from input if available, otherwise use dummy */
    filename = read_string_from_input(Data, Size, &offset);
    if (!filename || strlen(filename) == 0) {
        filename = "./dummy_file";
    }
    
    /* Read flags from input */
    flags = read_int_from_input(Data, Size, &offset);
    
    /* Read VFS name for open_v2 */
    vfs_name = read_string_from_input(Data, Size, &offset);
    if (!vfs_name || strlen(vfs_name) == 0) {
        vfs_name = NULL;
    }

    /* Attempt to open the database */
    rc = sqlite3_open_v2(filename, &db, flags, vfs_name);

    /* Perform cleanup based on the result */
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    } else if (rc != SQLITE_OK) {
        /* Even if open failed, ensure we don't leak if db was partially set */
        /* The spec says: "Ensure that if sqlite3_open_v2 returns a non-SQLITE_OK error code 
         * or if the resulting database handle is NULL, the handle should still be passed 
         * to sqlite3_close() before calling sqlite3_shutdown()..." 
         * However, if db is NULL, we cannot close it. 
         * The instruction implies checking if db is not NULL even on error. 
         * If db is NULL, we simply proceed. */
    }

    /* 5. Cleanup */
    sqlite3_shutdown();

    /* Free any allocated strings */
    if (vfs_name && vfs_name != NULL && vfs_name != "fuzz_vfs" && vfs_name != "./dummy_file") {
        /* Note: read_string_from_input allocates memory, but we passed the pointer 
           to sqlite3_open_v2. We need to free it if it was allocated. 
           However, in this logic, vfs_name is a local variable holding the result of malloc.
           We must free it. */
        free((void*)vfs_name);
    }
    /* filename was also allocated if it came from input */
    if (filename && filename != "./dummy_file") {
        free((void*)filename);
    }

    return 0;
}