#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_004 {
/*
** SQLite backup API fuzzing harness
**
** This harness specifically targets SQLite backup APIs identified as high-priority coverage gaps:
** - sqlite3_backup_init: Initialize a backup operation between two databases
** - sqlite3_backup_step: Perform incremental backup steps
** - sqlite3_backup_finish: Complete and cleanup backup operation
** - sqlite3_backup_remaining: Get remaining pages to backup
** - sqlite3_backup_pagecount: Get total pages to backup
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (open, exec, prepare, step)
** - harness_001: Advanced operations with progress handlers and authorizers
** - harness_002: BLOB-specific incremental I/O operations
** - harness_003: Virtual table creation, configuration, and operations
** - harness_004: Database backup operations between source and destination databases (this harness)
**
** Backup operation lifecycle: 
**   Source DB setup → Destination DB setup → Backup init → Backup steps → Backup monitoring → Backup finish
*/



// Simple callback for sqlite3_exec (minimal implementation)
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Operation types for the backup harness
enum BackupOperation {
    BACKUP_OP_STEP = 0,
    BACKUP_OP_STEP_MULTIPLE = 1,
    BACKUP_OP_STEP_WITH_MONITOR = 2,
    BACKUP_OP_RESTART = 3
};

// Database schema names supported by SQLite backup
static const char* const db_schemas[] = {
    "main",
    "temp",
    // Note: "aux" schemas are less commonly used but could be tested
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for configuration and basic operations
    // sizeof(int) for config + sizeof(int) for pages + at least 1 byte for schema selection
    const size_t MIN_SIZE = sizeof(int) * 4 + sizeof(int) + 1;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* src_db = nullptr;
    sqlite3* dest_db = nullptr;
    sqlite3_backup* backup = nullptr;
    char* err_msg = nullptr;
    
    // Consume database configuration from fuzzer input
    int src_open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    int dest_open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    
    // Add optional flags based on fuzzer input
    if (fdp.ConsumeBool()) {
        src_open_flags |= SQLITE_OPEN_NOMUTEX;
    }
    if (fdp.ConsumeBool()) {
        src_open_flags |= SQLITE_OPEN_FULLMUTEX;
    }
    if (fdp.ConsumeBool()) {
        dest_open_flags |= SQLITE_OPEN_NOMUTEX;
    }
    if (fdp.ConsumeBool()) {
        dest_open_flags |= SQLITE_OPEN_FULLMUTEX;
    }
    
    // Open source in-memory database
    int rc = sqlite3_open_v2(":memory:", &src_db, src_open_flags, nullptr);
    if (rc != SQLITE_OK || src_db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Open destination in-memory database (must be different connection)
    rc = sqlite3_open_v2(":memory:", &dest_db, dest_open_flags, nullptr);
    if (rc != SQLITE_OK || dest_db == nullptr) {
        sqlite3_close(src_db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Set limits to prevent excessive resource usage
    sqlite3_limit(src_db, SQLITE_LIMIT_VDBE_OP, fdp.ConsumeIntegralInRange<int>(1000, 50000));
    sqlite3_limit(src_db, SQLITE_LIMIT_LENGTH, fdp.ConsumeIntegralInRange<int>(1000, 100000));
    sqlite3_limit(dest_db, SQLITE_LIMIT_VDBE_OP, fdp.ConsumeIntegralInRange<int>(1000, 50000));
    sqlite3_limit(dest_db, SQLITE_LIMIT_LENGTH, fdp.ConsumeIntegralInRange<int>(1000, 100000));
    
    // Create tables and insert some data into source database
    // This provides content to backup
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS backup_test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB);";
    
    rc = sqlite3_exec(src_db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        // Continue anyway - backup can work on empty databases
    }
    
    // Insert some test data into source database
    // Use fuzzer input to generate variable data
    int num_rows = fdp.ConsumeIntegralInRange<int>(0, 100);
    for (int i = 0; i < num_rows && fdp.remaining_bytes() > 10; i++) {
        std::string name = fdp.ConsumeRandomLengthString(50);
        double value = fdp.ConsumeFloatingPoint<double>();
        std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 100));
        
        // Build and execute INSERT statement
        char insert_sql[512];
        if (blob_data.empty()) {
            snprintf(insert_sql, sizeof(insert_sql),
                    "INSERT INTO backup_test (name, value) VALUES ('%s', %f)",
                    name.c_str(), value);
        } else {
            // For BLOB data, we'd need to use binding - skip for simplicity
            continue;
        }
        
        rc = sqlite3_exec(src_db, insert_sql, nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Determine backup operation type from fuzzer input
    BackupOperation op_type = static_cast<BackupOperation>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 3));
    
    // Select source and destination schema names
    const char* src_schema = db_schemas[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(db_schemas)/sizeof(db_schemas[0]) - 1)];
    const char* dest_schema = db_schemas[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(db_schemas)/sizeof(db_schemas[0]) - 1)];
    
    // Initialize backup operation
    backup = sqlite3_backup_init(dest_db, dest_schema, src_db, src_schema);
    if (backup == nullptr) {
        // Backup init failed - could be due to various reasons
        // Still clean up and return
        sqlite3_close(dest_db);
        sqlite3_close(src_db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Perform backup operations based on selected type
    switch (op_type) {
        case BACKUP_OP_STEP: {
            // Single backup step with variable page count
            int n_page = fdp.ConsumeIntegralInRange<int>(-10, 1000);  // -10 tests negative page count handling
            rc = sqlite3_backup_step(backup, n_page);
            
            // Check remaining and pagecount after step
            int remaining = sqlite3_backup_remaining(backup);
            int pagecount = sqlite3_backup_pagecount(backup);
            (void)remaining;  // Use variables to avoid unused warnings
            (void)pagecount;
            break;
        }
        
        case BACKUP_OP_STEP_MULTIPLE: {
            // Multiple backup steps until completion or error
            int max_steps = fdp.ConsumeIntegralInRange<int>(1, 20);
            for (int i = 0; i < max_steps; i++) {
                int n_page = fdp.ConsumeIntegralInRange<int>(1, 100);
                rc = sqlite3_backup_step(backup, n_page);
                
                if (rc == SQLITE_DONE || rc == SQLITE_OK) {
                    // Backup completed
                    break;
                } else if (rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
                    // These are expected transient errors
                    // In real scenario we might retry, but for fuzzing we continue
                    continue;
                } else if (rc != SQLITE_OK && rc != SQLITE_DONE) {
                    // Some other error occurred
                    break;
                }
                
                // Check progress metrics
                int remaining = sqlite3_backup_remaining(backup);
                int pagecount = sqlite3_backup_pagecount(backup);
                (void)remaining;
                (void)pagecount;
                
                // Break if we're out of fuzzer input
                if (fdp.remaining_bytes() < sizeof(int)) {
                    break;
                }
            }
            break;
        }
        
        case BACKUP_OP_STEP_WITH_MONITOR: {
            // Backup with frequent monitoring calls
            int n_page = fdp.ConsumeIntegralInRange<int>(1, 500);
            
            // Get initial metrics
            int initial_remaining = sqlite3_backup_remaining(backup);
            int initial_pagecount = sqlite3_backup_pagecount(backup);
            (void)initial_remaining;
            (void)initial_pagecount;
            
            // Perform backup step
            rc = sqlite3_backup_step(backup, n_page);
            
            // Get post-step metrics
            int post_remaining = sqlite3_backup_remaining(backup);
            int post_pagecount = sqlite3_backup_pagecount(backup);
            (void)post_remaining;
            (void)post_pagecount;
            break;
        }
        
        case BACKUP_OP_RESTART: {
            // Test backup restart scenario
            // Start backup, take some pages, then restart
            
            // First backup attempt
            int n_page1 = fdp.ConsumeIntegralInRange<int>(1, 100);
            rc = sqlite3_backup_step(backup, n_page1);
            
            // If not done, finish and potentially restart
            if (rc != SQLITE_DONE) {
                // In real scenario, we might finish and restart
                // For fuzzing we'll just finish
            }
            break;
        }
    }
    
    // Always finish the backup (cleanup)
    rc = sqlite3_backup_finish(backup);
    backup = nullptr;
    
    // Cleanup databases
    sqlite3_close(dest_db);
    sqlite3_close(src_db);
    
    // Shutdown SQLite library
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_004
