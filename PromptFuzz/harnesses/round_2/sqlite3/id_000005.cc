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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 630, nr_unique_branch: 145
//<Quality> {"density":18,"unique_branches":{"sqlite3Realloc":[[30325,7,30325,14,0,0,4,0]],"vdbeMemClear":[[82526,7,82526,18,0,0,4,1],[23230,3,23230,38,1,0,4,0]],"sqlite3GetVarint32":[[35825,7,35825,23,0,0,4,1],[35830,7,35830,23,0,0,4,0],[35830,7,35830,23,0,0,4,1],[35838,7,35838,34,0,0,4,1]],"sqlite3BtreePayloadChecked":[[75033,7,75033,33,0,0,4,0]],"sqlite3RCStrNew":[[32133,7,32133,11,0,0,4,1]],"sqlite3BtreeLockTable":[[81008,7,81008,18,0,0,4,1]],"releaseMemArray":[[86145,11,86145,37,0,0,4,0],[14098,27,14098,28,2,0,4,0],[14098,27,14098,28,5,0,4,0],[14098,27,14098,28,5,0,4,1]],"sqlite3VdbeAddOp4Int":[[84302,7,84302,21,0,0,4,0]],"sqlite3_compileoption_get":[[182647,7,182647,11,0,0,4,0],[182647,15,182647,21,0,0,4,0],[182647,15,182647,21,0,0,4,1]],"sqlite3_db_cacheflush":[[178509,7,178509,32,0,0,4,1],[178513,12,178513,25,0,0,4,0],[178513,29,178513,38,0,0,4,0],[178513,29,178513,38,0,0,4,1],[178515,9,178515,12,0,0,4,0],[178515,9,178515,12,0,0,4,1],[178515,16,178515,59,0,0,4,1],[178526,12,178526,25,0,0,4,0],[178526,29,178526,38,0,0,4,1]],"sqlite3_blob_reopen":[[101784,7,101784,11,0,0,4,1],[101788,7,101788,18,0,0,4,1],[101797,9,101797,22,0,0,4,1]],"sqlite3VdbeFreeCursorNN":[[86699,7,86699,20,0,0,4,0]],"sqlite3VdbeMemSetStr":[[83174,17,83174,38,0,0,4,1]],"vdbeMemClearExternAndSetNull":[[82502,7,82502,23,0,0,4,1],[82507,7,82507,23,0,0,4,0]],"sqlite3VdbeExec":[[95323,7,95323,31,0,0,4,1],[95354,10,95354,49,0,0,4,1],[95359,9,95359,39,0,0,4,1],[95378,11,95378,13,0,0,4,1],[100991,7,100991,64,0,0,4,0],[100992,7,100992,33,0,0,4,0],[100993,7,100993,54,0,0,4,0],[100993,7,100993,54,0,0,4,1],[100993,18,100993,27,0,0,4,1],[100996,9,100996,41,0,0,4,0]],"findNextHostParameter":[[92040,10,92040,17,0,0,4,0],[92040,10,92040,17,0,0,4,1],[92043,9,92043,31,0,0,4,0],[92043,9,92043,31,0,0,4,1]],"addOp4IntSlow":[[84215,7,84215,29,0,0,4,0]],"sqlite3_trace":[[179790,7,179790,32,0,0,4,1],[179797,16,179797,22,0,0,4,0]],"blobSeekToRow":[[101363,7,101363,14,0,0,4,0]],"sqlite3VdbeSerialTypeLen":[[87915,7,87915,23,0,0,4,1]],"blobReadWrite":[[101684,7,101684,11,0,0,4,1],[101689,7,101689,10,0,0,4,1],[101689,14,101689,23,0,0,4,1],[101689,27,101689,62,0,0,4,1],[101692,13,101692,17,0,0,4,1],[101705,9,101705,35,0,0,4,1],[101731,9,101731,25,0,0,4,1]],"sqlite3GetVarint":[[35659,7,35659,30,0,0,4,1],[35663,7,35663,30,0,0,4,1],[35677,7,35677,16,0,0,4,1],[35693,7,35693,16,0,0,4,0]],"vdbeColumnFromOverflow":[[92920,7,92920,42,0,0,4,1],[92921,7,92921,17,0,0,4,0],[92921,21,92921,36,0,0,4,0],[92934,9,92934,24,0,0,4,0],[92936,11,92936,24,0,0,4,1],[92940,9,92940,27,0,0,4,0],[92946,11,92946,26,0,0,4,1],[92948,11,92948,18,0,0,4,1],[92950,11,92950,13,0,0,4,1],[92963,9,92963,12,0,0,4,1]],"sqlite3VdbeExpandSql":[[92095,7,92095,22,0,0,4,1],[92103,13,92103,23,0,0,4,0],[92103,13,92103,23,0,0,4,1],[92106,12,92106,22,0,0,4,0],[92112,11,92112,20,0,0,4,0],[92112,11,92112,20,0,0,4,1],[92113,11,92113,26,0,0,4,0],[92114,13,92114,21,0,0,4,1],[92134,11,92134,33,0,0,4,1],[92136,17,92136,52,0,0,4,1],[92138,17,92138,39,0,0,4,1],[92140,17,92140,38,0,0,4,1],[92171,17,92171,39,0,0,4,1],[92181,18,92181,24,0,0,4,0],[92181,18,92181,24,0,0,4,1],[92193,7,92193,19,0,0,4,1],[14679,20,14679,27,6,0,4,0]],"accessPayload":[[74820,7,74820,29,0,0,4,1],[74830,7,74830,78,0,0,4,1],[74840,7,74840,31,0,0,4,0],[74842,9,74842,35,0,0,4,0],[74842,9,74842,35,0,0,4,1],[74854,7,74854,20,0,0,4,0],[74854,24,74854,29,0,0,4,0],[74854,24,74854,29,0,0,4,1],[74867,9,74867,45,0,0,4,0],[74869,11,74869,29,0,0,4,0],[74875,13,74875,20,0,0,4,1],[74896,12,74896,20,0,0,4,0],[74898,11,74898,32,0,0,4,1],[74904,11,74904,27,0,0,4,1],[74924,13,74924,34,0,0,4,0],[74924,13,74924,34,0,0,4,1],[74962,16,74962,22,0,0,4,0],[74964,15,74964,28,0,0,4,0],[74973,13,74973,19,0,0,4,0],[74973,13,74973,19,0,0,4,1],[74976,11,74976,13,0,0,4,1],[74981,7,74981,20,0,0,4,0],[74981,24,74981,29,0,0,4,1]],"copyPayload":[[74758,7,74758,10,0,0,4,1]],"freeCursorWithCache":[[86691,7,86691,22,0,0,4,0]],"sqlite3_str_vappendf":[[30970,9,30970,17,0,0,4,0],[30983,19,30983,27,0,0,4,0],[30986,18,30986,35,0,0,4,0],[30986,39,30986,45,0,0,4,1],[30992,15,30992,50,0,0,4,1],[30996,15,30996,21,0,0,4,0],[30996,25,30996,31,0,0,4,0],[31058,13,31058,18,0,0,4,0],[31058,22,31058,37,0,0,4,0],[31096,7,31096,19,0,0,4,0],[31100,13,31100,39,0,0,4,1],[31124,15,31124,23,0,0,4,1],[31126,21,31126,30,0,0,4,1],[31138,13,31138,25,0,0,4,0],[31138,29,31138,56,0,0,4,0],[31168,19,31168,30,0,0,4,0],[31171,16,31171,32,0,0,4,0],[14098,27,14098,28,3,0,4,1]],"sqlite3VdbeMemRelease":[[23230,3,23230,38,1,0,4,0]],"sqlite3BtreeTableMoveto":[[75452,9,75452,32,0,0,4,0]],"sqlite3RCStrUnref":[[32115,7,32115,19,0,0,4,0],[32115,7,32115,19,0,0,4,1]],"sqlite3_stmt_explain":[[91455,7,91455,15,0,0,4,1],[91458,7,91458,31,0,0,4,0],[91475,7,91475,17,0,0,4,1]],"sqlite3DbMallocSize":[[30175,7,30175,12,0,0,4,0],[30183,7,30183,9,0,0,4,1]],"sqlite3StrAccumEnlarge":[[31686,9,31686,14,0,0,4,1],[31693,29,31693,39,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_trace","sqlite3_compileoption_get","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_stmt_explain","sqlite3_step","sqlite3_db_cacheflush","sqlite3_snapshot_free","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_trace","sqlite3_compileoption_get","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_stmt_explain","sqlite3_step","sqlite3_db_cacheflush","sqlite3_snapshot_free","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


// Trace callback function for sqlite3_trace
static void  id_000005_traceCallback(void* userData, const char* sql) {
    // Simple trace callback that does nothing
    (void)userData;
    (void)sql;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Create a FILE* to read the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 2: Create a FILE* to write output data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* blob = nullptr;
    sqlite3_str* str_obj = nullptr;
    sqlite3_snapshot* snapshot = nullptr;
    
    // Step 3: Open an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Create a string object using sqlite3_str_new
    // Note: sqlite3_str_new is not in the provided list, but we'll simulate it
    // by using sqlite3_str object allocation through other means
    // Since sqlite3_str_new isn't available, we'll skip direct usage
    
    // Step 5: Set trace callback using sqlite3_trace
    void* trace_data = nullptr;
    sqlite3_trace(db,  id_000005_traceCallback, trace_data);
    
    // Step 6: Get compile options using sqlite3_compileoption_get
    int i = 0;
    while (const char* option = sqlite3_compileoption_get(i)) {
        // Write compile options to output file
        fwrite(option, strlen(option), 1, out_file);
        i++;
    }
    
    // Step 7: Create a simple table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        // Insert a row with blob data
        const char* insert_sql = "INSERT INTO test (data) VALUES (?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // Bind the input data as a blob
            sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size), SQLITE_STATIC);
            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
        
        // Step 8: Open a blob handle
        if (rc == SQLITE_DONE) {
            sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
            rc = sqlite3_blob_open(db, "main", "test", "data", rowid, 0, &blob);
            
            if (rc == SQLITE_OK && blob) {
                // Step 9: Use sqlite3_blob_reopen
                // Try to reopen to a different row (though we only have one)
                sqlite3_blob_reopen(blob, rowid);
                
                // Read blob data and write to output
                char buffer[1024];
                int blob_size = sqlite3_blob_bytes(blob);
                int bytes_to_read = blob_size < 1024 ? blob_size : 1024;
                
                if (bytes_to_read > 0) {
                    sqlite3_blob_read(blob, buffer, bytes_to_read, 0);
                    fwrite(buffer, bytes_to_read, 1, out_file);
                }
                
                sqlite3_blob_close(blob);
                blob = nullptr;
            }
        }
        
        // Step 10: Prepare a statement and use sqlite3_stmt_explain
        const char* select_sql = "SELECT * FROM test;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // Use sqlite3_stmt_explain
            sqlite3_stmt_explain(stmt, 0);  // 0 for normal EXPLAIN
            
            // Execute the statement
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                // Process row data if needed
            }
        }
    }
    
    // Step 11: Flush database cache
    sqlite3_db_cacheflush(db);
    
    // Step 12: Free snapshot (even though we don't have one, this is safe)
    sqlite3_snapshot_free(snapshot);
    
    // Step 13: Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}