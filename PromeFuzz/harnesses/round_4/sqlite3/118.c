// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:127555:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static int fuzz_db_filename(sqlite3 *db) {
    const char *db_names[] = {"main", "temp", NULL};
    for (int i = 0; db_names[i]; i++) {
        sqlite3_filename filename = sqlite3_db_filename(db, db_names[i]);
        (void)filename; // Use result to avoid unused variable warning
    }
    return 0;
}

static int fuzz_db_name(sqlite3 *db) {
    for (int i = -1; i < 10; i++) {
        const char *name = sqlite3_db_name(db, i);
        (void)name; // Use result to avoid unused variable warning
    }
    return 0;
}

static int fuzz_file_control(sqlite3 *db, const uint8_t *data, size_t size) {
    const char *db_names[] = {"main", "temp", NULL};
    int opcodes[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
        31, 32, 33, 34, 35, 36, 37, 38
    };
    
    for (int i = 0; db_names[i]; i++) {
        for (size_t j = 0; j < sizeof(opcodes)/sizeof(opcodes[0]); j++) {
            void *arg = NULL;
            if (size > 0) {
                arg = (void*)(data + (j % size));
            }
            int rc = sqlite3_file_control(db, db_names[i], opcodes[j], arg);
            (void)rc; // Use result to avoid unused variable warning
        }
    }
    return 0;
}

static int fuzz_load_extension(sqlite3 *db, const uint8_t *data, size_t size) {
    char *errmsg = NULL;
    const char *ext_file = "./dummy_file";
    
    // Write fuzzer data to dummy file
    dummy_file_write(data, size);
    
    // Try to load extension with various entry point names
    const char *entry_points[] = {"sqlite3_extension_init", "init", NULL, "test"};
    
    for (size_t i = 0; i < sizeof(entry_points)/sizeof(entry_points[0]); i++) {
        int rc = sqlite3_load_extension(db, ext_file, entry_points[i], &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        (void)rc; // Use result to avoid unused variable warning
    }
    
    return 0;
}

static int fuzz_open_functions(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Write fuzzer data to dummy file
    dummy_file_write(data, size);
    
    // Test sqlite3_open
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db) {
        // Test other functions with the opened database
        fuzz_db_filename(db);
        fuzz_db_name(db);
        fuzz_file_control(db, data, size);
        
        // Enable extension loading for testing
        sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, 1, NULL);
        fuzz_load_extension(db, data, size);
        
        sqlite3_close(db);
    }
    
    // Test sqlite3_open_v2 with various flags
    int flags_list[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_SHAREDCACHE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_PRIVATECACHE
    };
    
    for (size_t i = 0; i < sizeof(flags_list)/sizeof(flags_list[0]); i++) {
        rc = sqlite3_open_v2("./dummy_file", &db, flags_list[i], NULL);
        if (rc == SQLITE_OK && db) {
            fuzz_db_filename(db);
            fuzz_db_name(db);
            fuzz_file_control(db, data, size);
            sqlite3_close(db);
        }
    }
    
    // Test with special filenames
    const char *special_filenames[] = {
        ":memory:",
        "",
        "file:./dummy_file?mode=ro",
        "file:./dummy_file?mode=rwc",
        "file:./dummy_file?vfs=unix-none"
    };
    
    for (size_t i = 0; i < sizeof(special_filenames)/sizeof(special_filenames[0]); i++) {
        rc = sqlite3_open(special_filenames[i], &db);
        if (rc == SQLITE_OK && db) {
            fuzz_db_filename(db);
            fuzz_db_name(db);
            sqlite3_close(db);
        }
        
        rc = sqlite3_open_v2(special_filenames[i], &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc == SQLITE_OK && db) {
            fuzz_db_filename(db);
            fuzz_db_name(db);
            sqlite3_close(db);
        }
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Fuzz all target functions
    fuzz_open_functions(data, size);
    
    // Cleanup
    sqlite3_shutdown();
    
    return 0;
}