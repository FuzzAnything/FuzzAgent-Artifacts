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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 299, nr_unique_branch: 62
//<Quality> {"density":13,"unique_branches":{"memdbFromDbSchema":[[52935,7,52935,9,0,0,4,1],[52936,7,52936,42,0,0,4,1],[52939,7,52939,24,0,0,4,1]],"sqlite3_vfs_find":[[26014,9,26014,37,0,0,4,1]],"memdbUnlock":[[52626,7,52626,26,0,0,4,0],[52626,7,52626,26,0,0,4,1],[52630,7,52630,32,0,0,4,1],[52635,9,52635,40,0,0,4,1]],"sqlite3_file_control":[[181655,9,181655,38,0,0,4,0]],"memdbLock":[[52569,7,52569,26,0,0,4,1],[52576,7,52576,31,0,0,4,1],[52580,7,52580,30,0,0,4,0],[52582,13,52582,25,0,0,4,1],[52590,7,52590,32,0,0,4,1],[52591,7,52591,31,0,0,4,1],[52603,7,52603,14,0,0,4,1],[52615,7,52615,20,0,0,4,0]],"sqlite3_mutex_enter":[[28656,7,28656,8,0,0,4,1]],"sqlite3BtreeSchema":[[80975,7,80975,20,0,0,4,1]],"memdbRead":[[52457,7,52457,23,0,0,4,0],[52457,7,52457,23,0,0,4,1],[52459,9,52459,20,0,0,4,0],[52459,9,52459,20,0,0,4,1]],"memdbOpen":[[52753,7,52753,15,0,0,4,1],[52802,9,52802,13,0,0,4,1],[52810,7,52810,19,0,0,4,0]],"sqlite3_mutex_free":[[28645,7,28645,8,0,0,4,1]],"sqlite3_bind_zeroblob64":[[91326,7,91326,11,0,0,4,1],[91329,7,91329,48,0,0,4,1]],"sqlite3PagerOpen":[[61476,22,61476,27,0,0,4,0]],"databaseIsUnmoved":[[60558,7,60558,26,0,0,4,0]],"memdbFileControl":[[52665,7,52665,31,0,0,4,1],[52669,7,52669,34,0,0,4,1]],"sqlite3PagerOpenWal":[[64044,9,64044,42,0,0,4,0]],"memdbClose":[[52409,7,52409,16,0,0,4,1],[52433,7,52433,17,0,0,4,0],[52434,9,52434,51,0,0,4,0]],"sqlite3BtreeOpen":[[72502,41,72502,55,0,0,4,1]],"sqlite3_mutex_leave":[[28682,7,28682,8,0,0,4,1]],"attachFunc":[[119357,9,119357,16,0,0,4,1],[119359,9,119359,22,0,0,4,0],[119361,11,119361,21,0,0,4,0],[119374,9,119374,11,0,0,4,1],[119471,9,119471,25,0,0,4,1],[119346,31,119346,53,2,0,4,0],[14155,25,14155,46,5,0,4,0],[14155,26,14155,29,5,0,4,0]],"sqlite3_update_hook":[[179900,7,179900,32,0,0,4,1]],"sqlite3PagerWalSupported":[[63957,61,63957,78,0,0,4,1]],"sqlite3_deserialize":[[53043,7,53043,32,0,0,4,1],[53046,7,53046,13,0,0,4,1],[53047,7,53047,14,0,0,4,1],[53051,7,53051,17,0,0,4,1],[53054,7,53054,12,0,0,4,0],[53054,16,53054,22,0,0,4,1],[53059,7,53059,14,0,0,4,1],[53065,7,53065,9,0,0,4,1],[53070,7,53070,22,0,0,4,1],[53075,7,53075,11,0,0,4,1],[53084,9,53084,54,0,0,4,0],[53093,7,53093,12,0,0,4,1],[14098,27,14098,28,4,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_create_function_v2","sqlite3_result_text","sqlite3_result_subtype","sqlite3_result_text","sqlite3_result_subtype","sqlite3_prepare_v3","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_update_hook","sqlite3_malloc","sqlite3_deserialize","sqlite3_errmsg16","sqlite3_exec","sqlite3_exec","sqlite3_update_hook","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_create_function_v2","sqlite3_result_subtype","sqlite3_result_text","sqlite3_result_subtype","sqlite3_result_text","sqlite3_prepare_v3","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_update_hook","sqlite3_malloc","sqlite3_deserialize","sqlite3_errmsg16","sqlite3_exec","sqlite3_exec","sqlite3_update_hook","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open an in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a custom function that uses sqlite3_result_subtype
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_func",
        1,
        SQLITE_UTF8,
        nullptr,
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            // Set result with a subtype
            sqlite3_result_text(ctx, "test", 4, SQLITE_TRANSIENT);
            sqlite3_result_subtype(ctx, 123);  // Using sqlite3_result_subtype
        },
        nullptr,
        nullptr,
        nullptr
    );

    // Prepare a statement to bind zeroblob
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "SELECT fuzz_func(?)";
    rc = sqlite3_prepare_v3(db, sql, -1, 0, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind zeroblob using the input data size
        sqlite3_bind_zeroblob64(stmt, 1, static_cast<sqlite3_uint64>(size));  // Using sqlite3_bind_zeroblob64
        
        // Step through the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Do nothing, just consume rows
        }
        
        sqlite3_finalize(stmt);
    }

    // Set up update hook
    void *hook_data = nullptr;
    void (*update_hook_callback)(void *, int, const char *, const char *, sqlite3_int64) = 
        [](void *data, int op, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
            // Update hook callback
        };
    
    void *old_hook = sqlite3_update_hook(db, update_hook_callback, hook_data);  // Using sqlite3_update_hook

    // Deserialize database from the input data
    // First, read the entire input file into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size > 0) {
        unsigned char *buffer = static_cast<unsigned char*>(sqlite3_malloc(file_size));
        if (buffer) {
            fread(buffer, 1, file_size, in_file);
            
            // Try to deserialize the database
            rc = sqlite3_deserialize(
                db,
                "main",
                buffer,
                static_cast<sqlite3_int64>(file_size),
                static_cast<sqlite3_int64>(file_size),
                SQLITE_DESERIALIZE_READONLY | SQLITE_DESERIALIZE_FREEONCLOSE  // Using sqlite3_deserialize
            );
            
            if (rc != SQLITE_OK) {
                // Get error message using sqlite3_errmsg16
                const void *err_msg = sqlite3_errmsg16(db);  // Using sqlite3_errmsg16
                // Error message can be used for debugging
            }
        }
    }

    // Create a simple table and insert data to trigger update hook
    const char *create_table = "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data BLOB)";
    sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    
    const char *insert_data = "INSERT INTO fuzz_table (data) VALUES (zeroblob(100))";
    sqlite3_exec(db, insert_data, nullptr, nullptr, nullptr);

    // Clean up
    assert_file_closed(&in_file);;
    
    // Restore previous update hook
    if (old_hook) {
        sqlite3_update_hook(db, 
            reinterpret_cast<void(*)(void *, int, const char *, const char *, sqlite3_int64)>(old_hook), 
            nullptr);
    }
    
    // Close database
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}