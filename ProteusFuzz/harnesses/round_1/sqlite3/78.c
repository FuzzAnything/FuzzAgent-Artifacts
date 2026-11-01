// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Extract parameters from fuzz input with bounds checking
    int makeDflt = (Size > 0) ? (Data[0] % 2) : 0;
    int sleepMs = (Size > 1) ? ((int)(Data[1] * 10) - 10) : 0;
    int randomnessN = (Size > 2) ? (int)(Data[2] % 100) : 0;
    const char *vfsName = NULL;

    // Parse VFS name if available
    if (Size > 3 && Data[3] != 0) {
        // Find null terminator or limit to remaining size
        size_t nameLen = 0;
        while (nameLen < Size - 3 && Data[3 + nameLen] != 0) {
            nameLen++;
        }
        if (nameLen > 0) {
            char *nameBuf = (char *)malloc(nameLen + 1);
            if (nameBuf) {
                memcpy(nameBuf, Data + 3, nameLen);
                nameBuf[nameLen] = '\0';
                vfsName = nameBuf;
            }
        }
    }

    // Test sqlite3_vfs_find
    sqlite3_vfs *vfs = sqlite3_vfs_find(vfsName);
    if (vfs) {
        // Just verify we got a pointer, no deep usage to avoid complex setup
    }

    // Test sqlite3_sleep
    sqlite3_sleep(sleepMs);

    // Test sqlite3_randomness
    if (randomnessN > 0) {
        unsigned char *randBuf = (unsigned char *)malloc(randomnessN);
        if (randBuf) {
            sqlite3_randomness(randomnessN, randBuf);
            free(randBuf);
        }
    } else {
        // Seed the PRNG
        sqlite3_randomness(randomnessN, NULL);
    }

    // Test VFS registration with a dummy VFS if name was provided
    if (vfsName && Size > 4) {
        sqlite3_vfs vfsObj;
        memset(&vfsObj, 0, sizeof(vfsObj));
        vfsObj.iVersion = 3;
        vfsObj.szOsFile = 0;
        vfsObj.mxPathname = 255;
        vfsObj.zName = vfsName;
        vfsObj.pAppData = NULL;
        // All function pointers are NULL for this fuzz test
        // This might fail registration but tests the API path
        sqlite3_vfs_register(&vfsObj, makeDflt);
    }

    // Test sqlite3_shutdown
    sqlite3_shutdown();

    // Cleanup
    if (vfsName) {
        free((void *)vfsName);
    }

    return 0;
}