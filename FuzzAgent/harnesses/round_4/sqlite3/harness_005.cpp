#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"
#include <unistd.h>

namespace harness_005 {
/*
 * Fuzzing harness for SQLite library - targeting Write-Ahead Log (WAL) checkpoint operations
 * and database maintenance functions as identified by coverage analysis.
 * 
 * This harness targets:
 * - sqlite3_wal_checkpoint_v2 (primary target: 903 undiscovered branches)
 * - sqlite3_wal_checkpoint (calls v2)
 * - sqlite3_db_cacheflush (574 undiscovered branches)
 * - Helper functions: sqlite3_db_readonly, sqlite3_db_filename
 * 
 * Following the invocation sequence:
 * 1. sqlite3_open_v2 create/open database
 * 2. Execute SQL statements to create tables and insert data (generating WAL content)
 * 3. Call sqlite3_db_readonly and sqlite3_db_filename to check database state
 * 4. Call sqlite3_wal_checkpoint or sqlite3_wal_checkpoint_v2 with different modes
 * 5. Call sqlite3_db_cacheflush to flush database cache
 * 6. sqlite3_close cleanup
 * 
 * Semantic differentiation from existing harnesses:
 * - Focuses on WAL checkpoint operations vs prepared statements, BLOB ops, backup, virtual tables, serialization
 * - Tests different checkpoint modes (PASSIVE, FULL, RESTART)
 * - Exercises database maintenance functions
 */


#ifdef __unix__
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need at least: operation type + some SQL content
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Determine database type: memory or temporary file
    uint8_t db_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    sqlite3* db = nullptr;
    int rc = SQLITE_OK;
    
    const char* db_name = nullptr;
    char temp_filename[] = "/tmp/sqlite_fuzz_XXXXXX";
    
    if (db_type == 0) {
        // Use in-memory database
        db_name = ":memory:";
        rc = sqlite3_open_v2(db_name, &db,
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                            nullptr);
    } else if (db_type == 1) {
        // Use temporary file database (auto-deleted on close)
        db_name = "";
        rc = sqlite3_open_v2("", &db,
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_TEMP_DB,
                            nullptr);
    } else {
        // Use named temporary file
        int fd = mkstemp(temp_filename);
        if (fd != -1) {
#ifdef __unix__
            close(fd);
#endif
            db_name = temp_filename;
            rc = sqlite3_open_v2(db_name, &db,
                                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                nullptr);
        } else {
            sqlite3_shutdown();
            return 0;
        }
    }
    
    if (rc != SQLITE_OK || db == nullptr) {
        if (db_type == 2) {
#ifdef __unix__
            unlink(temp_filename);
#endif
        }
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits to prevent timeouts and excessive memory usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Enable WAL mode for the database to generate WAL content
    // This is necessary for checkpoint functions to have meaningful work
    char* errmsg = nullptr;
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
    }
    
    // Consume SQL statements from fuzzer input
    // Generate tables and insert data to create WAL content
    size_t sql_remaining = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes() / 2);
    std::string create_table_sql = fdp.ConsumeBytesAsString(sql_remaining);
    
    if (!create_table_sql.empty()) {
        sqlite3_exec(db, create_table_sql.c_str(), nullptr, nullptr, &errmsg);
        if (errmsg != nullptr) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
    } else {
        // Default SQL to create a table and insert some data using fuzzer input
        sqlite3_exec(db, 
            "CREATE TABLE IF NOT EXISTS fuzz_data (id INTEGER PRIMARY KEY, name TEXT, value REAL);",
            nullptr, nullptr, &errmsg);
        if (errmsg != nullptr) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
        
        // Insert data using fuzzer input
        if (fdp.remaining_bytes() >= 8) {
            // Consume some data for test values
            std::string name1 = fdp.ConsumeRandomLengthString(16);
            double value1 = fdp.ConsumeFloatingPoint<double>();
            
            // Build insert statement with consumed data
            char insert_sql1[256];
            snprintf(insert_sql1, sizeof(insert_sql1),
                    "INSERT INTO fuzz_data (name, value) VALUES ('%s', %f);",
                    name1.c_str(), value1);
            sqlite3_exec(db, insert_sql1, nullptr, nullptr, &errmsg);
            if (errmsg != nullptr) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
            
            // Insert more data if available
            if (fdp.remaining_bytes() >= 8) {
                std::string name2 = fdp.ConsumeRandomLengthString(16);
                double value2 = fdp.ConsumeFloatingPoint<double>();
                
                char insert_sql2[256];
                snprintf(insert_sql2, sizeof(insert_sql2),
                        "INSERT INTO fuzz_data (name, value) VALUES ('%s', %f);",
                        name2.c_str(), value2);
                sqlite3_exec(db, insert_sql2, nullptr, nullptr, &errmsg);
                if (errmsg != nullptr) {
                    sqlite3_free(errmsg);
                    errmsg = nullptr;
                }
            }
        }
    }
    
    // Insert more data using fuzzer input
    size_t insert_remaining = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    if (insert_remaining > 0) {
        std::string insert_sql = fdp.ConsumeBytesAsString(insert_remaining);
        if (!insert_sql.empty()) {
            sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &errmsg);
            if (errmsg != nullptr) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
        }
    }
    
    // Call database state checking functions
    // sqlite3_db_readonly - check if database is read-only
    int is_readonly = sqlite3_db_readonly(db, "main");
    (void)is_readonly; // Mark as used to avoid compiler warning
    
    // sqlite3_db_filename - get database filename
    const char* filename = (const char*)sqlite3_db_filename(db, "main");
    (void)filename; // Mark as used
    
    // Determine checkpoint operation type from fuzzer input
    uint8_t checkpoint_op = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Checkpoint mode constants
    const int checkpoint_modes[] = {
        SQLITE_CHECKPOINT_PASSIVE,
        SQLITE_CHECKPOINT_FULL,
        SQLITE_CHECKPOINT_RESTART,
        SQLITE_CHECKPOINT_TRUNCATE
    };
    
    // Determine database name for checkpoint (main, temp, or NULL)
    uint8_t db_name_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    const char* checkpoint_db_name = nullptr;
    if (db_name_choice == 0) {
        checkpoint_db_name = "main";
    } else if (db_name_choice == 1) {
        checkpoint_db_name = "temp";
    }
    // else: NULL (all attached databases)
    
    int pnLog = 0;
    int pnCkpt = 0;
    
    switch (checkpoint_op) {
        case 0:
            // Test sqlite3_wal_checkpoint_v2 with PASSIVE mode
            sqlite3_wal_checkpoint_v2(db, checkpoint_db_name,
                                     SQLITE_CHECKPOINT_PASSIVE,
                                     &pnLog, &pnCkpt);
            break;
            
        case 1:
            // Test sqlite3_wal_checkpoint_v2 with FULL mode
            sqlite3_wal_checkpoint_v2(db, checkpoint_db_name,
                                     SQLITE_CHECKPOINT_FULL,
                                     &pnLog, &pnCkpt);
            break;
            
        case 2:
            // Test sqlite3_wal_checkpoint_v2 with RESTART mode
            sqlite3_wal_checkpoint_v2(db, checkpoint_db_name,
                                     SQLITE_CHECKPOINT_RESTART,
                                     &pnLog, &pnCkpt);
            break;
            
        case 3:
            // Test sqlite3_wal_checkpoint_v2 with TRUNCATE mode
            sqlite3_wal_checkpoint_v2(db, checkpoint_db_name,
                                     SQLITE_CHECKPOINT_TRUNCATE,
                                     &pnLog, &pnCkpt);
            break;
            
        case 4:
            // Test sqlite3_wal_checkpoint (simpler interface)
            sqlite3_wal_checkpoint(db, checkpoint_db_name);
            break;
            
        case 5:
            // Test multiple checkpoint modes based on remaining input
            for (int i = 0; i < 2 && fdp.remaining_bytes() > 0; i++) {
                int mode_idx = fdp.ConsumeIntegralInRange<int>(0, 3);
                sqlite3_wal_checkpoint_v2(db, checkpoint_db_name,
                                         checkpoint_modes[mode_idx],
                                         &pnLog, &pnCkpt);
            }
            break;
    }
    
    // Call sqlite3_db_cacheflush to flush database cache
    // This function may return SQLITE_BUSY or SQLITE_OK
    int flush_rc = sqlite3_db_cacheflush(db);
    (void)flush_rc; // Mark as used
    
    // Optionally perform more operations after checkpoint
    if (fdp.remaining_bytes() > 0) {
        // Consume remaining input for additional SQL operations
        std::string remaining_sql = fdp.ConsumeRemainingBytesAsString();
        if (!remaining_sql.empty()) {
            sqlite3_exec(db, remaining_sql.c_str(), nullptr, nullptr, &errmsg);
            if (errmsg != nullptr) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
            
            // Try another checkpoint after additional operations
            if (fdp.remaining_bytes() == 0) { // Check if we actually consumed everything
                int final_mode = SQLITE_CHECKPOINT_PASSIVE;
                sqlite3_wal_checkpoint_v2(db, "main", final_mode, &pnLog, &pnCkpt);
            }
        }
    }
    
    // Cleanup
    sqlite3_close(db);
    
    if (db_type == 2) {
#ifdef __unix__
        unlink(temp_filename);
#endif
    }
    
    sqlite3_shutdown();
    return 0;
}

} // namespace harness_005
