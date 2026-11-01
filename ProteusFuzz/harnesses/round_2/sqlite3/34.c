// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_open -> sqlite3_db_name -> sqlite3_file_control -> sqlite3_file_control -> sqlite3_close -> sqlite3_open
 *
 * Fix for heap-buffer-overflow:
 * The previous crash occurred inside sqlite3_file_control when passing arbitrary opcodes
 * and potentially malformed arguments. The crash trace indicates an issue within the
 * internal formatting logic (sqlite3_str_vappendf) triggered by an opcode that expects
 * specific data structures or flags, but received garbage or insufficient data.
 *
 * To fix this:
 * 1. We will restrict the opcodes passed to sqlite3_file_control to a set of known safe
 *    opcodes that are less likely to trigger internal formatting errors with arbitrary data.
 *    Specifically, we will avoid opcodes that might trigger complex internal logic like
 *    SQLITE_FCNTL_POWERSAFE_OVERWRITE or others that might format strings based on the arg.
 * 2. We will ensure that the argument buffer is large enough and initialized properly
 *    for the opcodes we do allow.
 * 3. We will add checks to ensure we don't pass NULL to opcodes that require a pointer.
 *
 * Safe opcodes to test:
 * SQLITE_FCNTL_GET_LOCKPROXYFILE (returns a string, might be safe)
 * SQLITE_FCNTL_SET_LOCKPROXYFILE (takes a string)
 * SQLITE_FCNTL_WIN32_AV_RETRY (takes a struct, might be complex)
 * SQLITE_FCNTL_PERSIST_WAL (takes an int)
 * SQLITE_FCNTL_POWERSAFE_OVERWRITE (takes an int)
 * SQLITE_FCNTL_OVERWRITE (takes nothing? or int)
 * SQLITE_FCNTL_VFSNAME (takes nothing, returns string)
 * SQLITE_FCNTL_PRAGMA (takes a string)
 * SQLITE_FCNTL_BUSYHANDLER (takes a callback)
 * SQLITE_FCNTL_TEMPFILENAME (takes a string buffer)
 * SQLITE_FCNTL_MMAP_SIZE (takes int64)
 * SQLITE_FCNTL_TRACE (takes a callback)
 * SQLITE_FCNTL_HAS_MOVED (takes nothing)
 * SQLITE_FCNTL_SYNC (takes nothing)
 * SQLITE_FCNTL_COMMIT_PHASETWO (takes nothing)
 * SQLITE_FCNTL_FILE_POINTER (takes a pointer to sqlite3_file*)
 * SQLITE_FCNTL_JOURNAL_POINTER (takes a pointer to sqlite3_file*)
 * SQLITE_FCNTL_VFS_POINTER (takes a pointer to sqlite3_vfs*)
 * SQLITE_FCNTL_DATA_VERSION (takes nothing)
 * SQLITE_FCNTL_SIZE_HINT (takes int64)
 * SQLITE_FCNTL_CHUNK_SIZE (takes int)
 * SQLITE_FCNTL_CKPT_DONE (takes nothing)
 * SQLITE_FCNTL_CKPT_START (takes nothing)
 * SQLITE_FCNTL_CKPT_LOCK (takes nothing)
 * SQLITE_FCNTL_CKPT_UNLOCK (takes nothing)
 * SQLITE_FCNTL_CKPT_RESET (takes nothing)
 * SQLITE_FCNTL_CKPT_DELETE (takes nothing)
 * SQLITE_FCNTL_CKPT_RECOVER (takes nothing)
 * SQLITE_FCNTL_CKPT_ROLLBACK (takes nothing)
 * SQLITE_FCNTL_CKPT_TRUNCATE (takes nothing)
 * SQLITE_FCNTL_CKPT_TRUNCATE_ALL (takes nothing)
 * SQLITE_FCNTL_CKPT_TRUNCATE_AND_DELETE (takes nothing)
 * SQLITE_FCNTL_CKPT_TRUNCATE_AND_DELETE_ALL (takes nothing)
 * SQLITE_FCNTL_CKPT_TRUNCATE_AND_DELETE_AND_DELETE_ALL (takes nothing)
 *
 * However, many of these are complex. To be safe, we will only use a subset of opcodes
 * that are known to be safe with simple integer or NULL arguments, or we will provide
 * a valid buffer for all cases.
 *
 * Let's choose a small set of safe opcodes:
 * - SQLITE_FCNTL_DATA_VERSION (no arg needed, but we pass a buffer anyway)
 * - SQLITE_FCNTL_HAS_MOVED (no arg needed)
 * - SQLITE_FCNTL_SYNC (no arg needed)
 * - SQLITE_FCNTL_COMMIT_PHASETWO (no arg needed)
 * - SQLITE_FCNTL_SIZE_HINT (takes int64, we pass a buffer)
 * - SQLITE_FCNTL_CHUNK_SIZE (takes int, we pass a buffer)
 * - SQLITE_FCNTL_MMAP_SIZE (takes int64, we pass a buffer)
 *
 * We will map the input byte to one of these safe opcodes.
 */

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *db_name = NULL;
    int rc;
    int db_index;
    int op1, op2;
    void *arg1 = NULL;
    void *arg2 = NULL;
    int has_arg1 = 0;
    int has_arg2 = 0;

    /*
     * Step 1: Prepare environment
     */

    if (Size > 0) {
        db_index = Data[0] % 5; /* 0 to 4 */
    } else {
        db_index = 0;
    }

    /*
     * Map input bytes to safe opcodes.
     * We use a modulo operation to select from a list of safe opcodes.
     */
    static const int safe_opcodes[] = {
        SQLITE_FCNTL_DATA_VERSION,
        SQLITE_FCNTL_HAS_MOVED,
        SQLITE_FCNTL_SYNC,
        SQLITE_FCNTL_COMMIT_PHASETWO,
        SQLITE_FCNTL_SIZE_HINT,
        SQLITE_FCNTL_CHUNK_SIZE,
        SQLITE_FCNTL_MMAP_SIZE
    };
    int num_safe_opcodes = sizeof(safe_opcodes) / sizeof(safe_opcodes[0]);

    if (Size > 1) {
        op1 = safe_opcodes[Data[1] % num_safe_opcodes];
    } else {
        op1 = safe_opcodes[0];
    }

    if (Size > 2) {
        op2 = safe_opcodes[Data[2] % num_safe_opcodes];
    } else {
        op2 = safe_opcodes[0];
    }

    /*
     * Allocate buffers for arg1 and arg2.
     * Even for opcodes that don't strictly need an argument, we provide a buffer
     * to avoid NULL pointer issues. We allocate 64 bytes to be safe.
     */
    if (op1 != 0) {
        arg1 = malloc(64);
        if (arg1) {
            has_arg1 = 1;
            /* Initialize with zeros to avoid garbage data */
            memset(arg1, 0, 64);
            /* Optionally copy some input data if available */
            if (Size > 3) {
                size_t copy_len = (Size - 3) < 64 ? (Size - 3) : 64;
                memcpy(arg1, Data + 3, copy_len);
            }
        }
    }

    if (op2 != 0) {
        arg2 = malloc(64);
        if (arg2) {
            has_arg2 = 1;
            memset(arg2, 0, 64);
            if (Size > 4) {
                size_t copy_len = (Size - 4) < 64 ? (Size - 4) : 64;
                memcpy(arg2, Data + 4, copy_len);
            }
        }
    }

    /*
     * Step 2: Invoke the target function sequence
     */

    const char *filename;
    if (Size > 5 && Data[5] % 2 == 0) {
        filename = ":memory:";
    } else {
        filename = "./dummy_file";
    }

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
        if (has_arg1) free(arg1);
        if (has_arg2) free(arg2);
        return 0;
    }

    if (db) {
        db_name = sqlite3_db_name(db, db_index);
        (void)db_name;
    }

    if (db) {
        const char *zDbName = (Size > 6 && Data[6] % 2 == 0) ? NULL : "main";
        rc = sqlite3_file_control(db, zDbName, op1, arg1);
        (void)rc;
    }

    if (db) {
        const char *zDbName2 = (Size > 7 && Data[7] % 2 == 0) ? "temp" : NULL;
        rc = sqlite3_file_control(db, zDbName2, op2, arg2);
        (void)rc;
    }

    if (db) {
        rc = sqlite3_close(db);
        (void)rc;
        db = NULL;
    }

    const char *filename2;
    if (Size > 8 && Data[8] % 2 == 0) {
        filename2 = "./dummy_file";
    } else {
        filename2 = ":memory:";
    }

    rc = sqlite3_open(filename2, &db);
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    } else {
        if (db) {
            sqlite3_close(db);
            db = NULL;
        }
    }

    /*
     * Step 3: Cleanup
     */
    if (has_arg1) free(arg1);
    if (has_arg2) free(arg2);

    return 0;
}