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
//<score> 140, nr_unique_branch: 62
//<Quality> {"density":14,"unique_branches":{"sqlite3_cancel_auto_extension":[[136532,7,136532,15,0,0,4,0]],"sqlite3_complete":[[177348,13,177348,25,0,0,4,1],[177353,16,177353,23,0,0,4,0],[177353,16,177353,23,0,0,4,1],[177353,28,177353,40,0,0,4,0],[177353,28,177353,40,0,0,4,1],[177353,44,177353,56,0,0,4,0],[177354,13,177354,23,0,0,4,0],[177399,19,177399,25,0,0,4,0],[177399,29,177399,66,0,0,4,1],[177409,25,177409,31,0,0,4,0],[177409,35,177409,70,0,0,4,1],[177411,25,177411,31,0,0,4,0],[177411,35,177411,75,0,0,4,1],[177419,19,177419,25,0,0,4,0],[177419,29,177419,63,0,0,4,1]],"sqlite3_deserialize":[[53043,7,53043,32,0,0,4,1],[53046,7,53046,13,0,0,4,1],[53047,7,53047,14,0,0,4,1],[53051,7,53051,17,0,0,4,1],[53054,7,53054,12,0,0,4,0],[53054,16,53054,22,0,0,4,1],[53059,7,53059,14,0,0,4,1],[53065,7,53065,9,0,0,4,1],[53070,7,53070,22,0,0,4,1],[53075,7,53075,11,0,0,4,1],[53084,9,53084,54,0,0,4,0],[53093,7,53093,12,0,0,4,1],[14098,27,14098,28,4,0,4,1]],"memdbFileControl":[[52665,7,52665,31,0,0,4,1],[52669,7,52669,34,0,0,4,1]],"sqlite3PagerLockingMode":[[63740,19,63740,36,0,0,4,0],[63740,40,63740,75,0,0,4,0]],"memdbUnlock":[[52626,7,52626,26,0,0,4,0]],"sqlite3BtreeOpen":[[72502,11,72502,37,0,0,4,0],[72502,41,72502,55,0,0,4,1]],"fillInCell":[[14098,27,14098,28,6,0,4,0]],"sqlite3_free_filename":[[182315,7,182315,11,0,0,4,0]],"memdbFromDbSchema":[[52936,7,52936,42,0,0,4,1],[52939,7,52939,24,0,0,4,1]],"memdbRead":[[52457,7,52457,23,0,0,4,0],[52459,9,52459,20,0,0,4,1]],"sqlite3_vfs_find":[[26014,9,26014,37,0,0,4,1]],"sqlite3VdbeExec":[[95867,30,95867,39,0,0,4,1],[97906,7,97906,30,0,0,4,0]],"attachFunc":[[119357,9,119357,16,0,0,4,1],[119359,9,119359,22,0,0,4,0],[119361,11,119361,21,0,0,4,0],[119374,9,119374,11,0,0,4,1],[119471,9,119471,25,0,0,4,1],[119346,31,119346,53,2,0,4,0],[14155,25,14155,46,5,0,4,0],[14155,26,14155,29,5,0,4,0]],"sqlite3PagerOpen":[[61476,22,61476,27,0,0,4,0]],"memdbOpen":[[52753,7,52753,15,0,0,4,1],[52802,9,52802,13,0,0,4,1],[52810,7,52810,19,0,0,4,0]],"memdbClose":[[52409,7,52409,16,0,0,4,1],[52433,7,52433,17,0,0,4,0],[52434,9,52434,51,0,0,4,0]],"sqlite3BtreeSchema":[[80975,7,80975,20,0,0,4,1]],"sqlite3WalHeapMemory":[[68622,11,68622,15,0,0,4,1]]},"library_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_close","sqlite3_trace","sqlite3_complete16","sqlite3_str_new","sqlite3_errmsg16","sqlite3_str_appendall","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_stmt_status","sqlite3_stmt_status","sqlite3_finalize","sqlite3_open","sqlite3_deserialize","sqlite3_close","sqlite3_close","sqlite3_cancel_auto_extension","sqlite3_str_finish","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_trace","sqlite3_complete16","sqlite3_str_new","sqlite3_errmsg16","sqlite3_str_appendall","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_stmt_status","sqlite3_stmt_status","sqlite3_finalize","sqlite3_open","sqlite3_deserialize","sqlite3_close","sqlite3_cancel_auto_extension","sqlite3_str_finish","sqlite3_free","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<106) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    // 1. Check thread safety first
    int is_threadsafe = sqlite3_threadsafe();
    
    // 2. Create a temporary database for testing
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // 3. Set up trace callback (simplified version)
    void *trace_context = nullptr;
    sqlite3_trace(db, [](void *context, const char *sql) {
        // Simple trace callback
    }, trace_context);
    
    // 4. Check if input is a complete SQL statement (UTF-16)
    // Convert data to UTF-16 if possible
    if (size >= 2) {
        // Create a null-terminated UTF-16 string from data
        size_t utf16_size = size / 2;
        uint16_t *utf16_data = new uint16_t[utf16_size + 1];
        for (size_t i = 0; i < utf16_size; i++) {
            utf16_data[i] = (data[i*2] << 8) | data[i*2 + 1];
        }
        utf16_data[utf16_size] = 0;
        
        int is_complete = sqlite3_complete16(utf16_data);
        delete[] utf16_data;
    }
    
    // 5. Create a string builder and append error message
    sqlite3_str *str_builder = sqlite3_str_new(db);
    if (str_builder) {
        // Get error message in UTF-16 and convert to UTF-8 for appending
        const void *err_msg_utf16 = sqlite3_errmsg16(db);
        if (err_msg_utf16) {
            // Convert first 100 chars for simplicity
            char utf8_buf[256];
            int i = 0;
            const uint16_t *src = (const uint16_t *)err_msg_utf16;
            while (i < 255 && src[i]) {
                utf8_buf[i] = src[i] & 0xFF;
                i++;
            }
            utf8_buf[i] = 0;
            sqlite3_str_appendall(str_builder, utf8_buf);
        }
    }
    
    // 6. Prepare a statement and bind zeroblob
    sqlite3_stmt *stmt = nullptr;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // Use first 8 bytes of data as blob size
            sqlite3_uint64 blob_size = 0;
            if (size >= sizeof(blob_size)) {
                memcpy(&blob_size, data, sizeof(blob_size));
                blob_size = blob_size % 1024; // Limit size for safety
            }
            
            rc = sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_2, fuzz_uint64_t_3);
            
            if (rc == SQLITE_OK) {
                rc = sqlite3_step(stmt);
                
                // Get statement status
                if (rc == SQLITE_DONE) {
                    int run_count = sqlite3_stmt_status(stmt, fuzz_int32_t_4, fuzz_int32_t_5);
                    int vm_steps = sqlite3_stmt_status(stmt, fuzz_int32_t_6, fuzz_int32_t_7);
                }
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    // 7. Try to deserialize data as database
    if (size > 100) { // Need minimum size for deserialization
        // Use a subset of data for deserialization
        size_t deserialize_size = size > 1024 ? 1024 : size;
        unsigned char *db_data = new unsigned char[deserialize_size];
        memcpy(db_data, data, deserialize_size);
        
        // Create another database for deserialization
        sqlite3 *db2 = nullptr;
        rc = sqlite3_open(":memory:", &db2);
        if (rc == SQLITE_OK) {
            // Try to deserialize the data
            sqlite3_deserialize(db2, fuzz_str_8, db_data, 
                               (sqlite3_int64)deserialize_size,
                               (sqlite3_int64)deserialize_size,
                               SQLITE_DESERIALIZE_FREEONCLOSE);
            // Note: SQLITE_DESERIALIZE_FREEONCLOSE will free db_data
            sqlite3_close(db2);
        } else {
            delete[] db_data;
            if (db2) sqlite3_close(db2);
        }
    }
    
    // 8. Cancel auto-extension (try to cancel a non-existent one)
    sqlite3_cancel_auto_extension(nullptr);
    
    // 9. Clean up string builder
    if (str_builder) {
        char *str_result = sqlite3_str_finish(str_builder);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    // 10. Clean up database
    sqlite3_close(db);
    
    return 0;
}