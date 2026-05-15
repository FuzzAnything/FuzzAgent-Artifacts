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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 558, nr_unique_branch: 56
//<Quality> {"density":31,"unique_branches":{"vfsUnlink":[[26027,13,26027,26,0,0,4,0]],"downgradeAllSharedCacheTableLocks":[[70259,7,70259,22,0,0,4,1]],"writeJournalHdr":[[57918,25,57918,72,0,0,4,0]],"memdbWrite":[[52505,7,52505,23,0,0,4,0],[52505,7,52505,23,0,0,4,1],[52507,9,52507,30,0,0,4,0],[52507,9,52507,30,0,0,4,1],[52508,9,52508,54,0,0,4,1],[52513,9,52513,20,0,0,4,1],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1]],"invalidateIncrblobCursors":[[70342,31,70342,32,0,0,4,0],[70342,31,70342,32,0,0,4,1],[70343,9,70343,41,0,0,4,1]],"syncJournal":[[60706,32,60706,77,0,0,4,1]],"memdbUnlock":[[52626,7,52626,26,0,0,4,1],[52630,7,52630,32,0,0,4,0],[52630,7,52630,32,0,0,4,1],[52635,9,52635,40,0,0,4,1],[14155,25,14155,46,3,0,4,0],[14155,26,14155,29,3,0,4,0]],"sqlite3PagerFilename":[[63495,40,63495,68,0,0,4,0]],"memdbEnlarge":[[52473,7,52473,53,0,0,4,1],[52476,7,52476,21,0,0,4,1],[52480,7,52480,21,0,0,4,1],[52482,7,52482,14,0,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1]],"databaseIsUnmoved":[[60558,7,60558,26,0,0,4,0]],"blobSeekToRow":[[14098,27,14098,28,3,0,4,0]],"btreeEndTransaction":[[74043,32,74043,47,0,0,4,0]],"sqlite3_blob_open":[[101501,9,101501,15,0,0,4,1],[101599,13,101599,19,0,0,4,1]],"sqlite3BtreeDelete":[[79540,28,79540,45,0,0,4,0]],"memdbOpen":[[52753,7,52753,15,0,0,4,0],[52753,20,52753,33,0,0,4,1],[52753,37,52753,51,0,0,4,1]],"memdbLock":[[52569,7,52569,26,0,0,4,1],[52576,7,52576,31,0,0,4,0],[52576,7,52576,31,0,0,4,1],[52576,35,52576,76,0,0,4,1],[52580,7,52580,30,0,0,4,0],[52580,7,52580,30,0,0,4,1],[52582,13,52582,25,0,0,4,1],[52590,7,52590,32,0,0,4,0],[52590,7,52590,32,0,0,4,1],[52591,7,52591,31,0,0,4,1],[52594,15,52594,27,0,0,4,1],[52603,7,52603,14,0,0,4,0],[52603,7,52603,14,0,0,4,1],[52606,13,52606,25,0,0,4,1],[52608,19,52608,51,0,0,4,1],[52615,7,52615,20,0,0,4,0],[14155,25,14155,46,10,0,4,0],[14155,26,14155,29,10,0,4,0]],"memdbRead":[[52457,7,52457,23,0,0,4,1]]},"library_calls":["sqlite3_vfs_find","sqlite3_vfs_register","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_origin_name","sqlite3_stmt_isexplain","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_blob_open","sqlite3_last_insert_rowid","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_column_value","sqlite3_column_value","sqlite3_value_frombind","sqlite3_value_frombind","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_vfs_find","sqlite3_vfs_register","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_origin_name","sqlite3_stmt_isexplain","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_blob_open","sqlite3_last_insert_rowid","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_column_value","sqlite3_column_value","sqlite3_value_frombind","sqlite3_value_frombind","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<206) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_19, fuzz_str_sz_19, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file for database operations
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_blob *blob = nullptr;
    sqlite3_vfs *vfs = nullptr;
    
    // 4. Register a VFS (using default VFS)
    vfs = sqlite3_vfs_find(nullptr);
    if (vfs) {
        sqlite3_vfs_register(vfs, fuzz_int32_t_1); // API 1
    }
    
    // 5. Open database from input file
    if (sqlite3_open("input_file", &db) == SQLITE_OK) {
        // 6. Create a table with a blob column
        const char *create_table = 
            "CREATE TABLE IF NOT EXISTS test_table ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT, "
            "data BLOB);";
        sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);
        
        // 7. Insert some data using the input
        const char *insert_sql = 
            "INSERT INTO test_table (name, data) VALUES (?, ?);";
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            // Bind parameters
            sqlite3_bind_text(stmt, fuzz_int32_t_3, "test", -1, SQLITE_STATIC);
            sqlite3_bind_blob(stmt, fuzz_int32_t_4, data, (int)(size > 100 ? 100 : size), SQLITE_STATIC);
            
            // Execute
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
        
        // 8. Query the data back
        const char *select_sql = "SELECT * FROM test_table;";
        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Get column origin name - API 2
                const char *origin_name = sqlite3_column_origin_name(stmt, fuzz_int32_t_5);
                (void)origin_name; // Mark as used
                
                // Check if statement is in explain mode - API 5
                int is_explain = sqlite3_stmt_isexplain(stmt);
                (void)is_explain; // Mark as used
                
                // Try to get a blob value
                if (sqlite3_column_type(stmt, fuzz_int32_t_6) == SQLITE_BLOB) {
                    const void *blob_data = sqlite3_column_blob(stmt, fuzz_int32_t_7);
                    int blob_size = sqlite3_column_bytes(stmt, fuzz_int32_t_8);
                    
                    // Open blob for reading
                    if (sqlite3_blob_open(db, fuzz_str_9, fuzz_str_10, fuzz_str_11, 
                                          sqlite3_last_insert_rowid(db), 
                                          fuzz_int32_t_12, &blob) == SQLITE_OK) {
                        // Get blob size - API 4
                        int blob_bytes = sqlite3_blob_bytes(blob);
                        (void)blob_bytes; // Mark as used
                        
                        // Read blob content
                        char *buffer = (char *)malloc(blob_bytes + 1);
                        if (buffer) {
                            sqlite3_blob_read(blob, buffer, blob_bytes, fuzz_int32_t_13);
                            free(buffer);
                        }
                        
                        sqlite3_blob_close(blob);
                        blob = nullptr;
                    }
                }
            }
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
        
        // 9. Test value_frombind API - API 3
        const char *bind_sql = "SELECT ?, ?;";
        if (sqlite3_prepare_v2(db, bind_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, fuzz_int32_t_14, fuzz_int32_t_15);
            sqlite3_bind_text(stmt, fuzz_int32_t_16, "test", -1, SQLITE_STATIC);
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_value *value1 = sqlite3_column_value(stmt, fuzz_int32_t_17);
                sqlite3_value *value2 = sqlite3_column_value(stmt, fuzz_int32_t_18);
                
                if (value1) {
                    int from_bind1 = sqlite3_value_frombind(value1);
                    (void)from_bind1; // Mark as used
                }
                if (value2) {
                    int from_bind2 = sqlite3_value_frombind(value2);
                    (void)from_bind2; // Mark as used
                }
            }
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
        
        // 10. Clean up database
        sqlite3_exec(db, fuzz_str_19, nullptr, nullptr, nullptr);
        sqlite3_close(db);
    }
    
    // 11. Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 12. Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}