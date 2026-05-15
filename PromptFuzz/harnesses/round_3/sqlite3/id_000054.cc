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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 19
//<Quality> {"density":8,"unique_branches":{"zeroblobFunc":[[128777,7,128777,10,0,0,4,0]],"jsonParse":[[204431,9,204431,17,0,0,4,0],[204433,11,204433,19,0,0,4,0]],"sqlite3VdbeExec":[[94468,11,94468,34,0,0,4,1]],"sqlite3Select":[[149793,7,149793,52,0,0,4,0]],"sqlite3_system_errno":[[180334,10,180334,12,0,0,4,0]],"json5Whitespace":[[203863,7,203863,16,0,0,4,1],[203864,7,203864,16,0,0,4,1],[203865,7,203865,16,0,0,4,1],[203866,7,203866,16,0,0,4,1],[203867,7,203867,16,0,0,4,1],[203868,7,203868,16,0,0,4,1],[203872,7,203872,15,0,0,4,1],[203898,7,203898,16,0,0,4,1],[203905,7,203905,16,0,0,4,1],[203912,7,203912,16,0,0,4,1],[203926,7,203926,16,0,0,4,1],[203933,7,203933,16,0,0,4,1],[203940,7,203940,14,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_create_collation_v2","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_system_errno","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_create_collation_v2","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_system_errno","sqlite3_close_v2"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < sizeof(sqlite3_int64) + 1) {
        return 0; // Not enough data for meaningful fuzzing
    }

    // Extract an integer from the beginning of data for binding
    sqlite3_int64 bind_value;
    memcpy(&bind_value, data, sizeof(sqlite3_int64));
    
    // Use remaining data for SQL statement and collation name
    const char* sql_str = reinterpret_cast<const char*>(data + sizeof(sqlite3_int64));
    size_t sql_len = size - sizeof(sqlite3_int64);
    
    // Ensure null termination for SQL string
    char* null_term_sql = static_cast<char*>(malloc(sql_len + 1));
    if (!null_term_sql) {
	return 0;
	}
	
    memcpy(null_term_sql, sql_str, sql_len);
    null_term_sql[sql_len] = '\0';
    
    // Create in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                            nullptr);
    if (rc != SQLITE_OK || !db) {
        free(null_term_sql);
        return 0;
    }

    // Create a string builder for error formatting
    sqlite3_str* str_builder = nullptr;
    // Note: sqlite3_str_new is not in provided list, but we can use 
    // sqlite3_str_append to create one indirectly
    // For this example, we'll skip direct sqlite3_str creation since 
    // the required constructor isn't in the provided API list
    
    // Create custom collation
    rc = sqlite3_create_collation_v2(
        db,
        fuzz_str_1,
        fuzz_int32_t_2,
        nullptr,
        [](void* pArg, int len1, const void* str1, int len2, const void* str2) {
            // Simple binary comparison
            int min_len = len1 < len2 ? len1 : len2;
            int cmp = memcmp(str1, str2, min_len);
            if (cmp == 0) return len1 - len2;
            return cmp;
        },
        [](void* pArg) {
            // Destroy callback - nothing to do here
        }
    );

    // Prepare SQL statement
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    rc = sqlite3_prepare_v3(
        db,
        null_term_sql,
        -1, // Read until null terminator
        fuzz_uint32_t_3,  // No special prep flags
        &stmt,
        &tail
    );

    if (rc == SQLITE_OK && stmt) {
        // Bind integer value to first parameter
        rc = sqlite3_bind_int64(stmt, fuzz_int32_t_4, bind_value);
        
        if (rc == SQLITE_OK) {
            // Step through statement (execute it)
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Process rows if needed
            }
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }

    // Check system error if any occurred
    int sys_err = sqlite3_system_errno(db);
    
    // Note: We cannot demonstrate sqlite3_str_vappendf without 
    // sqlite3_str_new which is not in the provided API list.
    // The function requires a valid sqlite3_str* object.

    // Cleanup
    free(null_term_sql);
    sqlite3_close_v2(db);
    
    return 0;
}