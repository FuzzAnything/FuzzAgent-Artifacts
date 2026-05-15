// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Dummy sqlite3_module implementation for fuzzing */
static sqlite3_module dummy_module = {
    0, /* iVersion */
    NULL, /* xCreate */
    NULL, /* xConnect */
    NULL, /* xBestIndex */
    NULL, /* xDisconnect */
    NULL, /* xDestroy */
    NULL, /* xOpen */
    NULL, /* xClose */
    NULL, /* xFilter */
    NULL, /* xNext */
    NULL, /* xEof */
    NULL, /* xColumn */
    NULL, /* xRowid */
    NULL, /* xUpdate */
    NULL, /* xBegin */
    NULL, /* xSync */
    NULL, /* xCommit */
    NULL, /* xRollback */
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

/* Module destructor for sqlite3_create_module_v2 */
static void dummy_destructor(void *p) {
    /* Do nothing */
}

/* Helper to create database connection */
static sqlite3* create_database(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/* Helper to parse fuzz input into strings */
static void parse_input(const uint8_t *data, size_t size, 
                       char **str1, char **str2, char **str3) {
    if (size == 0) {
        *str1 = *str2 = *str3 = NULL;
        return;
    }
    
    /* Copy input to mutable buffer with null termination */
    char *buf = (char*)malloc(size + 1);
    if (!buf) return;
    memcpy(buf, data, size);
    buf[size] = 0;
    
    /* Split into strings at null bytes if present */
    *str1 = buf;
    
    /* Find second string */
    size_t len1 = strlen(*str1);
    if (len1 < size) {
        *str2 = buf + len1 + 1;
        size_t len2 = strlen(*str2);
        if (len1 + len2 + 1 < size) {
            *str3 = buf + len1 + len2 + 2;
        } else {
            *str3 = NULL;
        }
    } else {
        *str2 = *str3 = NULL;
    }
}

/* Wrapper for sqlite3_declare_vtab that handles the mutex requirement */
static void safe_declare_vtab(sqlite3 *db, const char *sql_str) {
    if (!sql_str) return;
    
    /* Create a simple virtual table module */
    sqlite3_module vtab_module = dummy_module;
    
    /* Register the module */
    sqlite3_create_module(db, "dummy_vtab", &vtab_module, NULL);
    
    /* Create a virtual table using the module - this will call declare_vtab internally */
    char create_sql[512];
    snprintf(create_sql, sizeof(create_sql), 
             "CREATE VIRTUAL TABLE temp.dummy_table USING dummy_vtab(%s)", sql_str);
    
    char *errmsg = NULL;
    sqlite3_exec(db, create_sql, NULL, NULL, &errmsg);
    
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    /* Drop the table if it was created */
    sqlite3_exec(db, "DROP TABLE IF EXISTS temp.dummy_table", NULL, NULL, NULL);
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    sqlite3 *db = create_database();
    if (!db) return 0;
    
    char *module_name = NULL;
    char *func_name = NULL;
    char *sql_str = NULL;
    parse_input(Data, Size, &module_name, &func_name, &sql_str);
    
    /* Test sqlite3_create_module */
    if (module_name) {
        sqlite3_create_module(db, module_name, &dummy_module, NULL);
    }
    
    /* Test sqlite3_create_module_v2 */
    if (module_name) {
        sqlite3_create_module_v2(db, module_name, &dummy_module, NULL, 
                                dummy_destructor);
    }
    
    /* Test sqlite3_overload_function */
    if (func_name) {
        sqlite3_overload_function(db, func_name, 1);
        sqlite3_overload_function(db, func_name, 2);
    }
    
    /* Test sqlite3_declare_vtab - using safe wrapper */
    safe_declare_vtab(db, sql_str);
    
    /* Test sqlite3_get_table */
    if (sql_str) {
        char **result = NULL;
        int rows = 0, cols = 0;
        char *errmsg = NULL;
        
        /* Try to execute the SQL */
        int rc = sqlite3_get_table(db, sql_str, &result, &rows, &cols, &errmsg);
        if (rc == SQLITE_OK && result) {
            sqlite3_free_table(result);
        }
        if (errmsg) {
            sqlite3_free(errmsg);
        }
    }
    
    /* Test sqlite3_drop_modules */
    if (module_name) {
        /* Try dropping all modules */
        sqlite3_drop_modules(db, NULL);
        
        /* Try keeping specific module */
        const char *keep_list[] = { module_name, NULL };
        sqlite3_drop_modules(db, keep_list);
    }
    
    /* Cleanup */
    if (module_name) {
        free(module_name);
    }
    sqlite3_close(db);
    
    return 0;
}