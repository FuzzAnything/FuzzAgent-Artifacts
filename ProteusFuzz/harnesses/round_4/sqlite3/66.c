// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:142379:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:142429:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Dummy extension entry point for testing auto_extension */
static void dummy_extension_1(void) {
    /* No-op extension */
}

static void dummy_extension_2(void) {
    /* No-op extension */
}

static void dummy_extension_3(void) {
    /* No-op extension */
}

/* Array of dummy extensions to iterate over */
static void (*dummy_extensions[])(void) = {
    dummy_extension_1,
    dummy_extension_2,
    dummy_extension_3,
    NULL
};

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Use input data to drive logic */
    unsigned int seed = 0;
    if (Size >= 4) {
        seed = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    } else if (Size > 0) {
        seed = Data[0];
    }

    /* Ensure SQLite is initialized before using any API that requires mutexes */
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Test sqlite3_threadsafe - safe to call anytime */
    (void)sqlite3_threadsafe();

    /* Register some auto-extensions */
    for (int i = 0; i < 3; i++) {
        if (dummy_extensions[i] != NULL) {
            (void)sqlite3_auto_extension(dummy_extensions[i]);
        }
    }

    /* Randomly cancel some auto-extensions */
    for (int i = 0; i < 3; i++) {
        if ((seed >> i) & 1) {
            if (dummy_extensions[i] != NULL) {
                (void)sqlite3_cancel_auto_extension(dummy_extensions[i]);
            }
        }
    }

    /* Test sqlite3_config with various options */
    /* SQLITE_CONFIG_SINGLETHREAD */
    if ((seed & 1)) {
        (void)sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }
    /* SQLITE_CONFIG_MULTITHREAD */
    if ((seed & 2)) {
        (void)sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    }
    /* SQLITE_CONFIG_SERIALIZED */
    if ((seed & 4)) {
        (void)sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    }
    /* SQLITE_CONFIG_LOG */
    if ((seed & 8)) {
        static void (*log_callback)(void *, int, const char *) = NULL;
        (void)sqlite3_config(SQLITE_CONFIG_LOG, log_callback, NULL);
    }
    /* SQLITE_CONFIG_URI */
    if ((seed & 16)) {
        int enable = (seed >> 4) & 1;
        (void)sqlite3_config(SQLITE_CONFIG_URI, enable);
    }
    /* SQLITE_CONFIG_MEMSTATUS */
    if ((seed & 32)) {
        int enable = (seed >> 5) & 1;
        (void)sqlite3_config(SQLITE_CONFIG_MEMSTATUS, enable);
    }
    /* SQLITE_CONFIG_MALLOC */
    if ((seed & 64)) {
        (void)sqlite3_config(SQLITE_CONFIG_MALLOC, NULL);
    }
    /* SQLITE_CONFIG_GETMALLOC */
    if ((seed & 128)) {
        (void)sqlite3_config(SQLITE_CONFIG_GETMALLOC, NULL);
    }
    /* SQLITE_CONFIG_SCRATCH */
    if ((seed & 256)) {
        (void)sqlite3_config(SQLITE_CONFIG_SCRATCH, NULL, 0, 0);
    }
    /* SQLITE_CONFIG_PAGECACHE */
    if ((seed & 512)) {
        (void)sqlite3_config(SQLITE_CONFIG_PAGECACHE, NULL, 0, 0);
    }
    /* SQLITE_CONFIG_HEAP */
    if ((seed & 1024)) {
        (void)sqlite3_config(SQLITE_CONFIG_HEAP, NULL, 0, 0);
    }
    /* SQLITE_CONFIG_MUTEX */
    if ((seed & 2048)) {
        (void)sqlite3_config(SQLITE_CONFIG_MUTEX, NULL);
    }
    /* SQLITE_CONFIG_GETMUTEX */
    if ((seed & 4096)) {
        (void)sqlite3_config(SQLITE_CONFIG_GETMUTEX, NULL);
    }
    /* SQLITE_CONFIG_LOOKASIDE */
    if ((seed & 8192)) {
        (void)sqlite3_config(SQLITE_CONFIG_LOOKASIDE, 0, 0);
    }
    /* SQLITE_CONFIG_PCACHE */
    if ((seed & 16384)) {
        (void)sqlite3_config(SQLITE_CONFIG_PCACHE, NULL);
    }
    /* SQLITE_CONFIG_GETPCACHE */
    if ((seed & 32768)) {
        (void)sqlite3_config(SQLITE_CONFIG_GETPCACHE, NULL);
    }
    /* SQLITE_CONFIG_LOG (again with different seed logic) */
    if ((seed & 65536)) {
        (void)sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
    }
    /* SQLITE_CONFIG_URI (again) */
    if ((seed & 131072)) {
        int enable = (seed >> 17) & 1;
        (void)sqlite3_config(SQLITE_CONFIG_URI, enable);
    }

    /* More random cancel operations */
    if (seed % 4 == 0) {
        (void)sqlite3_cancel_auto_extension(dummy_extension_1);
    }
    if (seed % 4 == 1) {
        (void)sqlite3_cancel_auto_extension(dummy_extension_2);
    }
    if (seed % 4 == 2) {
        (void)sqlite3_cancel_auto_extension(dummy_extension_3);
    }
    if (seed % 4 == 3) {
        (void)sqlite3_cancel_auto_extension(NULL);
    }

    /* More random auto-extension registrations */
    if (seed % 6 == 0) {
        (void)sqlite3_auto_extension(dummy_extension_1);
    }
    if (seed % 6 == 1) {
        (void)sqlite3_auto_extension(dummy_extension_2);
    }
    if (seed % 6 == 2) {
        (void)sqlite3_auto_extension(dummy_extension_3);
    }
    if (seed % 6 == 3) {
        (void)sqlite3_auto_extension(NULL);
    }

    /* Shutdown only at the very end to ensure mutexes are valid for all previous calls */
    (void)sqlite3_shutdown();

    return 0;
}