// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * Static helper to create a dummy shared library file for sqlite3_load_extension.
 * This file is a minimal ELF binary that sqlite3 might attempt to load.
 */
static int create_dummy_extension(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;

    /* Minimal ELF header to trick the loader into thinking it's a valid shared object */
    unsigned char elf_header[] = {
        0x7f, 'E', 'L', 'F', /* Magic */
        2, /* 64-bit */
        1, /* Little endian */
        1, /* ELF version */
        0, /* OS/ABI */
        0, 0, 0, 0, 0, 0, 0, 0, /* Padding */
        2, /* ET_DYN (Shared object) */
        0, 0, /* Machine (x86_64 is 0x3e, but we use 0 to be generic or try to load anyway) */
        0, 0, 0, 0, /* Version */
        0, 0, 0, 0, 0, 0, 0, 0, /* Entry point */
        0, 0, 0, 0, 0, 0, 0, 0, /* Program header offset */
        0, 0, 0, 0, 0, 0, 0, 0, /* Section header offset */
        0, 0, 0, 0, /* Flags */
        52, 0, /* Header size */
        56, 0, /* Program header entry size */
        1, 0,  /* Number of program headers */
        64, 0, /* Section header entry size */
        0, 0,  /* Number of section headers */
        0, 0,  /* String table index */
        0, 0   /* Reserved */
    };

    write(fd, elf_header, sizeof(elf_header));
    close(fd);
    return 0;
}

/*
 * Static helper to create a minimal virtual table module for testing.
 */
static int dummy_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_vtab_best_index(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    return SQLITE_OK;
}

static int dummy_vtab_close(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_vtab_filter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_vtab_next(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_vtab_eof(sqlite3_vtab_cursor *pCursor) {
    return 1; /* End of file immediately */
}

static int dummy_vtab_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int iCol) {
    sqlite3_result_null(pCtx);
    return SQLITE_OK;
}

static int dummy_vtab_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_vtab_begin(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_sync(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_commit(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_rollback(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

/* Global dummy module structure */
static sqlite3_module g_dummy_module = {
    0, /* iVersion */
    dummy_vtab_create,
    dummy_vtab_connect,
    dummy_vtab_best_index,
    dummy_vtab_disconnect,
    dummy_vtab_destroy,
    dummy_vtab_open,
    dummy_vtab_close,
    dummy_vtab_filter,
    dummy_vtab_next,
    dummy_vtab_eof,
    dummy_vtab_column,
    dummy_vtab_rowid,
    dummy_vtab_update,
    dummy_vtab_begin,
    dummy_vtab_sync,
    dummy_vtab_commit,
    dummy_vtab_rollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *err_msg = NULL;
    const char *dummy_file = "./dummy_file";
    const char *dummy_ext = "./dummy_ext.so";

    /* Ensure we have enough data to work with */
    if (Size < 1) return 0;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 1. Fuzz sqlite3_set_errmsg */
    /* We use the input data to generate error codes and messages */
    int err_code = Data[0];
    if (err_code == 0) err_code = SQLITE_ERROR; /* Avoid 0 if possible, though valid */

    /* Create a temporary buffer for the error message from input data */
    size_t msg_len = Size > 100 ? 100 : Size;
    char *msg_buf = (char *)malloc(msg_len + 1);
    if (msg_buf) {
        memcpy(msg_buf, Data, msg_len);
        msg_buf[msg_len] = '\0';
        /* Test with valid message */
        sqlite3_set_errmsg(db, err_code, msg_buf);
        /* Test with NULL message */
        sqlite3_set_errmsg(db, err_code, NULL);
        free(msg_buf);
    }

    /* 2. Fuzz sqlite3_errmsg and sqlite3_errcode */
    /* These are safe to call even if no error occurred */
    const char *err_str = sqlite3_errmsg(db);
    (void)err_str; /* Suppress unused warning */
    int code = sqlite3_errcode(db);
    (void)code;

    /* 3. Fuzz sqlite3_create_module */
    /* Register a dummy module */
    const char *mod_name = (const char *)Data;
    /* Ensure mod_name is null-terminated for safety, though fuzz input might not be */
    /* We limit the length to avoid huge allocations in module name */
    size_t name_len = 0;
    while (name_len < Size && Data[name_len] != '\0') name_len++;
    if (name_len == 0) name_len = 1;
    if (name_len > 64) name_len = 64;
    
    char safe_name[65];
    memcpy(safe_name, Data, name_len);
    safe_name[name_len] = '\0';

    /* Register module */
    sqlite3_create_module(db, safe_name, &g_dummy_module, NULL);
    
    /* Try to drop the module by passing NULL as pModule */
    sqlite3_create_module(db, safe_name, NULL, NULL);

    /* 4. Fuzz sqlite3_drop_modules */
    /* Drop all modules */
    sqlite3_drop_modules(db, NULL);
    
    /* Drop modules keeping a specific one (using input data as keep list) */
    const char *keep_list[2];
    keep_list[0] = safe_name;
    keep_list[1] = NULL;
    sqlite3_drop_modules(db, (const char **)keep_list);

    /* 5. Fuzz sqlite3_load_extension */
    /* Create a dummy extension file */
    if (create_dummy_extension(dummy_ext) == 0) {
        /* Enable load extension */
        sqlite3_enable_load_extension(db, 1);
        
        /* Try to load with various parameters derived from input */
        /* zFile is fixed to dummy_ext, zProc is derived from input or NULL */
        const char *proc_name = (Size > 1) ? (const char *)(Data + 1) : NULL;
        
        /* Ensure proc_name is null-terminated if not NULL */
        char safe_proc[256];
        if (proc_name) {
            size_t p_len = 0;
            while (p_len < Size - 1 && proc_name[p_len] != '\0') p_len++;
            if (p_len > 255) p_len = 255;
            memcpy(safe_proc, proc_name, p_len);
            safe_proc[p_len] = '\0';
            proc_name = safe_proc;
        } else {
            proc_name = NULL;
        }

        sqlite3_load_extension(db, dummy_ext, proc_name, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }

        /* Try loading with NULL proc */
        sqlite3_load_extension(db, dummy_ext, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
        
        /* Disable load extension */
        sqlite3_enable_load_extension(db, 0);
    }

    /* Cleanup */
    sqlite3_close(db);
    
    /* Remove dummy files */
    unlink(dummy_ext);
    
    return 0;
}