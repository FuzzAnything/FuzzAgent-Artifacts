// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Initialize database connection
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Fuzz sqlite3_extended_result_codes
    if (Size > 0) {
        int onoff = Data[0] % 2;
        rc = sqlite3_extended_result_codes(db, onoff);
        (void)rc;
    }

    // Trigger an error to test error code retrieval
    if (Size > 1) {
        const char *sql = "SELECT invalid_column FROM non_existent_table;";
        sqlite3_exec(db, sql, NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
    }

    if (db) {
        rc = sqlite3_errcode(db);
        (void)rc;
        rc = sqlite3_extended_errcode(db);
        (void)rc;
    }

    // Fuzz sqlite3_errstr with various codes derived from input
    if (Size > 2) {
        int code = (int)(Data[1] << 8) | Data[2];
        const char *str = sqlite3_errstr(code);
        (void)str;
    }

    // Fuzz sqlite3_log
    if (Size >= 10) {
        char fmt_buf[32];
        size_t fmt_len = (Size < 30) ? Size : 30;
        memcpy(fmt_buf, Data + 3, fmt_len);
        fmt_buf[fmt_len] = '\0';
        
        int err_code = (int)Data[3];
        sqlite3_log(err_code, fmt_buf, (int)Data[4], (int)Data[5]);
    }

    // Fuzz sqlite3_load_extension
    sqlite3_enable_load_extension(db, 1);

    // Create a dummy file
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    if (Size > 0) {
        const char *zFile = "./dummy_file";
        const char *zProc = NULL;
        
        if (Size > 10) {
            char proc_buf[20];
            size_t proc_len = (Size < 18) ? Size : 18;
            memcpy(proc_buf, Data + 10, proc_len);
            proc_buf[proc_len] = '\0';
            zProc = proc_buf;
        }

        rc = sqlite3_load_extension(db, zFile, zProc, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
    }

    // Cleanup
    sqlite3_close(db);
    return 0;
}