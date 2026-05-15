// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    // Create a database connection for testing
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Parse input data to create parameters for sqlite3_create_filename
    // Format: [db_len][journal_len][wal_len][param_count][data...]
    size_t offset = 0;
    
    if (offset + 3 > Size) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use first 3 bytes as lengths (clamped to reasonable values)
    uint8_t db_len = Data[offset] % 64;
    uint8_t journal_len = Data[offset + 1] % 64;
    uint8_t wal_len = Data[offset + 2] % 64;
    offset += 3;
    
    // Get parameter count
    if (offset >= Size) {
        sqlite3_close(db);
        return 0;
    }
    uint8_t param_count = Data[offset] % 8;  // Limit to 8 params max
    offset++;
    
    // Extract database name
    const char *zDatabase = "";
    if (db_len > 0 && offset + db_len <= Size) {
        char *db_buf = malloc(db_len + 1);
        if (db_buf) {
            memcpy(db_buf, Data + offset, db_len);
            db_buf[db_len] = '\0';
            zDatabase = db_buf;
            offset += db_len;
        }
    }
    
    // Extract journal name
    const char *zJournal = "";
    if (journal_len > 0 && offset + journal_len <= Size) {
        char *journal_buf = malloc(journal_len + 1);
        if (journal_buf) {
            memcpy(journal_buf, Data + offset, journal_len);
            journal_buf[journal_len] = '\0';
            zJournal = journal_buf;
            offset += journal_len;
        }
    }
    
    // Extract WAL name
    const char *zWal = "";
    if (wal_len > 0 && offset + wal_len <= Size) {
        char *wal_buf = malloc(wal_len + 1);
        if (wal_buf) {
            memcpy(wal_buf, Data + offset, wal_len);
            wal_buf[wal_len] = '\0';
            zWal = wal_buf;
            offset += wal_len;
        }
    }
    
    // Create parameter array
    const char **azParam = NULL;
    int nParam = 0;
    
    if (param_count > 0 && offset < Size) {
        // Allocate array for 2*param_count pointers
        azParam = malloc(2 * param_count * sizeof(const char *));
        if (azParam) {
            for (int i = 0; i < param_count && offset < Size; i++) {
                // Key
                uint8_t key_len = Data[offset] % 32;
                offset++;
                if (key_len > 0 && offset + key_len <= Size) {
                    char *key_buf = malloc(key_len + 1);
                    if (key_buf) {
                        memcpy(key_buf, Data + offset, key_len);
                        key_buf[key_len] = '\0';
                        azParam[2 * nParam] = key_buf;
                        offset += key_len;
                    } else {
                        azParam[2 * nParam] = "key";
                    }
                } else {
                    azParam[2 * nParam] = "key";
                }
                
                // Value
                uint8_t val_len = 0;
                if (offset < Size) {
                    val_len = Data[offset] % 32;
                    offset++;
                }
                if (val_len > 0 && offset + val_len <= Size) {
                    char *val_buf = malloc(val_len + 1);
                    if (val_buf) {
                        memcpy(val_buf, Data + offset, val_len);
                        val_buf[val_len] = '\0';
                        azParam[2 * nParam + 1] = val_buf;
                        offset += val_len;
                    } else {
                        azParam[2 * nParam + 1] = "value";
                    }
                } else {
                    azParam[2 * nParam + 1] = "value";
                }
                
                nParam++;
            }
        }
    }
    
    // Test 1: sqlite3_create_filename
    sqlite3_filename fname = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    if (fname) {
        // Test 2: sqlite3_uri_key
        for (int i = -2; i < nParam + 2; i++) {
            const char *key = sqlite3_uri_key(fname, i);
            (void)key; // Use result to avoid unused variable warning
        }
        
        // Test 3: sqlite3_uri_boolean
        if (nParam > 0) {
            for (int i = 0; i < nParam; i++) {
                const char *param = azParam[2 * i];
                int bDefault = i % 2;
                int result = sqlite3_uri_boolean(fname, param, bDefault);
                (void)result;
            }
        }
        
        // Test 4: sqlite3_uri_parameter
        if (nParam > 0) {
            for (int i = 0; i < nParam; i++) {
                const char *param = azParam[2 * i];
                const char *value = sqlite3_uri_parameter(fname, param);
                (void)value;
            }
        }
        
        // Test 5: sqlite3_db_filename (with our open database)
        sqlite3_filename db_fname = sqlite3_db_filename(db, "main");
        if (db_fname) {
            // Test uri functions on the database filename too
            if (nParam > 0) {
                for (int i = 0; i < nParam; i++) {
                    const char *param = azParam[2 * i];
                    const char *value = sqlite3_uri_parameter(db_fname, param);
                    (void)value;
                    
                    int bDefault = i % 2;
                    int bool_result = sqlite3_uri_boolean(db_fname, param, bDefault);
                    (void)bool_result;
                }
            }
            
            // Test uri_key on database filename
            for (int i = -1; i < 3; i++) {
                const char *key = sqlite3_uri_key(db_fname, i);
                (void)key;
            }
        }
        
        // Test 6: sqlite3_free_filename
        sqlite3_free_filename(fname);
    }
    
    // Cleanup
    if (azParam) {
        for (int i = 0; i < nParam; i++) {
            if (azParam[2 * i] && azParam[2 * i] != "key") {
                free((void *)azParam[2 * i]);
            }
            if (azParam[2 * i + 1] && azParam[2 * i + 1] != "value") {
                free((void *)azParam[2 * i + 1]);
            }
        }
        free(azParam);
    }
    
    if (zDatabase && zDatabase[0] != '\0') free((void *)zDatabase);
    if (zJournal && zJournal[0] != '\0') free((void *)zJournal);
    if (zWal && zWal[0] != '\0') free((void *)zWal);
    
    sqlite3_close(db);
    
    return 0;
}