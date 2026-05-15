// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
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

static void test_uri_parameter(sqlite3_filename z, const char *zParam) {
    const char *result = sqlite3_uri_parameter(z, zParam);
    (void)result;
}

static void test_filename_database(sqlite3_filename z) {
    const char *result = sqlite3_filename_database(z);
    (void)result;
}

static void test_filename_journal(sqlite3_filename z) {
    const char *result = sqlite3_filename_journal(z);
    (void)result;
}

static void test_database_file_object(const char *z) {
    sqlite3_file *result = sqlite3_database_file_object(z);
    (void)result;
}

static sqlite3_filename create_test_filename(const uint8_t *Data, size_t Size) {
    const char *zDatabase = "./dummy_file";
    const char *zJournal = "";
    const char *zWal = "";
    
    int nParam = 0;
    const char **azParam = NULL;
    
    if (Size > 0) {
        nParam = 1;
        azParam = (const char **)malloc(2 * sizeof(const char *));
        if (azParam) {
            azParam[0] = "key";
            azParam[1] = (const char *)Data;
        }
    }
    
    sqlite3_filename fname = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    if (azParam) {
        free(azParam);
    }
    
    return fname;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    sqlite3_filename fname = create_test_filename(Data, Size);
    
    if (fname) {
        test_uri_parameter(fname, "key");
        test_uri_parameter(fname, "nonexistent");
        test_filename_database(fname);
        test_filename_journal(fname);
        
        test_database_file_object(fname);
        
        sqlite3_free_filename(fname);
    }
    
    test_uri_parameter(NULL, "key");
    test_filename_database(NULL);
    test_filename_journal(NULL);
    test_database_file_object(NULL);
    
    remove("./dummy_file");
    
    return 0;
}