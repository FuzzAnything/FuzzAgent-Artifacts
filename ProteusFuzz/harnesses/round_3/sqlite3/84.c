// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Helper to ensure a string is null-terminated within a buffer */
static const char *ensure_null_terminated(const uint8_t *data, size_t size) {
    if (size == 0) return "";
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\0') {
            return (const char *)data;
        }
    }
    /* If no null terminator found, return a safe static empty string 
       or a copy with a terminator. For fuzzing safety, we return a 
       static empty string to avoid OOB reads if the caller expects 
       a null-terminated string, or we could construct a safe copy.
       Here we construct a safe copy to allow testing of non-empty content. */
    char *safe = malloc(size + 1);
    if (!safe) return "";
    memcpy(safe, data, size);
    safe[size] = '\0';
    /* Note: In a real fuzz harness, we might leak this or manage it, 
       but for simplicity in this specific driver, we rely on the 
       caller not using the pointer after this function if it's 
       not freed. However, sqlite3_create_filename takes const char*,
       so we can't free it immediately if we pass it. 
       To avoid leaks in the loop, we will use a static buffer for 
       this helper in the main loop or manage it carefully.
       Better approach for fuzzing: Use a static buffer for the 
       helper to avoid malloc/free complexity in the loop. */
    static char static_buf[4096];
    size_t copy_len = (size < sizeof(static_buf) - 1) ? size : sizeof(static_buf) - 1;
    memcpy(static_buf, data, copy_len);
    static_buf[copy_len] = '\0';
    return static_buf;
}

/* Helper to create a safe parameter array for sqlite3_create_filename */
static const char **create_param_array(const uint8_t *data, size_t size, int *out_n) {
    if (size < 2) {
        *out_n = 0;
        return NULL;
    }
    
    /* We interpret the data as pairs of strings. 
       Since raw data might not be null-terminated, we use the helper.
       To keep it simple and safe, we'll just create a small fixed set 
       of parameters based on the input size to avoid complex parsing 
       that might crash. */
    
    /* Let's try to extract up to 4 key-value pairs from the input data */
    const char **params = malloc(8 * sizeof(char *));
    if (!params) {
        *out_n = 0;
        return NULL;
    }

    int n = 0;
    size_t offset = 0;
    while (n < 4 && offset + 2 <= size) {
        /* Simple heuristic: take chunks as keys and values */
        /* This is a simplified parsing for fuzzing purposes */
        size_t key_len = 0;
        size_t val_len = 0;
        
        /* Find key length (until null or end) */
        while (offset + key_len < size && data[offset + key_len] != '\0') {
            key_len++;
        }
        if (offset + key_len >= size) break;
        offset += key_len + 1; /* skip null */
        
        /* Find value length */
        while (offset + val_len < size && data[offset + val_len] != '\0') {
            val_len++;
        }
        if (offset + val_len >= size) break;
        offset += val_len + 1; /* skip null */
        
        /* Use static buffers for key and value to avoid leaks in loop */
        static char s_key[256], s_val[256];
        size_t k_copy = (key_len < sizeof(s_key)-1) ? key_len : sizeof(s_key)-1;
        size_t v_copy = (val_len < sizeof(s_val)-1) ? val_len : sizeof(s_val)-1;
        memcpy(s_key, data + offset - key_len - 1, k_copy);
        s_key[k_copy] = '\0';
        memcpy(s_val, data + offset - val_len - 1, v_copy);
        s_val[v_copy] = '\0';
        
        params[n*2] = s_key;
        params[n*2+1] = s_val;
        n++;
    }
    
    *out_n = n;
    return params;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_filename filename = NULL;
    
    /* 1. Prepare environment: Open a database connection */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    /* Ensure we have some data to work with */
    if (Size < 1) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a safe database name from input */
    const char *db_name = ensure_null_terminated(Data, Size);
    
    /* Try to open a file-based database to get a valid filename from sqlite3_db_filename */
    /* We use a dummy file path */
    const char *dummy_path = "./dummy_file";
    sqlite3_close(db);
    
    if (sqlite3_open(dummy_path, &db) != SQLITE_OK) {
        /* If we can't open the dummy file, try :memory: again and use a constructed filename */
        sqlite3_close(db);
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        /* For :memory:, sqlite3_db_filename returns NULL or a special string. 
           We will handle the case where it returns NULL. */
    }

    const char *real_db_filename = sqlite3_db_filename(db, "main");
    
    /* 2. Fuzz sqlite3_filename_database, sqlite3_filename_journal, sqlite3_filename_wal */
    /* These require a valid filename from the core or sqlite3_db_filename */
    if (real_db_filename != NULL) {
        sqlite3_filename_database((sqlite3_filename)real_db_filename);
        sqlite3_filename_journal((sqlite3_filename)real_db_filename);
        sqlite3_filename_wal((sqlite3_filename)real_db_filename);
    }

    /* 3. Fuzz sqlite3_uri_boolean and sqlite3_uri_key with the real filename */
    if (real_db_filename != NULL) {
        sqlite3_uri_boolean((sqlite3_filename)real_db_filename, "mode", 0);
        sqlite3_uri_boolean((sqlite3_filename)real_db_filename, "readonly", 1);
        sqlite3_uri_key((sqlite3_filename)real_db_filename, 0);
        sqlite3_uri_key((sqlite3_filename)real_db_filename, 1);
        sqlite3_uri_key((sqlite3_filename)real_db_filename, -1); /* Invalid N */
    }

    /* 4. Fuzz sqlite3_create_filename and sqlite3_free_filename */
    /* We need to construct a valid filename structure */
    const char *zDatabase = (Size > 0) ? ensure_null_terminated(Data, Size) : "test.db";
    const char *zJournal = (Size > 10) ? ensure_null_terminated(Data + 10, Size - 10) : "test.db-journal";
    const char *zWal = (Size > 20) ? ensure_null_terminated(Data + 20, Size - 20) : "test.db-wal";
    
    /* Ensure non-NULL for create_filename requirements */
    if (zDatabase == NULL) zDatabase = "test.db";
    if (zJournal == NULL) zJournal = "test.db-journal";
    if (zWal == NULL) zWal = "test.db-wal";

    int nParam = 0;
    const char **azParam = create_param_array(Data, Size, &nParam);
    
    /* Create filename */
    sqlite3_filename created_filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    if (created_filename != NULL) {
        /* Test extraction functions on the created filename */
        sqlite3_filename_database(created_filename);
        sqlite3_filename_journal(created_filename);
        sqlite3_filename_wal(created_filename);
        
        sqlite3_uri_boolean(created_filename, "mode", 0);
        sqlite3_uri_key(created_filename, 0);
        sqlite3_uri_key(created_filename, 1);
        
        /* Free the created filename */
        sqlite3_free_filename(created_filename);
    }

    /* Cleanup */
    sqlite3_close(db);
    
    /* Free allocated param array if any */
    if (azParam) {
        free((void *)azParam);
    }

    return 0;
}