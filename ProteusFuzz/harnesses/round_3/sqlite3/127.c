// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to create a valid in-memory database connection.
 * Returns NULL on failure.
 */
static sqlite3 *create_in_memory_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Helper function to create a dummy database file if needed.
 * Writes a minimal valid SQLite header to the file.
 */
static int create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;

    /* Minimal SQLite header (100 bytes) */
    char header[100] = {0};
    const char *magic = "SQLite format 3\000";
    memcpy(header, magic, 16);
    /* Page size = 4096 (0x1000) */
    header[16] = 0x10;
    header[17] = 0x00;
    /* File format write version = 1 */
    header[18] = 1;
    /* File format read version = 1 */
    header[19] = 1;
    /* Reserved space at end of each page = 0 */
    header[20] = 0;
    /* Max embedded payload fraction = 64 */
    header[21] = 64;
    /* Min embedded payload fraction = 32 */
    header[22] = 32;
    /* Leaf payload fraction = 32 */
    header[23] = 32;
    /* File change counter = 1 */
    header[24] = 0; header[25] = 0; header[26] = 0; header[27] = 1;
    /* Database size in pages = 1 */
    header[28] = 0; header[29] = 0; header[30] = 0; header[31] = 1;
    /* First freelist trunk page = 0 */
    header[32] = 0; header[33] = 0; header[34] = 0; header[35] = 0;
    /* Total freelist pages = 0 */
    header[36] = 0; header[37] = 0; header[38] = 0; header[39] = 0;
    /* Schema cookie = 1 */
    header[40] = 0; header[41] = 0; header[42] = 0; header[43] = 1;
    /* Schema format number = 4 */
    header[44] = 0; header[45] = 0; header[46] = 0; header[47] = 4;
    /* Default page cache size = 0 */
    header[48] = 0; header[49] = 0; header[50] = 0; header[51] = 0;
    /* Largest root page = 0 */
    header[52] = 0; header[53] = 0; header[54] = 0; header[55] = 0;
    /* Text encoding = 1 (UTF-8) */
    header[56] = 0; header[57] = 0; header[58] = 0; header[59] = 1;
    /* User version = 0 */
    header[60] = 0; header[61] = 0; header[62] = 0; header[63] = 0;
    /* Incremental vacuum mode = 0 */
    header[64] = 0; header[65] = 0; header[66] = 0; header[67] = 0;
    /* Application ID = 0 */
    header[68] = 0; header[69] = 0; header[70] = 0; header[71] = 0;
    /* Reserved for expansion (20 bytes) */
    /* Version-valid-for number = 1 */
    header[92] = 0; header[93] = 0; header[94] = 0; header[95] = 1;
    /* SQLite version number = 3000000 */
    header[96] = 0; header[97] = 0; header[98] = 0; header[99] = 1;

    fwrite(header, 1, 100, f);
    fclose(f);
    return 1;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Use the first byte to decide which fuzzing scenario to run */
    uint8_t scenario = Data[0] % 5;

    /* Clean up resources at the end of each scenario */
    sqlite3 *db = NULL;
    unsigned char *serialized_data = NULL;
    void *malloc_ptr = NULL;
    void *realloc_ptr = NULL;
    sqlite3_int64 size = 0;

    /* Create a dummy file for scenarios that might need it */
    if (scenario == 4) {
        create_dummy_file("./dummy_file");
    }

    switch (scenario) {
        case 0:
            /* Fuzz sqlite3_malloc64 and sqlite3_free */
            if (Size >= 3) {
                sqlite3_uint64 n = ((sqlite3_uint64)Data[1] << 8) | Data[2];
                /* Limit allocation size to prevent OOM in fuzzer */
                if (n > 1024 * 1024) n = 1024 * 1024;
                malloc_ptr = sqlite3_malloc64(n);
                if (malloc_ptr) {
                    /* Write fuzzed data into the allocated memory */
                    memset(malloc_ptr, 0xFF, n);
                    sqlite3_free(malloc_ptr);
                    malloc_ptr = NULL;
                }
            }
            break;

        case 1:
            /* Fuzz sqlite3_malloc and sqlite3_free */
            if (Size >= 3) {
                int n = ((int)Data[1] << 8) | Data[2];
                if (n > 1024 * 1024) n = 1024 * 1024;
                if (n <= 0) n = 1;
                malloc_ptr = sqlite3_malloc(n);
                if (malloc_ptr) {
                    memset(malloc_ptr, 0xAA, n);
                    sqlite3_free(malloc_ptr);
                    malloc_ptr = NULL;
                }
            }
            break;

        case 2:
            /* Fuzz sqlite3_realloc64 */
            if (Size >= 3) {
                sqlite3_uint64 n = ((sqlite3_uint64)Data[1] << 8) | Data[2];
                if (n > 1024 * 1024) n = 1024 * 1024;
                
                /* First allocate some memory */
                void *old_ptr = sqlite3_malloc64(100);
                if (old_ptr) {
                    memset(old_ptr, 0xBB, 100);
                    realloc_ptr = sqlite3_realloc64(old_ptr, n);
                    if (realloc_ptr) {
                        memset(realloc_ptr, 0xCC, n);
                        sqlite3_free(realloc_ptr);
                        realloc_ptr = NULL;
                    } else {
                        /* If realloc fails, old_ptr is still valid */
                        sqlite3_free(old_ptr);
                    }
                }
            }
            break;

        case 3:
            /* Fuzz sqlite3_serialize and sqlite3_deserialize */
            db = create_in_memory_db();
            if (db) {
                /* Create a simple table to have something to serialize */
                const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);";
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                
                /* Serialize the database */
                serialized_data = sqlite3_serialize(db, NULL, &size, 0);
                if (serialized_data) {
                    /* Try to deserialize into a new connection */
                    sqlite3 *db2 = create_in_memory_db();
                    if (db2) {
                        /* Use the fuzzed data as the serialized buffer if possible */
                        unsigned char *deserialize_buf = NULL;
                        sqlite3_int64 deserialize_size = 0;
                        sqlite3_int64 deserialize_buf_size = 0;
                        
                        if (Size > 10) {
                            /* Use fuzzed data as the buffer */
                            deserialize_buf = (unsigned char *)sqlite3_malloc64(Size);
                            if (deserialize_buf) {
                                memcpy(deserialize_buf, Data, Size);
                                deserialize_size = Size;
                                deserialize_buf_size = Size;
                            }
                        }
                        
                        if (deserialize_buf) {
                            int flags = (Data[1] % 4) << 0; /* Vary flags */
                            sqlite3_deserialize(db2, NULL, deserialize_buf, deserialize_size, deserialize_buf_size, flags);
                            sqlite3_free(deserialize_buf);
                        } else {
                            /* Fallback: try to deserialize the original serialized data */
                            sqlite3_deserialize(db2, NULL, serialized_data, size, size, 0);
                        }
                        sqlite3_close(db2);
                    }
                    
                    sqlite3_free(serialized_data);
                    serialized_data = NULL;
                }
                
                /* Try NOCOPY mode */
                sqlite3_int64 ncopy_size = 0;
                unsigned char *ncopy_data = sqlite3_serialize(db, NULL, &ncopy_size, SQLITE_SERIALIZE_NOCOPY);
                /* ncopy_data might be NULL if no contiguous buffer exists, which is fine */
                (void)ncopy_data; /* Suppress unused variable warning */
                (void)ncopy_size;
                
                sqlite3_close(db);
                db = NULL;
            }
            break;

        case 4:
            /* Fuzz with a file-based database */
            {
                sqlite3 *temp_db = NULL;
                int rc = sqlite3_open("./dummy_file", &temp_db);
                if (rc == SQLITE_OK && temp_db != NULL) {
                    db = temp_db;
                    /* Serialize file-based DB */
                    serialized_data = sqlite3_serialize(db, NULL, &size, 0);
                    if (serialized_data) {
                        /* Deserialize into memory */
                        sqlite3 *db2 = create_in_memory_db();
                        if (db2) {
                            sqlite3_deserialize(db2, NULL, serialized_data, size, size, 0);
                            sqlite3_close(db2);
                        }
                        sqlite3_free(serialized_data);
                        serialized_data = NULL;
                    }
                    sqlite3_close(db);
                    db = NULL;
                }
            }
            break;
    }

    /* Cleanup */
    if (db) {
        sqlite3_close(db);
    }
    if (serialized_data) {
        sqlite3_free(serialized_data);
    }
    if (malloc_ptr) {
        sqlite3_free(malloc_ptr);
    }
    if (realloc_ptr) {
        sqlite3_free(realloc_ptr);
    }

    return 0;
}