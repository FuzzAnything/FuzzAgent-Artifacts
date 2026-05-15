// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Create dummy strings for database, journal, and WAL */
    const char *zDatabase = "./dummy_db";
    const char *zJournal = "./dummy_db-journal";
    const char *zWal = "./dummy_db-wal";

    /* Prepare URI parameters from fuzzer input */
    int nParam = 1;
    const char *azParam[2];
    
    /* Use first byte to determine parameter key length */
    size_t key_len = Data[0] % (Size > 1 ? Size - 1 : 1) + 1;
    if (key_len >= Size) key_len = Size > 1 ? Size - 1 : 1;
    
    /* Create temporary buffer for key */
    char *key = (char *)malloc(key_len + 1);
    if (!key) return 0;
    for (size_t i = 0; i < key_len && i + 1 < Size; i++) {
        key[i] = Data[i + 1] ? Data[i + 1] : 'a';
    }
    key[key_len] = '\0';
    
    /* Use remaining data for value if available */
    char *value = NULL;
    size_t val_start = 1 + key_len;
    if (val_start < Size) {
        size_t val_len = Size - val_start;
        value = (char *)malloc(val_len + 1);
        if (value) {
            for (size_t i = 0; i < val_len; i++) {
                value[i] = Data[val_start + i] ? Data[val_start + i] : 'b';
            }
            value[val_len] = '\0';
        }
    } else {
        value = (char *)malloc(1);
        if (value) value[0] = '\0';
    }
    
    if (!value) {
        free(key);
        return 0;
    }
    
    azParam[0] = key;
    azParam[1] = value;

    /* Create filename using sqlite3_create_filename */
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    if (filename) {
        /* Test sqlite3_uri_parameter */
        const char *param_val = sqlite3_uri_parameter(filename, key);
        (void)param_val;
        
        /* Test sqlite3_filename_database */
        const char *db_name = sqlite3_filename_database(filename);
        (void)db_name;
        
        /* Test sqlite3_filename_journal */
        const char *journal_name = sqlite3_filename_journal(filename);
        (void)journal_name;
        
        /* Test sqlite3_filename_wal */
        const char *wal_name = sqlite3_filename_wal(filename);
        (void)wal_name;
        
        /* Test sqlite3_uri_parameter with other hardcoded parameters */
        const char *test_params[] = {"mode", "cache", "psow", "immutable", "vfs"};
        for (int i = 0; i < 5; i++) {
            const char *test_val = sqlite3_uri_parameter(filename, test_params[i]);
            (void)test_val;
        }
        
        /* Test sqlite3_uri_key */
        const char *key_name = sqlite3_uri_key(filename, 0);
        (void)key_name;
        
        /* Clean up */
        sqlite3_free_filename(filename);
    }
    
    free(key);
    free(value);
    
    return 0;
}