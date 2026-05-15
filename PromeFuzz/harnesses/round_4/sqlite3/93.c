// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
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

static void initialize_database(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    // Create a table and insert some data to make the database non-empty
    sqlite3_exec(*db, "CREATE TABLE test(id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, NULL);
    sqlite3_exec(*db, "INSERT INTO test(data) VALUES('test data');", NULL, NULL, NULL);
    
    // Attach another database to have multiple databases
    sqlite3_exec(*db, "ATTACH DATABASE ':memory:' AS attached_db;", NULL, NULL, NULL);
    sqlite3_exec(*db, "CREATE TABLE attached_db.test2(id INTEGER);", NULL, NULL, NULL);
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static const char* get_db_name(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return "main";
    
    uint8_t choice = data[*offset] % 5;
    (*offset)++;
    
    switch (choice) {
        case 0: return "main";
        case 1: return "temp";
        case 2: return "attached_db";
        case 3: return NULL;
        default: return "invalid_db_name";
    }
}

static int get_opcode(const uint8_t *data, size_t size, size_t *offset, int max_op) {
    if (*offset >= size) return 0;
    int op = data[*offset] % max_op;
    (*offset)++;
    return op;
}

static int get_bool(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    int val = data[*offset] % 2;
    (*offset)++;
    return val;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    size_t offset = 0;
    
    // Initialize database
    initialize_database(&db);
    if (!db) return 0;
    
    // 1. Test sqlite3_db_readonly
    const char *db_name1 = get_db_name(Data, Size, &offset);
    sqlite3_db_readonly(db, db_name1);
    
    // 2. Test sqlite3_enable_load_extension
    int onoff = get_bool(Data, Size, &offset);
    sqlite3_enable_load_extension(db, onoff);
    
    // 3. Test sqlite3_db_status with various opcodes
    int op_status = get_opcode(Data, Size, &offset, 20); // SQLITE_DBSTATUS options
    int cur = 0, hiwtr = 0;
    int reset_flag = get_bool(Data, Size, &offset);
    sqlite3_db_status(db, op_status, &cur, &hiwtr, reset_flag);
    
    // 4. Test sqlite3_wal_checkpoint_v2
    const char *db_name2 = get_db_name(Data, Size, &offset);
    int mode = get_opcode(Data, Size, &offset, 5); // 5 checkpoint modes
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, db_name2, mode, &pnLog, &pnCkpt);
    
    // 5. Test sqlite3_db_config with various opcodes
    int op_config = get_opcode(Data, Size, &offset, 20); // SQLITE_DBCONFIG options
    
    // Try different configurations based on opcode
    switch (op_config) {
        case 0: // SQLITE_DBCONFIG_LOOKASIDE
            if (offset + 8 <= Size) {
                void *pBuf = malloc(128);
                int sz = 128;
                int cnt = 4;
                sqlite3_db_config(db, op_config, pBuf, sz, cnt);
                free(pBuf);
            }
            break;
        case 1: // SQLITE_DBCONFIG_ENABLE_FKEY
        case 2: // SQLITE_DBCONFIG_ENABLE_TRIGGER
        case 3: // SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER
        case 4: // SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION
            {
                int val = get_bool(Data, Size, &offset);
                sqlite3_db_config(db, op_config, val, NULL);
            }
            break;
        default:
            // Try with a single integer argument
            if (offset < Size) {
                int arg = Data[offset] % 256;
                offset++;
                sqlite3_db_config(db, op_config, arg, NULL);
            }
            break;
    }
    
    // 6. Test sqlite3_file_control
    const char *db_name3 = get_db_name(Data, Size, &offset);
    int file_op = get_opcode(Data, Size, &offset, 50); // Various file control opcodes
    
    // Prepare argument based on opcode
    void *arg = NULL;
    if (offset + 4 <= Size) {
        int int_arg = 0;
        memcpy(&int_arg, Data + offset, 4);
        offset += 4;
        arg = (void*)(intptr_t)int_arg;
    }
    
    sqlite3_file_control(db, db_name3, file_op, arg);
    
    // Cleanup
    cleanup_database(db);
    
    return 0;
}