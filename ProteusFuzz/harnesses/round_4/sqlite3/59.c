// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Helper to safely open a database for fuzzing */
static sqlite3 *fuzz_open_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/* Helper to trigger an error on the database to test error retrieval functions */
static int fuzz_trigger_error(sqlite3 *db) {
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, "SELECT invalid_column FROM non_existent_table;", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    return rc;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Use the first byte to determine the test scenario */
    int scenario = Data[0] % 5;
    const char *db_file = "./dummy_file";

    /* Initialize database connection */
    sqlite3 *db = fuzz_open_db(db_file);
    if (!db) {
        /* If we can't open the DB, we can still test sqlite3_errstr with static codes */
        const char *str1 = sqlite3_errstr(SQLITE_OK);
        const char *str2 = sqlite3_errstr(SQLITE_ERROR);
        const char *str3 = sqlite3_errstr(-99999);
        (void)str1; (void)str2; (void)str3;
        return 0;
    }

    /* Ensure extended result codes are enabled for some scenarios */
    sqlite3_extended_result_codes(db, 1);

    switch (scenario) {
        case 0:
            /* Test sqlite3_errcode and sqlite3_extended_errcode after an error */
            fuzz_trigger_error(db);
            int code_0 = sqlite3_errcode(db);
            int ext_code_0 = sqlite3_extended_errcode(db);
            (void)code_0; (void)ext_code_0;
            break;

        case 1:
            /* Test sqlite3_errstr with various codes derived from input */
            if (Size >= 3) {
                int code_val = ((int)Data[1] << 8) | (int)Data[2];
                const char *msg = sqlite3_errstr(code_val);
                (void)msg;
            } else if (Size == 2) {
                int code_val = (int)Data[1];
                const char *msg = sqlite3_errstr(code_val);
                (void)msg;
            }
            break;

        case 2:
            /* Test sqlite3_extended_result_codes toggling */
            sqlite3_extended_result_codes(db, 0);
            sqlite3_extended_result_codes(db, 1);
            fuzz_trigger_error(db);
            int ext_code_2 = sqlite3_extended_errcode(db);
            (void)ext_code_2;
            break;

        case 3:
            /* Test sqlite3_load_extension (expected to fail as no real extension exists) */
            char *err_msg = NULL;
            /* Use input data to form a fake filename */
            char fake_file[256];
            size_t len = Size < 255 ? Size : 255;
            memcpy(fake_file, Data, len);
            fake_file[len] = '\0';
            
            int load_rc = sqlite3_load_extension(db, fake_file, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            (void)load_rc;
            break;

        case 4:
            /* Test sqlite3_log */
            /* Ensure format string is valid */
            sqlite3_log(SQLITE_ERROR, "Fuzz test log message: %s", "test");
            break;
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}