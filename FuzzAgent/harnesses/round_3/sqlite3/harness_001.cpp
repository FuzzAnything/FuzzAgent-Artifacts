#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for BLOB operations
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t blob_flags = fdp.ConsumeIntegral<uint8_t>();
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different BLOB operations
  int64_t row_id = fdp.ConsumeIntegral<int64_t>();
  int64_t reopen_row_id = fdp.ConsumeIntegral<int64_t>();
  int max_blob_size = fdp.ConsumeIntegralInRange<int>(1, 1024 * 1024);  // Up to 1MB
  int offset = fdp.ConsumeIntegralInRange<int>(0, 1000);
  int read_write_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
  bool use_readonly = fdp.ConsumeBool();
  bool use_reopen = fdp.ConsumeBool();

  // Consume variable-size data for table/column names
  std::string db_name = fdp.ConsumeRandomLengthString(64);
  std::string table_name = fdp.ConsumeRandomLengthString(64);
  std::string column_name = fdp.ConsumeRandomLengthString(64);
  std::string blob_data = fdp.ConsumeRandomLengthString(max_blob_size);
  std::string remaining_input = fdp.ConsumeRemainingBytesAsString();

  sqlite3* db = nullptr;
  sqlite3_blob* blob_handle = nullptr;
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

  // Create a table with a BLOB column
  std::string create_sql = "CREATE TABLE IF NOT EXISTS " + table_name + 
                          " (id INTEGER PRIMARY KEY, " + column_name + " BLOB);";
  rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
  if (err_msg != nullptr) {
    sqlite3_free(err_msg);
    err_msg = nullptr;
  }

  // Insert a row with BLOB data to ensure we have something to open
  sqlite3_stmt* stmt = nullptr;
  std::string insert_sql = "INSERT OR REPLACE INTO " + table_name + " (id, " + column_name + ") VALUES(?, ?);";
  rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
  if (rc == SQLITE_OK && stmt != nullptr) {
    sqlite3_bind_int64(stmt, 1, row_id);
    sqlite3_bind_blob(stmt, 2, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;
  }

  // Determine BLOB open flags
  int open_flags = use_readonly ? 0 : 1;  // 0 = readonly, 1 = readwrite

  // Test BLOB operations based on operation selector
  switch (operation) {
    case 0: {
      // Test sqlite3_blob_open
      rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                            row_id, open_flags, &blob_handle);
      
      if (rc == SQLITE_OK && blob_handle != nullptr) {
        // Test sqlite3_blob_bytes
        int blob_size = sqlite3_blob_bytes(blob_handle);
        (void)blob_size;  // Use variable to avoid compiler warning
        
        // Test sqlite3_blob_read
        std::vector<uint8_t> read_buffer(blob_data.size());
        if (blob_data.size() > 0) {
          int read_size = std::min((size_t)read_write_size, blob_data.size());
          rc = sqlite3_blob_read(blob_handle, read_buffer.data(), read_size, 
                                offset % std::max(1, blob_size - read_size));
        }
        
        // Test sqlite3_blob_close
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      break;
    }
    
    case 1: {
      // Test complete BLOB lifecycle: open -> read -> close
      rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                            row_id, open_flags, &blob_handle);
      
      if (rc == SQLITE_OK && blob_handle != nullptr) {
        int blob_size = sqlite3_blob_bytes(blob_handle);
        
        // Read different portions of the BLOB
        if (blob_size > 0) {
          std::vector<uint8_t> buffer1(std::min(256, blob_size));
          std::vector<uint8_t> buffer2(std::min(512, blob_size));
          
          if (buffer1.size() > 0) {
            rc = sqlite3_blob_read(blob_handle, buffer1.data(), buffer1.size(), 0);
          }
          
          if (buffer2.size() > 0 && blob_size > buffer2.size()) {
            rc = sqlite3_blob_read(blob_handle, buffer2.data(), buffer2.size(), 
                                  blob_size - buffer2.size());
          }
        }
        
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      break;
    }
    
    case 2: {
      // Test BLOB write operations (requires readwrite handle)
      if (!use_readonly) {
        rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                              row_id, 1, &blob_handle);  // Force readwrite
        
        if (rc == SQLITE_OK && blob_handle != nullptr) {
          int blob_size = sqlite3_blob_bytes(blob_handle);
          
          if (blob_size > 0) {
            // Write data to BLOB
            std::vector<uint8_t> write_data(std::min(read_write_size, blob_size));
            for (size_t i = 0; i < write_data.size(); ++i) {
              write_data[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            
            if (write_data.size() > 0) {
              rc = sqlite3_blob_write(blob_handle, write_data.data(), write_data.size(), 
                                     offset % std::max(1, blob_size - (int)write_data.size()));
            }
          }
          
          rc = sqlite3_blob_close(blob_handle);
          blob_handle = nullptr;
        }
      }
      break;
    }
    
    case 3: {
      // Test sqlite3_blob_reopen
      rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                            row_id, open_flags, &blob_handle);
      
      if (rc == SQLITE_OK && blob_handle != nullptr && use_reopen) {
        // Insert another row for reopening
        sqlite3_stmt* stmt2 = nullptr;
        std::string insert2_sql = "INSERT OR REPLACE INTO " + table_name + 
                                 " (id, " + column_name + ") VALUES(?, ?);";
        rc = sqlite3_prepare_v2(db, insert2_sql.c_str(), -1, &stmt2, nullptr);
        if (rc == SQLITE_OK && stmt2 != nullptr) {
          sqlite3_bind_int64(stmt2, 1, reopen_row_id);
          sqlite3_bind_blob(stmt2, 2, remaining_input.data(), 
                           std::min(remaining_input.size(), (size_t)1024), 
                           SQLITE_TRANSIENT);
          rc = sqlite3_step(stmt2);
          sqlite3_finalize(stmt2);
        }
        
        // Reopen to different row
        rc = sqlite3_blob_reopen(blob_handle, reopen_row_id);
        
        if (rc == SQLITE_OK) {
          int new_blob_size = sqlite3_blob_bytes(blob_handle);
          (void)new_blob_size;
        }
        
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      } else if (blob_handle != nullptr) {
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      break;
    }
    
    case 4: {
      // Test error cases: invalid parameters
      // Try opening BLOB with invalid table/column names
      std::string invalid_table = fdp.ConsumeRandomLengthString(32);
      std::string invalid_column = fdp.ConsumeRandomLengthString(32);
      
      rc = sqlite3_blob_open(db, "main", invalid_table.c_str(), invalid_column.c_str(), 
                            row_id, open_flags, &blob_handle);
      
      if (blob_handle != nullptr) {
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      break;
    }
    
    case 5: {
      // Test BLOB operations with different offsets and sizes
      rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                            row_id, open_flags, &blob_handle);
      
      if (rc == SQLITE_OK && blob_handle != nullptr) {
        int blob_size = sqlite3_blob_bytes(blob_handle);
        
        if (blob_size > 0) {
          // Test various read operations
          for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; ++i) {
            int test_offset = fdp.ConsumeIntegralInRange<int>(0, std::max(0, blob_size - 1));
            int test_size = fdp.ConsumeIntegralInRange<int>(1, std::min(256, blob_size - test_offset));
            
            if (test_size > 0) {
              std::vector<uint8_t> test_buffer(test_size);
              rc = sqlite3_blob_read(blob_handle, test_buffer.data(), test_size, test_offset);
            }
          }
        }
        
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      break;
    }
    
    case 6: {
      // Test multiple BLOB handles
      sqlite3_blob* blob1 = nullptr;
      sqlite3_blob* blob2 = nullptr;
      
      rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                            row_id, open_flags, &blob1);
      
      if (rc == SQLITE_OK && blob1 != nullptr) {
        // Open another handle to same BLOB
        rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                              row_id, open_flags, &blob2);
        
        if (rc == SQLITE_OK && blob2 != nullptr) {
          int size1 = sqlite3_blob_bytes(blob1);
          int size2 = sqlite3_blob_bytes(blob2);
          
          if (size1 > 0 && size1 == size2) {
            std::vector<uint8_t> buf1(std::min(128, size1));
            std::vector<uint8_t> buf2(std::min(128, size2));
            
            if (buf1.size() > 0) {
              rc = sqlite3_blob_read(blob1, buf1.data(), buf1.size(), 0);
            }
            if (buf2.size() > 0) {
              rc = sqlite3_blob_read(blob2, buf2.data(), buf2.size(), 0);
            }
          }
          
          rc = sqlite3_blob_close(blob2);
        }
        
        rc = sqlite3_blob_close(blob1);
      }
      break;
    }
    
    case 7: {
      // Test BLOB operations with transaction
      rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      rc = sqlite3_blob_open(db, "main", table_name.c_str(), column_name.c_str(), 
                            row_id, open_flags, &blob_handle);
      
      if (rc == SQLITE_OK && blob_handle != nullptr) {
        int blob_size = sqlite3_blob_bytes(blob_handle);
        (void)blob_size;
        
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      
      rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      break;
    }
  }

  // Clean up
  if (blob_handle != nullptr) {
    sqlite3_blob_close(blob_handle);
  }
  
  if (db != nullptr) {
    sqlite3_close(db);
  }
  
  sqlite3_shutdown();
  return 0;
}

} // namespace harness_001
