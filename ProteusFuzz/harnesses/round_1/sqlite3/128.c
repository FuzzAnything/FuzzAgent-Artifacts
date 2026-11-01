// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int logFrames = -1;
    int ckptFrames = -1;

    if (Size == 0) {
        return 0;
    }

    const char *dbPath = "./dummy_file";

    rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK && rc != SQLITE_CANTOPEN) {
        if (db) {
            sqlite3_close_v2(db);
        }
        return 0;
    }

    if (db == NULL) {
        return 0;
    }

    int enableCode = (Size > 0) ? (Data[0] % 2) : 0;
    rc = sqlite3_extended_result_codes(db, enableCode);
    (void)rc;

    int interrupted = sqlite3_is_interrupted(db);
    (void)interrupted;

    int mode = SQLITE_CHECKPOINT_PASSIVE;
    if (Size > 1) {
        int modeIdx = Data[1] % 5;
        switch (modeIdx) {
            case 0: mode = SQLITE_CHECKPOINT_PASSIVE; break;
            case 1: mode = SQLITE_CHECKPOINT_FULL; break;
            case 2: mode = SQLITE_CHECKPOINT_RESTART; break;
            case 3: mode = SQLITE_CHECKPOINT_TRUNCATE; break;
            case 4: mode = SQLITE_CHECKPOINT_NOOP; break;
        }
    }

    const char *zDb = NULL;
    if (Size > 2) {
        if (Data[2] % 2 == 0) {
            zDb = NULL;
        } else {
            zDb = "main";
        }
    }

    rc = sqlite3_wal_checkpoint_v2(db, zDb, mode, &logFrames, &ckptFrames);
    (void)rc;

    int errCode = sqlite3_errcode(db);
    int extErrCode = sqlite3_extended_errcode(db);
    (void)errCode;
    (void)extErrCode;

    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}