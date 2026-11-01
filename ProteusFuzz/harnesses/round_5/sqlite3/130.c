// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Dummy extension entry point for testing auto-extension APIs.
 * This function is registered via sqlite3_auto_extension.
 */
static void dummy_extension_entry_point(void) {
    /* Do nothing, just a placeholder */
}

/*
 * Another dummy extension entry point to test multiple registrations.
 */
static void another_dummy_extension_entry_point(void) {
    /* Do nothing, just a placeholder */
}

/*
 * Helper to interpret fuzz data as an integer configuration option.
 * We map the first byte to a valid sqlite3_config option to ensure
 * we test valid paths, while using fuzz data for arguments.
 */
static int get_config_option(const uint8_t *data, size_t size) {
    if (size == 0) return SQLITE_CONFIG_SINGLETHREAD;
    
    uint8_t val = data[0];
    
    /* Map to known valid options to avoid SQLITE_MISUSE immediately */
    switch (val % 10) {
        case 0: return SQLITE_CONFIG_SINGLETHREAD;
        case 1: return SQLITE_CONFIG_MULTITHREAD;
        case 2: return SQLITE_CONFIG_SERIALIZED;
        case 3: return SQLITE_CONFIG_MALLOC;
        case 4: return SQLITE_CONFIG_GETMALLOC;
        case 5: return SQLITE_CONFIG_SCRATCH;
        case 6: return SQLITE_CONFIG_PAGECACHE;
        case 7: return SQLITE_CONFIG_HEAP;
        case 8: return SQLITE_CONFIG_MEMSTATUS;
        case 9: return SQLITE_CONFIG_MUTEX;
        default: return SQLITE_CONFIG_SINGLETHREAD;
    }
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    int rc;
    
    /* 
     * Scenario 1: Test sqlite3_threadsafe (safe to call anytime)
     */
    (void)sqlite3_threadsafe();
    
    /*
     * Scenario 2: Test sqlite3_config before initialization
     * We try to configure the library before it is initialized.
     */
    if (Size > 0) {
        int option = get_config_option(Data, Size);
        
        /* 
         * Pass fuzz data as arguments. 
         * Note: sqlite3_config expects variable arguments based on the option.
         * For safety, we only pass arguments for options that expect them 
         * and might not crash with garbage, or we pass NULL/0 where safe.
         * Many config options require specific structures; passing raw fuzz data
         * might trigger SQLITE_MISUSE or SQLITE_ERROR, which is fine for fuzzing.
         */
        switch (option) {
            case SQLITE_CONFIG_SINGLETHREAD:
            case SQLITE_CONFIG_MULTITHREAD:
            case SQLITE_CONFIG_SERIALIZED:
            case SQLITE_CONFIG_MEMSTATUS:
                rc = sqlite3_config(option);
                break;
            case SQLITE_CONFIG_MALLOC:
            case SQLITE_CONFIG_GETMALLOC:
            case SQLITE_CONFIG_SCRATCH:
            case SQLITE_CONFIG_PAGECACHE:
            case SQLITE_CONFIG_HEAP:
            case SQLITE_CONFIG_MUTEX:
            case SQLITE_CONFIG_GETMUTEX:
            case SQLITE_CONFIG_LOG:
            case SQLITE_CONFIG_URI:
            case SQLITE_CONFIG_COVERING_INDEX_SCAN:
            case SQLITE_CONFIG_SQLLOG:
            case SQLITE_CONFIG_MMAP_SIZE:
            case SQLITE_CONFIG_WIN32_HEAPSIZE:
            case SQLITE_CONFIG_PCACHE_HDRSZ:
            case SQLITE_CONFIG_PCACHE:
            case SQLITE_CONFIG_GETPCACHE:
            case SQLITE_CONFIG_SMALL_MALLOC:
                /* 
                 * For options expecting arguments, we pass NULL or 0.
                 * This might result in SQLITE_MISUSE or SQLITE_ERROR, 
                 * which is acceptable for fuzzing to explore error paths.
                 */
                rc = sqlite3_config(option, NULL, 0, 0);
                break;
            default:
                rc = sqlite3_config(option);
                break;
        }
    }
    
    /*
     * Scenario 3: Test sqlite3_auto_extension before initialization
     */
    rc = sqlite3_auto_extension(dummy_extension_entry_point);
    rc = sqlite3_auto_extension(another_dummy_extension_entry_point);
    rc = sqlite3_auto_extension(NULL); /* Test NULL handling if API armor is on */
    
    /*
     * Scenario 4: Test sqlite3_cancel_auto_extension before initialization
     */
    rc = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
    rc = sqlite3_cancel_auto_extension(another_dummy_extension_entry_point);
    rc = sqlite3_cancel_auto_extension(NULL);
    
    /*
     * Scenario 5: Initialize the library
     */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* Initialization failed, but we continue to test other states */
    }
    
    /*
     * Scenario 6: Test sqlite3_config after initialization (should return SQLITE_MISUSE for most)
     * This is a key state to fuzz: post-init configuration attempts.
     */
    if (Size > 0) {
        int option = get_config_option(Data, Size);
        rc = sqlite3_config(option);
        /* Try with arguments too */
        rc = sqlite3_config(option, NULL, 0, 0);
    }
    
    /*
     * Scenario 7: Test auto_extensions after initialization
     * These should still work (registering for future connections)
     */
    rc = sqlite3_auto_extension(dummy_extension_entry_point);
    rc = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
    
    /*
     * Scenario 8: Test sqlite3_threadsafe after initialization
     */
    (void)sqlite3_threadsafe();
    
    /*
     * Scenario 9: Shutdown the library
     */
    rc = sqlite3_shutdown();
    
    /*
     * Scenario 10: Test operations after shutdown
     * sqlite3_initialize should work again (re-init)
     */
    rc = sqlite3_initialize();
    
    /*
     * Scenario 11: Test config after shutdown (should be allowed for most options)
     */
    if (Size > 0) {
        int option = get_config_option(Data, Size);
        rc = sqlite3_config(option);
        rc = sqlite3_config(option, NULL, 0, 0);
    }
    
    /*
     * Scenario 12: Test auto_extensions after shutdown
     */
    rc = sqlite3_auto_extension(dummy_extension_entry_point);
    rc = sqlite3_cancel_auto_extension(dummy_extension_entry_point);
    
    /*
     * Scenario 13: Final shutdown
     */
    rc = sqlite3_shutdown();
    
    /*
     * Scenario 14: Multiple shutdowns (idempotent)
     */
    rc = sqlite3_shutdown();
    rc = sqlite3_shutdown();
    
    /*
     * Scenario 15: Multiple initializations without shutdown in between (idempotent)
     */
    rc = sqlite3_initialize();
    rc = sqlite3_initialize();
    
    /*
     * Scenario 16: Rapid init/shutdown cycles
     */
    for (int i = 0; i < 3; i++) {
        rc = sqlite3_shutdown();
        rc = sqlite3_initialize();
    }
    
    /*
     * Scenario 17: Test config with various "anytime" options during active state
     * We re-init to ensure we are in an active state
     */
    rc = sqlite3_initialize();
    
    /* SQLITE_CONFIG_LOG is an anytime option */
    rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
    
    /* SQLITE_CONFIG_URI is an anytime option */
    rc = sqlite3_config(SQLITE_CONFIG_URI, 1);
    rc = sqlite3_config(SQLITE_CONFIG_URI, 0);
    
    /* SQLITE_CONFIG_SMALL_MALLOC is an anytime option */
    rc = sqlite3_config(SQLITE_CONFIG_SMALL_MALLOC, 1);
    rc = sqlite3_config(SQLITE_CONFIG_SMALL_MALLOC, 0);
    
    /* Clean up */
    rc = sqlite3_shutdown();
    
    return 0;
}