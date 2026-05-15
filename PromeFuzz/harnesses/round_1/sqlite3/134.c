// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:127555:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;
    
    /* Create dummy file for potential extension loading */
    write_dummy_file(Data, Size);
    
    /* Try opening database with different modes */
    const char *db_names[] = {":memory:", "./test.db", "", NULL};
    int flags[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX
    };
    
    for (size_t i = 0; i < sizeof(db_names)/sizeof(db_names[0]) && db_names[i]; i++) {
        rc = sqlite3_open(db_names[i], &db);
        if (rc == SQLITE_OK && db) {
            /* Enable extension loading */
            sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, 1, NULL);
            
            /* Test sqlite3_load_extension with various parameters */
            const char *ext_files[] = {"./dummy_file", "./nonexistent.ext", "", NULL};
            const char *procs[] = {"sqlite3_extension_init", "init", "", NULL};
            
            for (size_t j = 0; j < sizeof(ext_files)/sizeof(ext_files[0]) && ext_files[j]; j++) {
                for (size_t k = 0; k < sizeof(procs)/sizeof(procs[0]) && procs[k]; k++) {
                    rc = sqlite3_load_extension(db, ext_files[j], procs[k], &err_msg);
                    if (err_msg) {
                        sqlite3_free(err_msg);
                        err_msg = NULL;
                    }
                }
            }
            
            /* Test sqlite3_db_filename with various database names */
            const char *dbname_list[] = {"main", "temp", "test", "", NULL};
            for (size_t j = 0; j < sizeof(dbname_list)/sizeof(dbname_list[0]) && dbname_list[j]; j++) {
                sqlite3_filename filename = sqlite3_db_filename(db, dbname_list[j]);
                (void)filename;
            }
            
            /* Test sqlite3_db_name with various indices */
            for (int n = 0; n < 10; n++) {
                const char *name = sqlite3_db_name(db, n);
                (void)name;
            }
            
            /* Test sqlite3_file_control with various opcodes */
            const char *control_dbnames[] = {"main", "temp", NULL};
            int opcodes[] = {
                SQLITE_FCNTL_LOCKSTATE,
                SQLITE_FCNTL_GET_LOCKPROXYFILE,
                SQLITE_FCNTL_SET_LOCKPROXYFILE,
                SQLITE_FCNTL_LAST_ERRNO,
                SQLITE_FCNTL_CHUNK_SIZE,
                SQLITE_FCNTL_SYNC_OMITTED,
                SQLITE_FCNTL_WIN32_AV_RETRY,
                SQLITE_FCNTL_PERSIST_WAL
            };
            
            /* Remove SQLITE_FCNTL_SIZE_HINT from the list since it expects an i64* 
               and we were passing an int* which caused the buffer overflow */
            
            for (size_t j = 0; j < sizeof(control_dbnames)/sizeof(control_dbnames[0]) && control_dbnames[j]; j++) {
                for (size_t k = 0; k < sizeof(opcodes)/sizeof(opcodes[0]); k++) {
                    /* Use a buffer that's large enough for any potential argument type */
                    char arg_buffer[64] = {0};
                    rc = sqlite3_file_control(db, control_dbnames[j], opcodes[k], arg_buffer);
                    (void)rc;
                }
            }
            
            /* Test sqlite3_open_v2 with various flags */
            sqlite3 *db2 = NULL;
            for (size_t j = 0; j < sizeof(flags)/sizeof(flags[0]); j++) {
                rc = sqlite3_open_v2(":memory:", &db2, flags[j], NULL);
                if (rc == SQLITE_OK && db2) {
                    sqlite3_close(db2);
                    db2 = NULL;
                }
            }
            
            /* Try opening with filename derived from fuzzer data */
            if (Size > 0) {
                char *temp_filename = malloc(Size + 1);
                if (temp_filename) {
                    memcpy(temp_filename, Data, Size);
                    temp_filename[Size] = '\0';
                    rc = sqlite3_open(temp_filename, &db2);
                    if (rc == SQLITE_OK && db2) {
                        sqlite3_close(db2);
                    }
                    free(temp_filename);
                }
            }
            
            sqlite3_close(db);
            db = NULL;
        }
    }
    
    /* Clean up */
    remove_dummy_file();
    
    return 0;
}