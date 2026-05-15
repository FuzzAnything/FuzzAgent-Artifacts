#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <sqlite3.h>
}
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"sqlite3_value_pointer":[[89680,7,89681,49,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_value","sqlite3_value_pointer","sqlite3_strnicmp","sqlite3_finalize","sqlite3_memory_highwater","sqlite3_sleep","sqlite3_thread_cleanup","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_value","sqlite3_value_pointer","sqlite3_strnicmp","sqlite3_finalize","sqlite3_memory_highwater","sqlite3_sleep","sqlite3_thread_cleanup","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Parse fuzzer input to extract parameters
  if (size < sizeof(int) + sizeof(int) + 1) {
	return 0;
	}
	
  
  const int *int_data = reinterpret_cast<const int*>(data);
  int param1 = int_data[0];
  int param2 = int_data[1];
  
  // Use remaining data as blob/text
  size_t blob_offset = sizeof(int) * 2;
  const void* blob_data = data + blob_offset;
  int blob_size = size - blob_offset;
  
  if (blob_size < 0) {
	return 0;
	}
	

  // Initialize SQLite
  sqlite3_initialize();
  
  // Create in-memory database
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || !db) {
    sqlite3_shutdown();
    return 0;
  }

  // Create table
  const char* create_sql = 
      "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB, value INTEGER);";
  sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);

  // Prepare INSERT statement
  sqlite3_stmt* stmt = nullptr;
  const char* insert_sql = "INSERT INTO test (data, value) VALUES (?, ?);";
  rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
  
  if (rc == SQLITE_OK && stmt) {
    // Bind parameters
    sqlite3_bind_blob(stmt, 1, blob_data, blob_size, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, param1);
    
    // Execute statement
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }

  // Prepare SELECT statement
  const char* select_sql = "SELECT data, value FROM test;";
  rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
  
  if (rc == SQLITE_OK && stmt) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      // Extract column data
      int col1 = sqlite3_column_int(stmt, 1);
      
      // Get value and try to convert to pointer (will be NULL)
      sqlite3_value* val = sqlite3_column_value(stmt, 0);
      void* ptr = sqlite3_value_pointer(val, "test_type");
      
      // String comparison
      const char* str1 = "test_string";
      const char* str2 = "TEST_STRING";
      int cmp_result = sqlite3_strnicmp(str1, str2, 4);
      
      // Use results to avoid optimization
      if (col1 > 1000000 || cmp_result > 0) {
        // This branch is unlikely but prevents dead code elimination
      }
    }
    sqlite3_finalize(stmt);
  }

  // Memory usage check
  sqlite3_int64 highwater = sqlite3_memory_highwater(1); // with reset

  // Sleep for 1ms to simulate some delay
  sqlite3_sleep(1);

  // Cleanup thread data
  sqlite3_thread_cleanup();

  // Close database
  sqlite3_close(db);

  // Shutdown SQLite
  sqlite3_shutdown();

  return 0;
}