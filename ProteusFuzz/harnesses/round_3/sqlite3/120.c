// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:189478:18 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:189478:18 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzzing target for sqlite3_autovacuum_pages, sqlite3_set_clientdata,
 * sqlite3_get_clientdata, sqlite3_open_v2, sqlite3_close_v2, and sqlite3_db_config.
 */

static unsigned int dummy_autovacuum_callback(void *pClientData,
                                              const char *zSchema,
                                              unsigned int nDbPage,
                                              unsigned int nFreePage,
                                              unsigned int nBytePerPage) {
    /* Simple arithmetic to avoid calling other SQLite APIs as per documentation */
    (void)pClientData;
    (void)zSchema;
    (void)nDbPage;
    (void)nBytePerPage;
    /* Return a value based on free pages to trigger different code paths */
    if (nFreePage == 0) return 0;
    return nFreePage;
}

static void dummy_destructor(void *p) {
    (void)p;
    /* Intentionally empty destructor for safety */
}

static void simple_destructor(void *p) {
    if (p) {
        free(p);
    }
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    int flags = 0;
    const char *vfs = NULL;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Use input data to vary flags and VFS selection if possible */
    /* Flags: Use first byte to determine flags, ensuring valid combinations */
    uint8_t flag_byte = Data[0];
    switch (flag_byte % 4) {
        case 0:
            flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
            break;
        case 1:
            flags = SQLITE_OPEN_READONLY;
            break;
        case 2:
            flags = SQLITE_OPEN_READWRITE;
            break;
        case 3:
            flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOFOLLOW;
            break;
    }

    /* Attempt to open the database */
    rc = sqlite3_open_v2(filename, &db, flags, vfs);
    if (rc != SQLITE_OK || db == NULL) {
        /* If open fails, we can't test the other APIs on a valid connection */
        /* But we can still test client data on NULL if the API allows (it shouldn't) */
        /* sqlite3_set_clientdata expects a valid db, so we skip if db is NULL */
        if (db) {
            sqlite3_close_v2(db);
        }
        return 0;
    }

    /* Test sqlite3_db_config with various operations */
    /* We use input data to select operation and arguments */
    if (Size > 1) {
        int op = Data[1] % 10; /* Limit to small range to avoid invalid ops */
        /* Only test SQLITE_DBCONFIG_LOOKASIDE (1) or SQLITE_DBCONFIG_ENABLE_FKEY (2) if available */
        /* Note: SQLITE_DBCONFIG constants are not guaranteed to be sequential or small */
        /* We'll use a safe subset based on common usage */
        if (op == 0) {
            /* SQLITE_DBCONFIG_LOOKASIDE: op, void*, int, int */
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, NULL, 0, 0);
        } else if (op == 1) {
            /* SQLITE_DBCONFIG_ENABLE_FKEY: op, int */
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, 0);
        } else if (op == 2) {
            /* SQLITE_DBCONFIG_ENABLE_TRIGGER: op, int */
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 1, 0);
        }
        /* Ignore other return values for fuzzing stability */
    }

    /* Test sqlite3_set_clientdata and sqlite3_get_clientdata */
    /* Create a unique key based on input data */
    char key[32];
    size_t key_len = Size > 32 ? 31 : Size - 1;
    memcpy(key, Data, key_len);
    key[key_len] = '\0';

    /* Allocate some data to attach */
    void *client_data = malloc(Size > 0 ? Size : 1);
    if (client_data) {
        memcpy(client_data, Data, Size > 0 ? Size : 1);
    }

    /* Set client data with destructor */
    rc = sqlite3_set_clientdata(db, key, client_data, simple_destructor);
    (void)rc;

    /* Retrieve client data */
    void *retrieved = sqlite3_get_clientdata(db, key);
    if (retrieved == client_data) {
        /* Data matches, good */
    }

    /* Test setting client data again with NULL to remove it */
    rc = sqlite3_set_clientdata(db, key, NULL, NULL);
    (void)rc;

    /* Verify removal */
    retrieved = sqlite3_get_clientdata(db, key);
    if (retrieved != NULL) {
        /* Should be NULL */
    }

    /* Test sqlite3_autovacuum_pages */
    /* Register callback */
    rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, (void *)0x1234, dummy_destructor);
    (void)rc;

    /* Override callback with NULL to cancel */
    rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
    (void)rc;

    /* Override callback again with a different value */
    rc = sqlite3_autovacuum_pages(db, dummy_autovacuum_callback, (void *)0x5678, NULL);
    (void)rc;

    /* Cleanup: Close the database */
    rc = sqlite3_close_v2(db);
    (void)rc;

    /* If client_data was not freed by destructor (e.g., if we passed NULL destructor earlier), free it manually */
    /* In our case, we passed simple_destructor which frees it, but if we set it to NULL later, we might leak */
    /* However, sqlite3_set_clientdata with NULL data removes the entry and calls destructor if present */
    /* In the second set_clientdata call, we passed NULL data and NULL destructor, so no leak there. */
    /* The first call set it with simple_destructor, so it was freed when we called set_clientdata with NULL data. */
    /* So client_data is already freed. */

    return 0;
}