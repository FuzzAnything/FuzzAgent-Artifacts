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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 130, nr_unique_branch: 4
//<Quality> {"density":26,"unique_branches":{"sqlite3_result_error16":[[89953,7,89953,14,0,0,4,1]],"sqlite3VdbeMemSetStr":[[83157,18,83157,34,0,0,4,1]],"columnName":[[90955,9,90955,18,0,0,4,0]],"sqlite3_value_blob":[[89654,12,89654,16,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_set_authorizer","sqlite3_close","sqlite3_shutdown","sqlite3_create_function","sqlite3_close","sqlite3_shutdown","sqlite3_vfs_register","sqlite3_close","sqlite3_shutdown","sqlite3_prepare","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_count","sqlite3_column_table_name16","sqlite3_column_type","sqlite3_column_int64","sqlite3_column_double","sqlite3_column_text","sqlite3_column_bytes","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_prepare","sqlite3_step","sqlite3_finalize","sqlite3_prepare","sqlite3_step","sqlite3_finalize","sqlite3_vfs_unregister","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_set_authorizer","sqlite3_create_function","sqlite3_vfs_register","sqlite3_prepare","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_count","sqlite3_column_table_name16","sqlite3_column_type","sqlite3_column_text","sqlite3_column_bytes","sqlite3_finalize","sqlite3_prepare","sqlite3_step","sqlite3_finalize","sqlite3_prepare","sqlite3_step","sqlite3_finalize","sqlite3_vfs_unregister","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Custom VFS structure for fuzzing
static sqlite3_vfs  id_000037_fuzz_vfs = {
    3,                            // iVersion
    0,                            // szOsFile
    1024,                         // mxPathname
    0,                            // pNext
    " id_000037_fuzz_vfs",                   // zName
    0,                            // pAppData
    0,                            // xOpen
    0,                            // xDelete
    0,                            // xAccess
    0,                            // xFullPathname
    0,                            // xDlOpen
    0,                            // xDlError
    0,                            // xDlSym
    0,                            // xDlClose
    0,                            // xRandomness
    0,                            // xSleep
    0,                            // xCurrentTime
    0,                            // xGetLastError
    0,                            // xCurrentTimeInt64
    0,                            // xSetSystemCall
    0,                            // xGetSystemCall
    0                             // xNextSystemCall
};

// Authorizer callback function
static int  id_000037_authorizer_callback(
    void* pUserData,
    int actionCode,
    const char* detail1,
    const char* detail2,
    const char* detail3,
    const char* detail4) {
  // Simple authorizer that allows most operations
  return SQLITE_OK;
}

// Custom function that uses sqlite3_result_error16 and sqlite3_result_zeroblob
static void  id_000037_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
  if (argc == 1) {
    int value = sqlite3_value_int(argv[0]);
    if (value < 0) {
      // Return error for negative values
      static const wchar_t error_msg[] = L"Negative value error";
      sqlite3_result_error16(context, error_msg, -1);
    } else {
      // Return zeroblob for non-negative values
      sqlite3_result_zeroblob(context, value);
    }
  } else {
    static const wchar_t error_msg[] = L"Invalid number of arguments";
    sqlite3_result_error16(context, error_msg, -1);
  }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Initialize SQLite
  if (sqlite3_initialize() != SQLITE_OK) {
    return 0;
  }

  // Create a FILE* from input data
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (!in_file) {
    sqlite3_shutdown();
    assert_file_closed(&in_file);
	return 0;
  }

  // Create output file
  FILE* out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    sqlite3_shutdown();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  sqlite3* db = nullptr;
  sqlite3_stmt* stmt = nullptr;
  int rc = SQLITE_OK;

  // Open an in-memory database
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || !db) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_shutdown();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Set authorizer
  rc = sqlite3_set_authorizer(db,  id_000037_authorizer_callback, nullptr);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_shutdown();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Register custom function
  rc = sqlite3_create_function(
      db,
      "custom_func",
      1,
      SQLITE_UTF8,
      nullptr,
       id_000037_custom_function,
      nullptr,
      nullptr);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_shutdown();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Register VFS
  rc = sqlite3_vfs_register(& id_000037_fuzz_vfs, 0);
  if (rc != SQLITE_OK) {
    // Continue even if VFS registration fails
  }

  // Read SQL from input file
  fseek(in_file, 0, SEEK_END);
  long input_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);

  char* sql = (char*)malloc(input_size + 1);
  if (!sql) {
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_shutdown();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  size_t bytes_read = fread(sql, 1, input_size, in_file);
  sql[bytes_read] = '\0';

  // Prepare statement
  rc = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  
  if (rc == SQLITE_OK && stmt) {
    // Check if statement is an EXPLAIN statement
    int is_explain = sqlite3_stmt_isexplain(stmt);
    
    // Execute the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      int col_count = sqlite3_column_count(stmt);
      
      for (int i = 0; i < col_count; i++) {
        // Get column table name (16-bit version)
        const void* table_name = sqlite3_column_table_name16(stmt, i);
        
        // Write column data to output file
        int col_type = sqlite3_column_type(stmt, i);
        switch (col_type) {
          case SQLITE_INTEGER: {
            sqlite3_int64 value = sqlite3_column_int64(stmt, i);
            fwrite(&value, sizeof(value), 1, out_file);
            break;
          }
          case SQLITE_FLOAT: {
            double value = sqlite3_column_double(stmt, i);
            fwrite(&value, sizeof(value), 1, out_file);
            break;
          }
          case SQLITE_TEXT: {
            const unsigned char* text = sqlite3_column_text(stmt, i);
            int text_len = sqlite3_column_bytes(stmt, i);
            fwrite(text, 1, text_len, out_file);
            break;
          }
          case SQLITE_BLOB: {
            const void* blob = sqlite3_column_blob(stmt, i);
            int blob_len = sqlite3_column_bytes(stmt, i);
            fwrite(blob, 1, blob_len, out_file);
            break;
          }
          case SQLITE_NULL:
            // Write null marker
            fwrite("NULL", 1, 4, out_file);
            break;
        }
      }
    }
    
    // Finalize the statement
    sqlite3_finalize(stmt);
  }

  // Test custom function
  rc = sqlite3_prepare(db, "SELECT custom_func(10)", -1, &stmt, nullptr);
  if (rc == SQLITE_OK && stmt) {
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }

  rc = sqlite3_prepare(db, "SELECT custom_func(-5)", -1, &stmt, nullptr);
  if (rc == SQLITE_OK && stmt) {
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }

  // Unregister VFS
  sqlite3_vfs_unregister(& id_000037_fuzz_vfs);

  // Cleanup
  free(sql);
  sqlite3_close(db);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  
  // Shutdown SQLite
  sqlite3_shutdown();

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}