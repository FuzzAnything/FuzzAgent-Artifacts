// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    const char *filename = "./dummy_file";
    
    write_dummy_file(Data, Size);
    
    rc = sqlite3_open(filename, &db);
    if (rc == SQLITE_OK && db != NULL) {
        sqlite3_db_release_memory(db);
        
        int cur = 0, hiwtr = 0;
        int reset_flag = (int)(Data[0] & 1);
        
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur, &hiwtr, reset_flag);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &cur, &hiwtr, reset_flag);
        sqlite3_db_status(db, SQLITE_DBSTATUS_SCHEMA_USED, &cur, &hiwtr, reset_flag);
        
        int op = (int)(Data[0] % 16);
        switch (op) {
            case 0:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &rc);
                break;
            case 1:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 0, &rc);
                break;
            case 2:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 1, &rc);
                break;
            case 3:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 0, &rc);
                break;
            case 4:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW, 1, &rc);
                break;
            case 5:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW, 0, &rc);
                break;
            default:
                break;
        }
        
        sqlite3_close(db);
        db = NULL;
    }
    
    rc = sqlite3_open(":memory:", &db2);
    if (rc == SQLITE_OK && db2 != NULL) {
        sqlite3_db_release_memory(db2);
        
        int cur = 0, hiwtr = 0;
        int reset_flag = (int)(Data[Size > 1 ? 1 : 0] & 1);
        
        sqlite3_db_status(db2, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur, &hiwtr, reset_flag);
        sqlite3_db_status(db2, SQLITE_DBSTATUS_CACHE_USED, &cur, &hiwtr, reset_flag);
        
        sqlite3_close_v2(db2);
        db2 = NULL;
    }
    
    sqlite3 *db3 = NULL;
    rc = sqlite3_open(NULL, &db3);
    if (rc == SQLITE_OK && db3 != NULL) {
        sqlite3_close(db3);
    }
    
    sqlite3 *db4 = NULL;
    rc = sqlite3_open("", &db4);
    if (rc == SQLITE_OK && db4 != NULL) {
        sqlite3_close(db4);
    }
    
    remove_dummy_file();
    
    return 0;
}