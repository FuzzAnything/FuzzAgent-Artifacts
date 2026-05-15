#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_013 {
/*
** SQLite Memory Management APIs Fuzzing Harness
**
** This harness specifically targets SQLite memory management APIs identified as 
** high-complexity and zero-coverage functions:
** - sqlite3_soft_heap_limit64: Set/get soft heap limit (64-bit)
** - sqlite3_hard_heap_limit64: Set/get hard heap limit (64-bit)
** - sqlite3_soft_heap_limit: Deprecated soft heap limit (32-bit)
** - sqlite3_db_cacheflush: Flush database caches to disk mid-transaction
** - sqlite3_memory_used: Get current memory usage
** - sqlite3_memory_highwater: Get peak memory usage
** - sqlite3_release_memory: Attempt to free memory
** - sqlite3_status: Get runtime memory statistics (32-bit)
** - sqlite3_status64: Get runtime memory statistics (64-bit)
** - sqlite3_db_status: Get database connection memory statistics (32-bit)
** - sqlite3_db_status64: Get database connection memory statistics (64-bit)
**
** Following CoverageAnalyzerAgent recommendation: Target memory management APIs with
** varied parameters to maximize coverage of heap limits, cache flushing, and memory
** management operations.
**
** SQLite memory management interfaces control memory allocation limits, cache behavior,
** and provide runtime statistics. This harness tests various memory operations with
** fuzzer-generated parameters to maximize coverage of internal memory management logic.
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (no memory management)
** - harness_001: Advanced operations with progress handlers (no memory management)
** - harness_002: BLOB-specific operations (no memory management)
** - harness_003: Virtual table operations (no memory management)
** - harness_004: Backup operations (no memory management)
** - harness_005: Serialization/deserialization (no memory management)
** - harness_006: UTF-16 operations (no memory management)
** - harness_007: Metadata and statement explanation (no memory management)
** - harness_008: Statement preparation and binding (no memory management)
** - harness_009: Custom functions and collations (no memory management)
** - harness_010: Advanced UTF-16 operations (no memory management)
** - harness_011: Error handling and stress testing (no memory management)
** - harness_012: Test control interface (no memory management)
** - harness_013: Comprehensive memory management API testing (this harness)
**
** Memory management operation lifecycle:
**   Database setup → Set heap limits → Test memory operations → 
**   Get memory statistics → Flush caches → Release memory → Cleanup
*/



// Simple callback for sqlite3_exec (minimal implementation)
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

// Memory operation types
enum MemoryOperation {
    MEM_OP_SOFT_LIMIT = 0,
    MEM_OP_HARD_LIMIT = 1,
    MEM_OP_DEPRECATED_SOFT = 2,
    MEM_OP_CACHE_FLUSH = 3,
    MEM_OP_MEMORY_USED = 4,
    MEM_OP_MEMORY_HIGHWATER = 5,
    MEM_OP_RELEASE_MEMORY = 6,
    MEM_OP_GLOBAL_STATUS = 7,
    MEM_OP_GLOBAL_STATUS64 = 8,
    MEM_OP_DB_STATUS = 9,
    MEM_OP_DB_STATUS64 = 10,
    MEM_OP_COMBINED_TEST = 11
};

// Status operation codes
enum StatusOperation {
    STATUS_MEMORY_USED = 0,
    STATUS_PAGECACHE_USED = 1,
    STATUS_PAGECACHE_OVERFLOW = 2,
    STATUS_MALLOC_SIZE = 5,
    STATUS_PARSER_STACK = 6,
    STATUS_PAGECACHE_SIZE = 7,
    STATUS_MALLOC_COUNT = 9
};

// Database status operation codes
enum DbStatusOperation {
    DBSTATUS_LOOKASIDE_USED = 0,
    DBSTATUS_LOOKASIDE_HIT = 1,
    DBSTATUS_LOOKASIDE_MISS_SIZE = 2,
    DBSTATUS_LOOKASIDE_MISS_FULL = 3,
    DBSTATUS_CACHE_USED = 4,
    DBSTATUS_CACHE_USED_SHARED = 5,
    DBSTATUS_SCHEMA_USED = 6,
    DBSTATUS_STMT_USED = 7,
    DBSTATUS_CACHE_HIT = 8,
    DBSTATUS_CACHE_MISS = 9,
    DBSTATUS_CACHE_WRITE = 10,
    DBSTATUS_DEFERRED_FKS = 11,
    DBSTATUS_CACHE_SPILL = 12
};

// Helper function to create a simple database and table for testing
static sqlite3* create_test_database(FuzzedDataProvider& fdp) {
    sqlite3* db = nullptr;
    const char* db_name = ":memory:";
    std::string custom_name = fdp.ConsumeRandomLengthString(64);
    
    // Randomly choose between in-memory and file database
    if (fdp.ConsumeBool() && !custom_name.empty()) {
        // Use file database for cache flush testing
        db_name = custom_name.c_str();
    }
    
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (fdp.ConsumeBool()) flags |= SQLITE_OPEN_NOMUTEX;
    if (fdp.ConsumeBool()) flags |= SQLITE_OPEN_FULLMUTEX;
    
    int rc = sqlite3_open_v2(db_name, &db, flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        // Fall back to in-memory database
        if (db) sqlite3_close(db);
        sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    }
    
    // Create a simple table to generate some database activity
    if (db) {
        char* errmsg = nullptr;
        sqlite3_exec(db, 
            "CREATE TABLE IF NOT EXISTS test_mem (id INTEGER PRIMARY KEY, data TEXT, value REAL);"
            "CREATE INDEX IF NOT EXISTS idx_test_mem ON test_mem(data);",
            nullptr, nullptr, &errmsg);
        if (errmsg) sqlite3_free(errmsg);
        
        // Insert some test data
        for (int i = 0; i < 10 && fdp.ConsumeBool(); i++) {
            std::string data = fdp.ConsumeRandomLengthString(50);
            char sql[256];
            snprintf(sql, sizeof(sql), 
                "INSERT INTO test_mem(data, value) VALUES ('%s', %f);",
                data.c_str(), fdp.ConsumeFloatingPoint<double>());
            sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
        }
    }
    
    return db;
}

// Helper to perform some database operations to generate memory activity
static void generate_memory_activity(sqlite3* db, FuzzedDataProvider& fdp) {
    if (!db) return;
    
    // Execute some queries to generate memory usage
    char* errmsg = nullptr;
    int num_queries = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    for (int i = 0; i < num_queries; i++) {
        int query_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        
        switch (query_type) {
            case 0:
                sqlite3_exec(db, "SELECT * FROM test_mem;", exec_callback, nullptr, &errmsg);
                break;
            case 1:
                sqlite3_exec(db, "SELECT COUNT(*) FROM test_mem;", exec_callback, nullptr, &errmsg);
                break;
            case 2:
                sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errmsg);
                sqlite3_exec(db, "INSERT INTO test_mem(data, value) VALUES ('test', 1.0);", 
                           nullptr, nullptr, nullptr);
                if (fdp.ConsumeBool()) {
                    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
                } else {
                    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
                }
                break;
            case 3:
                sqlite3_exec(db, "PRAGMA cache_size;", exec_callback, nullptr, &errmsg);
                break;
        }
        
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create test database
    sqlite3* db = create_test_database(fdp);
    if (!db) return 0;
    
    // Determine which memory operations to test
    MemoryOperation mem_op = static_cast<MemoryOperation>(
        fdp.ConsumeIntegralInRange<int>(0, MEM_OP_COMBINED_TEST));
    
    // Generate some memory activity first
    generate_memory_activity(db, fdp);
    
    // Test selected memory operation
    switch (mem_op) {
        case MEM_OP_SOFT_LIMIT: {
            // Test sqlite3_soft_heap_limit64
            sqlite3_int64 limit = fdp.ConsumeIntegral<sqlite3_int64>();
            sqlite3_int64 previous = sqlite3_soft_heap_limit64(limit);
            
            // Query current limit
            sqlite3_int64 current = sqlite3_soft_heap_limit64(-1);
            
            // Restore previous limit
            sqlite3_soft_heap_limit64(previous);
            
            // Generate more activity with limit set
            generate_memory_activity(db, fdp);
            break;
        }
        
        case MEM_OP_HARD_LIMIT: {
            // Test sqlite3_hard_heap_limit64
            sqlite3_int64 limit = fdp.ConsumeIntegral<sqlite3_int64>();
            sqlite3_int64 previous = sqlite3_hard_heap_limit64(limit);
            
            // Query current limit
            sqlite3_int64 current = sqlite3_hard_heap_limit64(-1);
            
            // Restore previous limit
            sqlite3_hard_heap_limit64(previous);
            
            // Generate more activity with limit set
            generate_memory_activity(db, fdp);
            break;
        }
        
        case MEM_OP_DEPRECATED_SOFT: {
            // Test deprecated sqlite3_soft_heap_limit
            int limit = fdp.ConsumeIntegral<int>();
            sqlite3_soft_heap_limit(limit);
            
            // Generate activity with limit set
            generate_memory_activity(db, fdp);
            break;
        }
        
        case MEM_OP_CACHE_FLUSH: {
            // Test sqlite3_db_cacheflush
            // Start a transaction to have something to flush
            sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "INSERT INTO test_mem(data, value) VALUES ('cache_test', 2.0);", 
                       nullptr, nullptr, nullptr);
            
            // Attempt to flush caches
            int flush_result = sqlite3_db_cacheflush(db);
            
            // Commit or rollback
            if (fdp.ConsumeBool()) {
                sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
            } else {
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            }
            
            // Try flushing again after transaction
            sqlite3_db_cacheflush(db);
            break;
        }
        
        case MEM_OP_MEMORY_USED: {
            // Test sqlite3_memory_used
            sqlite3_int64 used = sqlite3_memory_used();
            
            // Generate more activity and check again
            generate_memory_activity(db, fdp);
            sqlite3_int64 used_after = sqlite3_memory_used();
            break;
        }
        
        case MEM_OP_MEMORY_HIGHWATER: {
            // Test sqlite3_memory_highwater
            bool reset = fdp.ConsumeBool();
            sqlite3_int64 highwater = sqlite3_memory_highwater(reset ? 1 : 0);
            
            // Generate activity and check again
            generate_memory_activity(db, fdp);
            sqlite3_int64 highwater_after = sqlite3_memory_highwater(0);
            break;
        }
        
        case MEM_OP_RELEASE_MEMORY: {
            // Test sqlite3_release_memory
            int bytes_to_free = fdp.ConsumeIntegralInRange<int>(0, 1048576); // Up to 1MB
            int bytes_freed = sqlite3_release_memory(bytes_to_free);
            
            // Generate more activity and try again
            generate_memory_activity(db, fdp);
            sqlite3_release_memory(bytes_to_free / 2);
            break;
        }
        
        case MEM_OP_GLOBAL_STATUS: {
            // Test sqlite3_status with various operation codes
            StatusOperation status_op = static_cast<StatusOperation>(
                fdp.ConsumeIntegralInRange<int>(0, STATUS_MALLOC_COUNT));
            
            int current = 0;
            int highwater = 0;
            bool reset = fdp.ConsumeBool();
            
            int result = sqlite3_status(static_cast<int>(status_op), &current, &highwater, 
                                       reset ? 1 : 0);
            
            // Test a few more status operations
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                StatusOperation extra_op = static_cast<StatusOperation>(
                    fdp.ConsumeIntegralInRange<int>(0, STATUS_MALLOC_COUNT));
                int extra_current = 0, extra_highwater = 0;
                sqlite3_status(static_cast<int>(extra_op), &extra_current, &extra_highwater, 0);
            }
            break;
        }
        
        case MEM_OP_GLOBAL_STATUS64: {
            // Test sqlite3_status64 with various operation codes
            StatusOperation status_op = static_cast<StatusOperation>(
                fdp.ConsumeIntegralInRange<int>(0, STATUS_MALLOC_COUNT));
            
            sqlite3_int64 current = 0;
            sqlite3_int64 highwater = 0;
            bool reset = fdp.ConsumeBool();
            
            int result = sqlite3_status64(static_cast<int>(status_op), &current, &highwater, 
                                         reset ? 1 : 0);
            
            // Test a few more status operations
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                StatusOperation extra_op = static_cast<StatusOperation>(
                    fdp.ConsumeIntegralInRange<int>(0, STATUS_MALLOC_COUNT));
                sqlite3_int64 extra_current = 0, extra_highwater = 0;
                sqlite3_status64(static_cast<int>(extra_op), &extra_current, &extra_highwater, 0);
            }
            break;
        }
        
        case MEM_OP_DB_STATUS: {
            // Test sqlite3_db_status with various operation codes
            DbStatusOperation db_status_op = static_cast<DbStatusOperation>(
                fdp.ConsumeIntegralInRange<int>(0, DBSTATUS_CACHE_SPILL));
            
            int current = 0;
            int highwater = 0;
            bool reset = fdp.ConsumeBool();
            
            int result = sqlite3_db_status(db, static_cast<int>(db_status_op), &current, &highwater, 
                                          reset ? 1 : 0);
            
            // Test a few more database status operations
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                DbStatusOperation extra_op = static_cast<DbStatusOperation>(
                    fdp.ConsumeIntegralInRange<int>(0, DBSTATUS_CACHE_SPILL));
                int extra_current = 0, extra_highwater = 0;
                sqlite3_db_status(db, static_cast<int>(extra_op), &extra_current, &extra_highwater, 0);
            }
            break;
        }
        
        case MEM_OP_DB_STATUS64: {
            // Test sqlite3_db_status64 with various operation codes
            DbStatusOperation db_status_op = static_cast<DbStatusOperation>(
                fdp.ConsumeIntegralInRange<int>(0, DBSTATUS_CACHE_SPILL));
            
            sqlite3_int64 current = 0;
            sqlite3_int64 highwater = 0;
            bool reset = fdp.ConsumeBool();
            
            int result = sqlite3_db_status64(db, static_cast<int>(db_status_op), &current, &highwater, 
                                            reset ? 1 : 0);
            
            // Test a few more database status operations
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                DbStatusOperation extra_op = static_cast<DbStatusOperation>(
                    fdp.ConsumeIntegralInRange<int>(0, DBSTATUS_CACHE_SPILL));
                sqlite3_int64 extra_current = 0, extra_highwater = 0;
                sqlite3_db_status64(db, static_cast<int>(extra_op), &extra_current, &extra_highwater, 0);
            }
            break;
        }
        
        case MEM_OP_COMBINED_TEST: {
            // Combined test of multiple memory operations
            sqlite3_int64 soft_limit = fdp.ConsumeIntegral<sqlite3_int64>();
            sqlite3_int64 hard_limit = fdp.ConsumeIntegral<sqlite3_int64>();
            
            sqlite3_soft_heap_limit64(soft_limit);
            sqlite3_hard_heap_limit64(hard_limit);
            
            // Get memory statistics
            sqlite3_int64 mem_used = sqlite3_memory_used();
            sqlite3_int64 mem_highwater = sqlite3_memory_highwater(0);
            
            // Get global status
            int malloc_current = 0, malloc_highwater = 0;
            sqlite3_status(STATUS_MALLOC_COUNT, &malloc_current, &malloc_highwater, 0);
            
            // Get database status
            int cache_current = 0, cache_highwater = 0;
            sqlite3_db_status(db, DBSTATUS_CACHE_USED, &cache_current, &cache_highwater, 0);
            
            // Flush caches
            sqlite3_db_cacheflush(db);
            
            // Release memory
            sqlite3_release_memory(fdp.ConsumeIntegralInRange<int>(0, 65536));
            
            // Restore limits
            sqlite3_soft_heap_limit64(-1);
            sqlite3_hard_heap_limit64(-1);
            break;
        }
    }
    
    // Clean up
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}

} // namespace harness_013
