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

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Invoke sqlite3_compileoption_used
    // We use the input data to construct a potential option name.
    // We ensure it is null-terminated and limit its length.
    const char *optName = (const char *)Data;
    size_t optLen = Size;
    if (optLen > 256) {
        optLen = 256;
    }
    
    // Create a null-terminated string for the option name
    char optBuf[257];
    memcpy(optBuf, Data, optLen);
    optBuf[optLen] = '\0';

    // Invoke the target function
    int optionUsed = sqlite3_compileoption_used(optBuf);
    (void)optionUsed; // Result is used to explore state, but not strictly needed for logic

    // Step 2: Prepare for sqlite3_open_v2
    sqlite3 *db = NULL;
    
    // Use input data to determine flags and filename
    // We use a fixed dummy file path as per instructions, but vary flags based on input
    const char *filename = "./dummy_file";
    
    // Derive flags from input data to explore different states
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        // Use the first byte to modify flags
        uint8_t flagByte = Data[0];
        if (flagByte & 0x01) {
            flags |= SQLITE_OPEN_READONLY;
        }
        if (flagByte & 0x02) {
            flags |= SQLITE_OPEN_CREATE;
        }
        if (flagByte & 0x04) {
            flags |= SQLITE_OPEN_NOFOLLOW;
        }
        if (flagByte & 0x08) {
            flags |= SQLITE_OPEN_URI;
        }
    }

    // Derive VFS name from input if available, otherwise NULL
    // CRITICAL FIX: Do not pass raw input data as VFS name because it may not be null-terminated.
    // Passing a non-null-terminated string to sqlite3_open_v2 causes heap-buffer-overflow 
    // when the library tries to parse it (e.g., in sqlite3_mprintf or strlen).
    // We will only pass a VFS name if we can safely construct a null-terminated string from the input.
    const char *zVfs = NULL;
    
    // Only attempt to use input as VFS name if there is a null terminator within a safe range
    // or if we construct a safe string. For fuzzing robustness, we often skip VFS fuzzing 
    // unless we can guarantee null-termination, or we use a fixed valid VFS name.
    // Here, we skip passing dynamic VFS names from raw input to avoid the crash.
    // If we wanted to fuzz VFS names, we would need to find a '\0' in Data+1 or limit length.
    // To be safe and avoid the reported crash, we set zVfs to NULL.
    // Alternatively, we could search for a null byte, but NULL is safer for this specific error.
    
    // Invoke sqlite3_open_v2
    int rc = sqlite3_open_v2(filename, &db, flags, zVfs);

    // Step 3: Handle cleanup and invoke sqlite3_close
    // According to instructions: "Ensure that if sqlite3_open_v2 returns a non-SQLITE_OK error code 
    // or if the resulting database handle is NULL, the handle should still be passed to sqlite3_close()"
    
    if (db != NULL) {
        (void)sqlite3_close(db);
    } else {
        (void)sqlite3_close(db);
    }

    // Additional cleanup: Remove the dummy file if it was created
    remove(filename);

    return 0;
}