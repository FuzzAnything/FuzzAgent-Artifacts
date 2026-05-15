// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    /* Test sqlite3_create_filename and related functions */
    const char *zDatabase = "./dummy_file";
    const char *zJournal = "";
    const char *zWal = "";
    
    /* Create URI parameters from fuzzer data */
    int nParam = 0;
    const char **azParam = NULL;
    
    /* Simple parameter extraction: split on null bytes if present */
    if (Size > 10) {
        /* Use first few bytes to determine number of params (0-4) */
        nParam = Data[0] % 5;
        if (nParam > 0) {
            azParam = (const char **)malloc(2 * nParam * sizeof(const char *));
            if (azParam) {
                size_t offset = 1;
                for (int i = 0; i < 2 * nParam; i++) {
                    if (offset < Size) {
                        /* Use dummy strings for keys/values */
                        static const char *dummy_keys[] = {"key1", "key2", "key3", "key4"};
                        static const char *dummy_values[] = {"val1", "val2", "val3", "val4"};
                        if (i % 2 == 0) {
                            azParam[i] = dummy_keys[i/2 % 4];
                        } else {
                            azParam[i] = dummy_values[i/2 % 4];
                        }
                        offset++;
                    } else {
                        azParam[i] = "";
                    }
                }
            }
        }
    }
    
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    if (filename) {
        /* Test sqlite3_filename_journal */
        const char *journal_result = sqlite3_filename_journal(filename);
        (void)journal_result;
        
        /* Test sqlite3_filename_database */
        const char *db_result = sqlite3_filename_database(filename);
        (void)db_result;
        
        /* Test sqlite3_uri_parameter with various parameter names */
        const char *param_result = sqlite3_uri_parameter(filename, "key1");
        (void)param_result;
        param_result = sqlite3_uri_parameter(filename, "key2");
        (void)param_result;
        param_result = sqlite3_uri_parameter(filename, "");
        (void)param_result;
        
        /* Test sqlite3_database_file_object - note: this expects journal filename
           from xOpen with specific flags. We'll pass various strings but behavior
           is undefined per documentation. */
        if (Size > 20) {
            /* Create a temporary journal-like string */
            char journal_buf[256];
            size_t copy_len = Size < 255 ? Size : 255;
            memcpy(journal_buf, Data, copy_len);
            journal_buf[copy_len] = '\0';
            
            sqlite3_file *file_obj = sqlite3_database_file_object(journal_buf);
            (void)file_obj;
        }
        
        /* Clean up */
        sqlite3_free_filename(filename);
    }
    
    if (azParam) {
        free(azParam);
    }
    
    /* Test sqlite3_free_filename with NULL */
    sqlite3_free_filename(NULL);
    
    /* Additional test: create filename with different combinations */
    if (Size > 50) {
        /* Use different parts of fuzzer data for different components */
        size_t db_len = Size / 3;
        size_t journal_len = Size / 3;
        size_t wal_len = Size - db_len - journal_len;
        
        char *db_str = (char *)malloc(db_len + 1);
        char *journal_str = (char *)malloc(journal_len + 1);
        char *wal_str = (char *)malloc(wal_len + 1);
        
        if (db_str && journal_str && wal_str) {
            memcpy(db_str, Data, db_len);
            db_str[db_len] = '\0';
            memcpy(journal_str, Data + db_len, journal_len);
            journal_str[journal_len] = '\0';
            memcpy(wal_str, Data + db_len + journal_len, wal_len);
            wal_str[wal_len] = '\0';
            
            sqlite3_filename filename2 = sqlite3_create_filename(
                db_str, journal_str, wal_str, 0, NULL);
            
            if (filename2) {
                /* Exercise the functions again */
                const char *result = sqlite3_filename_journal(filename2);
                (void)result;
                result = sqlite3_filename_database(filename2);
                (void)result;
                
                sqlite3_free_filename(filename2);
            }
        }
        
        free(db_str);
        free(journal_str);
        free(wal_str);
    }
    
    remove_dummy_file();
    return 0;
}