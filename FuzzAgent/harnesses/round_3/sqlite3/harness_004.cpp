#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_004 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for advanced BLOB operations
  if (size < 64) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different advanced scenarios
  uint8_t blob_flags = fdp.ConsumeIntegral<uint8_t>();
  uint8_t schema_selector = fdp.ConsumeIntegral<uint8_t>() % 3;  // main, temp, or attach
  int64_t row_id = fdp.ConsumeIntegral<int64_t>();
  int64_t reopen_row_id = fdp.ConsumeIntegral<int64_t>();
  int blob_size = fdp.ConsumeIntegralInRange<int>(0, 65536);  // Up to 64KB
  int max_read_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
  int max_write_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
  bool use_zeroblob = fdp.ConsumeBool();
  bool use_transaction = fdp.ConsumeBool();
  bool test_overflow = fdp.ConsumeBool();
  bool test_error_paths = fdp.ConsumeBool();

  // Consume variable-size data
  std::string db_name = fdp.ConsumeRandomLengthString(32);
  std::string table_name = fdp.ConsumeRandomLengthString(32);
  std::string column_name = fdp.ConsumeRandomLengthString(32);
  std::string attached_db_name = fdp.ConsumeRandomLengthString(32);
  std::string blob_data = fdp.ConsumeRandomLengthString(blob_size);
  std::string remaining_input = fdp.ConsumeRemainingBytesAsString();

  sqlite3* db = nullptr;
  sqlite3_blob* blob_handle = nullptr;
  sqlite3_stmt* stmt = nullptr;
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
    // Create an attached database
    std::string attach_sql = "ATTACH DATABASE ':memory:' AS " + attached_db_name + ";";
    rc = sqlite3_exec(db, attach_sql.c_str(), nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
    schema = attached_db_name.c_str();
  }

  // Create a table with BLOB column(s)
  std::string create_sql = "CREATE TABLE IF NOT EXISTS " + std::string(schema) + "." + table_name + 
                          " (id INTEGER PRIMARY KEY, " + column_name + " BLOB, data2 BLOB, data3 BLOB);";
  rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
  if (err_msg != nullptr) {
    sqlite3_free(err_msg);
    err_msg = nullptr;
  }

  // Insert initial data - using zeroblob for some rows
  for (int i = 0; i < 3; i++) {
    std::string insert_sql = "INSERT OR REPLACE INTO " + std::string(schema) + "." + table_name + 
                            " (id, " + column_name + ", data2) VALUES(?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt != nullptr) {
      sqlite3_bind_int64(stmt, 1, row_id + i);
      
      if (use_zeroblob && (i % 2 == 0)) {
        // Use zeroblob for even rows
        sqlite3_bind_zeroblob(stmt, 2, blob_size);
      } else {
        // Use actual blob data for odd rows
        sqlite3_bind_blob(stmt, 2, blob_data.data(), 
                         std::min(blob_data.size(), (size_t)blob_size), 
                         SQLITE_TRANSIENT);
      }
      
      // Third column with different data
      std::vector<uint8_t> extra_data(blob_size / 2);
      for (size_t j = 0; j < extra_data.size(); j++) {
        extra_data[j] = static_cast<uint8_t>((i * j) % 256);
      }
      sqlite3_bind_blob(stmt, 3, extra_data.data(), extra_data.size(), SQLITE_TRANSIENT);
      
      rc = sqlite3_step(stmt);
      sqlite3_finalize(stmt);
      stmt = nullptr;
    }
  }

  // Test different advanced scenarios based on the test_scenario selector
  switch (test_scenario) {
    case 0: {
      // Scenario 0: Complex BLOB lifecycle with multiple reopen operations
      rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                            row_id, blob_flags & 0x01 ? 0 : 1, &blob_handle);
      
      if (rc == SQLITE_OK && blob_handle != nullptr) {
        // Get initial blob size
        int initial_size = sqlite3_blob_bytes(blob_handle);
        
        // Test reading from various offsets
        if (initial_size > 0) {
          for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            int read_offset = fdp.ConsumeIntegralInRange<int>(0, std::max(0, initial_size - 1));
            int read_size = fdp.ConsumeIntegralInRange<int>(1, std::min(max_read_size, initial_size - read_offset));
            
            if (read_size > 0) {
              std::vector<uint8_t> read_buffer(read_size);
              rc = sqlite3_blob_read(blob_handle, read_buffer.data(), read_size, read_offset);
            }
          }
        }
        
        // Test reopen to different row
        if (reopen_row_id != row_id) {
          rc = sqlite3_blob_reopen(blob_handle, reopen_row_id);
          if (rc == SQLITE_OK) {
            int new_size = sqlite3_blob_bytes(blob_handle);
            
            // Test writing if opened for writing
            if ((blob_flags & 0x01) == 0 && new_size > 0) {  // Write enabled
              int write_offset = fdp.ConsumeIntegralInRange<int>(0, std::max(0, new_size - 1));
              int write_size = fdp.ConsumeIntegralInRange<int>(1, std::min(max_write_size, new_size - write_offset));
              
              if (write_size > 0) {
                std::vector<uint8_t> write_data(write_size);
                for (int i = 0; i < write_size; i++) {
                  write_data[i] = fdp.ConsumeIntegral<uint8_t>();
                }
                rc = sqlite3_blob_write(blob_handle, write_data.data(), write_size, write_offset);
              }
            }
          }
        }
        
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      break;
    }
    
    case 1: {
      // Scenario 1: Multiple BLOB handles on same row with different columns
      sqlite3_blob* blob1 = nullptr;
      sqlite3_blob* blob2 = nullptr;
      
      // Open handle on first column
      rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                            row_id, blob_flags & 0x01 ? 0 : 1, &blob1);
      
      if (rc == SQLITE_OK && blob1 != nullptr) {
        // Open handle on second column
        rc = sqlite3_blob_open(db, schema, table_name.c_str(), "data2", 
                              row_id, blob_flags & 0x01 ? 0 : 1, &blob2);
        
        if (rc == SQLITE_OK && blob2 != nullptr) {
          int size1 = sqlite3_blob_bytes(blob1);
          int size2 = sqlite3_blob_bytes(blob2);
          
          // Test interleaved operations
          if (size1 > 0) {
            std::vector<uint8_t> buf1(std::min(256, size1));
            if (buf1.size() > 0) {
              rc = sqlite3_blob_read(blob1, buf1.data(), buf1.size(), 0);
            }
          }
          
          if (size2 > 0) {
            std::vector<uint8_t> buf2(std::min(256, size2));
            if (buf2.size() > 0) {
              rc = sqlite3_blob_read(blob2, buf2.data(), buf2.size(), 0);
            }
          }
          
          // Test writing to second blob if opened for writing
          if ((blob_flags & 0x01) == 0 && size2 > 0) {
            int write_size = fdp.ConsumeIntegralInRange<int>(1, std::min(128, size2));
            if (write_size > 0) {
              std::vector<uint8_t> write_data(write_size);
              for (int i = 0; i < write_size; i++) {
                write_data[i] = fdp.ConsumeIntegral<uint8_t>();
              }
              rc = sqlite3_blob_write(blob2, write_data.data(), write_size, 0);
            }
          }
          
          rc = sqlite3_blob_close(blob2);
        }
        
        rc = sqlite3_blob_close(blob1);
      }
      break;
    }
    
    case 2: {
      // Scenario 2: BLOB operations within transactions
      if (use_transaction) {
        rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
      }
      
      rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                            row_id, blob_flags & 0x01 ? 0 : 1, &blob_handle);
      
      if (rc == SQLITE_OK && blob_handle != nullptr) {
        int blob_size_bytes = sqlite3_blob_bytes(blob_handle);
        
        // Perform multiple operations
        if (blob_size_bytes > 0) {
          // Read operation
          int read_size = std::min(512, blob_size_bytes);
          if (read_size > 0) {
            std::vector<uint8_t> read_buf(read_size);
            rc = sqlite3_blob_read(blob_handle, read_buf.data(), read_size, 0);
          }
          
          // Write operation if enabled
          if ((blob_flags & 0x01) == 0 && blob_size_bytes > 0) {
            int write_size = std::min(256, blob_size_bytes);
            if (write_size > 0) {
              std::vector<uint8_t> write_buf(write_size);
              for (int i = 0; i < write_size; i++) {
                write_buf[i] = fdp.ConsumeIntegral<uint8_t>();
              }
              rc = sqlite3_blob_write(blob_handle, write_buf.data(), write_size, 
                                     blob_size_bytes - write_size);
            }
          }
        }
        
        rc = sqlite3_blob_close(blob_handle);
        blob_handle = nullptr;
      }
      
      if (use_transaction) {
        rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
      }
      break;
    }
    
    case 3: {
      // Scenario 3: Error path testing - invalid parameters and edge cases
      if (test_error_paths) {
        // Test with invalid table name
        std::string invalid_table = "invalid_table_" + fdp.ConsumeRandomLengthString(16);
        rc = sqlite3_blob_open(db, schema, invalid_table.c_str(), column_name.c_str(), 
                              row_id, 0, &blob_handle);
        
        if (blob_handle != nullptr) {
          rc = sqlite3_blob_close(blob_handle);
          blob_handle = nullptr;
        }
        
        // Test with invalid column name
        std::string invalid_column = "invalid_column_" + fdp.ConsumeRandomLengthString(16);
        rc = sqlite3_blob_open(db, schema, table_name.c_str(), invalid_column.c_str(), 
                              row_id, 0, &blob_handle);
        
        if (blob_handle != nullptr) {
          rc = sqlite3_blob_close(blob_handle);
          blob_handle = nullptr;
        }
        
        // Test with invalid row ID
        int64_t invalid_row_id = fdp.ConsumeIntegral<int64_t>();
        rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                              invalid_row_id, 0, &blob_handle);
        
        if (blob_handle != nullptr) {
          rc = sqlite3_blob_close(blob_handle);
          blob_handle = nullptr;
        }
      }
      break;
    }
    
    case 4: {
      // Scenario 4: Large BLOB testing (potential overflow pages)
      if (test_overflow) {
        // Create a large BLOB row
        int large_blob_size = fdp.ConsumeIntegralInRange<int>(1024, 16384);
        std::vector<uint8_t> large_blob(large_blob_size);
        for (int i = 0; i < large_blob_size; i++) {
          large_blob[i] = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // Insert large blob
        std::string insert_large_sql = "INSERT OR REPLACE INTO " + std::string(schema) + "." + table_name + 
                                      " (id, " + column_name + ") VALUES(?, ?);";
        rc = sqlite3_prepare_v2(db, insert_large_sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt != nullptr) {
          sqlite3_bind_int64(stmt, 1, row_id + 1000);  // Different row ID
          sqlite3_bind_blob(stmt, 2, large_blob.data(), large_blob_size, SQLITE_TRANSIENT);
          rc = sqlite3_step(stmt);
          sqlite3_finalize(stmt);
          stmt = nullptr;
        }
        
        // Open and test the large blob
        rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                              row_id + 1000, 0, &blob_handle);
        
        if (rc == SQLITE_OK && blob_handle != nullptr) {
          int actual_size = sqlite3_blob_bytes(blob_handle);
          
          // Test reading from various offsets in large blob
          if (actual_size > 1024) {
            // Read from beginning
            std::vector<uint8_t> buf_start(512);
            rc = sqlite3_blob_read(blob_handle, buf_start.data(), 512, 0);
            
            // Read from middle
            std::vector<uint8_t> buf_middle(512);
            rc = sqlite3_blob_read(blob_handle, buf_middle.data(), 512, actual_size / 2);
            
            // Read from near end
            std::vector<uint8_t> buf_end(512);
            int end_offset = std::max(0, actual_size - 512);
            rc = sqlite3_blob_read(blob_handle, buf_end.data(), 512, end_offset);
          }
          
          rc = sqlite3_blob_close(blob_handle);
          blob_handle = nullptr;
        }
      }
      break;
    }
    
    case 5: {
      // Scenario 5: Zero-blob specific testing
      if (use_zeroblob) {
        // Find a row that was inserted with zeroblob
        rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                              row_id, 0, &blob_handle);
        
        if (rc == SQLITE_OK && blob_handle != nullptr) {
          int zero_blob_size = sqlite3_blob_bytes(blob_handle);
          
          // Zero-blobs should be readable (all zeros)
          if (zero_blob_size > 0) {
            std::vector<uint8_t> zero_buf(std::min(256, zero_blob_size));
            rc = sqlite3_blob_read(blob_handle, zero_buf.data(), zero_buf.size(), 0);
            
            // Verify it's zeros (though we don't need to assert in fuzzer)
            // The read should succeed even for zero-blobs
          }
          
          // Zero-blobs can be written to if opened for writing
          rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                                row_id, 1, &blob_handle);  // Force readwrite
          
          if (rc == SQLITE_OK && blob_handle != nullptr) {
            int zero_blob_size2 = sqlite3_blob_bytes(blob_handle);
            if (zero_blob_size2 > 0) {
              std::vector<uint8_t> write_data(std::min(128, zero_blob_size2));
              for (size_t i = 0; i < write_data.size(); i++) {
                write_data[i] = fdp.ConsumeIntegral<uint8_t>();
              }
              rc = sqlite3_blob_write(blob_handle, write_data.data(), write_data.size(), 0);
            }
          }
          
          if (blob_handle != nullptr) {
            rc = sqlite3_blob_close(blob_handle);
            blob_handle = nullptr;
          }
        }
      }
      break;
    }
    
    case 6: {
      // Scenario 6: Concurrent BLOB operations simulation
      sqlite3_blob* blob_a = nullptr;
      sqlite3_blob* blob_b = nullptr;
      
      // Open two handles to same blob
      rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                            row_id, 0, &blob_a);
      
      if (rc == SQLITE_OK && blob_a != nullptr) {
        rc = sqlite3_blob_open(db, schema, table_name.c_str(), column_name.c_str(), 
                              row_id, 0, &blob_b);
        
        if (rc == SQLITE_OK && blob_b != nullptr) {
          int size_a = sqlite3_blob_bytes(blob_a);
          int size_b = sqlite3_blob_bytes(blob_b);
          
          // Perform alternating operations
          if (size_a > 0 && size_a == size_b) {
            // Read from first handle
            int read_size = std::min(128, size_a);
            std::vector<uint8_t> buf_a(read_size);
            rc = sqlite3_blob_read(blob_a, buf_a.data(), read_size, 0);
            
            // Read from second handle (same data)
            std::vector<uint8_t> buf_b(read_size);
            rc = sqlite3_blob_read(blob_b, buf_b.data(), read_size, size_a - read_size);
            
            // Test reopen on one handle while other is open
            if (reopen_row_id != row_id) {
              rc = sqlite3_blob_reopen(blob_b, reopen_row_id);
            }
          }
          
          rc = sqlite3_blob_close(blob_b);
        }
        
        rc = sqlite3_blob_close(blob_a);
      }
      break;
    }
    
    case 7: {
      // Scenario 7: Schema testing across different databases
      // Test BLOB operations across main, temp, and attached schemas
      const char* schemas[] = {"main", "temp"};
      if (schema_selector == 2) {
        // Include attached schema if created
        // Note: We can't easily pass array with variable size in this simple test
      }
      
      for (int i = 0; i < 2; i++) {
        const char* test_schema = schemas[i];
        
        // Try to open blob in this schema (may fail if table doesn't exist)
        rc = sqlite3_blob_open(db, test_schema, table_name.c_str(), column_name.c_str(), 
                              row_id, 0, &blob_handle);
        
        if (rc == SQLITE_OK && blob_handle != nullptr) {
          int schema_blob_size = sqlite3_blob_bytes(blob_handle);
          if (schema_blob_size > 0) {
            std::vector<uint8_t> schema_buf(std::min(64, schema_blob_size));
            rc = sqlite3_blob_read(blob_handle, schema_buf.data(), schema_buf.size(), 0);
          }
          
          rc = sqlite3_blob_close(blob_handle);
          blob_handle = nullptr;
        }
      }
      break;
    }
  }

  // Clean up any remaining blob handles
  if (blob_handle != nullptr) {
    sqlite3_blob_close(blob_handle);
  }
  
  // Clean up statements
  if (stmt != nullptr) {
    sqlite3_finalize(stmt);
  }
  
  // Clean up database
  if (db != nullptr) {
    sqlite3_close(db);
  }
  
  sqlite3_shutdown();
  return 0;
}

} // namespace harness_004
