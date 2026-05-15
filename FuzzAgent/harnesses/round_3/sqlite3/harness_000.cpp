#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_000 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 8) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t db_flags = fdp.ConsumeIntegral<uint8_t>();
  uint8_t open_flags = fdp.ConsumeIntegral<uint8_t>();
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
  int limit_vdbe_ops = fdp.ConsumeIntegralInRange<int>(100, 10000);
  int limit_length = fdp.ConsumeIntegralInRange<int>(1000, 50000);
  bool enable_fkey = fdp.ConsumeBool();
  bool enable_debug = fdp.ConsumeBool();
  
  // Consume variable-size data
  std::string sql_statement = fdp.ConsumeRandomLengthString(4096);
  std::string remaining_input = fdp.ConsumeRemainingBytesAsString();

  sqlite3* db = nullptr;
  char* err_msg = nullptr;
  int rc;

  // Initialize SQLite
  if (sqlite3_initialize() != SQLITE_OK) {
    return 0;
  }

  // Open database with various flags
  unsigned int open_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
  
  // Add optional flags based on fuzzed input
  if (open_flags & 0x01) open_mode |= SQLITE_OPEN_NOMUTEX;
  if (open_flags & 0x02) open_mode |= SQLITE_OPEN_FULLMUTEX;
  if (open_flags & 0x04) open_mode |= SQLITE_OPEN_PRIVATECACHE;
  if (open_flags & 0x08) open_mode |= SQLITE_OPEN_SHAREDCACHE;

  rc = sqlite3_open_v2(":memory:", &db, open_mode, nullptr);
  if (rc != SQLITE_OK || db == nullptr) {
    sqlite3_shutdown();
    return 0;
  }

  // Configure database based on fuzzed parameters
  sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, limit_vdbe_ops);
  sqlite3_limit(db, SQLITE_LIMIT_LENGTH, limit_length);
  sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, enable_fkey ? 1 : 0, &rc);
  
  // Set memory limits
  sqlite3_hard_heap_limit64(50000000); // 50MB limit

  // Test different operations based on the operation selector
  switch (operation) {
    case 0: {
      // Test sqlite3_exec with the SQL statement
      rc = sqlite3_exec(db, sql_statement.c_str(), nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      // Also test with a callback function
      auto exec_callback = [](void* unused, int argc, char** argv, char** col_names) -> int {
        return 0;
      };
      rc = sqlite3_exec(db, "SELECT 1, 2, 3;", exec_callback, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      break;
    }
    
    case 1: {
      // Test prepared statements
      sqlite3_stmt* stmt = nullptr;
      const char* tail = nullptr;
      
      rc = sqlite3_prepare_v2(db, sql_statement.c_str(), -1, &stmt, &tail);
      if (rc == SQLITE_OK && stmt != nullptr) {
        // Step through results
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
          // Get column count
          int col_count = sqlite3_column_count(stmt);
          for (int i = 0; i < col_count; i++) {
            // Access column data (but don't use it meaningfully to avoid crashes)
            int col_type = sqlite3_column_type(stmt, i);
            // Just touch the column to ensure it's accessed
            (void)col_type;
          }
        }
        sqlite3_finalize(stmt);
      }
      break;
    }
    
    case 2: {
      // Test multiple operations - create tables and insert data with fuzzed schema
      std::string create_table = "CREATE TABLE IF NOT EXISTS t" + 
                                std::to_string(fdp.ConsumeIntegral<uint8_t>() % 10) + "(";
      int num_columns = fdp.ConsumeIntegralInRange<int>(1, 5);
      for (int i = 0; i < num_columns; i++) {
        if (i > 0) create_table += ", ";
        create_table += "col" + std::to_string(i);
        uint8_t col_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (col_type) {
          case 0: create_table += " INTEGER"; break;
          case 1: create_table += " TEXT"; break;
          case 2: create_table += " REAL"; break;
          case 3: create_table += " BLOB"; break;
        }
      }
      create_table += ");";
      
      rc = sqlite3_exec(db, create_table.c_str(), nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      // Insert some data with fuzzed values
      if (num_columns > 0) {
        std::string insert_stmt = "INSERT INTO t" + std::to_string(fdp.ConsumeIntegral<uint8_t>() % 10) + " VALUES(";
        for (int i = 0; i < num_columns; i++) {
          if (i > 0) insert_stmt += ", ";
          uint8_t val_type = fdp.ConsumeIntegral<uint8_t>() % 4;
          switch (val_type) {
            case 0: insert_stmt += std::to_string(fdp.ConsumeIntegral<int>()); break;
            case 1: insert_stmt += "'" + fdp.ConsumeRandomLengthString(50) + "'"; break;
            case 2: insert_stmt += std::to_string(fdp.ConsumeFloatingPoint<double>()); break;
            case 3: insert_stmt += "X'" + fdp.ConsumeRandomLengthString(20) + "'"; break;
          }
        }
        insert_stmt += ");";
        
        rc = sqlite3_exec(db, insert_stmt.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
      }
      
      // Execute the fuzzed SQL
      rc = sqlite3_exec(db, sql_statement.c_str(), nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      break;
    }
    
    case 3: {
      // Test blob and text operations
      // Create a table with various column types
      rc = sqlite3_exec(db, 
        "CREATE TABLE IF NOT EXISTS test_types("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB, "
        "flag BOOLEAN);",
        nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      // Insert data using prepared statement
      sqlite3_stmt* stmt = nullptr;
      rc = sqlite3_prepare_v2(db, 
        "INSERT INTO test_types(name, value, data, flag) VALUES(?, ?, ?, ?);",
        -1, &stmt, nullptr);
      
      if (rc == SQLITE_OK && stmt != nullptr) {
        // Bind parameters
        std::string name = fdp.ConsumeRandomLengthString(100);
        double value = fdp.ConsumeFloatingPoint<double>();
        std::string blob_data = fdp.ConsumeRandomLengthString(500);
        int flag = fdp.ConsumeBool() ? 1 : 0;
        
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 2, value);
        sqlite3_bind_blob(stmt, 3, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, flag);
        
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
      }
      
      // Execute fuzzed SQL
      rc = sqlite3_exec(db, sql_statement.c_str(), nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      break;
    }
  }

  // Test remaining input as additional SQL
  if (!remaining_input.empty()) {
    rc = sqlite3_exec(db, remaining_input.c_str(), nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Test pragmas if debug is enabled
  if (enable_debug) {
    rc = sqlite3_exec(db, "PRAGMA integrity_check;", nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Clean up
  sqlite3_close(db);
  sqlite3_shutdown();
  
  return 0;
}

} // namespace harness_000
