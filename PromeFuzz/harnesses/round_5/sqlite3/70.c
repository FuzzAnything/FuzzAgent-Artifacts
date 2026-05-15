// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:172197:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_file_cleanup(void) {
    remove("./dummy_file");
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    // Create dummy file with fuzzer data
    write_dummy_file(Data, Size);
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    // Execute some SQL to create tables and change states
    sqlite3_exec(db, "CREATE TABLE t1(a, b);", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE TABLE t2(x, y);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2);", NULL, NULL, NULL);
    
    // Use fuzzer data to decide which operations to perform
    uint8_t op = Data[0];
    
    // 1. Test sqlite3_get_autocommit
    int autocommit = sqlite3_get_autocommit(db);
    
    // Begin/commit transactions based on fuzzer data
    if (op & 0x01) {
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        autocommit = sqlite3_get_autocommit(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else if (op & 0x02) {
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        autocommit = sqlite3_get_autocommit(db);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    
    // 2. Test sqlite3_db_readonly
    const char *db_names[] = {"main", "temp", "attached", NULL};
    for (int i = 0; db_names[i] && i < 3; i++) {
        sqlite3_db_readonly(db, db_names[i]);
    }
    
    // Use fuzzer data as database name (limited to valid chars)
    if (Size > 1) {
        char db_name[256];
        size_t name_len = Size > 255 ? 255 : Size;
        memcpy(db_name, Data, name_len);
        db_name[name_len] = '\0';
        sqlite3_db_readonly(db, db_name);
    }
    
    // 3. Test sqlite3_txn_state
    const char *schemas[] = {"main", "temp", NULL};
    for (int i = 0; schemas[i]; i++) {
        sqlite3_txn_state(db, schemas[i]);
    }
    sqlite3_txn_state(db, NULL);
    
    // Use fuzzer data as schema name
    if (Size > 2) {
        char schema_name[256];
        size_t schema_len = (Size - 1) > 255 ? 255 : (Size - 1);
        memcpy(schema_name, Data + 1, schema_len);
        schema_name[schema_len] = '\0';
        sqlite3_txn_state(db, schema_name);
    }
    
    // 4. Test sqlite3_db_config with various operations
    int config_ops[] = {
        SQLITE_DBCONFIG_LOOKASIDE,
        SQLITE_DBCONFIG_ENABLE_FKEY,
        SQLITE_DBCONFIG_ENABLE_TRIGGER,
        SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER,
        SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION,
        SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE,
        SQLITE_DBCONFIG_ENABLE_QPSG,
        SQLITE_DBCONFIG_TRIGGER_EQP,
        SQLITE_DBCONFIG_RESET_DATABASE,
        SQLITE_DBCONFIG_DEFENSIVE,
        SQLITE_DBCONFIG_WRITABLE_SCHEMA,
        SQLITE_DBCONFIG_LEGACY_ALTER_TABLE,
        SQLITE_DBCONFIG_DQS_DML,
        SQLITE_DBCONFIG_DQS_DDL,
        SQLITE_DBCONFIG_ENABLE_VIEW,
        SQLITE_DBCONFIG_LEGACY_FILE_FORMAT,
        SQLITE_DBCONFIG_TRUSTED_SCHEMA
    };
    
    int config_val = (int)Data[Size > 1 ? 1 : 0];
    int *config_out = &config_val;
    
    for (size_t i = 0; i < sizeof(config_ops)/sizeof(config_ops[0]); i++) {
        sqlite3_db_config(db, config_ops[i], config_val, config_out);
    }
    
    // Test lookaside configuration
    void *lookaside_buf[128];
    if (Size > 100) {
        sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, 
                         lookaside_buf, 128, 1200);
    }
    
    // 5. Test sqlite3_db_status with various operations
    int status_ops[] = {
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
        SQLITE_DBSTATUS_CACHE_SPILL
    };
    
    int cur, hiwtr;
    int reset_flag = (op & 0x04) ? 1 : 0;
    
    for (size_t i = 0; i < sizeof(status_ops)/sizeof(status_ops[0]); i++) {
        sqlite3_db_status(db, status_ops[i], &cur, &hiwtr, reset_flag);
    }
    
    // 6. Test sqlite3_file_control
    int file_ops[] = {
        SQLITE_FCNTL_LOCKSTATE,
        SQLITE_FCNTL_GET_LOCKPROXYFILE,
        SQLITE_FCNTL_SET_LOCKPROXYFILE,
        SQLITE_FCNTL_LAST_ERRNO,
        SQLITE_FCNTL_SIZE_HINT,
        SQLITE_FCNTL_CHUNK_SIZE,
        SQLITE_FCNTL_FILE_POINTER,
        SQLITE_FCNTL_SYNC_OMITTED,
        SQLITE_FCNTL_WIN32_AV_RETRY,
        SQLITE_FCNTL_PERSIST_WAL,
        SQLITE_FCNTL_OVERWRITE,
        SQLITE_FCNTL_VFSNAME,
        SQLITE_FCNTL_POWERSAFE_OVERWRITE,
        SQLITE_FCNTL_PRAGMA,
        SQLITE_FCNTL_BUSYHANDLER,
        SQLITE_FCNTL_TEMPFILENAME,
        SQLITE_FCNTL_MMAP_SIZE,
        SQLITE_FCNTL_TRACE,
        SQLITE_FCNTL_HAS_MOVED,
        SQLITE_FCNTL_SYNC,
        SQLITE_FCNTL_COMMIT_PHASETWO,
        SQLITE_FCNTL_WIN32_SET_HANDLE,
        SQLITE_FCNTL_WAL_BLOCK,
        SQLITE_FCNTL_ZIPVFS,
        SQLITE_FCNTL_RBU,
        SQLITE_FCNTL_VFS_POINTER,
        SQLITE_FCNTL_JOURNAL_POINTER,
        SQLITE_FCNTL_WIN32_GET_HANDLE,
        SQLITE_FCNTL_PDB,
        SQLITE_FCNTL_BEGIN_ATOMIC_WRITE,
        SQLITE_FCNTL_COMMIT_ATOMIC_WRITE,
        SQLITE_FCNTL_ROLLBACK_ATOMIC_WRITE,
        SQLITE_FCNTL_LOCK_TIMEOUT,
        SQLITE_FCNTL_DATA_VERSION,
        SQLITE_FCNTL_SIZE_LIMIT,
        SQLITE_FCNTL_CKPT_DONE,
        SQLITE_FCNTL_RESERVE_BYTES,
        SQLITE_FCNTL_CKPT_START,
        SQLITE_FCNTL_EXTERNAL_READER,
        SQLITE_FCNTL_CKSM_FILE,
        SQLITE_FCNTL_RESET_CACHE
    };
    
    void *file_arg = NULL;
    if (Size > 50) {
        file_arg = (void*)(Data + 10);
    }
    
    for (size_t i = 0; i < sizeof(file_ops)/sizeof(file_ops[0]); i++) {
        sqlite3_file_control(db, "main", file_ops[i], file_arg);
        sqlite3_file_control(db, "temp", file_ops[i], file_arg);
    }
    
    // Test with fuzzer-provided database name
    if (Size > 3) {
        char fuzz_db_name[256];
        size_t fuzz_len = (Size - 2) > 255 ? 255 : (Size - 2);
        memcpy(fuzz_db_name, Data + 2, fuzz_len);
        fuzz_db_name[fuzz_len] = '\0';
        sqlite3_file_control(db, fuzz_db_name, SQLITE_FCNTL_LOCKSTATE, NULL);
    }
    
    // Cleanup
    sqlite3_close(db);
    dummy_file_cleanup();
    
    return 0;
}