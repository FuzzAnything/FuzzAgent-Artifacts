// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to avoid name conflicts */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    sqlite3_result_null(ctx);
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_final(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_null(ctx);
}

static void dummy_destroy(void *pApp) {
    if (pApp) {
        free(pApp);
    }
}

static int dummy_compare(void *pArg, int len1, const void *s1, int len2, const void *s2) {
    (void)pArg;
    if (len1 == len2 && s1 && s2) {
        return memcmp(s1, s2, len1);
    }
    return len1 - len2;
}

static void collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pArg;
    (void)db;
    (void)eTextRep;
    (void)zName;
    /* Register a default collation if needed */
    sqlite3_create_collation(db, zName, eTextRep, NULL, dummy_compare);
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Ensure we have some data to work with */
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file for the database */
    const char *db_file = "./dummy_file";
    FILE *f = fopen(db_file, "w");
    if (f) {
        fclose(f);
    }

    /* Open database in memory or file */
    rc = sqlite3_open(db_file, &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Extract parameters from input data */
    size_t idx = 0;
    char func_name[64] = "fuzz_func";
    char coll_name[64] = "fuzz_coll";
    int nArg = 0;
    int eTextRep = SQLITE_UTF8;
    void *pApp = NULL;

    /* Parse function name (up to 63 bytes) */
    size_t name_len = (Size > idx + 1) ? Data[idx] : 0;
    idx++;
    if (name_len > 63) name_len = 63;
    if (Size >= idx + name_len) {
        memcpy(func_name, Data + idx, name_len);
        func_name[name_len] = '\0';
        idx += name_len;
    }

    /* Parse collation name (up to 63 bytes) */
    size_t coll_name_len = (Size > idx + 1) ? Data[idx] : 0;
    idx++;
    if (coll_name_len > 63) coll_name_len = 63;
    if (Size >= idx + coll_name_len) {
        memcpy(coll_name, Data + idx, coll_name_len);
        coll_name[coll_name_len] = '\0';
        idx += coll_name_len;
    }

    /* Parse nArg */
    if (Size > idx) {
        nArg = Data[idx] & 0x0F; /* Limit to 0-15 */
        idx++;
    }

    /* Parse eTextRep */
    if (Size > idx) {
        int enc = Data[idx] & 0x03;
        switch (enc) {
            case 0: eTextRep = SQLITE_UTF8; break;
            case 1: eTextRep = SQLITE_UTF16LE; break;
            case 2: eTextRep = SQLITE_UTF16BE; break;
            case 3: eTextRep = SQLITE_UTF16; break;
            default: eTextRep = SQLITE_UTF8; break;
        }
        idx++;
    }

    /* Allocate pApp if remaining data exists */
    if (Size > idx) {
        size_t app_size = Size - idx;
        if (app_size > 1024) app_size = 1024; /* Limit allocation */
        pApp = malloc(app_size);
        if (pApp) {
            memcpy(pApp, Data + idx, app_size);
        }
    }

    /* Fuzz sqlite3_create_function_v2 */
    rc = sqlite3_create_function_v2(
        db,
        func_name,
        nArg,
        eTextRep,
        pApp,
        dummy_func,
        dummy_step,
        dummy_final,
        dummy_destroy
    );
    (void)rc;

    /* Fuzz sqlite3_create_collation_v2 */
    rc = sqlite3_create_collation_v2(
        db,
        coll_name,
        eTextRep,
        pApp,
        dummy_compare,
        dummy_destroy
    );
    (void)rc;

    /* Fuzz sqlite3_create_collation16 */
    /* Create a simple UTF-16 string from input if available */
    if (Size > idx + 2) {
        wchar_t *utf16_name = malloc((Size - idx) * sizeof(wchar_t));
        if (utf16_name) {
            size_t len = (Size - idx) / sizeof(wchar_t);
            if (len > 32) len = 32;
            for (size_t i = 0; i < len; i++) {
                utf16_name[i] = (wchar_t)Data[idx + i];
            }
            utf16_name[len] = 0;

            rc = sqlite3_create_collation16(
                db,
                utf16_name,
                eTextRep,
                pApp,
                dummy_compare
            );
            (void)rc;
            free(utf16_name);
        }
    }

    /* Fuzz sqlite3_create_collation */
    rc = sqlite3_create_collation(
        db,
        coll_name,
        eTextRep,
        pApp,
        dummy_compare
    );
    (void)rc;

    /* Fuzz sqlite3_collation_needed */
    rc = sqlite3_collation_needed(db, pApp, collation_needed_callback);
    (void)rc;

    /* Fuzz sqlite3_stricmp */
    const char *str1 = (Size > idx) ? (const char *)(Data + idx) : NULL;
    const char *str2 = (Size > idx + 1) ? (const char *)(Data + idx + 1) : NULL;
    int cmp_result = sqlite3_stricmp(str1, str2);
    (void)cmp_result;

    /* Cleanup */
    sqlite3_close(db);
    if (pApp) {
        free(pApp);
    }

    /* Remove dummy file */
    remove(db_file);

    return 0;
}