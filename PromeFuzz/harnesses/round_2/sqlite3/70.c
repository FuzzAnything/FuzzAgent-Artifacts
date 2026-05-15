// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    create_dummy_file(Data, Size);
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        const char *db_filename = sqlite3_db_filename(db, "main");
        
        if (db_filename) {
            sqlite3_filename z = (sqlite3_filename)db_filename;
            
            for (int i = -1; i < 10; i++) {
                const char *key = sqlite3_uri_key(z, i);
                (void)key;
            }
            
            const char *journal = sqlite3_filename_journal(z);
            (void)journal;
            
            const char *wal = sqlite3_filename_wal(z);
            (void)wal;
            
            const char *param_names[] = {"mode", "cache", "immutable", "psow", "nolock"};
            for (size_t j = 0; j < sizeof(param_names)/sizeof(param_names[0]); j++) {
                const char *param = sqlite3_uri_parameter(z, param_names[j]);
                (void)param;
                
                sqlite3_int64 int_val = sqlite3_uri_int64(z, param_names[j], 0);
                (void)int_val;
            }
            
            sqlite3_filename created = sqlite3_create_filename(
                "./dummy_file",
                "./dummy_file-journal",
                "./dummy_file-wal",
                0,
                NULL
            );
            
            if (created) {
                const char *test_key = sqlite3_uri_key(created, 0);
                (void)test_key;
                
                sqlite3_free_filename(created);
            }
        }
        
        sqlite3_close(db);
    }
    
    cleanup_dummy_file();
    return 0;
}