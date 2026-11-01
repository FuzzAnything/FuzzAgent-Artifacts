// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_interrupt
 * - sqlite3_column_type
 * - sqlite3_reset
 * - sqlite3_prepare_v2
 * - sqlite3_db_handle
 * - sqlite3_column_int64
 */

/* Helper to write data to dummy file if needed */
static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    if (size > 0) {
        if (write(fd, data, size) != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

/* Helper to generate a simple SQL query from input data */
static const char *generate_sql_query(const uint8_t *data, size_t size) {
    static char sql_buf[256];
    size_t len = (size > 250) ? 250 : size;
    
    /* Ensure the SQL is valid by constructing a simple SELECT or CREATE */
    if (len == 0) {
        strcpy(sql_buf, "SELECT 1");
        return sql_buf;
    }

    /* Copy data but ensure it's null-terminated and safe-ish */
    memcpy(sql_buf, data, len);
    sql_buf[len] = '\0';

    /* If it looks like garbage, prepend a valid command */
    if (len < 5 || (sql_buf[0] != 'S' && sql_buf[0] != 's' && 
                    sql_buf[0] != 'C' && sql_buf[0] != 'c' &&
                    sql_buf[0] != 'D' && sql_buf[0] != 'd')) {
        /* Prepend a valid SELECT to make it more likely to parse */
        memmove(sql_buf + 8, sql_buf, len + 1);
        memcpy(sql_buf, "SELECT 1", 8);
    }
    
    return sql_buf;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;

    /* Initialize database in memory to avoid file I/O issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Optionally write input to dummy file for VFS testing if needed */
    if (Size > 0) {
        write_dummy_file(Data, Size);
    }

    /* Generate a SQL query from input */
    const char *sql = generate_sql_query(Data, Size);
    
    /* Fuzz sqlite3_prepare_v2 */
    rc = sqlite3_prepare_v2(db, sql, (int)strlen(sql), &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        /* Fuzz sqlite3_step to get to a row */
        rc = sqlite3_step(stmt);
        
        if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
            /* Fuzz sqlite3_column_type */
            int col_type = sqlite3_column_type(stmt, 0);
            (void)col_type; /* Suppress unused warning */
            
            /* Try invalid column indices */
            if (Size > 0) {
                int col_idx = Data[0] % 10;
                col_type = sqlite3_column_type(stmt, col_idx);
                (void)col_type;
                
                col_idx = -(Data[0] % 10);
                col_type = sqlite3_column_type(stmt, col_idx);
                (void)col_type;
            }

            /* Fuzz sqlite3_column_int64 */
            sqlite3_int64 int_val = sqlite3_column_int64(stmt, 0);
            (void)int_val;
            
            if (Size > 0) {
                int col_idx = Data[0] % 10;
                int_val = sqlite3_column_int64(stmt, col_idx);
                (void)int_val;
            }
        }

        /* Fuzz sqlite3_reset */
        rc = sqlite3_reset(stmt);
        (void)rc;
        
        /* Reset again to test state */
        rc = sqlite3_reset(stmt);
        (void)rc;

        /* Fuzz sqlite3_db_handle */
        sqlite3 *handle = sqlite3_db_handle(stmt);
        if (handle != db) {
            /* This should not happen normally, but we test it */
        }
        (void)handle;

        /* Fuzz sqlite3_interrupt on an active statement */
        sqlite3_interrupt(db);
        
        /* Try stepping again after interrupt */
        rc = sqlite3_step(stmt);
        (void)rc;

        /* Reset and step again */
        rc = sqlite3_reset(stmt);
        (void)rc;
        rc = sqlite3_step(stmt);
        (void)rc;
        
        /* Interrupt again when no statement is running (no-op case) */
        sqlite3_interrupt(db);
    } else {
        /* Even if prepare fails, test sqlite3_interrupt on the connection */
        sqlite3_interrupt(db);
        
        /* Try to prepare with invalid SQL (garbage) */
        if (Size > 0) {
            char garbage[64];
            memset(garbage, 0, sizeof(garbage));
            memcpy(garbage, Data, Size < 63 ? Size : 63);
            sqlite3_prepare_v2(db, garbage, (int)strlen(garbage), &stmt, &tail);
            if (stmt != NULL) {
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }

    /* Cleanup */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}