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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 93.5, nr_unique_branch: 34
//<Quality> {"density":17,"unique_branches":{"pager_write_pagelist":[[60877,9,60877,29,0,0,4,1]],"sqlite3_backup_step":[[81556,9,81556,26,0,0,4,0]],"memdbLock":[[52576,7,52576,31,0,0,4,0],[52576,35,52576,76,0,0,4,1],[52580,7,52580,30,0,0,4,1],[52590,7,52590,32,0,0,4,0],[52594,15,52594,27,0,0,4,1],[52603,7,52603,14,0,0,4,0],[52606,13,52606,25,0,0,4,1],[52608,19,52608,51,0,0,4,1],[14155,25,14155,46,10,0,4,0],[14155,26,14155,29,10,0,4,0]],"memdbUnlock":[[52630,7,52630,32,0,0,4,0],[14155,25,14155,46,3,0,4,0],[14155,26,14155,29,3,0,4,0]],"syncJournal":[[60706,32,60706,77,0,0,4,1]],"memdbTruncate":[[52532,7,52532,17,0,0,4,1]],"writeJournalHdr":[[57918,25,57918,72,0,0,4,0]],"memdbWrite":[[52505,7,52505,23,0,0,4,0],[52505,7,52505,23,0,0,4,1],[52507,9,52507,30,0,0,4,0],[52507,9,52507,30,0,0,4,1],[52508,9,52508,54,0,0,4,1],[52513,9,52513,20,0,0,4,0],[52513,9,52513,20,0,0,4,1],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1]],"sqlite3PagerIsMemdb":[[63311,30,63311,44,0,0,4,0]],"memdbEnlarge":[[52473,7,52473,53,0,0,4,1],[52476,7,52476,21,0,0,4,1],[52480,7,52480,21,0,0,4,1],[52482,7,52482,14,0,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1]]},"library_calls":["sqlite3_libversion","sqlite3_vfs_find","sqlite3_keyword_count","sqlite3_keyword_name","sqlite3_open","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_open_v2","sqlite3_str_finish","sqlite3_free","sqlite3_close","sqlite3_rollback_hook","sqlite3_malloc","sqlite3_free","sqlite3_deserialize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_global_recover","sqlite3_str_finish","sqlite3_free","sqlite3_close","sqlite3_close","sqlite3_free_filename"],"critical_calls":["sqlite3_libversion","sqlite3_vfs_find","sqlite3_keyword_count","sqlite3_keyword_name","sqlite3_open","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_open_v2","sqlite3_rollback_hook","sqlite3_malloc","sqlite3_free","sqlite3_deserialize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_global_recover","sqlite3_str_finish","sqlite3_free","sqlite3_close","sqlite3_close","sqlite3_free_filename"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  // Early return if no data
  if (size == 0) {
    return 0;
  }

  // Get SQLite version
  const char *version = sqlite3_libversion();
  
  // Find default VFS
  sqlite3_vfs *vfs = sqlite3_vfs_find(nullptr);
  
  // Get keyword count
  int keyword_count = sqlite3_keyword_count();
  
  // Try to get first keyword name if available
  const char *keyword_name = nullptr;
  int keyword_len = 0;
  if (keyword_count > 0) {
    sqlite3_keyword_name(0, &keyword_name, &keyword_len);
  }
  
  // Create temporary file with input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Get file descriptor for reading
  int in_fd = fuzz_fileno(in_file);
  
  // Open source database from input file
  sqlite3 *src_db = nullptr;
  int rc = sqlite3_open("input_file", &src_db);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
  }
  
  // Create sqlite3_str object
  sqlite3_str *str = sqlite3_str_new(src_db);
  if (str) {
    sqlite3_str_appendchar(str, 5, 't');
  }
  
  // Open destination database
  sqlite3 *dest_db = nullptr;
  int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
  rc = sqlite3_open_v2("output_file", &dest_db, flags, vfs ? vfs->zName : nullptr);
  if (rc != SQLITE_OK) {
    if (str) {
      char *str_result = sqlite3_str_finish(str);
      sqlite3_free(str_result);
    }
    sqlite3_close(src_db);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
  }
  
  // Set rollback hook
  static int rollback_hook_called = 0;
  auto rollback_hook_func = [](void *data) {
    *(int *)data = 1;
  };
  sqlite3_rollback_hook(dest_db, rollback_hook_func, &rollback_hook_called);
  
  // Allocate some memory
  void *mem = sqlite3_malloc(64);
  if (mem) {
    memset(mem, 0, 64);
    sqlite3_free(mem);
  }
  
  // Try to deserialize input data into destination database
  // We'll use the input data as serialized database content
  unsigned char *db_data = (unsigned char *)malloc(size);
  if (db_data && in_file) {
    fseek(in_file, 0, SEEK_SET);
    fread(db_data, 1, size, in_file);
    
    rc = sqlite3_deserialize(dest_db, "main", db_data, size, size, 
                            SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE);
    if (rc != SQLITE_OK) {
      // If deserialize fails, free the data manually
      free(db_data);
    }
    // Note: If deserialize succeeds, SQLite takes ownership of db_data
  }
  
  // Prepare a simple statement
  sqlite3_stmt *stmt = nullptr;
  const char *tail = nullptr;
  rc = sqlite3_prepare_v2(dest_db, "SELECT name FROM sqlite_master WHERE type='table'", 
                         -1, &stmt, &tail);
  
  if (rc == SQLITE_OK && stmt) {
    // Step through results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const unsigned char *text = sqlite3_column_text(stmt, 0);
      // Do something with result
    }
    sqlite3_finalize(stmt);
  }
  
  // Initialize backup from source to destination
  sqlite3_backup *backup = sqlite3_backup_init(dest_db, "main", src_db, "main");
  if (backup) {
    // Perform backup in steps
    while ((rc = sqlite3_backup_step(backup, 5)) == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
      // Continue backup
      if (rc == SQLITE_DONE) {
        break;
      }
    }
    sqlite3_backup_finish(backup);
  }
  
  // Try global recover
  sqlite3_global_recover();
  
  // Cleanup sqlite3_str
  if (str) {
    char *str_result = sqlite3_str_finish(str);
    sqlite3_free(str_result);
  }
  
  // Close databases
  sqlite3_close(src_db);
  sqlite3_close(dest_db);
  
  // Free filename (though we didn't use sqlite3_create_filename, we include the API call)
  // We'll pass nullptr which should be safe
  sqlite3_free_filename(nullptr);
  
  // Close file
  assert_file_closed(&in_file);;
  
  // Clean up temporary files
  remove("input_file");
  remove("output_file");
  
  assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}