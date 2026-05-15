#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"
#include "sqlite3session.h"

namespace harness_011 {
/*
 * Fuzzing harness for SQLite library - targeting Session Extension APIs
 * This harness targets SQLite session extension for change tracking and synchronization,
 * which are semantically distinct from existing harnesses (000-010).
 * 
 * This harness targets SQLite session extension APIs:
 * - sqlite3session_create - creates session object for change tracking
 * - sqlite3session_attach - attaches tables to session for monitoring
 * - sqlite3session_changeset - generates changeset from recorded changes
 * - sqlite3session_patchset - generates patchset from recorded changes
 * - sqlite3session_diff - computes difference between two tables
 * - sqlite3session_table_filter - sets table filter callback
 * - sqlite3session_indirect - marks changes as indirect
 * - sqlite3session_isempty - checks if session has recorded changes
 * - sqlite3session_memory_used - returns memory used by session
 * - sqlite3session_config - global session configuration
 * - sqlite3session_enable - enables/disables change recording
 * - sqlite3session_delete - deletes session object
 * 
 * Following the invocation sequence:
 * 1. Initialize SQLite and open database
 * 2. Create tables and insert/update/delete data
 * 3. Create session object using sqlite3session_create
 * 4. Configure session with sqlite3session_enable, sqlite3session_indirect
 * 5. Attach tables to session using sqlite3session_attach
 * 6. Perform more database operations to generate changes
 * 7. Generate changeset using sqlite3session_changeset
 * 8. Generate patchset using sqlite3session_patchset
 * 9. Check session status with sqlite3session_isempty, sqlite3session_memory_used
 * 10. Compute table differences with sqlite3session_diff
 * 11. Test session configuration with sqlite3session_config
 * 12. Clean up session and database resources
 * 
 * Semantic differentiation from existing harnesses:
 * - Focuses specifically on session extension for change tracking vs JSON (010), FTS5 (008), R*Tree (009)
 * - Targets change recording, changeset generation, and synchronization APIs
 * - Exercises session management and configuration functions
 * - Tests table diffing and patchset generation for data synchronization
 */


// Include session header if available
#ifdef SQLITE_ENABLE_SESSION
#define HAS_SESSION 1
#else
#define HAS_SESSION 0
#endif

// Table filter callback for testing
#if HAS_SESSION
static int session_table_filter(void* pCtx, const char* zTab) {
    // Simple filter that accepts tables with "test" in name
    if (zTab && strstr(zTab, "test") != nullptr) {
        return 1; // Accept table
    }
    return 0; // Reject table
}
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Global initialization
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    
    // Use in-memory database for fuzzing
    int rc = sqlite3_open_v2(":memory:", &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                             nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set limits to prevent excessive resource usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
#if HAS_SESSION
    // Step 2: Create test tables
    const char* create_table1_sql = 
        "CREATE TABLE test_table1 (id INTEGER PRIMARY KEY, name TEXT, value INTEGER);";
    const char* create_table2_sql = 
        "CREATE TABLE test_table2 (id INTEGER PRIMARY KEY, data TEXT, flag INTEGER);";
    const char* create_table3_sql = 
        "CREATE TABLE other_table (id INTEGER PRIMARY KEY, content TEXT);";
    
    rc = sqlite3_exec(db, create_table1_sql, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, create_table2_sql, nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, create_table3_sql, nullptr, nullptr, nullptr);
    
    // Insert initial data into tables
    // Use fuzzer input to determine number of rows and values
    int num_rows = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int i = 0; i < num_rows && fdp.remaining_bytes() > 0; i++) {
        std::string name = fdp.ConsumeRandomLengthString(32);
        int value = fdp.ConsumeIntegral<int>();
        
        char insert_sql[256];
        sqlite3_snprintf(sizeof(insert_sql), insert_sql,
                        "INSERT INTO test_table1 (name, value) VALUES ('%q', %d);",
                        name.c_str(), value);
        rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
    }
    
    // Step 3: Create session object
    sqlite3_session* pSession = nullptr;
    rc = sqlite3session_create(db, "main", &pSession);
    
    if (rc != SQLITE_OK || pSession == nullptr) {
        // Clean up and exit if session creation failed
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 4: Configure session
    // Enable/disable session based on fuzzer input
    bool enable_session = fdp.ConsumeBool();
    rc = sqlite3session_enable(pSession, enable_session ? 1 : 0);
    
    // Set indirect flag based on fuzzer input
    bool indirect_changes = fdp.ConsumeBool();
    rc = sqlite3session_indirect(pSession, indirect_changes ? 1 : 0);
    
    // Set table filter callback
    void* filter_ctx = nullptr;
    sqlite3session_table_filter(pSession, session_table_filter, filter_ctx);
    
    // Step 5: Attach tables to session
    rc = sqlite3session_attach(pSession, "test_table1");
    rc = sqlite3session_attach(pSession, "test_table2");
    // Note: other_table should be filtered out by our callback
    
    // Step 6: Perform more database operations to generate changes
    // Only if session is enabled
    if (enable_session) {
        // Insert more rows
        int more_rows = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < more_rows && fdp.remaining_bytes() > 0; i++) {
            std::string name = fdp.ConsumeRandomLengthString(32);
            int value = fdp.ConsumeIntegral<int>();
            
            char insert_sql[256];
            sqlite3_snprintf(sizeof(insert_sql), insert_sql,
                            "INSERT INTO test_table1 (name, value) VALUES ('%q', %d);",
                            name.c_str(), value);
            rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
        }
        
        // Update some rows
        int update_count = fdp.ConsumeIntegralInRange<int>(0, 3);
        for (int i = 0; i < update_count && fdp.remaining_bytes() > 0; i++) {
            std::string new_name = fdp.ConsumeRandomLengthString(32);
            int new_value = fdp.ConsumeIntegral<int>();
            int row_id = fdp.ConsumeIntegralInRange<int>(1, num_rows);
            
            char update_sql[256];
            sqlite3_snprintf(sizeof(update_sql), update_sql,
                            "UPDATE test_table1 SET name='%q', value=%d WHERE id=%d;",
                            new_name.c_str(), new_value, row_id);
            rc = sqlite3_exec(db, update_sql, nullptr, nullptr, nullptr);
        }
        
        // Delete some rows
        int delete_count = fdp.ConsumeIntegralInRange<int>(0, 2);
        for (int i = 0; i < delete_count && fdp.remaining_bytes() > 0; i++) {
            int row_id = fdp.ConsumeIntegralInRange<int>(1, num_rows + more_rows);
            
            char delete_sql[256];
            sqlite3_snprintf(sizeof(delete_sql), delete_sql,
                            "DELETE FROM test_table1 WHERE id=%d;", row_id);
            rc = sqlite3_exec(db, delete_sql, nullptr, nullptr, nullptr);
        }
    }
    
    // Step 7: Generate changeset
    int changeset_size = 0;
    void* pChangeset = nullptr;
    rc = sqlite3session_changeset(pSession, &changeset_size, &pChangeset);
    
    // If we got a changeset, test some operations on it
    if (rc == SQLITE_OK && pChangeset != nullptr && changeset_size > 0) {
        // Create a second database to apply changeset
        sqlite3* db2 = nullptr;
        rc = sqlite3_open_v2(":memory:", &db2,
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                            nullptr);
        
        if (rc == SQLITE_OK && db2 != nullptr) {
            // Create same table structure in second database
            rc = sqlite3_exec(db2, create_table1_sql, nullptr, nullptr, nullptr);
            rc = sqlite3_exec(db2, create_table2_sql, nullptr, nullptr, nullptr);
            
            // Apply changeset (simplified - in real scenario would use changeset apply API)
            // For fuzzing, we just test that we can generate it
            
            sqlite3_close(db2);
        }
        
        // Free changeset memory
        sqlite3_free(pChangeset);
    }
    
    // Step 8: Generate patchset
    int patchset_size = 0;
    void* pPatchset = nullptr;
    rc = sqlite3session_patchset(pSession, &patchset_size, &pPatchset);
    
    if (rc == SQLITE_OK && pPatchset != nullptr && patchset_size > 0) {
        // Free patchset memory
        sqlite3_free(pPatchset);
    }
    
    // Step 9: Check session status
    int is_empty = sqlite3session_isempty(pSession);
    sqlite3_int64 memory_used = sqlite3session_memory_used(pSession);
    
    // Step 10: Compute table differences
    // Create a copy of test_table1 with modified data for diff testing
    if (fdp.remaining_bytes() > 0) {
        // Create temporary table with same structure but different data
        rc = sqlite3_exec(db, "CREATE TABLE temp.test_table1_copy AS SELECT * FROM main.test_table1;", 
                         nullptr, nullptr, nullptr);
        
        // Modify some data in the copy
        int modify_rows = fdp.ConsumeIntegralInRange<int>(0, 3);
        for (int i = 0; i < modify_rows && fdp.remaining_bytes() > 0; i++) {
            std::string modified_name = fdp.ConsumeRandomLengthString(32);
            int modified_value = fdp.ConsumeIntegral<int>();
            int row_id = fdp.ConsumeIntegralInRange<int>(1, num_rows + more_rows - delete_count);
            
            char update_sql[256];
            sqlite3_snprintf(sizeof(update_sql), update_sql,
                            "UPDATE temp.test_table1_copy SET name='%q', value=%d WHERE id=%d;",
                            modified_name.c_str(), modified_value, row_id);
            rc = sqlite3_exec(db, update_sql, nullptr, nullptr, nullptr);
        }
        
        // Compute diff between main.test_table1 and temp.test_table1_copy
        rc = sqlite3session_diff(pSession, "temp", "test_table1", nullptr);
        
        // Clean up temporary table
        rc = sqlite3_exec(db, "DROP TABLE temp.test_table1_copy;", nullptr, nullptr, nullptr);
    }
    
    // Step 11: Test session configuration
    // Test global session configuration if enough bytes remain
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int config_op = fdp.ConsumeIntegral<int>();
        void* config_arg = nullptr;
        
        // Try session config with different operations
        // Note: actual valid opcodes would need to be determined from header
        sqlite3session_config(config_op, config_arg);
    }
    
    // Step 12: Clean up
    sqlite3session_delete(pSession);
    
#else // HAS_SESSION == 0
    // If session extension not available, do minimal testing
    // Create and drop a simple table
    const char* create_sql = "CREATE TABLE simple_test (id INTEGER);";
    rc = sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    
    if (fdp.remaining_bytes() > 0) {
        std::string insert_data = fdp.ConsumeRandomLengthString(100);
        // Not actually using insert_data, just consuming input
    }
    
    const char* drop_sql = "DROP TABLE simple_test;";
    rc = sqlite3_exec(db, drop_sql, nullptr, nullptr, nullptr);
#endif // HAS_SESSION
    
    // Final cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_011
