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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"btreeParseCellPtr":[[14098,27,14098,28,1,0,4,0]],"cellSizePtrTableLeaf":[[14098,27,14098,28,1,0,4,0]]},"library_calls":["sqlite3_mutex_alloc","sqlite3_mutex_enter","sqlite3_open_v2","sqlite3_mutex_leave","sqlite3_mutex_free","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_bind_text64","sqlite3_step","sqlite3_finalize","sqlite3_log","sqlite3_malloc","sqlite3_create_collation16","sqlite3_free","sqlite3_create_window_function","sqlite3_result_int64","sqlite3_value_int64","sqlite3_result_int64","sqlite3_value_int64","sqlite3_result_int64","sqlite3_result_int64","sqlite3_result_int64","sqlite3_result_int64","sqlite3_close_v2","sqlite3_mutex_leave","sqlite3_mutex_free"],"critical_calls":["sqlite3_mutex_alloc","sqlite3_mutex_enter","sqlite3_open_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_bind_text64","sqlite3_step","sqlite3_finalize","sqlite3_log","sqlite3_malloc","sqlite3_create_collation16","sqlite3_free","sqlite3_create_window_function","sqlite3_result_int64","sqlite3_result_int64","sqlite3_result_int64","sqlite3_result_int64","sqlite3_result_int64","sqlite3_value_int64","sqlite3_result_int64","sqlite3_value_int64","sqlite3_close_v2","sqlite3_mutex_leave","sqlite3_mutex_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for basic operations
    if (size < 4) {
	return 0;
	}
	

    // Allocate mutex first to synchronize operations
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
    if (!mutex) {
	return 0;
	}
	
    
    sqlite3_mutex_enter(mutex);

    // Create in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        sqlite3_mutex_leave(mutex);
        sqlite3_mutex_free(mutex);
        return 0;
    }

    // Create a temporary table
    const char *create_table_sql = "CREATE TABLE temp (id INTEGER, content TEXT)";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);

    // Prepare insert statement
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO temp (id, content) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind integer parameter
        sqlite3_bind_int64(stmt, 1, size);
        
        // Bind text parameter using sqlite3_bind_text64 with input data
        // Use up to first half of input as text
        size_t text_len = size / 2;
        const char *text_data = reinterpret_cast<const char*>(data);
        rc = sqlite3_bind_text64(stmt, 2, text_data, text_len, 
                                 SQLITE_TRANSIENT, SQLITE_UTF8);
        
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }

    // Use sqlite3_log with part of input as format string
    if (size > 10) {
        // Create safe format string (copy first 10 bytes + null terminator)
        char format_buf[11];
        memcpy(format_buf, data, 10);
        format_buf[10] = '\0';
        sqlite3_log(SQLITE_NOTICE, "Fuzzer: %s", format_buf);
    }

    // Create collation using sqlite3_create_collation16
    // Use second half of input for collation name
    if (size > 20) {
        size_t collation_start = size / 2;
        size_t collation_len = size - collation_start;
        
        // Create UTF-16 version of collation name
        // Simple conversion: treat each byte as low byte of UTF-16 character
        void *collation_name = sqlite3_malloc(collation_len * 2 + 2);
        if (collation_name) {
            uint16_t *utf16_name = static_cast<uint16_t*>(collation_name);
            for (size_t i = 0; i < collation_len; i++) {
                utf16_name[i] = static_cast<uint16_t>(data[collation_start + i]);
            }
            utf16_name[collation_len] = 0;  // Null terminator
            
            sqlite3_create_collation16(db, collation_name, SQLITE_UTF16, 
                reinterpret_cast<void*>(const_cast<uint8_t*>(data)),
                [](void *pArg, int len1, const void *str1, int len2, const void *str2) -> int {
                    // Simple comparison: compare first bytes
                    const uint8_t *data = static_cast<const uint8_t*>(pArg);
                    const uint16_t *s1 = static_cast<const uint16_t*>(str1);
                    const uint16_t *s2 = static_cast<const uint16_t*>(str2);
                    
                    if (len1 > 0 && len2 > 0) {
                        return (int)(s1[0] - s2[0]);
                    }
                    return len1 - len2;
                });
            
            sqlite3_free(collation_name);
        }
    }

    // Create window function using sqlite3_create_window_function
    // Use first 4 bytes as function name
    char func_name[5];
    if (size >= 4) {
        memcpy(func_name, data, 4);
        func_name[4] = '\0';
        
        sqlite3_create_window_function(
            db,
            func_name,
            1,  // nArg
            SQLITE_UTF8,
            nullptr,  // pApp
            // xStep callback
            [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
                if (argc > 0) {
                    sqlite3_result_int64(ctx, sqlite3_value_int64(argv[0]));
                }
            },
            // xFinal callback
            [](sqlite3_context *ctx) {
                sqlite3_result_int64(ctx, 0);
            },
            // xValue callback
            [](sqlite3_context *ctx) {
                sqlite3_result_int64(ctx, 0);
            },
            // xInverse callback
            [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
                // Do nothing for inverse
            },
            // xDestroy callback
            nullptr  // No destruction needed
        );
    }

    // Cleanup
    sqlite3_close_v2(db);
    sqlite3_mutex_leave(mutex);
    sqlite3_mutex_free(mutex);

    return 0;
}