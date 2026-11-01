// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Callback function for sqlite3_exec.
 * This is a dummy callback to satisfy the API requirement.
 */
static int dummy_callback(void *arg, int argc, char **argv, char **col_name) {
    (void)arg;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *errmsg = NULL;
    const char *filename = "./dummy_file";
    
    /* 
     * Prepare a VFS name buffer on the heap to ensure lifetime safety.
     */
    const char *zVfs = NULL;
    char *vfs_buf = NULL;

    /* 
     * Strategy: 
     * 1. Open a database (sqlite3_open_v2)
     * 2. Perform various configurations and queries (sqlite3_db_config, sqlite3_exec, etc.)
     * 3. Check state (sqlite3_total_changes)
     * 4. Toggle features (sqlite3_enable_load_extension)
     * 5. Close the database (sqlite3_close_v2)
     */

    size_t sql_len = 0;
    const char *sql = "";
    
    if (Size > 0) {
        /* 
         * We will use the input data as the SQL string if it's valid.
         * We'll copy up to the first null or the size limit, ensuring termination.
         */
        size_t copy_len = Size;
        if (Size > 1024) {
            copy_len = 1024; /* Limit SQL size to prevent excessive memory usage */
        }
        
        /* Allocate SQL buffer */
        char *sql_buf = malloc(copy_len + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, copy_len);
            sql_buf[copy_len] = '\0';
            sql = sql_buf;
            
            /* 
             * Use Data[0] to pick flags for sqlite3_open_v2.
             */
            int flags = 0;
            int f = Data[0];
            if (f & 1) flags |= SQLITE_OPEN_READONLY;
            if (f & 2) flags |= SQLITE_OPEN_READWRITE;
            if (f & 4) flags |= SQLITE_OPEN_CREATE;
            
            /* Ensure at least one valid mode is set if none selected */
            if (flags == 0) flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

            /* 
             * Try to use a portion of input as VFS name if it looks like a string.
             */
            if (Size > 10) {
                size_t vfs_len = 0;
                for (size_t i = 1; i < Size && i < 20; i++) {
                    if (Data[i] == '\0') break;
                    vfs_len++;
                }
                if (vfs_len > 0) {
                    vfs_buf = malloc(vfs_len + 1);
                    if (vfs_buf) {
                        memcpy(vfs_buf, Data + 1, vfs_len);
                        vfs_buf[vfs_len] = '\0';
                        zVfs = vfs_buf;
                    }
                }
            }

            rc = sqlite3_open_v2(filename, &db, flags, zVfs);

            if (rc != SQLITE_OK) {
                /* 
                 * If opening fails, clean up allocated memory and return.
                 */
                if (vfs_buf) free(vfs_buf);
                if (sql_buf) free(sql_buf);
                return 0;
            }

            /* 
             * 1. sqlite3_db_config: Try various operations.
             */
            if (Size > 2) {
                int op = Data[2] % 10; 
                int arg1 = (Size > 3) ? Data[3] : 0;
                
                /* 
                 * Specific handling for known ops safely.
                 * Removed SQLITE_DBCONFIG_ENABLE_FK_CASCADE_DELETE as it may not be defined in all versions.
                 */
                if (op == 0) {
                    /* SQLITE_DBCONFIG_LOOKASIDE */
                    sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, (void*)0, 0, 0);
                } else if (op == 1) {
                    /* SQLITE_DBCONFIG_ENABLE_FKEY */
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, arg1, NULL);
                } else if (op == 2) {
                    /* SQLITE_DBCONFIG_ENABLE_TRIGGER */
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, arg1, NULL);
                } else if (op == 3) {
                     /* SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE */
                     int prev = 0;
                     sqlite3_db_config(db, SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE, arg1, &prev);
                } else if (op == 4) {
                    /* SQLITE_DBCONFIG_ENABLE_QPSG */
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_QPSG, arg1, NULL);
                }
            }

            /* 
             * 2. sqlite3_enable_load_extension
             */
            if (Size > 5) {
                int onoff = (Data[5] % 2 == 0) ? 1 : 0;
                sqlite3_enable_load_extension(db, onoff);
            }

            /* 
             * 3. sqlite3_exec: Execute the SQL string derived from input.
             */
            rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            if (rc != SQLITE_OK && errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }

            /* 
             * 4. sqlite3_total_changes: Check the state.
             */
            int changes = sqlite3_total_changes(db);
            (void)changes;

            /* 
             * 5. Try another exec with a simple query to ensure state changes.
             */
            const char *simple_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY); INSERT OR IGNORE INTO fuzz_test VALUES (1);";
            sqlite3_exec(db, simple_sql, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            
            changes = sqlite3_total_changes(db);
            (void)changes;

            /* 
             * 6. Close the database.
             */
            sqlite3_close_v2(db);
            db = NULL;

            /* Cleanup */
            if (vfs_buf) free(vfs_buf);
            if (sql_buf) free(sql_buf);
            
        } else {
            /* Allocation failed, just return */
        }
    }

    return 0;
}