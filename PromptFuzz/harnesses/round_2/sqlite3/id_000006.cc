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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 864, nr_unique_branch: 155
//<Quality> {"density":24,"unique_branches":{"sqlite3HexToBlob":[[35926,7,35926,12,0,0,4,0],[35927,14,35927,17,0,0,4,0],[35927,14,35927,17,0,0,4,1]],"sqlite3VdbeNextOpcode":[[86223,7,86223,14,0,0,4,0],[86224,9,86224,29,0,0,4,1],[86230,14,86230,20,0,0,4,1],[86237,9,86237,16,0,0,4,0],[86237,9,86237,16,0,0,4,1],[86242,9,86242,17,0,0,4,0],[86265,9,86265,16,0,0,4,0],[86265,20,86265,48,0,0,4,1],[86284,9,86284,17,0,0,4,0]],"sqlite3VdbeExec":[[97900,9,97900,35,0,0,4,1],[98016,7,98016,28,0,0,4,0],[98018,7,98018,17,0,0,4,0],[98019,7,98019,50,0,0,4,0],[98034,7,98034,28,0,0,4,0],[18358,25,18358,63,615,0,4,0],[92294,31,92294,55,617,0,4,1],[92294,57,92294,78,617,0,4,1]],"allocateSpace":[[71598,8,71598,19,0,0,4,0],[71598,39,71598,49,0,0,4,0],[71600,9,71600,15,0,0,4,1],[71609,15,71609,17,0,0,4,1]],"sqlite3_sql":[[91553,10,91553,11,0,0,4,0]],"sqlite3VdbeDisplayComment":[[85704,7,85704,25,0,0,4,0],[85704,7,85704,25,0,0,4,1],[85708,9,85708,38,0,0,4,1],[85712,15,85712,37,0,0,4,0],[85712,15,85712,37,0,0,4,1],[85713,11,85713,17,0,0,4,0],[85713,11,85713,17,0,0,4,1],[85715,13,85715,19,0,0,4,0],[85715,13,85715,19,0,0,4,1],[85717,19,85717,25,0,0,4,0],[85717,19,85717,25,0,0,4,1],[85718,15,85718,28,0,0,4,0],[85718,15,85718,28,0,0,4,1],[85718,32,85718,48,0,0,4,0],[85726,15,85726,50,0,0,4,0],[85726,15,85726,50,0,0,4,1],[85729,17,85729,50,0,0,4,1],[85733,17,85733,21,0,0,4,1],[85738,21,85738,57,0,0,4,1],[85752,17,85752,54,0,0,4,1],[85761,9,85761,17,0,0,4,0],[85761,9,85761,17,0,0,4,1],[85761,21,85761,34,0,0,4,0],[85761,21,85761,34,0,0,4,1],[85764,13,85764,26,0,0,4,0],[85764,13,85764,26,0,0,4,1],[85767,7,85767,37,0,0,4,1]],"sqlite3BtreeInsert":[[79083,9,79083,26,0,0,4,0]],"sqlite3Reprepare":[[141111,7,141111,9,0,0,4,1]],"sqlite3_str_new":[[31884,7,31884,8,0,0,4,0],[31886,13,31886,15,0,0,4,0]],"dropCell":[[14098,27,14098,28,2,0,4,1]],"sqlite3_stmt_isexplain":[[91445,10,91445,15,0,0,4,0]],"translateP":[[85669,7,85669,13,0,0,4,0],[85669,7,85669,13,0,0,4,1],[85670,7,85670,13,0,0,4,0],[85670,7,85670,13,0,0,4,1],[85671,7,85671,13,0,0,4,0]],"sqlite3_stmt_explain":[[91458,7,91458,31,0,0,4,1],[91460,13,91460,20,0,0,4,1],[91460,24,91460,31,0,0,4,1],[91462,13,91462,55,0,0,4,1],[91464,13,91464,44,0,0,4,1],[91466,13,91466,24,0,0,4,1],[91475,7,91475,17,0,0,4,0]],"sqlite3VdbeDisplayP4":[[85860,5,85860,20,0,0,4,1],[85883,5,85883,20,0,0,4,1],[85891,5,85891,20,0,0,4,1],[85896,5,85896,20,0,0,4,1],[85901,5,85901,18,0,0,4,1],[85905,5,85905,18,0,0,4,0],[85905,5,85905,18,0,0,4,1],[85909,5,85909,17,0,0,4,1],[85913,5,85913,16,0,0,4,1],[85930,5,85930,17,0,0,4,1],[85936,5,85936,21,0,0,4,1],[85947,5,85947,23,0,0,4,1],[85951,5,85951,18,0,0,4,1],[85955,5,85955,12,0,0,4,0],[85955,5,85955,12,0,0,4,1],[85959,7,85959,10,0,0,4,0],[85959,7,85959,10,0,0,4,1],[85960,7,85960,37,0,0,4,1]],"sqlite3_str_vappendf":[[30973,9,30973,17,0,0,4,0],[30976,15,30976,21,0,0,4,0],[31104,21,31104,30,0,0,4,0],[31105,17,31105,29,0,0,4,0]],"sqlite3Step":[[90303,7,90303,17,0,0,4,0]],"sqlite3_str_finish":[[31818,7,31818,11,0,0,4,0],[31818,15,31818,32,0,0,4,0]],"sqlite3ExprCodeTarget":[[112532,5,112532,17,0,0,4,0]],"sqlite3CompleteInsertion":[[134262,19,134262,31,0,0,4,0]],"sqlite3VdbeExpandSql":[[92106,12,92106,22,0,0,4,1],[92136,17,92136,52,0,0,4,0]],"yy_reduce":[[174712,7,174712,15,0,0,4,0],[14098,27,14098,28,294,0,4,1]],"sqlite3_trace":[[179797,16,179797,22,0,0,4,1]],"sqlite3BtreeTableMoveto":[[75457,11,75457,44,0,0,4,1],[75465,11,75465,36,0,0,4,0],[75468,13,75468,26,0,0,4,0],[75470,15,75470,38,0,0,4,0],[75531,13,75531,20,0,0,4,0],[75554,9,75554,20,0,0,4,0]],"sqlite3VdbeList":[[86351,24,86351,37,0,0,4,0],[86365,7,86365,26,0,0,4,1],[86372,7,86372,20,0,0,4,0],[86386,7,86386,20,0,0,4,0],[86386,7,86386,20,0,0,4,1],[86394,11,86394,24,0,0,4,1],[86421,11,86421,27,0,0,4,1],[13868,32,13868,71,6,0,4,1]],"sqlite3TransferBindings":[[91384,12,91384,25,0,0,4,0],[91384,12,91384,25,0,0,4,1]],"sqlite3VdbeMakeReady":[[86631,7,86631,22,0,0,4,0],[86632,9,86632,16,0,0,4,0]],"freeSpace":[[71733,7,71733,16,0,0,4,1]],"sqlite3Update":[[152145,7,152145,24,0,0,4,0],[152281,11,152281,26,0,0,4,0],[152282,11,152282,20,0,0,4,0],[152283,11,152283,17,0,0,4,0],[152284,11,152284,19,0,0,4,0],[152285,11,152285,20,0,0,4,0],[152286,12,152286,21,0,0,4,1],[152286,25,152286,41,0,0,4,0],[152487,9,152487,23,0,0,4,0],[152628,9,152628,24,0,0,4,0],[152685,7,152685,24,0,0,4,0],[152685,28,152685,50,0,0,4,0],[152685,54,152685,64,0,0,4,0]],"sqlite3GetToken":[[176855,11,176855,21,0,0,4,0],[176858,13,176858,23,0,0,4,1],[176858,27,176858,30,0,0,4,1],[176862,13,176862,17,0,0,4,0],[20401,30,20401,72,111,0,4,0],[20401,30,20401,72,111,0,4,1]],"sqlite3Insert":[[132656,9,132656,21,0,0,4,0],[132656,25,132656,62,0,0,4,1]],"sqlite3_str_appendchar":[[31712,7,31712,35,0,0,4,0],[31712,7,31712,35,0,0,4,1],[31712,39,31712,76,0,0,4,1],[31715,10,31715,17,0,0,4,0],[31715,10,31715,17,0,0,4,1],[14098,27,14098,28,1,0,4,1]],"pageFindSlot":[[71494,10,71494,19,0,0,4,1],[71535,7,71535,23,0,0,4,1]],"sqlite3Prepare":[[140915,7,140915,17,0,0,4,0]],"invalidateIncrblobCursors":[[70342,31,70342,32,0,0,4,0],[70342,31,70342,32,0,0,4,1],[70343,9,70343,41,0,0,4,1]],"sqlite3StrAccumFinish":[[31779,7,31779,15,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_enable_shared_cache","sqlite3_exec","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_trace","sqlite3_compileoption_get","sqlite3_str_append","sqlite3_str_append","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_stmt_explain","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_db_cacheflush","sqlite3_snapshot_free","sqlite3_str_finish","sqlite3_free","sqlite3_trace","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_enable_shared_cache","sqlite3_exec","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_trace","sqlite3_compileoption_get","sqlite3_str_append","sqlite3_str_append","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_stmt_explain","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_db_cacheflush","sqlite3_snapshot_free","sqlite3_str_finish","sqlite3_free","sqlite3_trace","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful operations

    // Open memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Enable shared cache for tracing
    sqlite3_enable_shared_cache(1);

    // Create table with blob column
    const char *create_table_sql = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test VALUES (1, x'01020304');"
        "INSERT INTO test VALUES (2, x'05060708');";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);

    // Use sqlite3_str_new to create string builder
    sqlite3_str *str_builder = sqlite3_str_new(db);
    if (str_builder) {
        const char *prefix = "Data: ";
        sqlite3_str_append(str_builder, prefix, strlen(prefix));
        sqlite3_str_append(str_builder, reinterpret_cast<const char*>(data), 
                          size > 100 ? 100 : size);
    }

    // Setup trace callback using first 50 bytes as trace data
    struct TraceData {
        FILE *out_file;
        size_t bytes_written;
    };
    
    FILE *out_file = fopen("output_file", "wb");
    TraceData trace_data = {out_file, 0};
    
    auto trace_callback = [](void *user_data, const char *sql) {
        TraceData *td = static_cast<TraceData*>(user_data);
        if (td && td->out_file && sql) {
            fwrite(sql, 1, strlen(sql), td->out_file);
            td->bytes_written += strlen(sql);
        }
    };
    
    void *old_trace = sqlite3_trace(db, trace_callback, &trace_data);

    // Get compilation option using data-derived index
    int opt_index = static_cast<int>(data[0] % 50);
    const char *compile_option = sqlite3_compileoption_get(opt_index);
    if (compile_option && str_builder) {
        sqlite3_str_append(str_builder, "\nOption: ", 9);
        sqlite3_str_append(str_builder, compile_option, strlen(compile_option));
    }

    // Open blob handle
    sqlite3_blob *blob = nullptr;
    if (sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob) == SQLITE_OK) {
        // Try to reopen blob with rowid derived from input data
        sqlite3_int64 new_rowid = static_cast<sqlite3_int64>(data[1] % 2 + 1);
        sqlite3_blob_reopen(blob, new_rowid);
        
        // Read some blob data
        unsigned char buffer[10];
        if (sqlite3_blob_bytes(blob) >= 4) {
            sqlite3_blob_read(blob, buffer, 4, 0);
        }
        
        sqlite3_blob_close(blob);
    }

    // Prepare statement and explain it
    sqlite3_stmt *stmt = nullptr;
    const char *select_sql = "SELECT * FROM test WHERE id = ?";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind parameter from input data
        int param_value = static_cast<int>(data[2] % 10);
        sqlite3_bind_int(stmt, 1, param_value);
        
        // Explain the statement
        sqlite3_stmt_explain(stmt, 1);  // Use EXPLAIN mode
        
        // Execute and fetch
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row
        }
        sqlite3_finalize(stmt);
    }

    // Create statement for cache flush scenario
    sqlite3_stmt *stmt2 = nullptr;
    const char *update_sql = "UPDATE test SET data = ? WHERE id = ?";
    if (sqlite3_prepare_v2(db, update_sql, -1, &stmt2, nullptr) == SQLITE_OK) {
        // Bind blob data from input
        sqlite3_bind_blob(stmt2, 1, data, size > 50 ? 50 : size, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt2, 2, static_cast<int>(data[3] % 2 + 1));
        sqlite3_step(stmt2);
        sqlite3_finalize(stmt2);
    }

    // Flush database cache
    sqlite3_db_cacheflush(db);

    // Free snapshot (dummy call since we don't have a snapshot)
    sqlite3_snapshot_free(nullptr);

    // Cleanup
    if (str_builder) {
        // Get and use the built string
        char *built_str = sqlite3_str_finish(str_builder);
        if (built_str) {
            // Write to output file
            if (out_file) {
                fwrite(built_str, 1, strlen(built_str), out_file);
            }
            sqlite3_free(built_str);
        }
    }
    
    // Restore old trace and cleanup
    sqlite3_trace(db, nullptr, nullptr);  // Remove our trace
    if (old_trace) {
        // old_trace is function pointer, nothing to free
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }

    // Final cleanup
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	return 0;
}