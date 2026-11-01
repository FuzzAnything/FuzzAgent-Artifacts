// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
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
#include <stdio.h>

static int parse_fuzz_input(const uint8_t *Data, size_t Size, char **zDatabase, char **zJournal, char **zWal, int *nParam, char ***azParam) {
    if (Size < 1) return 0;

    // Allocate buffers for database, journal, wal
    *zDatabase = malloc(Size + 1);
    *zJournal = malloc(Size + 1);
    *zWal = malloc(Size + 1);
    if (!*zDatabase || !*zJournal || !*zWal) {
        free(*zDatabase); free(*zJournal); free(*zWal);
        return 0;
    }

    // Simple parsing: split data into 3 parts for D, J, W
    size_t d_len = Size / 3;
    size_t j_len = Size / 3;
    size_t w_len = Size - d_len - j_len;

    memcpy(*zDatabase, Data, d_len);
    (*zDatabase)[d_len] = '\0';

    memcpy(*zJournal, Data + d_len, j_len);
    (*zJournal)[j_len] = '\0';

    memcpy(*zWal, Data + d_len + j_len, w_len);
    (*zWal)[w_len] = '\0';

    // Parse URI parameters from the rest of the buffer if any
    // For simplicity, we treat the last part as a list of "key=value" strings separated by nulls
    // But since input is binary, we just try to extract some parameters if size is large enough
    *nParam = 0;
    *azParam = NULL;

    if (Size > 10) {
        // Try to extract up to 5 parameters from the end of the buffer
        int max_params = 5;
        int count = 0;
        char **params = malloc(max_params * 2 * sizeof(char *));
        if (!params) return 0;

        size_t offset = d_len + j_len + w_len;
        size_t remaining = Size - offset;
        
        // We'll try to parse key/value pairs from the remaining bytes
        // This is a heuristic for fuzzing
        while (remaining > 4 && count < max_params) {
            // Find a potential key (non-null sequence)
            size_t key_len = 0;
            while (offset + key_len < Size && Data[offset + key_len] != '\0' && key_len < 20) {
                key_len++;
            }
            if (key_len == 0) break;

            size_t val_start = offset + key_len;
            if (val_start >= Size) break;
            
            // Skip null separator if present
            if (Data[val_start] == '\0') val_start++;
            if (val_start >= Size) break;

            size_t val_len = 0;
            while (val_start + val_len < Size && Data[val_start + val_len] != '\0' && val_len < 20) {
                val_len++;
            }
            if (val_len == 0) break;

            // Allocate and copy key
            char *key = malloc(key_len + 1);
            if (!key) { free(params); return 0; }
            memcpy(key, Data + offset, key_len);
            key[key_len] = '\0';

            // Allocate and copy value
            char *val = malloc(val_len + 1);
            if (!val) { free(key); free(params); return 0; }
            memcpy(val, Data + val_start, val_len);
            val[val_len] = '\0';

            params[count * 2] = key;
            params[count * 2 + 1] = val;
            count++;

            offset = val_start + val_len;
            if (Data[offset] == '\0') offset++;
            remaining = Size - offset;
        }

        if (count > 0) {
            *nParam = count;
            *azParam = params;
        } else {
            free(params);
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    char *zDatabase = NULL;
    char *zJournal = NULL;
    char *zWal = NULL;
    int nParam = 0;
    char **azParam = NULL;

    if (!parse_fuzz_input(Data, Size, &zDatabase, &zJournal, &zWal, &nParam, &azParam)) {
        return 0;
    }

    // Ensure non-NULL pointers for sqlite3_create_filename
    // The API requires D, J, W to be non-NULL (can be empty strings)
    if (!zDatabase) zDatabase = "";
    if (!zJournal) zJournal = "";
    if (!zWal) zWal = "";

    // Create a filename structure
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, (const char **)azParam);

    // Test sqlite3_filename_database
    if (filename) {
        const char *db_name = sqlite3_filename_database(filename);
        (void)db_name; // Use the result to avoid unused warning

        // Test sqlite3_filename_journal
        const char *journal_name = sqlite3_filename_journal(filename);
        (void)journal_name;

        // Test sqlite3_filename_wal
        const char *wal_name = sqlite3_filename_wal(filename);
        (void)wal_name;

        // Test sqlite3_uri_parameter with various parameter names
        // Try some common parameter names
        const char *params[] = {"mode", "immutable", "cache", "journal_mode", "temp_store", "locking_mode", "main", "temp", "file", "size", "key", "value", "test", "param"};
        int num_params = sizeof(params) / sizeof(params[0]);
        for (int i = 0; i < num_params; i++) {
            const char *val = sqlite3_uri_parameter(filename, params[i]);
            (void)val;
        }

        // Also try to extract parameters from the fuzzed input if any were created
        if (nParam > 0 && azParam) {
            for (int i = 0; i < nParam; i++) {
                if (azParam[i * 2]) {
                    const char *val = sqlite3_uri_parameter(filename, azParam[i * 2]);
                    (void)val;
                }
            }
        }
    }

    // Cleanup
    sqlite3_free_filename(filename);

    // Free allocated buffers
    free(zDatabase);
    free(zJournal);
    free(zWal);

    if (azParam) {
        for (int i = 0; i < nParam; i++) {
            free(azParam[i * 2]);
            free(azParam[i * 2 + 1]);
        }
        free(azParam);
    }

    return 0;
}