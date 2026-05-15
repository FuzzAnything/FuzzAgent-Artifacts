#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_005 {
/*
 * Fuzzing harness for SQLite advanced features not covered in previous harnesses.
 * Targets uncovered advanced APIs:
 * - Backup/restore APIs: sqlite3_backup_init, sqlite3_backup_step, sqlite3_backup_finish, sqlite3_backup_remaining, sqlite3_backup_pagecount
 * - Memory management APIs: sqlite3_malloc, sqlite3_free, sqlite3_realloc
 * - Configuration APIs: sqlite3_config, sqlite3_db_config
 * 
 * This complements previous harnesses with semantic diversity:
 * - harness_000.cpp (basic SQL operations)
 * - harness_001.cpp (binding/column APIs)
 * - harness_002.cpp (transaction/schema/blob APIs)
 * - harness_003.cpp (user-defined functions/collation)
 * - harness_004.cpp (Full Text Search)
 * 
 * Focuses on advanced features requiring complex setup and specialized APIs.
 * Uses FuzzedDataProvider to consume fuzzer input for backup parameters and memory operations.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Test memory management APIs
    size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    void* mem_block = sqlite3_malloc(alloc_size);
    if (mem_block) {
        // Test realloc
        size_t new_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
        void* realloced = sqlite3_realloc(mem_block, new_size);
        if (realloced) {
            mem_block = realloced;
            alloc_size = new_size;
        }
        
        // Test using the memory
        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(0, 256));
        if (!test_data.empty() && mem_block) {
            size_t copy_size = test_data.size() < alloc_size ? test_data.size() : alloc_size;
            memcpy(mem_block, test_data.data(), copy_size);
        }
        
        // Free the memory
        sqlite3_free(mem_block);
    }
    
    // Open source database (in-memory)
    sqlite3* src_db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &src_db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || src_db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Configure database
    sqlite3_db_config(src_db, SQLITE_DBCONFIG_ENABLE_FKEY, 
                     fdp.ConsumeBool() ? 1 : 0, nullptr);
    sqlite3_db_config(src_db, SQLITE_DBCONFIG_TRIGGER_EQP,
                     fdp.ConsumeBool() ? 1 : 0, nullptr);
    
    // Create some test data in source database
    char* err_msg = nullptr;
    const char* create_table_sql = 
        "CREATE TABLE test_data(id INTEGER PRIMARY KEY, name TEXT, value INTEGER);";
    sqlite3_exec(src_db, create_table_sql, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Insert some test data using fuzzer input
    int num_rows = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_rows && fdp.remaining_bytes() > 0; i++) {
        std::string name = fdp.ConsumeRandomLengthString(32);
        int value = fdp.ConsumeIntegral<int>();
        
        std::string insert_sql = "INSERT INTO test_data(name, value) VALUES('" + 
                                name + "', " + std::to_string(value) + ");";
        sqlite3_exec(src_db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Open destination database for backup testing
    sqlite3* dest_db = nullptr;
    rc = sqlite3_open_v2(":memory:", &dest_db, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc == SQLITE_OK && dest_db != nullptr) {
        // Initialize backup from src to dest
        sqlite3_backup* backup = sqlite3_backup_init(dest_db, "main", src_db, "main");
        
        if (backup) {
            // Perform backup in steps
            int pages_per_step = fdp.ConsumeIntegralInRange<int>(1, 10);
            int backup_result;
            
            do {
                backup_result = sqlite3_backup_step(backup, pages_per_step);
                
                // Check backup progress
                int remaining = sqlite3_backup_remaining(backup);
                int total = sqlite3_backup_pagecount(backup);
                (void)remaining; (void)total; // Use values to avoid unused warnings
                
                // Consume more data for next step parameters if needed
                if (backup_result == SQLITE_OK && fdp.remaining_bytes() > 0) {
                    pages_per_step = fdp.ConsumeIntegralInRange<int>(1, 10);
                }
                
            } while (backup_result == SQLITE_OK || backup_result == SQLITE_BUSY || 
                    backup_result == SQLITE_LOCKED);
            
            // Finish backup
            int finish_result = sqlite3_backup_finish(backup);
            (void)finish_result; // Use value to avoid unused warning
            
            // Verify backup by querying destination
            const char* verify_sql = "SELECT COUNT(*) FROM test_data;";
            sqlite3_exec(dest_db, verify_sql, nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Close destination database
        sqlite3_close(dest_db);
    }
    
    // Test configuration APIs
    int config_op = fdp.ConsumeIntegralInRange<int>(0, 2);
    switch (config_op) {
        case 0:
            sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
        case 1:
            sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
            break;
        case 2:
            sqlite3_config(SQLITE_CONFIG_SERIALIZED);
            break;
    }
    
    // Test more memory management
    if (fdp.remaining_bytes() > 0) {
        size_t extra_alloc = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        void* extra_mem = sqlite3_malloc(extra_alloc);
        if (extra_mem) {
            // Write some fuzzer data to the memory
            std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(
                fdp.ConsumeIntegralInRange<size_t>(0, extra_alloc));
            if (!extra_data.empty()) {
                memcpy(extra_mem, extra_data.data(), 
                      extra_data.size() < extra_alloc ? extra_data.size() : extra_alloc);
            }
            sqlite3_free(extra_mem);
        }
    }
    
    // Clean up source database
    sqlite3_close(src_db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_005
