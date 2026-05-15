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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 350, nr_unique_branch: 14
//<Quality> {"density":35,"unique_branches":{"sqlite3WalReadFrame":[[67791,45,67791,52,0,0,4,0]],"btreeBeginTrans":[[73416,11,73416,45,0,0,4,0],[73436,11,73436,38,0,0,4,0]],"sqlite3BtreeSetVersion":[[81109,9,81109,32,0,0,4,1],[81109,36,81109,59,0,0,4,1]],"unixShmSystemLock":[[42301,15,42301,32,0,0,4,0]],"walBeginReadTransaction":[[14098,27,14098,28,5,0,4,0]],"unixShmLock":[[42874,11,42874,24,0,0,4,1]],"walTryBeginRead":[[67243,11,67243,31,0,0,4,1],[67252,17,67252,72,0,0,4,1],[67255,17,67255,32,0,0,4,0]],"walLockShared":[[14123,23,14123,24,4,0,4,0]],"sqlite3_backup_step":[[81555,10,81555,41,0,0,4,0],[81605,13,81605,44,0,0,4,0]]},"library_calls":["sqlite3_soft_heap_limit","sqlite3_open_v2","sqlite3_open","sqlite3_close","sqlite3_close","sqlite3_busy_timeout","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_int64","sqlite3_column_text","sqlite3_strnicmp","sqlite3_column_double","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_soft_heap_limit","sqlite3_open_v2","sqlite3_open","sqlite3_busy_timeout","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_int64","sqlite3_column_text","sqlite3_strnicmp","sqlite3_column_double","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_close","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Set heap limits
    sqlite3_int64 hard_limit = (sqlite3_int64)data[0] * 1024 * 1024;
    sqlite3_soft_heap_limit((int)data[1] * 1024 * 1024);
    
    // Open database
    sqlite3* db1 = nullptr;
    sqlite3* db2 = nullptr;
    const char* db_filename = "input_file";
    
    // Write input data to file first
    FILE* in_file = fopen(db_filename, "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open with two different methods
    int rc1 = sqlite3_open_v2(db_filename, &db1, SQLITE_OPEN_READWRITE, nullptr);
    int rc2 = sqlite3_open(db_filename, &db2);
    
    if (rc1 != SQLITE_OK || rc2 != SQLITE_OK) {
        if (db1) sqlite3_close(db1);
        if (db2) sqlite3_close(db2);
        remove(db_filename);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Set busy timeout
    sqlite3_busy_timeout(db1, (int)data[2] * 100);
    
    // Prepare and execute a simple query
    sqlite3_stmt* stmt1 = nullptr;
    const char* sql1 = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT, value REAL)";
    const char* tail1 = nullptr;
    
    sqlite3_prepare_v2(db1, sql1, -1, &stmt1, &tail1);
    if (stmt1) {
        sqlite3_step(stmt1);
        sqlite3_finalize(stmt1);
    }
    
    // Prepare INSERT statement
    sqlite3_stmt* stmt2 = nullptr;
    const char* sql2 = "INSERT INTO test (id, name, value) VALUES (?, ?, ?)";
    const char* tail2 = nullptr;
    
    sqlite3_prepare_v2(db1, sql2, -1, &stmt2, &tail2);
    if (stmt2) {
        // Bind values from input data
        sqlite3_bind_int(stmt2, 1, (int)data[3]);
        
        // Use part of input as text (ensure null termination)
        size_t text_len = (size - 10 > 50) ? 50 : size - 10;
        char* text_buf = (char*)malloc(text_len + 1);
        if (text_buf) {
            memcpy(text_buf, data + 4, text_len);
            text_buf[text_len] = 0;
            sqlite3_bind_text(stmt2, 2, text_buf, -1, SQLITE_TRANSIENT);
            free(text_buf);
        }
        
        // Bind double from input bytes
        double dbl_val;
        memcpy(&dbl_val, data + (size % 100), sizeof(double));
        sqlite3_bind_double(stmt2, 3, dbl_val);
        
        // Execute
        sqlite3_step(stmt2);
        sqlite3_reset(stmt2);
        
        // Rebind with different values
        sqlite3_bind_int(stmt2, 1, (int)data[4]);
        sqlite3_bind_text(stmt2, 2, "fuzz", 4, SQLITE_STATIC);
        sqlite3_bind_double(stmt2, 3, (double)data[5] / 100.0);
        
        sqlite3_step(stmt2);
        
        // Get statement status
        int status = sqlite3_stmt_status(stmt2, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        (void)status;  // Use to avoid unused variable warning
        
        // Get data count (should be 0 after insert)
        int data_count = sqlite3_data_count(stmt2);
        (void)data_count;
        
        sqlite3_finalize(stmt2);
    }
    
    // Prepare SELECT statement
    sqlite3_stmt* stmt3 = nullptr;
    const char* sql3 = "SELECT id, name, value FROM test";
    sqlite3_prepare_v2(db1, sql3, -1, &stmt3, nullptr);
    
    if (stmt3) {
        while (sqlite3_step(stmt3) == SQLITE_ROW) {
            // Get column values
            int col_count = sqlite3_data_count(stmt3);
            if (col_count >= 3) {
                int id = sqlite3_column_int64(stmt3, 0);
                (void)id;
                
                const unsigned char* name = sqlite3_column_text(stmt3, 1);
                if (name) {
                    // Compare with string
                    int cmp = sqlite3_strnicmp((const char*)name, "fuzz", 4);
                    (void)cmp;
                }
                
                double value = sqlite3_column_double(stmt3, 2);
                (void)value;
            }
        }
        sqlite3_finalize(stmt3);
    }
    
    // Backup database
    sqlite3_backup* backup = sqlite3_backup_init(db2, "main", db1, "main");
    if (backup) {
        sqlite3_backup_step(backup, 5);  // Copy 5 pages
        sqlite3_backup_finish(backup);
    }
    
    // Memory usage
    sqlite3_int64 highwater = sqlite3_memory_highwater(0);
    (void)highwater;
    
    int released = sqlite3_release_memory(1024);
    (void)released;
    
    // Cleanup
    if (db1) sqlite3_close(db1);
    if (db2) sqlite3_close(db2);
    
    remove(db_filename);
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}