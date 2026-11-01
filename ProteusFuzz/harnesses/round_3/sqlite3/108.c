// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Dummy extension entry point for testing sqlite3_auto_extension */
static void dummy_extension_entry_point(void) {
    /* Intentionally empty for fuzzing purposes */
}

/* Another dummy extension to test multiple registrations */
static void another_dummy_extension_entry_point(void) {
    /* Intentionally empty for fuzzing purposes */
}

/* Helper to safely call sqlite3_config with various options based on input */
static int fuzz_config(const uint8_t *data, size_t size) {
    int rc = SQLITE_OK;
    int header_size = 0;
    sqlite3_mem_methods dummy_mem_methods = {0};
    
    if (size == 0) return SQLITE_OK;

    switch (data[0] % 10) {
        case 0:
            /* SQLITE_CONFIG_SINGLETHREAD */
            rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
        case 1:
            /* SQLITE_CONFIG_MULTITHREAD */
            rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
            break;
        case 2:
            /* SQLITE_CONFIG_SERIALIZED */
            rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
            break;
        case 3:
            /* SQLITE_CONFIG_MALLOC - Pass a valid struct, not NULL */
            rc = sqlite3_config(SQLITE_CONFIG_MALLOC, &dummy_mem_methods);
            break;
        case 4:
            /* SQLITE_CONFIG_GETMALLOC - Pass a valid pointer to receive data, not NULL */
            rc = sqlite3_config(SQLITE_CONFIG_GETMALLOC, &dummy_mem_methods);
            break;
        case 5:
            /* SQLITE_CONFIG_SCRATCH - Passing NULL for scratch buffer is often allowed, 
               but let's provide a dummy buffer if needed or just 0s */
            rc = sqlite3_config(SQLITE_CONFIG_SCRATCH, NULL, 0, 0);
            break;
        case 6:
            /* SQLITE_CONFIG_PAGECACHE */
            rc = sqlite3_config(SQLITE_CONFIG_PAGECACHE, NULL, 0, 0);
            break;
        case 7:
            /* SQLITE_CONFIG_HEAP - Passing NULL for heap is often allowed to reset, 
               but let's be safe */
            rc = sqlite3_config(SQLITE_CONFIG_HEAP, NULL, 0, 0);
            break;
        case 8:
            /* SQLITE_CONFIG_MEMSTATUS */
            rc = sqlite3_config(SQLITE_CONFIG_MEMSTATUS, 1);
            break;
        case 9:
            /* SQLITE_CONFIG_PCACHE_HDRSZ - Must not pass NULL */
            rc = sqlite3_config(SQLITE_CONFIG_PCACHE_HDRSZ, &header_size);
            break;
    }
    return rc;
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    int rc;
    int threadsafe;
    
    /* 
     * Strategy:
     * 1. Test sqlite3_threadsafe (stateless check)
     * 2. Test sqlite3_config (requires pre-init or post-shutdown)
     * 3. Test sqlite3_initialize
     * 4. Test sqlite3_auto_extension and sqlite3_cancel_auto_extension
     * 5. Test sqlite3_shutdown
     * 6. Repeat cycles to test idempotency and state transitions
     */

    if (Size == 0) {
        return 0;
    }

    /* Phase 1: Check threadsafe status (can be done anytime) */
    threadsafe = sqlite3_threadsafe();
    (void)threadsafe; /* Suppress unused warning */

    /* Phase 2: Try to configure before initialization */
    rc = fuzz_config(Data, Size);
    (void)rc;

    /* Phase 3: Initialize */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* Initialization failed, try to clean up and return */
        sqlite3_shutdown();
        return 0;
    }

    /* Phase 4: Register auto-extensions */
    /* Use data to decide which extensions to register */
    if (Size > 0 && (Data[0] & 0x01)) {
        rc = sqlite3_auto_extension(dummy_extension_entry_point);
        (void)rc;
    }
    if (Size > 1 && (Data[1] & 0x01)) {
        rc = sqlite3_auto_extension(another_dummy_extension_entry_point);
        (void)rc;
    }
    /* Attempt to register the same one again (idempotency test) */
    rc = sqlite3_auto_extension(dummy_extension_entry_point);
    (void)rc;

    /* Phase 5: Cancel auto-extensions based on input */
    if (Size > 2 && (Data[2] & 0x01)) {
        rc = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
        (void)rc;
    }
    if (Size > 3 && (Data[3] & 0x01)) {
        rc = sqlite3_cancel_auto_extension(another_dummy_extension_entry_point);
        (void)rc;
    }

    /* Phase 6: Shutdown */
    rc = sqlite3_shutdown();
    if (rc != SQLITE_OK) {
        /* Shutdown failed, but we continue to test idempotency */
    }

    /* Phase 7: Test post-shutdown config and re-initialization */
    rc = fuzz_config(Data, Size);
    (void)rc;

    /* Re-initialize to test effective call logic */
    rc = sqlite3_initialize();
    if (rc == SQLITE_OK) {
        /* Register again after re-init */
        rc = sqlite3_auto_extension(dummy_extension_entry_point);
        (void)rc;
        
        /* Shutdown again */
        rc = sqlite3_shutdown();
        (void)rc;
    }

    /* Phase 8: Test multiple shutdowns (idempotency) */
    rc = sqlite3_shutdown();
    (void)rc;
    rc = sqlite3_shutdown();
    (void)rc;

    /* Final cleanup if needed, though shutdown is idempotent */
    return 0;
}