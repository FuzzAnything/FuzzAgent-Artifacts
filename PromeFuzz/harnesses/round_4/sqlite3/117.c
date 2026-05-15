// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

static void create_test_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    // Enable WAL mode
    sqlite3_exec(*db, "PRAGMA journal_mode=WAL", callback, 0, 0);
    
    // Create a table and insert some data to generate WAL entries
    sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT)", callback, 0, 0);
    sqlite3_exec(*db, "INSERT INTO test (data) VALUES ('test1')", callback, 0, 0);
    sqlite3_exec(*db, "INSERT INTO test (data) VALUES ('test2')", callback, 0, 0);
}

static void fuzz_snapshot_recover(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    create_test_db(&db);
    if (!db) return;
    
    // Use fuzz data to determine database name
    const char *zDb = "main";
    if (Size > 0) {
        // Use first byte to choose between main, temp, or attached db
        switch (Data[0] % 3) {
            case 0: zDb = "main"; break;
            case 1: zDb = "temp"; break;
            case 2: zDb = NULL; break; // Test NULL parameter
        }
    }
    
    sqlite3_snapshot_recover(db, zDb);
    
    sqlite3_close(db);
}

static void fuzz_wal_checkpoint_v2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    create_test_db(&db);
    if (!db) return;
    
    int pnLog = 0, pnCkpt = 0;
    const char *zDb = "main";
    int eMode = SQLITE_CHECKPOINT_PASSIVE;
    
    if (Size > 0) {
        // Use fuzz data to determine parameters
        switch (Data[0] % 5) {
            case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
            case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
            case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
            case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
            case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
        }
        
        if (Size > 1) {
            switch (Data[1] % 3) {
                case 0: zDb = "main"; break;
                case 1: zDb = "temp"; break;
                case 2: zDb = NULL; break;
            }
        }
    }
    
    sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
    
    sqlite3_close(db);
}

static void fuzz_wal_checkpoint(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    create_test_db(&db);
    if (!db) return;
    
    const char *zDb = "main";
    if (Size > 0) {
        switch (Data[0] % 3) {
            case 0: zDb = "main"; break;
            case 1: zDb = "temp"; break;
            case 2: zDb = NULL; break;
        }
    }
    
    sqlite3_wal_checkpoint(db, zDb);
    
    sqlite3_close(db);
}

static void fuzz_snapshot_cmp(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    create_test_db(&db);
    if (!db) return;
    
    // Begin a transaction to take snapshots
    sqlite3_exec(db, "BEGIN TRANSACTION", callback, 0, 0);
    
    sqlite3_snapshot *p1 = NULL, *p2 = NULL;
    
    // Try to get two snapshots
    int rc1 = sqlite3_snapshot_get(db, "main", &p1);
    int rc2 = sqlite3_snapshot_get(db, "main", &p2);
    
    if (rc1 == SQLITE_OK && rc2 == SQLITE_OK && p1 && p2) {
        // Compare snapshots
        sqlite3_snapshot_cmp(p1, p2);
        
        // Free snapshots
        sqlite3_snapshot_free(p1);
        sqlite3_snapshot_free(p2);
    } else {
        if (p1) sqlite3_snapshot_free(p1);
        if (p2) sqlite3_snapshot_free(p2);
    }
    
    sqlite3_exec(db, "COMMIT", callback, 0, 0);
    sqlite3_close(db);
}

static void fuzz_snapshot_get(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    create_test_db(&db);
    if (!db) return;
    
    // Begin a transaction (required for snapshot_get)
    sqlite3_exec(db, "BEGIN TRANSACTION", callback, 0, 0);
    
    const char *zSchema = "main";
    if (Size > 0) {
        switch (Data[0] % 3) {
            case 0: zSchema = "main"; break;
            case 1: zSchema = "temp"; break;
            case 2: zSchema = NULL; break;
        }
    }
    
    sqlite3_snapshot *ppSnapshot = NULL;
    sqlite3_snapshot_get(db, zSchema, &ppSnapshot);
    
    if (ppSnapshot) {
        sqlite3_snapshot_free(ppSnapshot);
    }
    
    sqlite3_exec(db, "COMMIT", callback, 0, 0);
    sqlite3_close(db);
}

static void fuzz_snapshot_open(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    create_test_db(&db);
    if (!db) return;
    
    // Begin a transaction to take a snapshot first
    sqlite3_exec(db, "BEGIN TRANSACTION", callback, 0, 0);
    
    sqlite3_snapshot *ppSnapshot = NULL;
    int rc = sqlite3_snapshot_get(db, "main", &ppSnapshot);
    
    if (rc == SQLITE_OK && ppSnapshot) {
        // Try to open a read transaction on the snapshot
        sqlite3_snapshot_open(db, "main", ppSnapshot);
        
        sqlite3_snapshot_free(ppSnapshot);
    }
    
    sqlite3_exec(db, "COMMIT", callback, 0, 0);
    sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to determine which function to fuzz
    switch (Data[0] % 6) {
        case 0:
            fuzz_snapshot_recover(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 1:
            fuzz_wal_checkpoint_v2(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 2:
            fuzz_wal_checkpoint(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 3:
            fuzz_snapshot_cmp(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 4:
            fuzz_snapshot_get(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 5:
            fuzz_snapshot_open(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
    }
    
    return 0;
}