// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * Static helper to create a dummy shared library file for sqlite3_load_extension.
 */
static int create_dummy_extension(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (fd < 0) return -1;

    unsigned char elf_header[64] = {
        0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x00, 0x3e, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x38, 0x00,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    write(fd, elf_header, sizeof(elf_header));
    close(fd);
    return 0;
}

/* Dummy module functions */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) { return SQLITE_OK; }
static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) { return SQLITE_OK; }
static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) { return SQLITE_OK; }
static int dummy_xDisconnect(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xDestroy(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) { return SQLITE_OK; }
static int dummy_xClose(sqlite3_vtab_cursor *pCursor) { return SQLITE_OK; }
static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) { return SQLITE_OK; }
static int dummy_xNext(sqlite3_vtab_cursor *pCursor) { return SQLITE_OK; }
static int dummy_xEof(sqlite3_vtab_cursor *pCursor) { return 1; }
static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int i) { sqlite3_result_null(pCtx); return SQLITE_OK; }
static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) { *pRowid = 1; return SQLITE_OK; }
static int dummy_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) { return SQLITE_OK; }
static int dummy_xBegin(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xSync(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xCommit(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xRollback(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xSavepoint(sqlite3_vtab *pVTab, int) { return SQLITE_OK; }
static int dummy_xRelease(sqlite3_vtab *pVTab, int) { return SQLITE_OK; }
static int dummy_xRollbackTo(sqlite3_vtab *pVTab, int) { return SQLITE_OK; }

static sqlite3_module g_dummy_module = {
    3,
    dummy_xCreate, dummy_xConnect, dummy_xBestIndex,
    dummy_xDisconnect, dummy_xDestroy, dummy_xOpen,
    dummy_xClose, dummy_xFilter, dummy_xNext,
    dummy_xEof, dummy_xColumn, dummy_xRowid,
    dummy_xUpdate, dummy_xBegin, dummy_xSync,
    dummy_xCommit, dummy_xRollback, NULL, NULL,
    dummy_xSavepoint, dummy_xRelease, dummy_xRollbackTo,
    NULL, NULL
};

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    const char *dummy_file = "./dummy_file";

    sqlite3_reset_auto_extension();

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    unsigned char op = Data[0] % 10;
    unsigned char val1 = Data[1];

    switch (op) {
        case 0: {
            int errcode = (val1 % 100) + 1;
            const char *msg = NULL;
            if (Size > 2) {
                size_t len = Size - 2;
                if (len > 256) len = 256;
                char *safe_msg = (char *)sqlite3_malloc(len + 1);
                if (safe_msg) {
                    memcpy(safe_msg, Data + 2, len);
                    safe_msg[len] = '\0';
                    msg = safe_msg;
                }
            }
            sqlite3_set_errmsg(db, errcode, msg);
            if (msg && msg != NULL && (void*)msg != (void*)Data) {
                sqlite3_free((void*)msg);
            }
            break;
        }
        case 1: {
            create_dummy_extension(dummy_file);
            const char *zFile = dummy_file;
            const char *zProc = (Size > 2 && Data[2] == 0) ? NULL : "sqlite3_extension_init";
            sqlite3_enable_load_extension(db, 1);
            rc = sqlite3_load_extension(db, zFile, zProc, &zErrMsg);
            if (zErrMsg) sqlite3_free(zErrMsg);
            break;
        }
        case 2: {
            const char *zName = "fuzz_module";
            void *pClientData = (void *)(size_t)(val1 * 100);
            rc = sqlite3_create_module(db, zName, &g_dummy_module, pClientData);
            break;
        }
        case 3: {
            rc = sqlite3_drop_modules(db, NULL);
            break;
        }
        case 4: {
            const char *keep_list[] = { "fuzz_module", NULL };
            rc = sqlite3_drop_modules(db, keep_list);
            break;
        }
        case 5: {
            rc = sqlite3_close_v2(db);
            db = NULL;
            if (rc == SQLITE_OK) {
                rc = sqlite3_open(":memory:", &db);
                if (rc != SQLITE_OK || db == NULL) return 0;
            }
            break;
        }
        case 6: {
            rc = sqlite3_create_module(db, "temp_mod", &g_dummy_module, NULL);
            rc = sqlite3_drop_modules(db, NULL);
            break;
        }
        case 7: {
            int errcode = (val1 % 100) + 1;
            sqlite3_set_errmsg(db, errcode, NULL);
            break;
        }
        case 8: {
            create_dummy_extension(dummy_file);
            sqlite3_enable_load_extension(db, 1);
            rc = sqlite3_load_extension(db, dummy_file, NULL, &zErrMsg);
            if (zErrMsg) sqlite3_free(zErrMsg);
            break;
        }
        case 9: {
            rc = sqlite3_create_module(db, "fuzz_module", NULL, NULL);
            break;
        }
        default:
            break;
    }

    if (db) {
        sqlite3_close_v2(db);
    }
    
    unlink(dummy_file);

    return 0;
}