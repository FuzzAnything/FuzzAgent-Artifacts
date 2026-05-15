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
//<ID> 491
//<Prompt> []
/*<Combination>: [
*/
//<score> 1692, nr_unique_branch: 58
//<Quality> {"density":36,"unique_branches":{"sqlite3SchemaGet":[[126366,7,126366,9,0,0,4,0]],"sqlite3Select":[[148556,7,148556,11,0,0,4,0]],"sqlite3VdbeChangeToNoop":[[85423,7,85423,26,0,0,4,0]],"sqlite3_exec":[[134865,7,134865,14,0,0,4,0]],"sqlite3CreateFunc":[[179565,7,179565,9,0,0,4,0]],"tokenExpr":[[170541,9,170541,10,0,0,4,1]],"sqlite3BtreeClose":[[72691,29,72691,41,0,0,4,1]],"sqlite3IdListAppend":[[124833,9,124833,16,0,0,4,0]],"sqlite3FindFunction":[[126293,7,126293,67,0,0,4,1],[126302,9,126302,22,0,0,4,0]],"pcache1Create":[[55254,7,55254,13,0,0,4,1]],"dbReallocFinish":[[30548,11,30548,16,0,0,4,0]],"sqlite3SrcListAppend":[[125009,9,125009,17,0,0,4,0]],"sqlite3AddColumn":[[121682,7,121682,11,0,0,4,0]],"sqlite3Insert":[[132388,7,132388,19,0,0,4,0]],"sqlite3VdbeCreate":[[84003,7,84003,11,0,0,4,0]],"sqlite3VtabCreateModule":[[153715,9,153715,19,0,0,4,0]],"yy_reduce":[[174284,7,174284,8,0,0,4,1]],"sqlite3EndTable":[[123077,9,123077,13,0,0,4,0]],"sqlite3Realloc":[[30351,31,30351,62,0,0,4,0]],"clearSelect":[[141398,9,141398,14,0,0,4,1]],"sqlite3WhereBegin":[[166545,9,166545,25,0,0,4,0]],"sqlite3ClearOnOrUsing":[[109352,13,109352,19,0,0,4,1],[109354,13,109354,22,0,0,4,1]],"sqlite3SrcListAppendFromTerm":[[125114,7,125114,11,0,0,4,0]],"sqlite3WalOpen":[[65932,7,65932,12,0,0,4,0]],"btreeCreateTable":[[79708,9,79708,22,0,0,4,0]],"sqlite3ExprListSetSortOrder":[[110054,7,110054,11,0,0,4,0]],"sqlite3VdbeMemFromBtree":[[83218,7,83218,66,0,0,4,1]],"sqlite3RegisterPerConnectionBuiltinFunctions":[[129637,7,129637,23,0,0,4,0]],"resizeResolveLabel":[[84598,7,84598,19,0,0,4,0]],"sqlite3PagerOpen":[[61193,9,61193,21,0,0,4,0]],"sqlite3SelectNew":[[141435,7,141435,14,0,0,4,0],[141466,7,141466,31,0,0,4,0]],"sqlite3HashInsert":[[36637,7,36637,18,0,0,4,0]],"sqlite3ExprAssignVarNumber":[[109239,7,109239,15,0,0,4,0]],"sqlite3WalCheckpoint":[[68490,11,68490,20,0,0,4,0],[68490,24,68490,58,0,0,4,0],[68513,7,68513,16,0,0,4,0]],"sqlite3ParseUri":[[180770,9,180770,15,0,0,4,0]],"wherePathSolver":[[165435,7,165435,16,0,0,4,0]],"sqlite3_backup_init":[[81353,9,81353,11,0,0,4,0],[81359,7,81359,8,0,0,4,1],[81379,7,81379,8,0,0,4,1]],"findCollSeqEntry":[[125970,11,125970,18,0,0,4,0]],"sqlite3_backup_step":[[81537,47,81537,75,0,0,4,0]],"sqlite3VdbeScanStatus":[[85164,9,85164,13,0,0,4,1]],"unixOpenSharedMemory":[[42504,9,42504,20,0,0,4,0]],"createFunctionApi":[[179620,9,179620,14,0,0,4,0],[179632,15,179632,28,0,0,4,0]],"pcache1FetchStage2":[[55382,43,55382,77,0,0,4,0]],"openDatabase":[[181050,7,181050,20,0,0,4,0],[181051,9,181051,25,0,0,4,0],[181052,33,181052,40,0,0,4,1],[181075,7,181075,24,0,0,4,1],[181090,7,181090,23,0,0,4,0]],"unixShmPurge":[[42342,7,42342,8,0,0,4,1]],"sqlite3BtreeOpen":[[72310,7,72310,9,0,0,4,0]]},"library_calls":["sqlite3_hard_heap_limit64","sqlite3_soft_heap_limit","sqlite3_open_v2","sqlite3_open","sqlite3_close","sqlite3_busy_timeout","sqlite3_busy_timeout","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_bind_text","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_type","sqlite3_column_int64","sqlite3_column_double","sqlite3_column_text","sqlite3_column_blob","sqlite3_column_text","sqlite3_strnicmp","sqlite3_stmt_status","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_hard_heap_limit64","sqlite3_soft_heap_limit","sqlite3_open_v2","sqlite3_open","sqlite3_busy_timeout","sqlite3_busy_timeout","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_bind_text","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_type","sqlite3_column_int64","sqlite3_column_text","sqlite3_strnicmp","sqlite3_stmt_status","sqlite3_finalize","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_close","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize heap limits
    sqlite3_hard_heap_limit64(static_cast<sqlite3_int64>(size * 2));
    sqlite3_soft_heap_limit(static_cast<int>(size));
    
    // Open input database from memory stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    const char *input_filename = "input_file";
    FILE *input_db_file = fopen(input_filename, "wb");
    if (!input_db_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&input_db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Copy input data to file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, input_db_file);
    }
    assert_file_closed(&in_file);;
    assert_file_closed(&input_db_file);;
    
    // Open databases
    sqlite3 *src_db = nullptr;
    sqlite3 *dst_db = nullptr;
    
    int rc = sqlite3_open_v2(input_filename, &src_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !src_db) {
        remove(input_filename);
        assert_file_closed(&input_db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    rc = sqlite3_open(":memory:", &dst_db);
    if (rc != SQLITE_OK || !dst_db) {
        sqlite3_close(src_db);
        remove(input_filename);
        assert_file_closed(&input_db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Set busy timeout
    sqlite3_busy_timeout(src_db, 1000);
    sqlite3_busy_timeout(dst_db, 1000);
    
    // Prepare and execute statements on source database
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB);";
    
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    
    // Prepare and execute create table
    rc = sqlite3_prepare_v2(src_db, create_table_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Insert data with bindings
    const char *insert_sql = "INSERT INTO test_table (name, value, data) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(src_db, insert_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Bind text parameter (truncate if size > 0)
        const char *text_data = size > 0 ? reinterpret_cast<const char*>(data) : "";
        int text_len = static_cast<int>(size > 100 ? 100 : size);
        sqlite3_bind_text(stmt, 1, text_data, text_len, SQLITE_TRANSIENT);
        
        // Bind integer parameter
        int int_value = static_cast<int>(size);
        sqlite3_bind_int(stmt, 2, int_value);
        
        // Bind double parameter
        double double_value = static_cast<double>(size) / 1000.0;
        sqlite3_bind_double(stmt, 3, double_value);
        
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Bind different values
        sqlite3_bind_text(stmt, 1, "fuzz_test", 9, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, 42);
        sqlite3_bind_double(stmt, 3, 3.14159);
        sqlite3_step(stmt);
        
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Query data
    const char *select_sql = "SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(src_db, select_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_data_count(stmt);
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                switch (col_type) {
                    case SQLITE_INTEGER:
                        sqlite3_column_int64(stmt, i);
                        break;
                    case SQLITE_FLOAT:
                        sqlite3_column_double(stmt, i);
                        break;
                    case SQLITE_TEXT:
                        sqlite3_column_text(stmt, i);
                        break;
                    case SQLITE_BLOB:
                        sqlite3_column_blob(stmt, i);
                        break;
                    case SQLITE_NULL:
                        break;
                }
            }
            
            // Use string comparison
            const unsigned char *col_text = sqlite3_column_text(stmt, 1);
            if (col_text) {
                sqlite3_strnicmp(reinterpret_cast<const char*>(col_text), 
                                "fuzz", 4);
            }
        }
        
        // Get statement status
        int vm_used = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_VM_STEP, 1);
        (void)vm_used; // Prevent unused variable warning
        
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Perform backup
    sqlite3_backup *backup = sqlite3_backup_init(dst_db, "main", src_db, "main");
    if (backup) {
        while (sqlite3_backup_step(backup, 5) == SQLITE_OK) {
            // Continue backup
        }
        sqlite3_backup_finish(backup);
    }
    
    // Check memory usage
    sqlite3_int64 highwater = sqlite3_memory_highwater(1);
    (void)highwater; // Prevent unused variable warning
    
    // Release memory
    sqlite3_release_memory(static_cast<int>(size / 2));
    
    // Cleanup
    sqlite3_close(dst_db);
    sqlite3_close(src_db);
    
    // Remove temporary file
    remove(input_filename);
    
    assert_file_closed(&input_db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}