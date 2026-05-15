#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_007 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for table column metadata operations
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: basic metadata, 1: different table types, 2: error cases, 3: complex combinations
  uint8_t schema_selector = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: main, 1: temp, 2: NULL
  uint8_t column_types = fdp.ConsumeIntegral<uint8_t>() % 8;  // Bitmask for column types to create
  bool with_rowid = fdp.ConsumeBool();
  bool without_rowid = fdp.ConsumeBool();
  bool create_views = fdp.ConsumeBool();
  bool test_null_column = fdp.ConsumeBool();
  bool test_invalid_names = fdp.ConsumeBool();
  
  // Consume variable-size data for table/column names
  std::string table_name = fdp.ConsumeRandomLengthString(32);
  std::string column_name1 = fdp.ConsumeRandomLengthString(32);
  std::string column_name2 = fdp.ConsumeRandomLengthString(32);
  std::string column_name3 = fdp.ConsumeRandomLengthString(32);
  std::string column_name4 = fdp.ConsumeRandomLengthString(32);
  std::string attached_db_name = fdp.ConsumeRandomLengthString(32);
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
    schema = nullptr;  // NULL schema name
  }

  // Create a table with various column types based on the column_types bitmask
  std::string create_sql = "CREATE TABLE IF NOT EXISTS " + table_name + " (";
  
  std::vector<std::string> column_defs;
  std::vector<std::string> column_names;
  
  if (column_types & 0x01) {
    column_defs.push_back(column_name1 + " INTEGER");
    column_names.push_back(column_name1);
  }
  if (column_types & 0x02) {
    column_defs.push_back(column_name2 + " TEXT COLLATE NOCASE");
    column_names.push_back(column_name2);
  }
  if (column_types & 0x04) {
    column_defs.push_back(column_name3 + " REAL NOT NULL");
    column_names.push_back(column_name3);
  }
  if (column_types & 0x08) {
    column_defs.push_back(column_name4 + " BLOB");
    column_names.push_back(column_names.size() > 0 ? column_name4 + " PRIMARY KEY" : column_name4 + " INTEGER PRIMARY KEY");
    column_names.push_back(column_name4);
  }
  
  // Add rowid column if requested
  if (with_rowid && column_defs.empty()) {
    column_defs.push_back("id INTEGER PRIMARY KEY");
    column_names.push_back("id");
  }
  
  // If no columns were added, add a default column
  if (column_defs.empty()) {
    column_defs.push_back("col1 TEXT");
    column_names.push_back("col1");
  }
  
  // Build the CREATE TABLE statement
  for (size_t i = 0; i < column_defs.size(); i++) {
    create_sql += column_defs[i];
    if (i < column_defs.size() - 1) {
      create_sql += ", ";
    }
  }
  
  if (without_rowid) {
    create_sql += ") WITHOUT ROWID;";
  } else {
    create_sql += ");";
  }
  
  // Execute table creation
  rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
  if (err_msg != nullptr) {
    sqlite3_free(err_msg);
    err_msg = nullptr;
  }

  // Create a view if requested
  if (create_views) {
    std::string view_sql = "CREATE VIEW IF NOT EXISTS " + table_name + "_view AS SELECT * FROM " + table_name + ";";
    rc = sqlite3_exec(db, view_sql.c_str(), nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Create an attached database if we have schema selector 2 and want to test attached DBs
  if (schema_selector == 2 && !attached_db_name.empty()) {
    std::string attach_sql = "ATTACH DATABASE ':memory:' AS " + attached_db_name + ";";
    rc = sqlite3_exec(db, attach_sql.c_str(), nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
    
    // Create table in attached database
    std::string attached_create_sql = "CREATE TABLE IF NOT EXISTS " + attached_db_name + "." + table_name + 
                                      "_attached (col1 INTEGER, col2 TEXT);";
    rc = sqlite3_exec(db, attached_create_sql.c_str(), nullptr, nullptr, &err_msg);
    if (err_msg != nullptr) {
      sqlite3_free(err_msg);
      err_msg = nullptr;
    }
  }

  // Test different operations based on the operation selector
  switch (operation) {
    case 0: {
      // Basic metadata retrieval for each column
      for (const auto& col_name : column_names) {
        const char* data_type = nullptr;
        const char* coll_seq = nullptr;
        int not_null = 0;
        int primary_key = 0;
        int autoinc = 0;
        
        rc = sqlite3_table_column_metadata(db, schema, table_name.c_str(), col_name.c_str(),
                                          &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
        // The return code and outputs are part of the test, we don't need to validate them
        (void)rc;
        (void)data_type;
        (void)coll_seq;
        (void)not_null;
        (void)primary_key;
        (void)autoinc;
      }
      break;
    }
    
    case 1: {
      // Test metadata for special column names
      const char* special_names[] = {"rowid", "oid", "_rowid_"};
      
      for (const char* special_name : special_names) {
        const char* data_type = nullptr;
        const char* coll_seq = nullptr;
        int not_null = 0;
        int primary_key = 0;
        int autoinc = 0;
        
        rc = sqlite3_table_column_metadata(db, schema, table_name.c_str(), special_name,
                                          &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
        // Accept any result - valid or error
        (void)rc;
        (void)data_type;
        (void)coll_seq;
        (void)not_null;
        (void)primary_key;
        (void)autoinc;
      }
      break;
    }
    
    case 2: {
      // Test error cases: non-existent tables and columns
      std::string fake_table = "non_existent_table_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
      std::string fake_column = "non_existent_column_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
      
      const char* data_type = nullptr;
      const char* coll_seq = nullptr;
      int not_null = 0;
      int primary_key = 0;
      int autoinc = 0;
      
      // Test with non-existent table
      rc = sqlite3_table_column_metadata(db, schema, fake_table.c_str(), "col1",
                                        &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
      (void)rc;
      
      // Test with non-existent column in existing table
      rc = sqlite3_table_column_metadata(db, schema, table_name.c_str(), fake_column.c_str(),
                                        &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
      (void)rc;
      
      // Test with NULL column name (should just check table existence)
      if (test_null_column) {
        rc = sqlite3_table_column_metadata(db, schema, table_name.c_str(), nullptr,
                                          &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
        (void)rc;
      }
      break;
    }
    
    case 3: {
      // Complex combination: test metadata for all possible combinations
      // Test with different schema names
      const char* schemas_to_test[] = {"main", "temp", nullptr};
      
      for (const char* test_schema : schemas_to_test) {
        for (const auto& col_name : column_names) {
          const char* data_type = nullptr;
          const char* coll_seq = nullptr;
          int not_null = 0;
          int primary_key = 0;
          int autoinc = 0;
          
          rc = sqlite3_table_column_metadata(db, test_schema, table_name.c_str(), col_name.c_str(),
                                            &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
          (void)rc;
          (void)data_type;
          (void)coll_seq;
          (void)not_null;
          (void)primary_key;
          (void)autoinc;
        }
      }
      
      // Test attached database if created
      if (schema_selector == 2 && !attached_db_name.empty()) {
        const char* data_type = nullptr;
        const char* coll_seq = nullptr;
        int not_null = 0;
        int primary_key = 0;
        int autoinc = 0;
        
        rc = sqlite3_table_column_metadata(db, attached_db_name.c_str(), 
                                          (table_name + "_attached").c_str(), "col1",
                                          &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
        (void)rc;
      }
      break;
    }
  }

  // Test with invalid UTF-8 names if requested
  if (test_invalid_names && remaining_input.size() > 0) {
    const char* data_type = nullptr;
    const char* coll_seq = nullptr;
    int not_null = 0;
    int primary_key = 0;
    int autoinc = 0;
    
    // Use raw bytes from remaining input as table/column names
    // This tests SQLite's handling of invalid UTF-8
    std::string invalid_name = remaining_input.substr(0, std::min((size_t)16, remaining_input.size()));
    
    rc = sqlite3_table_column_metadata(db, schema, invalid_name.c_str(), "col1",
                                      &data_type, &coll_seq, &not_null, &primary_key, &autoinc);
    (void)rc;
  }

  // Clean up
  if (db != nullptr) {
    sqlite3_close(db);
  }
  sqlite3_shutdown();

  return 0;
}

} // namespace harness_007
