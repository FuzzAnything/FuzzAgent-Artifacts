// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_destroy(void *p) {
    /* Do nothing */
}

static int dummy_create(sqlite3 *db, void *pAux,
                       int argc, const char *const *argv,
                       sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_connect(sqlite3 *db, void *pAux,
                        int argc, const char *const *argv,
                        sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_bestindex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_disconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_destroy_vtab(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    return SQLITE_OK;
}

static int dummy_close(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_filter(sqlite3_vtab_cursor *pCursor, int idxNum,
                       const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_next(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_eof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx,
                       int i) {
    return SQLITE_OK;
}

static int dummy_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_update(sqlite3_vtab *pVTab, int n, sqlite3_value **argv,
                       sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_findfunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                             void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                             void **ppArg) {
    return 0;
}

static int dummy_rename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    1,                          /* iVersion */
    dummy_create,              /* xCreate */
    dummy_connect,             /* xConnect */
    dummy_bestindex,           /* xBestIndex */
    dummy_disconnect,          /* xDisconnect */
    dummy_destroy_vtab,        /* xDestroy */
    dummy_open,                /* xOpen */
    dummy_close,               /* xClose */
    dummy_filter,              /* xFilter */
    dummy_next,                /* xNext */
    dummy_eof,                 /* xEof */
    dummy_column,              /* xColumn */
    dummy_rowid,               /* xRowid */
    dummy_update,              /* xUpdate */
    NULL,                      /* xBegin */
    NULL,                      /* xSync */
    NULL,                      /* xCommit */
    NULL,                      /* xRollback */
    dummy_findfunction,        /* xFindFunction */
    dummy_rename,              /* xRename */
    NULL,                      /* xSavepoint */
    NULL,                      /* xRelease */
    NULL,                      /* xRollbackTo */
    NULL,                      /* xShadowName */
    NULL                       /* xIntegrity */
};

static void parse_strings(const uint8_t *data, size_t size, 
                         const char ***out_array, int *out_count) {
    if (size == 0) {
        *out_array = NULL;
        *out_count = 0;
        return;
    }
    
    /* Count strings (separated by null bytes) */
    int count = 0;
    const uint8_t *ptr = data;
    const uint8_t *end = data + size;
    
    while (ptr < end) {
        const uint8_t *str_start = ptr;
        while (ptr < end && *ptr != 0) {
            ptr++;
        }
        if (ptr > str_start) {
            count++;
        }
        if (ptr < end) {
            ptr++; /* Skip null terminator */
        }
    }
    
    if (count == 0) {
        *out_array = NULL;
        *out_count = 0;
        return;
    }
    
    /* Allocate array */
    const char **array = (const char **)malloc((count + 1) * sizeof(const char *));
    if (!array) {
        *out_array = NULL;
        *out_count = 0;
        return;
    }
    
    /* Fill array */
    ptr = data;
    int idx = 0;
    while (ptr < end && idx < count) {
        const uint8_t *str_start = ptr;
        while (ptr < end && *ptr != 0) {
            ptr++;
        }
        if (ptr > str_start) {
            size_t len = ptr - str_start;
            char *str = (char *)malloc(len + 1);
            if (str) {
                memcpy(str, str_start, len);
                str[len] = 0;
                array[idx++] = str;
            }
        }
        if (ptr < end) {
            ptr++;
        }
    }
    array[idx] = NULL; /* NULL-terminate */
    *out_array = array;
    *out_count = idx;
}

static void free_string_array(const char **array) {
    if (array) {
        for (int i = 0; array[i] != NULL; i++) {
            free((void *)array[i]);
        }
        free(array);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use first byte to determine which functions to test */
    uint8_t selector = Size > 0 ? Data[0] : 0;
    const uint8_t *fuzz_data = Size > 0 ? Data + 1 : Data;
    size_t fuzz_size = Size > 0 ? Size - 1 : 0;
    
    /* Test sqlite3_create_module and sqlite3_create_module_v2 */
    if ((selector & 0x01) && fuzz_size > 0) {
        /* Use first 64 bytes as module name */
        size_t name_len = fuzz_size > 64 ? 64 : fuzz_size;
        char *module_name = (char *)malloc(name_len + 1);
        if (module_name) {
            memcpy(module_name, fuzz_data, name_len);
            module_name[name_len] = 0;
            
            /* Test both create_module variants */
            sqlite3_create_module(db, module_name, &dummy_module, NULL);
            sqlite3_create_module_v2(db, module_name, &dummy_module, NULL, dummy_destroy);
            
            free(module_name);
        }
    }
    
    /* Test sqlite3_drop_modules */
    if ((selector & 0x02) && fuzz_size > 0) {
        const char **keep_array = NULL;
        int keep_count = 0;
        
        parse_strings(fuzz_data, fuzz_size, &keep_array, &keep_count);
        
        /* Test with keep list */
        sqlite3_drop_modules(db, keep_array);
        
        /* Test with NULL (remove all) */
        sqlite3_drop_modules(db, NULL);
        
        free_string_array(keep_array);
    }
    
    /* Test sqlite3_overload_function */
    if ((selector & 0x04) && fuzz_size > 2) {
        /* Use first byte as function name length, second as arg count */
        uint8_t name_len = fuzz_data[0];
        uint8_t arg_count = fuzz_data[1];
        
        if (name_len > 0 && name_len + 2 <= fuzz_size) {
            char *func_name = (char *)malloc(name_len + 1);
            if (func_name) {
                memcpy(func_name, fuzz_data + 2, name_len);
                func_name[name_len] = 0;
                
                sqlite3_overload_function(db, func_name, arg_count);
                
                free(func_name);
            }
        }
    }
    
    /* Test sqlite3_vtab_config - note: requires valid vtab context */
    if ((selector & 0x08) && fuzz_size > 0) {
        /* We can't call this without being in xCreate/xConnect context,
           but we can still try to call it to see how it handles invalid context */
        int op = fuzz_size > 4 ? *(int *)fuzz_data : 0;
        sqlite3_vtab_config(db, op);
    }
    
    /* Test sqlite3_declare_vtab - note: requires valid vtab context */
    if ((selector & 0x10) && fuzz_size > 0) {
        /* Use first 256 bytes as SQL */
        size_t sql_len = fuzz_size > 256 ? 256 : fuzz_size;
        char *sql = (char *)malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, fuzz_data, sql_len);
            sql[sql_len] = 0;
            
            /* This will fail since we're not in xCreate/xConnect, but we test it anyway */
            sqlite3_declare_vtab(db, sql);
            
            free(sql);
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}