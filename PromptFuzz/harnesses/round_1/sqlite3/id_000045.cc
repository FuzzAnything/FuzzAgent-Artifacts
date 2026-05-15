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
//<ID> 294
//<Prompt> []
/*<Combination>: [
*/
//<score> 85, nr_unique_branch: 9
//<Quality> {"density":17,"unique_branches":{"sqlite3CreateFunc":[[179511,5,179511,22,0,0,4,0],[179530,5,179530,21,0,0,4,1],[179557,26,179557,35,0,0,4,0]],"sqlite3_mutex_alloc":[[28625,7,28625,33,0,0,4,1],[28626,7,28626,32,0,0,4,0],[28626,36,28626,54,0,0,4,1]],"pthreadMutexFree":[[29151,35,29151,64,0,0,4,1]],"pthreadMutexAlloc":[[29127,11,29127,20,0,0,4,0],[29127,24,29127,42,0,0,4,0]]},"library_calls":["sqlite3_mutex_alloc","sqlite3_open","sqlite3_mutex_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_close_v2","sqlite3_mutex_free","sqlite3_mutex_enter","sqlite3_bind_int64","sqlite3_bind_text64","sqlite3_mutex_leave","sqlite3_step","sqlite3_reset","sqlite3_create_function16","sqlite3_create_window_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2","sqlite3_mutex_free"],"critical_calls":["sqlite3_mutex_alloc","sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_mutex_enter","sqlite3_bind_int64","sqlite3_bind_text64","sqlite3_mutex_leave","sqlite3_step","sqlite3_reset","sqlite3_create_function16","sqlite3_create_window_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2","sqlite3_mutex_free"],"visited":0}
/**/


// Callback functions for window function
static void  id_000045_windowStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Do nothing for fuzzing
}

static void  id_000045_windowFinal(sqlite3_context *ctx) {
    sqlite3_result_int64(ctx, 0);
}

static void  id_000045_windowValue(sqlite3_context *ctx) {
    sqlite3_result_int64(ctx, 0);
}

static void  id_000045_windowInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Do nothing for fuzzing
}

static void  id_000045_windowDestroy(void *p) {
    // Do nothing for fuzzing
}

// Callback for collation
static int  id_000045_collationCompare(void *pArg, int n1, const void *v1, int n2, const void *v2) {
    // Simple byte comparison for fuzzing
    int len = n1 < n2 ? n1 : n2;
    int cmp = memcmp(v1, v2, len);
    if (cmp == 0) {
        return n1 - n2;
    }
    return cmp;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	  // Need minimum data
    
    // Parse data for configuration
    // First 4 bytes: mutex type
    // Next 8 bytes: text64 length
    // Remaining: string data
    
    int mutex_type = *(int*)data;
    sqlite3_uint64 text_len = *(sqlite3_uint64*)(data + 4);
    
    // Ensure text_len doesn't exceed available data
    size_t available_data = size - 12;
    if (text_len > available_data) {
        text_len = available_data;
    }
    
    const char *text_data = (const char*)(data + 12);
    
    // 1. Allocate mutex
    sqlite3_mutex *mutex = sqlite3_mutex_alloc(mutex_type);
    if (!mutex) {
	return 0;
	}
	
    
    // 2. Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        sqlite3_mutex_free(mutex);
        return 0;
    }
    
    // 3. Prepare a statement for binding
    sqlite3_stmt *stmt = nullptr;
    const char *create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER, content TEXT)";
    rc = sqlite3_prepare_v2(db, create_table, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    const char *insert_sql = "INSERT INTO test VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
        sqlite3_mutex_free(mutex);
        return 0;
    }
    
    // 4. Use mutex to protect binding operation
    sqlite3_mutex_enter(mutex);
    
    // Bind parameters
    sqlite3_bind_int64(stmt, 1, 1);
    
    // 5. Bind text64 with data from input
    rc = sqlite3_bind_text64(stmt, 2, text_data, text_len, 
                            SQLITE_TRANSIENT, SQLITE_UTF8);
    
    sqlite3_mutex_leave(mutex);
    
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
    }
    sqlite3_reset(stmt);
    
    // 6. Create collation (using create_collation16 equivalent)
    // Note: sqlite3_create_collation16 isn't in exported list, but we can use
    // create_function16 for similar purpose with a collation-like function
    rc = sqlite3_create_function16(db, (const void*)u"test_collation", 
                                   2, SQLITE_UTF16, nullptr, 
                                   nullptr, nullptr, nullptr);
    
    // 7. Create window function
    rc = sqlite3_create_window_function(
        db, "test_window", 1, SQLITE_UTF8, nullptr,
         id_000045_windowStep,  id_000045_windowFinal,  id_000045_windowValue,  id_000045_windowInverse,  id_000045_windowDestroy);
    
    // 8. Execute a query using the window function
    const char *window_sql = "SELECT test_window(id) OVER (ORDER BY id) FROM test";
    sqlite3_stmt *window_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, window_sql, -1, &window_stmt, nullptr);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(window_stmt) == SQLITE_ROW) {
            // Do nothing, just consume results
        }
        sqlite3_finalize(window_stmt);
    }
    
    // 9. Log operations (simulate logging)
    // Note: sqlite3_log isn't in exported list, so we'll skip this
    // but we can use other logging mechanisms if needed
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    sqlite3_mutex_free(mutex);
    
    return 0;
}