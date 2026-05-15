// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:173268:18 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:173320:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned int autovacuum_callback(void *pArg, const char *zSchema, 
                                        unsigned int nDbPage, unsigned int nFreePage, 
                                        unsigned int nBytePerPage) {
    (void)pArg;
    (void)zSchema;
    (void)nDbPage;
    (void)nBytePerPage;
    return nFreePage;
}

static void autovacuum_destructor(void *pArg) {
    (void)pArg;
}

static void rollback_hook_callback(void *pArg) {
    (void)pArg;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int cur, hiwtr;
    void *prev_rollback_arg;
    unsigned int (*prev_autovac)(void *, const char *, unsigned int, unsigned int, unsigned int);
    void *prev_autovac_arg;
    void (*prev_autovac_destr)(void *);
    
    if (Size < 1) {
        return 0;
    }
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    uint8_t op_selector = Data[0];
    size_t data_index = 1;
    
    for (int i = 0; i < 6 && data_index < Size; i++) {
        uint8_t action = Data[data_index++] % 6;
        
        switch (action) {
            case 0: {
                int op = (data_index < Size) ? Data[data_index++] % 30 : 0;
                int reset = (data_index < Size) ? Data[data_index++] % 2 : 0;
                sqlite3_db_status(db, op, &cur, &hiwtr, reset);
                break;
            }
            
            case 1: {
                int config_op = (data_index < Size) ? Data[data_index++] % 20 : 0;
                int arg1 = (data_index < Size) ? (int)Data[data_index++] : 0;
                int arg2 = (data_index < Size) ? (int)Data[data_index++] : 0;
                sqlite3_db_config(db, config_op, arg1, arg2);
                break;
            }
            
            case 2: {
                void *hook_arg = (void *)(intptr_t)((data_index < Size) ? Data[data_index++] : 0);
                prev_rollback_arg = sqlite3_rollback_hook(db, rollback_hook_callback, hook_arg);
                (void)prev_rollback_arg;
                break;
            }
            
            case 3: {
                void *autovac_arg = (void *)(intptr_t)((data_index < Size) ? Data[data_index++] : 0);
                rc = sqlite3_autovacuum_pages(db, autovacuum_callback, autovac_arg, autovacuum_destructor);
                (void)rc;
                break;
            }
            
            case 4: {
                const char *schema = NULL;
                if (data_index < Size) {
                    uint8_t schema_choice = Data[data_index++] % 4;
                    switch (schema_choice) {
                        case 0: schema = NULL; break;
                        case 1: schema = "main"; break;
                        case 2: schema = "temp"; break;
                        case 3: schema = ""; break;
                    }
                }
                int txn_state = sqlite3_txn_state(db, schema);
                (void)txn_state;
                break;
            }
            
            case 5: {
                if (db) {
                    sqlite3_close(db);
                    db = NULL;
                    rc = sqlite3_open(":memory:", &db);
                    if (rc != SQLITE_OK || db == NULL) {
                        if (db) {
                            sqlite3_close(db);
                        }
                        return 0;
                    }
                }
                break;
            }
        }
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}