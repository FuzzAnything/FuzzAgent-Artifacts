// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:95598:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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

/*
 * Fuzz driver for sqlite3_value_* functions.
 * 
 * Strategy:
 * 1. Create an in-memory database.
 * 2. Prepare a statement with bound parameters to ensure we have 
 *    sqlite3_value objects that might have the MEM_FromBind flag set.
 * 3. Execute the statement to populate the result set.
 * 4. Iterate through columns and invoke the target API functions.
 * 5. Use the fuzz input data to construct SQL statements and bind values.
 */

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)azColName;
    
    // This callback is not used for the specific value inspection logic
    // as we use sqlite3_column_value directly on the prepared statement.
    return 0;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to ensure we can execute queries
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT, val REAL);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Use input data to construct a dynamic SQL query.
    // We ensure it's a valid query by using a safe pattern or limiting length.
    // We will try to insert data first to have something to query, or just query the table.
    
    // Limit SQL size to prevent excessive memory usage or stack overflow
    size_t sql_len = Size < 256 ? Size : 256;
    
    // Construct a safe SQL statement using the input data as a literal or parameter
    // We'll use a prepared statement with binding to exercise the "frombind" logic.
    
    // Attempt to prepare a statement that selects from our table
    // We construct a query that might use the input data as a WHERE clause value
    // to ensure we get a result set with values.
    
    // Create a buffer for the SQL string
    char *sql_buf = malloc(sql_len + 100);
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }

    // Safe SQL construction: SELECT * FROM test_table WHERE id = ?
    // We will bind the first few bytes of input as the parameter.
    // This ensures we have a statement with bound parameters.
    snprintf(sql_buf, sql_len + 100, "SELECT * FROM test_table WHERE id = ?");
    sql = sql_buf;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // If prepare fails, try a simple SELECT without WHERE to ensure we get rows
        snprintf(sql_buf, sql_len + 100, "SELECT 1, 'fuzz_data', 3.14");
        sql = sql_buf;
        sqlite3_finalize(stmt);
        stmt = NULL;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            free(sql_buf);
            sqlite3_close(db);
            return 0;
        }
    }

    // Bind a value from the fuzz input to exercise sqlite3_value_frombind
    // We bind the first byte as an integer
    if (Size > 0) {
        sqlite3_bind_int(stmt, 1, (int)Data[0]);
    }

    // Step through the result set
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        
        for (int i = 0; i < col_count; i++) {
            sqlite3_value *val = sqlite3_column_value(stmt, i);
            
            if (val == NULL) continue;

            // 1. sqlite3_value_type
            int type = sqlite3_value_type(val);
            (void)type; // Use the result to avoid unused warning

            // 2. sqlite3_value_frombind
            // This checks if the value came from a bound parameter.
            // In our "SELECT 1, 'fuzz_data', 3.14" case, these are literals, so it returns 0.
            // In the "SELECT * FROM ... WHERE id = ?" case, the 'id' column might be from bind if it matches.
            int from_bind = sqlite3_value_frombind(val);
            (void)from_bind;

            // 3. sqlite3_value_numeric_type
            // Returns numeric type, applying affinity.
            int numeric_type = sqlite3_value_numeric_type(val);
            (void)numeric_type;

            // 4. sqlite3_value_int
            // Retrieves as signed 32-bit integer.
            int int_val = sqlite3_value_int(val);
            (void)int_val;

            // 5. sqlite3_value_bytes16
            // Returns byte length as UTF-16.
            int bytes16 = sqlite3_value_bytes16(val);
            (void)bytes16;

            // 6. sqlite3_value_encoding
            // Only meaningful for TEXT types.
            if (type == SQLITE_TEXT) {
                int encoding = sqlite3_value_encoding(val);
                (void)encoding;
            }
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    free(sql_buf);
    sqlite3_close(db);

    return 0;
}