// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int create_test_database(sqlite3 **db) {
    int rc = sqlite3_open("./dummy_file", db);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS t1(x INTEGER);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "INSERT INTO t1 VALUES(1);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    return SQLITE_OK;
}

static void fuzz_snapshot_cmp(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap1 = NULL;
    sqlite3_snapshot *snap2 = NULL;
    
    if (create_test_database(&db) != SQLITE_OK) goto cleanup;
    
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) goto cleanup;
    
    if (sqlite3_snapshot_get(db, "main", &snap1) != SQLITE_OK) goto cleanup;
    
    if (size > 0) {
        char sql[256];
        snprintf(sql, sizeof(sql), "INSERT INTO t1 VALUES(%d);", (int)data[0]);
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }
    
    if (sqlite3_snapshot_get(db, "main", &snap2) != SQLITE_OK) goto cleanup;
    
    if (snap1 && snap2) {
        sqlite3_snapshot_cmp(snap1, snap2);
    }
    
cleanup:
    if (snap1) sqlite3_snapshot_free(snap1);
    if (snap2) sqlite3_snapshot_free(snap2);
    if (db) {
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        sqlite3_close(db);
    }
}

static void fuzz_snapshot_open(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap = NULL;
    
    if (create_test_database(&db) != SQLITE_OK) goto cleanup;
    
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) goto cleanup;
    
    if (sqlite3_snapshot_get(db, "main", &snap) != SQLITE_OK) goto cleanup;
    
    if (snap) {
        sqlite3_snapshot_open(db, "main", snap);
    }
    
cleanup:
    if (snap) sqlite3_snapshot_free(snap);
    if (db) {
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        sqlite3_close(db);
    }
}

static void fuzz_wal_checkpoint_v2(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int pnLog = 0, pnCkpt = 0;
    int mode = SQLITE_CHECKPOINT_PASSIVE;
    
    if (size > 0) {
        mode = data[0] % 5;
    }
    
    if (create_test_database(&db) != SQLITE_OK) goto cleanup;
    
    sqlite3_wal_checkpoint_v2(db, "main", mode, &pnLog, &pnCkpt);
    
cleanup:
    if (db) sqlite3_close(db);
}

static void fuzz_snapshot_recover(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    
    if (create_test_database(&db) != SQLITE_OK) goto cleanup;
    
    sqlite3_snapshot_recover(db, "main");
    
cleanup:
    if (db) sqlite3_close(db);
}

static void fuzz_snapshot_get(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap = NULL;
    
    if (create_test_database(&db) != SQLITE_OK) goto cleanup;
    
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) goto cleanup;
    
    sqlite3_snapshot_get(db, "main", &snap);
    
cleanup:
    if (snap) sqlite3_snapshot_free(snap);
    if (db) {
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        sqlite3_close(db);
    }
}

static void fuzz_wal_checkpoint(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    
    if (create_test_database(&db) != SQLITE_OK) goto cleanup;
    
    sqlite3_wal_checkpoint(db, "main");
    
cleanup:
    if (db) sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uint8_t selector = data[0] % 6;
    const uint8_t *fuzz_data = size > 1 ? data + 1 : data;
    size_t fuzz_size = size > 1 ? size - 1 : 1;
    
    switch (selector) {
        case 0:
            fuzz_snapshot_cmp(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_snapshot_open(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_wal_checkpoint_v2(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_snapshot_recover(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_snapshot_get(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_wal_checkpoint(fuzz_data, fuzz_size);
            break;
    }
    
    remove("./dummy_file");
    return 0;
}