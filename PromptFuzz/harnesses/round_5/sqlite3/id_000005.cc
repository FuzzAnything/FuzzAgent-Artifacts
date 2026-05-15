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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 171, nr_unique_branch: 36
//<Quality> {"density":9,"unique_branches":{"sqlite3VdbeExec":[[95165,17,95165,50,0,0,4,0],[95167,13,95167,15,0,0,4,1]],"windowCodeOp":[[169636,17,169636,32,0,0,4,1],[169637,19,169637,23,0,0,4,1]],"sqlite3WindowRewrite":[[168284,9,168284,14,0,0,4,1],[168324,26,168324,34,0,0,4,1]],"sqlite3_hard_heap_limit64":[[29962,9,29962,30,0,0,4,1],[29962,34,29962,56,0,0,4,1]],"sqlite3WindowCodeStep":[[170165,18,170165,26,0,0,4,1],[170244,18,170244,26,0,0,4,1]],"sqlite3BtreeCursorRestore":[[70718,7,70718,9,0,0,4,1],[70722,7,70722,33,0,0,4,1],[70664,4,70664,33,1,0,4,0]],"sqlite3MallocAlarm":[[30033,7,30033,29,0,0,4,0]],"sqlite3VdbeHandleMovedCursor":[[87760,7,87760,21,0,0,4,1]],"windowReadPeerValues":[[168902,7,168902,15,0,0,4,1]],"sqlite3_value_dup":[[89818,7,89818,37,0,0,4,1],[89825,13,89825,35,0,0,4,1]],"sqlite3BtreeDelete":[[79461,7,79461,33,0,0,4,0],[79462,9,79462,41,0,0,4,0],[79465,11,79465,13,0,0,4,1],[79465,17,79465,43,0,0,4,1]],"flattenSubquery":[[145650,7,145650,14,0,0,4,0],[17754,41,17754,71,1,0,4,1]],"sqlite3_compileoption_used":[[182624,7,182624,49,0,0,4,0],[182631,9,182631,62,0,0,4,1]],"sqlite3Realloc":[[30351,11,30351,27,0,0,4,1]],"sqlite3Select":[[148773,9,148773,26,0,0,4,1],[148806,9,148806,26,0,0,4,1],[148815,9,148815,45,0,0,4,1],[148821,9,148821,25,0,0,4,1],[148822,9,148822,26,0,0,4,0]],"translateColumnToCopy":[[160990,9,160990,31,0,0,4,1],[160996,15,160996,36,0,0,4,1]],"mallocWithAlarm":[[30057,7,30057,28,0,0,4,1]],"windowIfNewPeer":[[169337,7,169337,15,0,0,4,1]]},"library_calls":["sqlite3_hard_heap_limit64","sqlite3_compileoption_used","sqlite3_open","sqlite3_profile","sqlite3_create_window_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_db_filename","sqlite3_close"],"critical_calls":["sqlite3_hard_heap_limit64","sqlite3_compileoption_used","sqlite3_open","sqlite3_profile","sqlite3_create_window_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_db_filename","sqlite3_close"],"visited":0}
/**/


// Window function callbacks
static void  id_000005_window_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Use sqlite3_value_dup to duplicate the first argument
    if (argc > 0) {
        sqlite3_value *dup_val = sqlite3_value_dup(argv[0]);
        if (dup_val) {
            // Get integer from duplicated value
            int val = sqlite3_value_int(dup_val);
            // Free the duplicated value
            sqlite3_free(dup_val);
            // Store in aggregate context
            int *sum = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
            if (sum) *sum += val;
        }
    }
}

static void  id_000005_window_final(sqlite3_context *ctx) {
    int *sum = (int*)sqlite3_aggregate_context(ctx, 0);
    if (sum) {
        sqlite3_result_int(ctx, *sum);  // Use sqlite3_result_int
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

static void  id_000005_window_value(sqlite3_context *ctx) {
     id_000005_window_final(ctx);
}

static void  id_000005_window_inverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Optional inverse function for window frames
    if (argc > 0) {
        int *sum = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
        if (sum) {
            int val = sqlite3_value_int(argv[0]);
            *sum -= val;
        }
    }
}

// Profile callback
static void  id_000005_profile_callback(void *data, const char *sql, sqlite3_uint64 elapsed) {
    FILE *out_file = (FILE*)data;
    if (out_file) {
        fprintf(out_file, "SQL: %s, Time: %llu\n", sql, (unsigned long long)elapsed);
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Parse input data
    if (size < 1) {
	return 0;
	}
	
    
    // Use first byte as heap limit multiplier
    sqlite3_int64 heap_limit = sqlite3_hard_heap_limit64((sqlite3_int64)data[0] * 1024 * 1024);
    
    // Use next bytes as compile option name (null-terminated)
    const char *compile_option = "ENABLE_JSON1";  // Default
    if (size > 1) {
        // Find null terminator in remaining data
        size_t opt_len = 0;
        while (opt_len < size - 1 && data[1 + opt_len] != 0) {
            opt_len++;
        }
        if (opt_len > 0 && opt_len < size - 1) {
            char *opt_name = (char*)malloc(opt_len + 1);
            if (opt_name) {
                memcpy(opt_name, data + 1, opt_len);
                opt_name[opt_len] = 0;
                int used = sqlite3_compileoption_used(opt_name);
                free(opt_name);
                (void)used;  // Use result to avoid unused warning
            }
        }
    }
    
    // Open in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Set profile callback with output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        sqlite3_profile(db,  id_000005_profile_callback, out_file);
    }
    
    // Create window function
    const char *func_name = "fuzz_window_sum";
    sqlite3_create_window_function(
        db,
        func_name,
        1,  // nArg
        SQLITE_UTF8,
        nullptr,  // pApp
         id_000005_window_step,
         id_000005_window_final,
         id_000005_window_value,
         id_000005_window_inverse,
        nullptr  // xDestroy
    );
    
    // Create a test table and insert data
    const char *create_sql = 
        "CREATE TABLE test(id INTEGER PRIMARY KEY, value INTEGER);"
        "INSERT INTO test(value) VALUES (10), (20), (30);";
    sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    
    // Execute a query using the window function
    const char *query = "SELECT fuzz_window_sum(value) OVER () FROM test;";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row
        }
        sqlite3_finalize(stmt);
    }
    
    // Get database filename (for main database)
    sqlite3_filename db_file = sqlite3_db_filename(db, "main");
    (void)db_file;  // Use to avoid unused warning
    
    // Cleanup
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	return 0;
}