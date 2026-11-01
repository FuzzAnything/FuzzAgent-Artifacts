// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int fuzz_sqlite_sequence(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql;
    size_t sql_len;

    /* 1. Prepare environment: Open database */
    /* Use in-memory database to avoid file I/O issues during fuzzing */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* 2. Set busy timeout */
    /* Use fuzz data to determine timeout, clamping to reasonable range */
    int timeout = 0;
    if (Size >= 1) {
        timeout = Data[0] * 10; /* 0 to 2550 ms */
    }
    sqlite3_busy_timeout(db, timeout);

    /* 3. Prepare a statement using fuzz data as SQL */
    /* Ensure null termination and reasonable length */
    if (Size > 0) {
        /* Limit SQL length to prevent huge allocations */
        sql_len = (Size > 4096) ? 4096 : Size;
        
        /* Create a buffer with null termination */
        char *sql_buf = (char *)malloc(sql_len + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, sql_len);
            sql_buf[sql_len] = '\0';
            sql = sql_buf;

            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            
            /* 4. Execute statement if prepared successfully */
            if (rc == SQLITE_OK && stmt) {
                /* Step through results */
                rc = sqlite3_step(stmt);
                
                /* 5. Access column data if a row is returned */
                if (rc == SQLITE_ROW) {
                    /* Try to read column 0 as integer */
                    (void)sqlite3_column_int(stmt, 0);
                    
                    /* Try to read column 1 if it exists (fuzzing state exploration) */
                    int col_count = sqlite3_column_count(stmt);
                    if (col_count > 1) {
                        (void)sqlite3_column_int(stmt, 1);
                    }
                }
            }
            free(sql_buf);
        }
    }

    /* 6. Finalize statement */
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    /* 7. Sleep (using fuzz data for duration, clamped) */
    int sleep_ms = 0;
    if (Size >= 2) {
        sleep_ms = Data[1]; /* 0 to 255 ms */
    }
    sqlite3_sleep(sleep_ms);

    /* 8. Close database */
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_sqlite_sequence(Data, Size);
    
    return 0;
}