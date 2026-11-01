// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <fcntl.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    // Step 1: Invoke sqlite3_compileoption_used
    const char *optName = NULL;
    if (Size > 0) {
        size_t len = 0;
        while (len < Size && len < 1000 && Data[len] != 0) {
            len++;
        }
        if (len > 0 && len < Size) {
            optName = (const char *)Data;
        } else {
            optName = "SQLITE_ENABLE_FTS3";
        }
    } else {
        optName = "SQLITE_ENABLE_FTS3";
    }

    (void)sqlite3_compileoption_used(optName);

    // Step 2: Prepare environment for sqlite3_open_v2
    write_dummy_file(Data, Size);

    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        if (Data[0] & 0x01) flags |= SQLITE_OPEN_CREATE;
        if (Data[0] & 0x02) flags |= SQLITE_OPEN_READONLY;
        // Replaced SQLITE_OPEN_NOFSYNC with SQLITE_OPEN_NOMUTEX which is widely supported
        if (Data[0] & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (Data[0] & 0x08) flags |= SQLITE_OPEN_URI;
    }

    rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);

    // Step 3: Invoke sqlite3_close
    if (db != NULL) {
        rc = sqlite3_close(db);
        (void)rc;
    } else {
        rc = sqlite3_close(NULL);
        (void)rc;
    }

    return 0;
}