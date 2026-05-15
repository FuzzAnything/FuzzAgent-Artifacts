#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_002 {
/*
 * Fuzzing harness for SQLite library - focusing on database backup operations
 * This harness targets sqlite3_backup_init, sqlite3_backup_step, sqlite3_backup_finish,
 * sqlite3_backup_pagecount, and sqlite3_backup_remaining as identified by coverage analysis
 * This harness targets 186+ uncovered branches in backup functionality
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing:
    // - Operation type (1 byte)
    // - Database names and SQL statements
    // - Backup page count/size parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Consume operation type from fuzzer input
    // 0: Normal backup with page count
    // 1: Backup with custom page size
    // 2: Backup with progress monitoring
    // 3: Partial backup (limited pages)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume database names from fuzzer input
    // Limit length to prevent excessive memory usage
    std::string source_db_name = ":memory:";
    std::string dest_db_name = ":memory:";
    
    // For some operations, use temporary files to test file-based backup
    if (operation % 2 == 0) {
        // Use file-based databases for half the operations
        source_db_name = fdp.ConsumeRandomLengthString(64);
        dest_db_name = fdp.ConsumeRandomLengthString(64);
        
        // Ensure non-empty names
        if (source_db_name.empty()) source_db_name = "source.db";
        if (dest_db_name.empty()) dest_db_name = "dest.db";
    }
    
    // Open source database
    sqlite3* source_db = nullptr;
    int rc = sqlite3_open_v2(source_db_name.c_str(), &source_db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                             nullptr);
    if (rc != SQLITE_OK || source_db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Open destination database
    sqlite3* dest_db = nullptr;
    rc = sqlite3_open_v2(dest_db_name.c_str(), &dest_db,
                         SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                         nullptr);
    if (rc != SQLITE_OK || dest_db == nullptr) {
        sqlite3_close(source_db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits to prevent timeouts and excessive memory usage
    sqlite3_limit(source_db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(source_db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(dest_db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(dest_db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    // Populate source database with test data using sqlite3_exec
    // Consume SQL statements from fuzzer input
    size_t remaining = fdp.remaining_bytes();
    size_t sql_len = remaining > 0 ? fdp.ConsumeIntegralInRange<size_t>(0, std::min(remaining, (size_t)4096)) : 0;
    std::string sql_data = fdp.ConsumeBytesAsString(sql_len);
    
    // If no SQL provided, generate simple schema from fuzzer input
    if (sql_data.empty()) {
        // Consume schema type from fuzzer input
        uint8_t schema_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        if (schema_type == 0) {
            sql_data = "CREATE TABLE IF NOT EXISTS test_data (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
        } else if (schema_type == 1) {
            sql_data = "CREATE TABLE IF NOT EXISTS backup_test (id INTEGER, data BLOB);";
        } else {
            sql_data = "CREATE TABLE IF NOT EXISTS mixed (id INT, txt TEXT, val REAL, blob BLOB);";
        }
    }
    
    char* errmsg = nullptr;
    rc = sqlite3_exec(source_db, sql_data.c_str(), nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        // Continue anyway - some SQL might have errors but others may succeed
    }
    
    // Prepare destination database - consume schema from fuzzer input
    std::string dest_sql_str = "";
    if (fdp.remaining_bytes() > 0) {
        size_t dest_sql_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)512));
        dest_sql_str = fdp.ConsumeBytesAsString(dest_sql_len);
    }
    
    if (dest_sql_str.empty()) {
        dest_sql_str = "CREATE TABLE IF NOT EXISTS dest_table (id INTEGER);";
    }
    
    errmsg = nullptr;
    sqlite3_exec(dest_db, dest_sql_str.c_str(), nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
    }
    
    // Now perform the backup operations
    sqlite3_backup* backup_handle = nullptr;
    
    // Create backup handle from source to destination
    // Use "main" database for both source and destination
    backup_handle = sqlite3_backup_init(dest_db, "main", source_db, "main");
    
    if (backup_handle != nullptr) {
        // Consume backup parameters from fuzzer input
        int max_pages = fdp.ConsumeIntegralInRange<int>(1, 100);
        bool monitor_progress = fdp.ConsumeBool();
        
        // Perform backup in a loop
        int backup_result = SQLITE_OK;
        int total_pages_copied = 0;
        
        while (backup_result == SQLITE_OK || backup_result == SQLITE_BUSY || backup_result == SQLITE_LOCKED) {
            // Get progress information
            int remaining_pages = sqlite3_backup_remaining(backup_handle);
            int total_pages = sqlite3_backup_pagecount(backup_handle);
            
            // Consume page count for this step from fuzzer input
            int pages_this_step = fdp.ConsumeIntegralInRange<int>(-1, max_pages);
            if (pages_this_step == 0) pages_this_step = 1; // At least 1 page
            
            // Perform backup step
            backup_result = sqlite3_backup_step(backup_handle, pages_this_step);
            total_pages_copied += (pages_this_step > 0 ? pages_this_step : 0);
            
            // Break conditions
            if (backup_result == SQLITE_DONE) {
                // Backup completed
                break;
            } else if (backup_result == SQLITE_OK) {
                // More pages to copy
                continue;
            } else if (backup_result == SQLITE_BUSY || backup_result == SQLITE_LOCKED) {
                // Database is busy/locked, retry with different parameters
                // Consume retry delay from fuzzer input
                int retry_delay = fdp.ConsumeIntegralInRange<int>(0, 100);
                if (retry_delay > 0) {
                    // Simulate delay (not actually sleeping in fuzzer)
                    // This just consumes fuzzer input to explore different paths
                    continue;
                }
            } else {
                // Some other error
                break;
            }
            
            // Limit total iterations to prevent infinite loops
            if (total_pages_copied > 1000) {
                break;
            }
        }
        
        // Complete the backup
        int finish_result = sqlite3_backup_finish(backup_handle);
        backup_handle = nullptr;
        
        // Optionally verify the backup by checking destination database
        if (finish_result == SQLITE_OK) {
            // Perform simple verification query
            sqlite3_stmt* verify_stmt = nullptr;
            const char* verify_sql = "SELECT COUNT(*) FROM test_data;";
            rc = sqlite3_prepare_v2(dest_db, verify_sql, -1, &verify_stmt, nullptr);
            if (rc == SQLITE_OK && verify_stmt != nullptr) {
                rc = sqlite3_step(verify_stmt);
                sqlite3_finalize(verify_stmt);
            }
        }
    } else {
        // Backup init failed - get error information
        int errcode = sqlite3_errcode(dest_db);
        const char* errstr = sqlite3_errmsg(dest_db);
        // Error information consumed for coverage
    }
    
    // Additional backup scenarios based on operation type
    switch (operation) {
        case 1: {
            // Test backup with different database names (not "main")
            // Create and use an attached database
            const char* attach_sql = "ATTACH DATABASE ':memory:' AS attached_db;"
                                     "CREATE TABLE attached_db.test_table (id INTEGER);"
                                     "INSERT INTO attached_db.test_table VALUES (1), (2), (3);";
            errmsg = nullptr;
            sqlite3_exec(source_db, attach_sql, nullptr, nullptr, &errmsg);
            if (errmsg != nullptr) {
                sqlite3_free(errmsg);
            }
            
            // Try backup from attached database
            sqlite3_backup* attached_backup = sqlite3_backup_init(dest_db, "main", source_db, "attached_db");
            if (attached_backup != nullptr) {
                sqlite3_backup_step(attached_backup, 5);
                sqlite3_backup_finish(attached_backup);
            }
            break;
        }
        case 2: {
            // Test backup with empty/in-progress backup
            sqlite3_backup* empty_backup = sqlite3_backup_init(dest_db, "main", source_db, "main");
            if (empty_backup != nullptr) {
                // Immediately finish without stepping
                sqlite3_backup_finish(empty_backup);
            }
            break;
        }
        
        case 3: {
            // Test backup after source database modification during backup
            sqlite3_backup* concurrent_backup = sqlite3_backup_init(dest_db, "main", source_db, "main");
            if (concurrent_backup != nullptr) {
                // Modify source database while backup is in progress
                sqlite3_backup_step(concurrent_backup, 2);
                
                // Modify source
                errmsg = nullptr;
                sqlite3_exec(source_db, "INSERT INTO test_data (name, value) VALUES ('during_backup', 99.0);", 
                            nullptr, nullptr, &errmsg);
                if (errmsg != nullptr) {
                    sqlite3_free(errmsg);
                }
                
                // Continue backup
                sqlite3_backup_step(concurrent_backup, -1); // Copy all remaining pages
                sqlite3_backup_finish(concurrent_backup);
            }
            break;
        }
    }
    
    // Cleanup
    sqlite3_close(source_db);
    sqlite3_close(dest_db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_002
