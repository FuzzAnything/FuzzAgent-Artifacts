// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dummy_progress_handler(void *arg) {
    (void)arg;
    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";

    // Create a dummy file to ensure sqlite3_open_v2 doesn't fail due to missing file
    FILE *f = fopen(filename, "w");
    if (f) {
        fclose(f);
    }

    // 2. Invoke target functions in the specified order
    // sqlite3_open_v2
    int rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    // sqlite3_progress_handler
    // Use data from input to determine N, ensuring it's positive to enable or negative to disable
    int n_ops = 0;
    if (Size > 0) {
        n_ops = Data[0];
        if (n_ops < 1) n_ops = -1; // Disable if 0
        else if (n_ops > 1000000) n_ops = 1000000; // Cap to avoid extreme values
    }
    sqlite3_progress_handler(db, n_ops, dummy_progress_handler, NULL);

    // sqlite3_limit (1st call)
    // Use data to select limit ID and new value
    int limit_id_1 = (Size > 1) ? (Data[1] % 10) : 0;
    int limit_val_1 = (Size > 2) ? (Data[2] * 100) : 100;
    sqlite3_limit(db, limit_id_1, limit_val_1);

    // sqlite3_limit (2nd call)
    int limit_id_2 = (Size > 3) ? (Data[3] % 10) : 0;
    int limit_val_2 = (Size > 4) ? (Data[4] * 50) : 50;
    sqlite3_limit(db, limit_id_2, limit_val_2);

    // sqlite3_hard_heap_limit64
    sqlite3_int64 heap_limit = 0;
    if (Size > 5) {
        heap_limit = (sqlite3_int64)Data[5] * 1024 * 1024; // MB
        if (heap_limit < 0) heap_limit = -1; // Query current
    }
    sqlite3_hard_heap_limit64(heap_limit);

    // sqlite3_limit (3rd call)
    int limit_id_3 = (Size > 6) ? (Data[6] % 10) : 0;
    int limit_val_3 = (Size > 7) ? (Data[7] * 200) : 200;
    sqlite3_limit(db, limit_id_3, limit_val_3);

    // sqlite3_db_config
    // Use data to select operation code and arguments
    int db_config_op = (Size > 8) ? (Data[8] % 5) : 0;
    int arg1 = (Size > 9) ? Data[9] : 0;
    int arg2 = (Size > 10) ? Data[10] : 0;
    int arg3 = (Size > 11) ? Data[11] : 0;
    
    // sqlite3_db_config expects variable arguments based on op
    // We pass dummy values. Only specific ops (like SQLITE_DBCONFIG_LOOKASIDE) use them meaningfully.
    // For safety, we only pass arguments if the op is likely to need them, or just pass 0.
    // Here we pass 0s for simplicity as the fuzzer explores states via the op code primarily.
    sqlite3_db_config(db, db_config_op, arg1, arg2, arg3);

    // 3. Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    // Shutdown library
    sqlite3_shutdown();

    // Remove dummy file
    remove(filename);

    return 0;
}