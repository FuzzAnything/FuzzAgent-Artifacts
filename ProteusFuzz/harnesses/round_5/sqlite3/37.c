// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_compileoption_used -> sqlite3_open_v2 -> sqlite3_close
 */

static int write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (size > 0) {
        size_t written = 0;
        while (written < size) {
            ssize_t n = write(fd, data + written, size - written);
            if (n <= 0) {
                close(fd);
                return -1;
            }
            written += n;
        }
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Step 1: Prepare environment and invoke sqlite3_compileoption_used */
    /* We can fuzz the option name from the input data if possible, or use fixed options */
    if (Size > 0) {
        /* Create a null-terminated string from input for option name */
        char opt_name[256];
        size_t len = (Size < sizeof(opt_name) - 1) ? Size : sizeof(opt_name) - 1;
        memcpy(opt_name, Data, len);
        opt_name[len] = '\0';

        /* Call sqlite3_compileoption_used with fuzzed option name */
        sqlite3_compileoption_used(opt_name);

        /* Also try some known options to ensure coverage */
        sqlite3_compileoption_used("SQLITE_ENABLE_FTS3");
        sqlite3_compileoption_used("SQLITE_ENABLE_FTS4");
        sqlite3_compileoption_used("SQLITE_ENABLE_JSON1");
        sqlite3_compileoption_used(NULL);
    } else {
        sqlite3_compileoption_used("SQLITE_ENABLE_FTS3");
        sqlite3_compileoption_used(NULL);
    }

    /* Step 2: Prepare file for sqlite3_open_v2 */
    const char *db_file = "./dummy_file";
    /* Write some data to the dummy file to simulate a database file */
    write_dummy_file(db_file, Data, Size);

    /* Step 3: Invoke sqlite3_open_v2 with various flags */
    /* Use different flags based on input to explore more states */
    int flags = 0;
    if (Size > 0) {
        /* Use first byte to determine flags */
        switch (Data[0] % 4) {
            case 0:
                flags = SQLITE_OPEN_READONLY;
                break;
            case 1:
                flags = SQLITE_OPEN_READWRITE;
                break;
            case 2:
                flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
                break;
            case 3:
                flags = SQLITE_OPEN_URI;
                break;
        }
    } else {
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }

    /* Try opening with different VFS names */
    const char *vfs = NULL;
    if (Size > 1) {
        /* Use second byte to select VFS */
        switch (Data[1] % 3) {
            case 0:
                vfs = NULL; /* Default VFS */
                break;
            case 1:
                vfs = "unix"; /* Unix VFS if available */
                break;
            case 2:
                vfs = "memdb"; /* Memory database VFS */
                break;
        }
    }

    rc = sqlite3_open_v2(db_file, &db, flags, vfs);
    if (rc != SQLITE_OK) {
        /* If open fails, db is NULL, safe to return */
        return 0;
    }

    /* Step 4: Invoke sqlite3_close */
    /* Only call close if db is valid */
    if (db != NULL) {
        rc = sqlite3_close(db);
        /* Do not check rc for SQLITE_BUSY here as it's expected behavior */
        /* Do not call close again if it returns SQLITE_BUSY to avoid use-after-free */
        /* The pointer 'db' is now invalid or zombie, do not use it */
        db = NULL;
    }

    return 0;
}