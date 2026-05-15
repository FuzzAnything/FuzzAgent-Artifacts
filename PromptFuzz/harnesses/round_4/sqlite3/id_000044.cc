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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"sqlite3_vfs_find":[[26012,23,26012,27,0,0,4,1]],"pcache1Shrink":[[55321,7,55321,25,0,0,4,1]],"sqlite3_enable_load_extension":[[136428,7,136428,12,0,0,4,1]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_enable_load_extension","sqlite3_vfs_find","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_open","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_db_release_memory","sqlite3_backup_finish","sqlite3_close","sqlite3_finalize","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_enable_load_extension","sqlite3_vfs_find","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_open","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_db_release_memory","sqlite3_backup_finish","sqlite3_close","sqlite3_finalize","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_backup *backup = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_vfs *vfs = nullptr;
    char *expanded_sql = nullptr;
    
    // 1. Get library version
    const char *version = sqlite3_libversion();
    (void)version; // Use version to avoid unused variable warning
    
    // 2. Open a database using the input file
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK) {
        // 3. Enable load extension (use first byte to decide on/off)
        int enable_ext = (size > 0) ? (data[0] & 1) : 0;
        sqlite3_enable_load_extension(db, enable_ext);
        
        // 4. Find a VFS (use first few bytes as VFS name if possible)
        char vfs_name[32] = {0};
        if (size > 1) {
            size_t name_len = (size - 1 < 31) ? size - 1 : 31;
            memcpy(vfs_name, data + 1, name_len);
            vfs_name[name_len] = '\0';
        }
        vfs = sqlite3_vfs_find(vfs_name[0] ? vfs_name : nullptr);
        
        // 5. Prepare a simple statement and get expanded SQL
        const char *sql = "SELECT 1";
        rc = sqlite3_prepare_v2(db, fuzz_str_1, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt) {
            expanded_sql = sqlite3_expanded_sql(stmt);
            
            // 6. Create a backup (in-memory to in-memory)
            sqlite3 *db2 = nullptr;
            if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
                backup = sqlite3_backup_init(db2, fuzz_str_2, db, fuzz_str_3);
                if (backup) {
                    // 7. Get backup page count
                    int page_count = sqlite3_backup_pagecount(backup);
                    (void)page_count; // Use to avoid warning
                    
                    // 8. Release memory from database
                    sqlite3_db_release_memory(db);
                    
                    // Clean up backup
                    sqlite3_backup_finish(backup);
                }
                sqlite3_close(db2);
            }
            
            sqlite3_finalize(stmt);
        }
        
        // Free expanded SQL if allocated
        if (expanded_sql) {
            sqlite3_free(expanded_sql);
        }
        
        sqlite3_close(db);
    }
    
    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}