// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
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

static int setup_database(sqlite3 **db) {
    int rc;
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return rc;
    
    // Enable WAL mode
    rc = sqlite3_exec(*db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    // Create a table and insert some data to generate WAL entries
    rc = sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "INSERT INTO test (data) VALUES ('test1')", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "INSERT INTO test (data) VALUES ('test2')", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    return SQLITE_OK;
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static int test_snapshot_recover(sqlite3 *db, const uint8_t *data, size_t size) {
    // Use data to determine which database name to use
    const char *db_name = "main";
    if (size > 0 && data[0] % 2 == 0) {
        db_name = NULL; // Test with NULL
    }
    
    return sqlite3_snapshot_recover(db, db_name);
}

static int test_wal_checkpoint_v2(sqlite3 *db, const uint8_t *data, size_t size) {
    int pnLog = 0, pnCkpt = 0;
    const char *db_name = "main";
    int eMode = SQLITE_CHECKPOINT_PASSIVE;
    
    if (size > 0) {
        // Use data to select different modes
        switch (data[0] % 5) {
            case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
            case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
            case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
            case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
            case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
        }
        
        // Use data to determine database name
        if (size > 1 && data[1] % 2 == 0) {
            db_name = NULL;
        }
    }
    
    return sqlite3_wal_checkpoint_v2(db, db_name, eMode, &pnLog, &pnCkpt);
}

static int test_wal_checkpoint(sqlite3 *db, const uint8_t *data, size_t size) {
    const char *db_name = "main";
    
    if (size > 0 && data[0] % 2 == 0) {
        db_name = NULL;
    }
    
    return sqlite3_wal_checkpoint(db, db_name);
}

static int test_snapshot_cmp(sqlite3 *db, const uint8_t *data, size_t size) {
    sqlite3_snapshot *snap1 = NULL, *snap2 = NULL;
    int rc1, rc2, cmp_result = 0;
    
    // Begin a transaction to get snapshots
    rc1 = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc1 != SQLITE_OK) return rc1;
    
    // Get first snapshot
    rc1 = sqlite3_snapshot_get(db, "main", &snap1);
    if (rc1 == SQLITE_OK && snap1) {
        // Insert more data to create a different snapshot
        sqlite3_exec(db, "INSERT INTO test (data) VALUES ('snapshot_test')", NULL, NULL, NULL);
        
        // Get second snapshot
        rc2 = sqlite3_snapshot_get(db, "main", &snap2);
        if (rc2 == SQLITE_OK && snap2) {
            cmp_result = sqlite3_snapshot_cmp(snap1, snap2);
        }
    }
    
    // Clean up
    if (snap1) sqlite3_snapshot_free(snap1);
    if (snap2) sqlite3_snapshot_free(snap2);
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
    
    return (rc1 == SQLITE_OK || rc2 == SQLITE_OK) ? SQLITE_OK : rc1;
}

static int test_snapshot_get(sqlite3 *db, const uint8_t *data, size_t size) {
    sqlite3_snapshot *snap = NULL;
    int rc;
    
    // Begin a transaction (required for snapshot_get)
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    // Use data to determine schema name
    const char *schema = "main";
    if (size > 0 && data[0] % 3 == 0) {
        schema = NULL; // Test with NULL
    } else if (size > 0 && data[0] % 3 == 1) {
        schema = "temp"; // Test with temp schema
    }
    
    rc = sqlite3_snapshot_get(db, schema, &snap);
    
    // Clean up
    if (snap) sqlite3_snapshot_free(snap);
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
    
    return rc;
}

static int test_snapshot_open(sqlite3 *db, const uint8_t *data, size_t size) {
    sqlite3_snapshot *snap = NULL;
    int rc;
    
    // Begin a transaction to get snapshot
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    // Get a snapshot first
    rc = sqlite3_snapshot_get(db, "main", &snap);
    if (rc != SQLITE_OK || !snap) {
        sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
        return rc;
    }
    
    // Close the current transaction
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
    
    // Begin a new transaction for snapshot_open
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_snapshot_free(snap);
        return rc;
    }
    
    // Use data to determine schema name
    const char *schema = "main";
    if (size > 0 && data[0] % 3 == 0) {
        schema = NULL;
    } else if (size > 0 && data[0] % 3 == 1) {
        schema = "temp";
    }
    
    // Try to open the snapshot
    rc = sqlite3_snapshot_open(db, schema, snap);
    
    // Clean up
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
    sqlite3_snapshot_free(snap);
    
    return rc;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Setup database
    rc = setup_database(&db);
    if (rc != SQLITE_OK || !db) {
        cleanup_database(db);
        return 0;
    }
    
    // Select which function to test based on input
    int function_to_test = 0;
    if (size > 0) {
        function_to_test = data[0] % 6;
    }
    
    // Execute the selected function
    switch (function_to_test) {
        case 0:
            test_snapshot_recover(db, data, size);
            break;
        case 1:
            test_wal_checkpoint_v2(db, data, size);
            break;
        case 2:
            test_wal_checkpoint(db, data, size);
            break;
        case 3:
            test_snapshot_cmp(db, data, size);
            break;
        case 4:
            test_snapshot_get(db, data, size);
            break;
        case 5:
            test_snapshot_open(db, data, size);
            break;
    }
    
    // Cleanup
    cleanup_database(db);
    
    return 0;
}