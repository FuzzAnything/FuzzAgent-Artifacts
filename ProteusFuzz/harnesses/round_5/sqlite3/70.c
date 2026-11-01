// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzzer_parse_params(const uint8_t *Data, size_t Size, int *nParam, const char ***azParam) {
    *nParam = 0;
    *azParam = NULL;
    if (Size < 4) return 0;

    // Use first 2 bytes to determine number of parameters (capped to avoid overflow)
    int count = (Data[0] & 0xFF) | ((Data[1] & 0xFF) << 8);
    if (count > 100) count = 100; // Cap to prevent excessive allocation

    if (count == 0) return 0;

    // Allocate array for 2*count pointers
    *azParam = (const char **)malloc(sizeof(const char *) * 2 * count);
    if (!*azParam) return 0;

    size_t offset = 2;
    int i;
    for (i = 0; i < count; i++) {
        size_t key_len = 0, val_len = 0;
        if (offset < Size) key_len = Data[offset++];
        if (offset < Size) val_len = Data[offset++];

        // Cap lengths to prevent huge allocations
        if (key_len > 1000) key_len = 1000;
        if (val_len > 1000) val_len = 1000;

        if (offset + key_len + val_len > Size) {
            free(*azParam);
            *azParam = NULL;
            return 0;
        }

        // Allocate and copy key
        char *key = (char *)malloc(key_len + 1);
        if (!key) {
            for (int j = 0; j < i; j++) {
                free((void *)(*azParam)[2*j]);
                free((void *)(*azParam)[2*j+1]);
            }
            free(*azParam);
            *azParam = NULL;
            return 0;
        }
        memcpy(key, &Data[offset], key_len);
        key[key_len] = '\0';
        (*azParam)[2*i] = key;
        offset += key_len;

        // Allocate and copy value
        char *val = (char *)malloc(val_len + 1);
        if (!val) {
            free(key);
            for (int j = 0; j < i; j++) {
                free((void *)(*azParam)[2*j]);
                free((void *)(*azParam)[2*j+1]);
            }
            free(*azParam);
            *azParam = NULL;
            return 0;
        }
        memcpy(val, &Data[offset], val_len);
        val[val_len] = '\0';
        (*azParam)[2*i+1] = val;
        offset += val_len;
    }

    *nParam = count;
    return 1;
}

static void fuzzer_free_params(int nParam, const char **azParam) {
    if (!azParam) return;
    for (int i = 0; i < nParam; i++) {
        free((void *)azParam[2*i]);
        free((void *)azParam[2*i+1]);
    }
    free((void *)azParam);
}

static const char *fuzzer_get_string(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) return NULL;
    size_t len = Data[*offset];
    (*offset)++;
    if (*offset + len > Size) return NULL;
    
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, &Data[*offset], len);
    str[len] = '\0';
    *offset += len;
    return str;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Strategy: Use data to construct database, journal, wal names and URI params
    size_t offset = 0;
    
    const char *zDatabase = fuzzer_get_string(Data, Size, &offset);
    const char *zJournal = fuzzer_get_string(Data, Size, &offset);
    const char *zWal = fuzzer_get_string(Data, Size, &offset);

    if (!zDatabase || !zJournal || !zWal) {
        // If we can't parse, try fallback with minimal valid strings
        if (zDatabase) free((void *)zDatabase);
        if (zJournal) free((void *)zJournal);
        if (zWal) free((void *)zWal);
        
        zDatabase = "test.db";
        zJournal = "test.db-journal";
        zWal = "test.db-wal";
    }

    int nParam = 0;
    const char **azParam = NULL;
    fuzzer_parse_params(Data, Size, &nParam, &azParam);

    // Create filename
    sqlite3_filename fn = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);

    if (fn) {
        // Test sqlite3_filename_database
        const char *db_name = sqlite3_filename_database(fn);
        (void)db_name; // Use result to avoid unused warning

        // Test sqlite3_filename_wal
        const char *wal_name = sqlite3_filename_wal(fn);
        (void)wal_name;

        // Test sqlite3_uri_key with various indices
        for (int i = -2; i <= nParam + 2; i++) {
            const char *key = sqlite3_uri_key(fn, i);
            (void)key;
        }

        // Test sqlite3_uri_boolean with various parameters
        // Try some common parameter names and random ones
        const char *test_params[] = {"mode", "immutable", "cache", "temp_store", "temp_store", "random"};
        int test_defaults[] = {0, 1, 0, 1, 0, 1};
        for (int i = 0; i < 6; i++) {
            int result = sqlite3_uri_boolean(fn, test_params[i], test_defaults[i]);
            (void)result;
        }

        // Free the filename
        sqlite3_free_filename(fn);
    }

    // Cleanup
    if (zDatabase && zDatabase != "test.db") free((void *)zDatabase);
    if (zJournal && zJournal != "test.db-journal") free((void *)zJournal);
    if (zWal && zWal != "test.db-wal") free((void *)zWal);
    fuzzer_free_params(nParam, azParam);

    return 0;
}