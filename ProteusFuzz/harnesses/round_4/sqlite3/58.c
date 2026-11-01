// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void parse_fuzz_input(const uint8_t *Data, size_t Size, 
                             char **zDatabase, char **zJournal, char **zWal,
                             int *nParam, char ***azParam) {
    size_t offset = 0;
    *zDatabase = NULL;
    *zJournal = NULL;
    *zWal = NULL;
    *nParam = 0;
    *azParam = NULL;

    if (Size < 12) return;

    // Parse lengths for D, J, W
    size_t lenD = Data[offset++];
    size_t lenJ = Data[offset++];
    size_t lenW = Data[offset++];
    offset += 3; // Skip 3 bytes for alignment or future use

    if (offset + lenD + lenJ + lenW > Size) return;

    // Allocate and copy Database
    *zDatabase = malloc(lenD + 1);
    if (!*zDatabase) return;
    memcpy(*zDatabase, &Data[offset], lenD);
    (*zDatabase)[lenD] = '\0';
    offset += lenD;

    // Allocate and copy Journal
    *zJournal = malloc(lenJ + 1);
    if (!*zJournal) { free(*zDatabase); *zDatabase = NULL; return; }
    memcpy(*zJournal, &Data[offset], lenJ);
    (*zJournal)[lenJ] = '\0';
    offset += lenJ;

    // Allocate and copy WAL
    *zWal = malloc(lenW + 1);
    if (!*zWal) { free(*zDatabase); free(*zJournal); *zDatabase = NULL; *zJournal = NULL; return; }
    memcpy(*zWal, &Data[offset], lenW);
    (*zWal)[lenW] = '\0';
    offset += lenW;

    // Parse URI parameters
    if (offset >= Size) return;
    int n = Data[offset++];
    if (n > 50) n = 50; // Limit number of parameters to avoid excessive allocation

    *nParam = n;
    if (n > 0) {
        *azParam = malloc(sizeof(char*) * 2 * n);
        if (!*azParam) {
            free(*zDatabase); free(*zJournal); free(*zWal);
            *zDatabase = NULL; *zJournal = NULL; *zWal = NULL;
            *nParam = 0;
            return;
        }
        
        // Initialize all pointers to NULL to handle early breaks safely
        for (int i = 0; i < 2 * n; i++) {
            (*azParam)[i] = NULL;
        }

        for (int i = 0; i < n; i++) {
            if (offset >= Size) break;
            size_t lenKey = Data[offset++];
            // Safety check: ensure lenKey is reasonable and fits in remaining buffer
            if (lenKey > 1000 || offset + lenKey > Size) break;
            
            char *key = malloc(lenKey + 1);
            if (!key) break;
            memcpy(key, &Data[offset], lenKey);
            key[lenKey] = '\0';
            offset += lenKey;

            if (offset >= Size) break;
            size_t lenVal = Data[offset++];
            // Safety check: ensure lenVal is reasonable and fits in remaining buffer
            if (lenVal > 1000 || offset + lenVal > Size) break;

            char *val = malloc(lenVal + 1);
            if (!val) { free(key); break; }
            memcpy(val, &Data[offset], lenVal);
            val[lenVal] = '\0';
            offset += lenVal;

            (*azParam)[i * 2] = key;
            (*azParam)[i * 2 + 1] = val;
        }
        
        // Count how many we actually successfully added
        int actual_n = 0;
        for(int i = 0; i < n; i++) {
            if ((*azParam)[i*2] != NULL && (*azParam)[i*2+1] != NULL) {
                actual_n++;
            } else {
                // Clean up partials if any
                if ((*azParam)[i*2]) free((*azParam)[i*2]);
                if ((*azParam)[i*2+1]) free((*azParam)[i*2+1]);
                break;
            }
        }
        *nParam = actual_n;
    }
}

static void cleanup_input(char *zDatabase, char *zJournal, char *zWal, int nParam, char **azParam) {
    if (zDatabase) free(zDatabase);
    if (zJournal) free(zJournal);
    if (zWal) free(zWal);
    if (azParam && nParam > 0) {
        for (int i = 0; i < nParam; i++) {
            if (azParam[i * 2]) free(azParam[i * 2]);
            if (azParam[i * 2 + 1]) free(azParam[i * 2 + 1]);
        }
        free(azParam);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    char *zDatabase = NULL;
    char *zJournal = NULL;
    char *zWal = NULL;
    int nParam = 0;
    char **azParam = NULL;

    parse_fuzz_input(Data, Size, &zDatabase, &zJournal, &zWal, &nParam, &azParam);

    // Ensure non-NULL pointers for sqlite3_create_filename as per requirements
    // If parsing failed or resulted in NULL, use safe defaults
    const char *safeD = zDatabase ? zDatabase : "test.db";
    const char *safeJ = zJournal ? zJournal : "test.db-journal";
    const char *safeW = zWal ? zWal : "test.db-wal";

    // Ensure azParam is NULL if nParam is 0 to satisfy API contract
    const char **safeAzParam = (nParam > 0) ? (const char **)azParam : NULL;

    sqlite3_filename filename = sqlite3_create_filename(safeD, safeJ, safeW, nParam, safeAzParam);

    if (filename) {
        // Test sqlite3_filename_journal
        const char *journal = sqlite3_filename_journal(filename);
        (void)journal; // Use to avoid unused warning, logic is tested

        // Test sqlite3_filename_database
        const char *database = sqlite3_filename_database(filename);
        (void)database;

        // Test sqlite3_uri_key with various indices
        for (int i = -1; i <= nParam + 1; i++) {
            const char *key = sqlite3_uri_key(filename, i);
            (void)key;
        }

        // Test sqlite3_uri_boolean with various parameters
        const char *testParams[] = {"mode", "immutable", "temp_store", "cache_size", "journal_size_limit"};
        int testDefaults[] = {0, 1, 0, 2000, 0};
        for (int i = 0; i < 5; i++) {
            int val = sqlite3_uri_boolean(filename, testParams[i], testDefaults[i]);
            (void)val;
        }

        // Cleanup filename
        sqlite3_free_filename(filename);
    }

    // Cleanup allocated input buffers
    cleanup_input(zDatabase, zJournal, zWal, nParam, azParam);

    return 0;
}