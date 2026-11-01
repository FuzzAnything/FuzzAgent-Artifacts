// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_mutex_alloc at sqlite3.c:29874:27 in sqlite3.h
// sqlite3_mutex_enter at sqlite3.c:29906:17 in sqlite3.h
// sqlite3_mutex_try at sqlite3.c:29917:16 in sqlite3.h
// sqlite3_mutex_leave at sqlite3.c:29932:17 in sqlite3.h
// sqlite3_mutex_free at sqlite3.c:29895:17 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 mutex and configuration APIs.
 * Target APIs:
 * 1. sqlite3_mutex_alloc
 * 2. sqlite3_reset_auto_extension
 * 3. sqlite3_initialize
 * 4. sqlite3_threadsafe
 * 5. sqlite3_os_end
 * 6. sqlite3_config
 */

static int fuzz_mutex_operations(const uint8_t *data, size_t size) {
    sqlite3_mutex *mutex = NULL;
    int type = 0;
    int enter_count = 0;
    
    /* Only use dynamic mutex types: SQLITE_MUTEX_FAST (1) and SQLITE_MUTEX_RECURSIVE (2) */
    if (size > 0) {
        type = data[0] % 3; 
        if (type == 0) type = 1; /* Map 0 to SQLITE_MUTEX_FAST */
        else if (type == 2) type = 2; /* Map 2 to SQLITE_MUTEX_RECURSIVE */
        else type = 1;
    }
    
    /* Allocate mutex */
    mutex = sqlite3_mutex_alloc(type);
    if (mutex == NULL) {
        return 0; /* Allocation failed, nothing to test */
    }
    
    /* 
     * Determine how many times to enter based on input to test recursion logic safely.
     * For SQLITE_MUTEX_FAST, we should only enter once.
     * For SQLITE_MUTEX_RECURSIVE, we can enter multiple times.
     */
    if (size > 1) {
        enter_count = data[1] % 5; /* 0 to 4 */
    } else {
        enter_count = 1;
    }
    
    /* If it's a FAST mutex, limit to 1 entry to avoid undefined behavior */
    if (type == SQLITE_MUTEX_FAST && enter_count > 1) {
        enter_count = 1;
    }
    
    /* Enter the mutex */
    for (int i = 0; i < enter_count; i++) {
        sqlite3_mutex_enter(mutex);
    }
    
    /* 
     * Attempt to try-enter (non-blocking).
     * CRITICAL FIX: Only attempt try-enter if the mutex is RECURSIVE.
     * For SQLITE_MUTEX_FAST, if we are already holding it (enter_count >= 1),
     * calling sqlite3_mutex_try() triggers an assertion failure in SQLite's internal logic
     * because it detects a non-recursive mutex being re-entered.
     */
    if (type == SQLITE_MUTEX_RECURSIVE && enter_count < 5) {
        if (sqlite3_mutex_try(mutex) == SQLITE_OK) {
            enter_count++;
        }
    }
    
    /* Leave the mutex the same number of times we entered */
    for (int i = 0; i < enter_count; i++) {
        sqlite3_mutex_leave(mutex);
    }
    
    /* Free the mutex */
    sqlite3_mutex_free(mutex);
    
    return 0;
}

static int fuzz_config_operations(const uint8_t *data, size_t size) {
    int op = 0;
    int ret = SQLITE_OK;
    
    /* Select a configuration option based on input data */
    if (size > 0) {
        op = data[0] % 10; /* Limit to first 10 common options */
    }
    
    /* 
     * Note: sqlite3_config must be called before sqlite3_initialize or after sqlite3_shutdown.
     * We will handle initialization/shutdown in the main loop.
     * Here we just attempt to call it with various options.
     */
    switch (op) {
        case 0: /* SQLITE_CONFIG_SINGLETHREAD */
            ret = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
        case 1: /* SQLITE_CONFIG_MULTITHREAD */
            ret = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
            break;
        case 2: /* SQLITE_CONFIG_SERIALIZED */
            ret = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
            break;
        case 8: /* SQLITE_CONFIG_MEMSTATUS */
            int flag = (size > 1) ? (data[1] % 2) : 0;
            ret = sqlite3_config(SQLITE_CONFIG_MEMSTATUS, flag);
            break;
        default:
            /* Skip complex options that require specific arguments or state */
            break;
    }
    
    return ret;
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    int ret = SQLITE_OK;
    int op_mode = 0;
    
    if (Size == 0) {
        return 0;
    }
    
    op_mode = Data[0] % 5;
    
    /* Ensure we have a clean state for each test case if possible */
    /* Note: sqlite3_shutdown is not threadsafe, so we must be careful in a multi-threaded fuzzer.
       However, libFuzzer runs single-threaded by default. */
    
    switch (op_mode) {
        case 0:
            /* Test sqlite3_initialize and sqlite3_threadsafe */
            ret = sqlite3_initialize();
            if (ret == SQLITE_OK) {
                (void)sqlite3_threadsafe();
                /* Try to call config after init (should fail for most options) */
                (void)sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
                /* Cleanup */
                (void)sqlite3_shutdown();
            }
            break;
            
        case 1:
            /* Test sqlite3_config before initialization */
            ret = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            if (ret == SQLITE_OK) {
                (void)sqlite3_initialize();
                (void)sqlite3_shutdown();
            }
            break;
            
        case 2:
            /* Test mutex allocation and operations */
            fuzz_mutex_operations(Data, Size);
            break;
            
        case 3:
            /* Test auto extension reset */
            (void)sqlite3_reset_auto_extension();
            break;
            
        case 4:
            /* Test os_end (usually called by shutdown, but we test it directly if safe) */
            /* Note: Calling sqlite3_os_end directly is risky if not properly initialized.
               We will skip direct calls unless we are sure about the state. */
            /* For safety, we rely on sqlite3_shutdown to call it. */
            (void)sqlite3_initialize();
            (void)sqlite3_shutdown();
            break;
    }
    
    return 0;
}