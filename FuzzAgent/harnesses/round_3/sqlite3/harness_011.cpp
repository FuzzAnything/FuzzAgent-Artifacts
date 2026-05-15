#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_011 {


// Custom destructor function for pointer bindings
static void pointer_destructor(void* ptr) {
    // Do nothing - just a placeholder for testing
    (void)ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive binding operations
    // Need enough data for multiple bindings and various data types
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size configuration parameters
    uint8_t db_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t binding_strategy = fdp.ConsumeIntegral<uint8_t>() % 8;
    int num_params = fdp.ConsumeIntegralInRange<int>(1, 20);
    bool use_64bit_encodings = fdp.ConsumeBool();
    bool test_pointer_bindings = fdp.ConsumeBool();
    bool test_value_bindings = fdp.ConsumeBool();
    
    // Consume variable-size data for SQL and bindings
    std::string sql_template = fdp.ConsumeRandomLengthString(512);
    std::string remaining_input = fdp.ConsumeRemainingBytesAsString();

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* err_msg = nullptr;
    int rc;

    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Open an in-memory database
    unsigned int open_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    if (db_flags & 0x01) open_mode |= SQLITE_OPEN_NOMUTEX;
    if (db_flags & 0x02) open_mode |= SQLITE_OPEN_FULLMUTEX;
    if (db_flags & 0x04) open_mode |= SQLITE_OPEN_PRIVATECACHE;
    if (db_flags & 0x08) open_mode |= SQLITE_OPEN_SHAREDCACHE;

    rc = sqlite3_open_v2(":memory:", &db, open_mode, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }

    // Create a test table with various column types
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS bind_test ("
        "id INTEGER PRIMARY KEY, "
        "text_col TEXT, "
        "int_col INTEGER, "
        "real_col REAL, "
        "blob_col BLOB, "
        "null_col INTEGER"
        ");";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }

    // Prepare a parameterized INSERT statement
    std::string insert_sql = "INSERT INTO bind_test VALUES (?, ?, ?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }

    // Get parameter count to verify our binding
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count != 6) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }

    // Reset the FuzzedDataProvider for binding data
    // We'll create a new one from the remaining input
    FuzzedDataProvider bind_fdp(reinterpret_cast<const uint8_t*>(remaining_input.data()), 
                                remaining_input.size());

    // Bind parameters using different strategies based on fuzzed input
    for (int i = 1; i <= param_count && bind_fdp.remaining_bytes() > 0; i++) {
        uint8_t bind_type = bind_fdp.ConsumeIntegral<uint8_t>() % 12;
        
        switch (bind_type) {
            case 0: {
                // sqlite3_bind_null
                rc = sqlite3_bind_null(stmt, i);
                break;
            }
            case 1: {
                // sqlite3_bind_int
                int int_val = bind_fdp.ConsumeIntegral<int>();
                rc = sqlite3_bind_int(stmt, i, int_val);
                break;
            }
            case 2: {
                // sqlite3_bind_int64
                sqlite3_int64 int64_val = bind_fdp.ConsumeIntegral<sqlite3_int64>();
                rc = sqlite3_bind_int64(stmt, i, int64_val);
                break;
            }
            case 3: {
                // sqlite3_bind_double
                double double_val = bind_fdp.ConsumeFloatingPoint<double>();
                rc = sqlite3_bind_double(stmt, i, double_val);
                break;
            }
            case 4: {
                // sqlite3_bind_text (regular text binding)
                std::string text_val = bind_fdp.ConsumeRandomLengthString(256);
                rc = sqlite3_bind_text(stmt, i, text_val.c_str(), text_val.size(), 
                                      SQLITE_TRANSIENT);
                break;
            }
            case 5: {
                // sqlite3_bind_text64 (64-bit text binding)
                if (use_64bit_encodings && bind_fdp.remaining_bytes() > 100) {
                    std::string text64_val = bind_fdp.ConsumeRandomLengthString(1024);
                    unsigned char encoding = bind_fdp.ConsumeIntegral<unsigned char>() % 4;
                    // Valid encodings: SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16
                    if (encoding > 3) encoding = SQLITE_UTF8;
                    rc = sqlite3_bind_text64(stmt, i, text64_val.c_str(), 
                                            text64_val.size(),
                                            SQLITE_TRANSIENT, encoding);
                } else {
                    rc = sqlite3_bind_null(stmt, i);
                }
                break;
            }
            case 6: {
                // sqlite3_bind_blob (regular blob binding)
                if (bind_fdp.remaining_bytes() > 50) {
                    size_t blob_size = bind_fdp.ConsumeIntegralInRange<size_t>(1, 512);
                    std::vector<uint8_t> blob_data = bind_fdp.ConsumeBytes<uint8_t>(blob_size);
                    if (!blob_data.empty()) {
                        rc = sqlite3_bind_blob(stmt, i, blob_data.data(), blob_data.size(),
                                              SQLITE_TRANSIENT);
                    } else {
                        rc = sqlite3_bind_null(stmt, i);
                    }
                } else {
                    rc = sqlite3_bind_null(stmt, i);
                }
                break;
            }
            case 7: {
                // sqlite3_bind_blob64 (64-bit blob binding)
                if (use_64bit_encodings && bind_fdp.remaining_bytes() > 200) {
                    size_t blob64_size = bind_fdp.ConsumeIntegralInRange<size_t>(1, 2048);
                    std::vector<uint8_t> blob64_data = bind_fdp.ConsumeBytes<uint8_t>(blob64_size);
                    if (!blob64_data.empty()) {
                        rc = sqlite3_bind_blob64(stmt, i, blob64_data.data(), 
                                                blob64_data.size(),
                                                SQLITE_TRANSIENT);
                    } else {
                        rc = sqlite3_bind_null(stmt, i);
                    }
                } else {
                    rc = sqlite3_bind_null(stmt, i);
                }
                break;
            }
            case 8: {
                // sqlite3_bind_zeroblob
                int zeroblob_size = bind_fdp.ConsumeIntegralInRange<int>(0, 1024);
                rc = sqlite3_bind_zeroblob(stmt, i, zeroblob_size);
                break;
            }
            case 9: {
                // sqlite3_bind_zeroblob64
                if (use_64bit_encodings) {
                    sqlite3_uint64 zeroblob64_size = bind_fdp.ConsumeIntegral<sqlite3_uint64>() % 4096;
                    rc = sqlite3_bind_zeroblob64(stmt, i, zeroblob64_size);
                } else {
                    rc = sqlite3_bind_null(stmt, i);
                }
                break;
            }
            case 10: {
                // sqlite3_bind_value (requires creating a sqlite3_value first)
                if (test_value_bindings) {
                    // Create a simple value using sqlite3_value
                    // We need to execute a simple query to get a value
                    sqlite3_stmt* value_stmt = nullptr;
                    const char* value_sql = "SELECT ?";
                    if (sqlite3_prepare_v2(db, value_sql, -1, &value_stmt, nullptr) == SQLITE_OK) {
                        // Bind a simple value to get a sqlite3_value
                        sqlite3_bind_int(value_stmt, 1, bind_fdp.ConsumeIntegral<int>());
                        if (sqlite3_step(value_stmt) == SQLITE_ROW) {
                            const sqlite3_value* value = sqlite3_column_value(value_stmt, 0);
                            rc = sqlite3_bind_value(stmt, i, value);
                        }
                        sqlite3_finalize(value_stmt);
                    } else {
                        rc = sqlite3_bind_null(stmt, i);
                    }
                } else {
                    rc = sqlite3_bind_null(stmt, i);
                }
                break;
            }
            case 11: {
                // sqlite3_bind_pointer
                if (test_pointer_bindings) {
                    // Create some dummy data for pointer binding
                    static int dummy_data = 42;
                    static double dummy_double = 3.14159;
                    static const char* dummy_type = "fuzzer_test_pointer";
                    
                    // Choose which pointer to bind
                    uint8_t ptr_choice = bind_fdp.ConsumeIntegral<uint8_t>() % 3;
                    void* ptr_to_bind = nullptr;
                    const char* type_name = nullptr;
                    
                    switch (ptr_choice) {
                        case 0:
                            ptr_to_bind = &dummy_data;
                            type_name = "integer_pointer";
                            break;
                        case 1:
                            ptr_to_bind = &dummy_double;
                            type_name = "double_pointer";
                            break;
                        case 2:
                            ptr_to_bind = (void*)dummy_type;
                            type_name = "string_pointer";
                            break;
                    }
                    
                    rc = sqlite3_bind_pointer(stmt, i, ptr_to_bind, type_name, pointer_destructor);
                } else {
                    rc = sqlite3_bind_null(stmt, i);
                }
                break;
            }
            default:
                rc = sqlite3_bind_null(stmt, i);
                break;
        }
        
        if (rc != SQLITE_OK) {
            // Continue binding even if some fail
        }
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    
    // Reset the statement to test re-binding
    sqlite3_reset(stmt);
    
    // Test binding with different parameter indices (including edge cases)
    if (bind_fdp.remaining_bytes() > 10) {
        // Test binding with invalid index (should fail gracefully)
        int invalid_index = param_count + bind_fdp.ConsumeIntegralInRange<int>(1, 10);
        rc = sqlite3_bind_null(stmt, invalid_index);
        
        // Test binding with index 0 (should fail)
        rc = sqlite3_bind_null(stmt, 0);
        
        // Test negative index
        rc = sqlite3_bind_null(stmt, -1);
    }

    // Final cleanup
    sqlite3_finalize(stmt);
    
    // Also test parameter name lookups
    sqlite3_stmt* name_stmt = nullptr;
    const char* name_sql = "SELECT :named_param, @another_param, $third_param";
    if (sqlite3_prepare_v2(db, name_sql, -1, &name_stmt, nullptr) == SQLITE_OK) {
        // Test sqlite3_bind_parameter_index
        int idx1 = sqlite3_bind_parameter_index(name_stmt, ":named_param");
        int idx2 = sqlite3_bind_parameter_index(name_stmt, "@another_param");
        int idx3 = sqlite3_bind_parameter_index(name_stmt, "$third_param");
        
        // Test sqlite3_bind_parameter_name
        const char* name1 = sqlite3_bind_parameter_name(name_stmt, 1);
        const char* name2 = sqlite3_bind_parameter_name(name_stmt, 2);
        const char* name3 = sqlite3_bind_parameter_name(name_stmt, 3);
        
        // Bind some values to named parameters
        if (idx1 > 0) sqlite3_bind_int(name_stmt, idx1, 100);
        if (idx2 > 0) sqlite3_bind_text(name_stmt, idx2, "test", 4, SQLITE_STATIC);
        if (idx3 > 0) sqlite3_bind_double(name_stmt, idx3, 3.14);
        
        sqlite3_finalize(name_stmt);
    }

    // Close database
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_011
