#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for serialization/deserialization operations
  if (size < 64) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;  // 4 different operation types
  uint8_t schema_selector = fdp.ConsumeIntegral<uint8_t>() % 3;  // main, temp, or other
  uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
  int64_t max_serialized_size = fdp.ConsumeIntegralInRange<int64_t>(1024, 1024 * 1024);  // 1KB to 1MB
  int row_limit = fdp.ConsumeIntegralInRange<int>(1, 100);
  int column_limit = fdp.ConsumeIntegralInRange<int>(1, 20);
  
  // Consume variable-size data
  std::string table_name = fdp.ConsumeRandomLengthString(64);
  std::string column_defs = fdp.ConsumeRandomLengthString(256);
  std::string sql_query = fdp.ConsumeRandomLengthString(1024);
  std::string serialization_data = fdp.ConsumeRandomLengthString(max_serialized_size);
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

  // Determine schema based on selector
  const char* schema = "main";
  if (schema_selector == 1) {
    schema = "temp";
  } else if (schema_selector == 2) {
    schema = "main";  // Use main as fallback for other values
  }

  // Create a simple table for testing
  std::string create_sql = "CREATE TABLE IF NOT EXISTS " + table_name + 
                          " (id INTEGER PRIMARY KEY, name TEXT, value REAL, data BLOB);";
  rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
  if (err_msg != nullptr) {
    sqlite3_free(err_msg);
    err_msg = nullptr;
  }

  // Insert some sample data
  for (int i = 0; i < 5; i++) {
    std::string insert_sql = "INSERT INTO " + table_name + " (name, value, data) VALUES ('test" + 
                            std::to_string(i) + "', " + std::to_string(i * 1.5) + ", ?);";
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt != nullptr) {
      std::vector<uint8_t> blob_data(16);
      for (size_t j = 0; j < blob_data.size(); j++) {
        blob_data[j] = static_cast<uint8_t>((i * j) % 256);
      }
      sqlite3_bind_blob(stmt, 1, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
      rc = sqlite3_step(stmt);
      sqlite3_finalize(stmt);
    }
  }

  // Test different operations based on the operation selector
  switch (operation) {
    case 0: {
      // Test serialization: serialize the database to memory
      sqlite3_int64 serialized_size = 0;
      unsigned int serialize_flags = 0;
      
      // Set flags based on fuzzed input
      if (flags & 0x01) {
        serialize_flags |= SQLITE_SERIALIZE_NOCOPY;
      }
      
      unsigned char* serialized_db = sqlite3_serialize(db, schema, &serialized_size, serialize_flags);
      
      if (serialized_db != nullptr) {
        // Do something with the serialized data
        (void)serialized_size;  // Use variable to avoid compiler warning
        
        // If we didn't use NOCOPY flag, we should free the buffer
        if (!(serialize_flags & SQLITE_SERIALIZE_NOCOPY)) {
          sqlite3_free(serialized_db);
        }
      }
      break;
    }
    
    case 1: {
      // Test deserialization: deserialize data into the database
      unsigned int deserialize_flags = 0;
      
      // Set flags based on fuzzed input, but NOT SQLITE_DESERIALIZE_FREEONCLOSE
      // when using std::vector buffers (they're not allocated by sqlite3_malloc64)
      // if (flags & 0x02) {
      //   deserialize_flags |= SQLITE_DESERIALIZE_FREEONCLOSE;  // REMOVED: causes heap-buffer-overflow
      // }
      if (flags & 0x04) {
        deserialize_flags |= SQLITE_DESERIALIZE_RESIZEABLE;
      }
      if (flags & 0x08) {
        deserialize_flags |= SQLITE_DESERIALIZE_READONLY;
      }
      
      // Prepare data for deserialization
      std::vector<unsigned char> deserialize_data;
      if (!serialization_data.empty()) {
        deserialize_data.assign(serialization_data.begin(), serialization_data.end());
      } else {
        // Fallback: use some minimal valid data
        deserialize_data = {0x53, 0x51, 0x4c, 0x69, 0x74, 0x65, 0x20, 0x66, 0x6f, 0x72, 0x6d, 0x61, 0x74, 0x20, 0x33, 0x00};
      }
      
      if (!deserialize_data.empty()) {
        rc = sqlite3_deserialize(db, schema, deserialize_data.data(),
                                deserialize_data.size(), deserialize_data.size() * 2,
                                deserialize_flags);
        // Ignore result - we're fuzzing, not expecting success
      }
      break;
    }
    
    case 2: {
      // Test sqlite3_get_table with a simple query
      char** result_table = nullptr;
      int nrow = 0;
      int ncol = 0;
      
      std::string query = "SELECT * FROM " + table_name + " LIMIT " + std::to_string(row_limit) + ";";
      rc = sqlite3_get_table(db, query.c_str(), &result_table, &nrow, &ncol, &err_msg);
      
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      // Always free the table if it was allocated
      if (result_table != nullptr) {
        sqlite3_free_table(result_table);
      }
      break;
    }
    
    case 3: {
      // Combined test: serialize, then deserialize, then query
      sqlite3_int64 serialized_size = 0;
      unsigned char* serialized_db = sqlite3_serialize(db, schema, &serialized_size, 0);
      
      if (serialized_db != nullptr && serialized_size > 0) {
        // Create a new database connection for deserialization test
        sqlite3* db2 = nullptr;
        rc = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, nullptr);
        
        if (rc == SQLITE_OK && db2 != nullptr) {
          // Try to deserialize into the new database
          rc = sqlite3_deserialize(db2, schema, serialized_db, serialized_size, serialized_size * 2, 0);
          
          // If successful, try to query from the deserialized database
          if (rc == SQLITE_OK) {
            char** result_table = nullptr;
            int nrow = 0;
            int ncol = 0;
            std::string query = "SELECT COUNT(*) FROM " + table_name + ";";
            rc = sqlite3_get_table(db2, query.c_str(), &result_table, &nrow, &ncol, &err_msg);
            
            if (err_msg != nullptr) {
              sqlite3_free(err_msg);
              err_msg = nullptr;
            }
            
            if (result_table != nullptr) {
              sqlite3_free_table(result_table);
            }
          }
          
          sqlite3_close(db2);
        }
        
        // Free the serialized buffer
        sqlite3_free(serialized_db);
      }
      break;
    }
  }

  // Clean up
  if (db != nullptr) {
    sqlite3_close(db);
  }
  
  sqlite3_shutdown();
  return 0;
}

} // namespace harness_003
