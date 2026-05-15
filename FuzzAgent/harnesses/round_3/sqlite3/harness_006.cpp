#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_006 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for WAL checkpoint operations
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: wal_checkpoint, 1: wal_checkpoint_v2, 2: db_cacheflush, 3: combination
  uint8_t checkpoint_mode = fdp.ConsumeIntegral<uint8_t>() % 5;  // 0-4 for different checkpoint modes
  uint8_t db_selector = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: main, 1: temp, 2: NULL (all databases)
  bool enable_wal = fdp.ConsumeBool();
  bool create_temp_db = fdp.ConsumeBool();
  bool perform_writes = fdp.ConsumeBool();
  
  // Consume variable-size data
  std::string table_data = fdp.ConsumeRandomLengthString(512);
  std::string remaining_input = fdp.ConsumeRemainingBytesAsString();

  sqlite3* db = nullptr;
  char* err_msg = nullptr;
  int rc;

  // Initialize SQLite
  if (sqlite3_initialize() != SQLITE_OK) {
    return 0;
  }

  // Open an in-memory database
  rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, nullptr);
  if (rc != SQLITE_OK || db == nullptr) {
    sqlite3_shutdown();
    return 0;
  }

  // Set memory limits
  sqlite3_hard_heap_limit64(50000000); // 50MB limit

  // Enable WAL mode if requested
  if (enable_wal) {
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Create a temporary database if requested
  if (create_temp_db) {
    rc = sqlite3_exec(db, "ATTACH DATABASE ':memory:' AS temp_db;", nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
    
    // Create table in temp database
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS temp_db.test_table (id INTEGER PRIMARY KEY, data TEXT);", 
                     nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Create table in main database and perform writes to generate WAL content
  rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT, value REAL);", 
                   nullptr, nullptr, &err_msg);
  if (err_msg != nullptr) {
    sqlite3_free(err_msg);
    err_msg = nullptr;
  }

  // Perform some writes to generate WAL entries
  if (perform_writes) {
    // Consume number of inserts from fuzzed data
    int num_inserts = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < num_inserts; i++) {
      // Consume value for this insert
      double insert_value = fdp.ConsumeFloatingPoint<double>();
      
      std::string insert_sql = "INSERT INTO test_table (data, value) VALUES ('" + 
                              table_data.substr(0, std::min((size_t)50, table_data.size())) + 
                              "', " + std::to_string(insert_value) + ");";
      rc = sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      // Break early if we run out of fuzzed data
      if (fdp.remaining_bytes() < sizeof(double)) {
        break;
      }
    }
    
    // Consume update multiplier from fuzzed data
    double update_multiplier = fdp.ConsumeFloatingPoint<double>();
    std::string update_sql = "UPDATE test_table SET value = value * " + 
                            std::to_string(update_multiplier) + " WHERE id % 2 = 0;";
    rc = sqlite3_exec(db, update_sql.c_str(), nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Determine database name based on selector
  const char* db_name = nullptr;
  switch (db_selector) {
    case 0:
      db_name = "main";
      break;
    case 1:
      db_name = "temp";
      break;
    case 2:
      db_name = nullptr;  // NULL means all attached databases
      break;
    default:
      db_name = "main";
  }

  // Determine checkpoint mode
  int checkpoint_mode_value;
  switch (checkpoint_mode) {
    case 0:
      checkpoint_mode_value = SQLITE_CHECKPOINT_PASSIVE;
      break;
    case 1:
      checkpoint_mode_value = SQLITE_CHECKPOINT_FULL;
      break;
    case 2:
      checkpoint_mode_value = SQLITE_CHECKPOINT_RESTART;
      break;
    case 3:
      checkpoint_mode_value = SQLITE_CHECKPOINT_TRUNCATE;
      break;
    case 4:
      checkpoint_mode_value = SQLITE_CHECKPOINT_NOOP;
      break;
    default:
      checkpoint_mode_value = SQLITE_CHECKPOINT_PASSIVE;
  }

  // Test different WAL checkpoint operations
  switch (operation) {
    case 0: {
      // Test sqlite3_wal_checkpoint (legacy API)
      rc = sqlite3_wal_checkpoint(db, db_name);
      // Return code is not checked as it depends on database state
      break;
    }
    
    case 1: {
      // Test sqlite3_wal_checkpoint_v2 with output parameters
      int pnLog = 0;
      int pnCkpt = 0;
      rc = sqlite3_wal_checkpoint_v2(db, db_name, checkpoint_mode_value, &pnLog, &pnCkpt);
      // Output parameters may contain useful information
      (void)pnLog;
      (void)pnCkpt;
      break;
    }
    
    case 2: {
      // Test sqlite3_db_cacheflush
      rc = sqlite3_db_cacheflush(db);
      break;
    }
    
    case 3: {
      // Test combination of operations
      // First checkpoint
      int pnLog1 = 0, pnCkpt1 = 0;
      rc = sqlite3_wal_checkpoint_v2(db, db_name, checkpoint_mode_value, &pnLog1, &pnCkpt1);
      
      // Then cache flush
      rc = sqlite3_db_cacheflush(db);
      
      // Then legacy checkpoint
      rc = sqlite3_wal_checkpoint(db, db_name);
      break;
    }
    
    default:
      // Should not reach here
      break;
  }

  // Clean up
  if (create_temp_db) {
    rc = sqlite3_exec(db, "DETACH DATABASE temp_db;", nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Close database
  if (db != nullptr) {
    sqlite3_close(db);
  }

  // Shutdown SQLite
  sqlite3_shutdown();

  return 0;
}

} // namespace harness_006
