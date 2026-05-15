#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_001 {
/*
 * Fuzzing harness for comprehensive SQLite prepared statement API testing.
 * Targets binding and column retrieval APIs for full type support:
 * - Binding: sqlite3_bind_int, sqlite3_bind_int64, sqlite3_bind_double, 
 *            sqlite3_bind_blob, sqlite3_bind_null
 * - Column retrieval: sqlite3_column_int, sqlite3_column_int64, sqlite3_column_double,
 *                     sqlite3_column_text, sqlite3_column_blob, sqlite3_column_bytes,
 *                     sqlite3_column_type
 * Uses FuzzedDataProvider to consume fuzzer input for various data types.
 * This harness complements harness_000.cpp by diving deeper into prepared statement usage.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for meaningful type testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    // Create test table with various column types
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS type_test("
        "id INTEGER PRIMARY KEY, "
        "int_val INTEGER, "
        "int64_val INTEGER, "
        "double_val REAL, "
        "text_val TEXT, "
        "blob_val BLOB, "
        "nullable_val INTEGER"
        ");";
    
    char* err_msg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Prepare INSERT statement with parameters
    const char* insert_sql = 
        "INSERT INTO type_test(int_val, int64_val, double_val, text_val, blob_val, nullable_val) "
        "VALUES(?, ?, ?, ?, ?, ?);";
    
    sqlite3_stmt* insert_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr);
    
    if (rc != SQLITE_OK || insert_stmt == nullptr) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Check parameter count
    int param_count = sqlite3_bind_parameter_count(insert_stmt);
    if (param_count != 6) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Consume data for binding from fuzzer input
    
    // 1. Bind integer
    int int_val = fdp.ConsumeIntegral<int>();
    rc = sqlite3_bind_int(insert_stmt, 1, int_val);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 2. Bind int64
    sqlite3_int64 int64_val = fdp.ConsumeIntegral<sqlite3_int64>();
    rc = sqlite3_bind_int64(insert_stmt, 2, int64_val);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 3. Bind double
    double double_val = fdp.ConsumeFloatingPoint<double>();
    rc = sqlite3_bind_double(insert_stmt, 3, double_val);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 4. Bind text
    std::string text_val = fdp.ConsumeRandomLengthString(256);
    rc = sqlite3_bind_text(insert_stmt, 4, text_val.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 5. Bind blob
    size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    if (fdp.remaining_bytes() < blob_size) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
    rc = sqlite3_bind_blob(insert_stmt, 5, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 6. Bind either integer or null based on fuzzer input
    if (fdp.ConsumeBool()) {
        int nullable_int = fdp.ConsumeIntegral<int>();
        rc = sqlite3_bind_int(insert_stmt, 6, nullable_int);
    } else {
        rc = sqlite3_bind_null(insert_stmt, 6);
    }
    if (rc != SQLITE_OK) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Execute INSERT
    rc = sqlite3_step(insert_stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    sqlite3_finalize(insert_stmt);
    
    // Prepare SELECT statement
    const char* select_sql = "SELECT * FROM type_test ORDER BY id DESC LIMIT 1;";
    sqlite3_stmt* select_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
    
    if (rc != SQLITE_OK || select_stmt == nullptr) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Execute SELECT and retrieve data
    rc = sqlite3_step(select_stmt);
    if (rc == SQLITE_ROW) {
        int col_count = sqlite3_column_count(select_stmt);
        
        for (int i = 0; i < col_count; i++) {
            int col_type = sqlite3_column_type(select_stmt, i);
            
            switch (col_type) {
                case SQLITE_INTEGER: {
                    if (i == 0 || i == 1) { // id or int_val
                        int int_col_val = sqlite3_column_int(select_stmt, i);
                        (void)int_col_val; // Use value to avoid unused warning
                    } else if (i == 2) { // int64_val
                        sqlite3_int64 int64_col_val = sqlite3_column_int64(select_stmt, i);
                        (void)int64_col_val;
                    }
                    break;
                }
                case SQLITE_FLOAT: {
                    double double_col_val = sqlite3_column_double(select_stmt, i);
                    (void)double_col_val;
                    break;
                }
                case SQLITE_TEXT: {
                    const unsigned char* text_col_val = sqlite3_column_text(select_stmt, i);
                    int text_len = sqlite3_column_bytes(select_stmt, i);
                    (void)text_col_val;
                    (void)text_len;
                    break;
                }
                case SQLITE_BLOB: {
                    const void* blob_col_val = sqlite3_column_blob(select_stmt, i);
                    int blob_len = sqlite3_column_bytes(select_stmt, i);
                    (void)blob_col_val;
                    (void)blob_len;
                    break;
                }
                case SQLITE_NULL: {
                    // NULL value, nothing to retrieve
                    break;
                }
            }
        }
    }
    
    sqlite3_finalize(select_stmt);
    
    // Clean up table
    rc = sqlite3_exec(db, "DROP TABLE IF EXISTS type_test;", nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
        sqlite3_free(err_msg);
    }
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_001
