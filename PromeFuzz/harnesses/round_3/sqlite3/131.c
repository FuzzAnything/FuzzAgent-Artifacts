// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_sqlite3_db_status(sqlite3 *db, const uint8_t *data, size_t size) {
    if (!db) return;
    
    int op_codes[] = {
        SQLITE_DBSTATUS_LOOKASIDE_USED,
        SQLITE_DBSTATUS_CACHE_USED,
        SQLITE_DBSTATUS_SCHEMA_USED,
        SQLITE_DBSTATUS_STMT_USED,
        SQLITE_DBSTATUS_LOOKASIDE_HIT,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
        SQLITE_DBSTATUS_CACHE_HIT,
        SQLITE_DBSTATUS_CACHE_MISS,
        SQLITE_DBSTATUS_CACHE_WRITE,
        SQLITE_DBSTATUS_DEFERRED_FKS,
        SQLITE_DBSTATUS_CACHE_USED_SHARED,
        SQLITE_DBSTATUS_CACHE_SPILL,
        SQLITE_DBSTATUS_MAX
    };
    
    int op_count = sizeof(op_codes) / sizeof(op_codes[0]);
    int op_index = 0;
    if (size > 0) {
        op_index = data[0] % op_count;
    }
    
    int current = 0;
    int highwater = 0;
    int reset_flag = 0;
    if (size > 1) {
        reset_flag = data[1] % 2;
    }
    
    sqlite3_db_status(db, op_codes[op_index], &current, &highwater, reset_flag);
}

static void fuzz_sqlite3_db_config(sqlite3 *db, const uint8_t *data, size_t size) {
    if (!db) return;
    
    size_t offset = 0;
    
    if (size - offset >= 4) {
        int op = *((int*)(data + offset));
        offset += 4;
        
        switch (op) {
            case SQLITE_DBCONFIG_LOOKASIDE: {
                if (size - offset >= 8) {
                    int sz = *((int*)(data + offset));
                    offset += 4;
                    int cnt = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, sz, cnt);
                }
                break;
            }
            case SQLITE_DBCONFIG_ENABLE_FKEY: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_ENABLE_TRIGGER: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_ENABLE_VIEW: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_ENABLE_QPSG: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_QPSG, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_TRIGGER_EQP: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_TRIGGER_EQP, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_RESET_DATABASE: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_RESET_DATABASE, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_DEFENSIVE: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_DEFENSIVE, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_WRITABLE_SCHEMA: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_WRITABLE_SCHEMA, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_LEGACY_ALTER_TABLE: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_LEGACY_ALTER_TABLE, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_DQS_DML: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_DQS_DML, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_DQS_DDL: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_DQS_DDL, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_LEGACY_FILE_FORMAT: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_LEGACY_FILE_FORMAT, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_TRUSTED_SCHEMA: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_TRUSTED_SCHEMA, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_STMT_SCANSTATUS: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_STMT_SCANSTATUS, val);
                }
                break;
            }
            case SQLITE_DBCONFIG_REVERSE_SCANORDER: {
                if (size - offset >= 4) {
                    int val = *((int*)(data + offset));
                    offset += 4;
                    sqlite3_db_config(db, SQLITE_DBCONFIG_REVERSE_SCANORDER, val);
                }
                break;
            }
        }
    }
}

int LLVMFuzzerTestOneInput_131(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    const char *filename = "./dummy_file";
    
    if (size > 0) {
        FILE *fp = fopen(filename, "wb");
        if (fp) {
            size_t write_size = size < 1024 ? size : 1024;
            fwrite(data, 1, write_size, fp);
            fclose(fp);
        }
    }
    
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    fuzz_sqlite3_db_status(db, data, size);
    
    fuzz_sqlite3_db_config(db, data, size);
    
    sqlite3_db_release_memory(db);
    
    if (size > 0) {
        int close_type = data[0] % 2;
        if (close_type == 0) {
            sqlite3_close(db);
        } else {
            sqlite3_close_v2(db);
        }
    } else {
        sqlite3_close(db);
    }
    
    return 0;
}