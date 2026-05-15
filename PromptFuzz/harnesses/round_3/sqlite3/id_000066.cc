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
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 111, nr_unique_branch: 7
//<Quality> {"density":37,"unique_branches":{"vdbeCommit":[[86915,22,86915,41,0,0,4,0],[86917,9,86917,11,0,0,4,1]],"sqlite3_get_table":[[150010,7,150010,20,0,0,4,1],[150014,7,150014,27,0,0,4,0],[150017,9,150017,17,0,0,4,1],[150025,7,150025,15,0,0,4,0],[150026,7,150026,12,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_snapshot_free","sqlite3_free_table","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_close","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_mutex_leave","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_get_table","sqlite3_free_table","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_commit_hook","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_snapshot_open","sqlite3_exec","sqlite3_snapshot_free","sqlite3_serialize","sqlite3_free","sqlite3_create_function_v2","sqlite3_set_auxdata","sqlite3_get_auxdata","sqlite3_result_int","sqlite3_result_int","sqlite3_set_auxdata","sqlite3_get_auxdata","sqlite3_result_int","sqlite3_result_int","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_parameter_index","sqlite3_bind_int","sqlite3_finalize"],"critical_calls":["sqlite3_open_v2","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_mutex_leave","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_get_table","sqlite3_free_table","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_commit_hook","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_snapshot_open","sqlite3_exec","sqlite3_snapshot_free","sqlite3_serialize","sqlite3_free","sqlite3_create_function_v2","sqlite3_result_int","sqlite3_result_int","sqlite3_get_auxdata","sqlite3_set_auxdata","sqlite3_result_int","sqlite3_result_int","sqlite3_get_auxdata","sqlite3_set_auxdata","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_parameter_index","sqlite3_bind_int","sqlite3_finalize","sqlite3_snapshot_free","sqlite3_free_table","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Custom callback functions for trace and commit hook
static int  id_000066_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    return 0;
}

static int  id_000066_commit_hook_callback(void* context) {
    return 0;
}

// Custom SQL function implementation
static void  id_000066_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    int value = sqlite3_value_int(argv[0]);
    sqlite3_result_int(context, value * 2);
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<274) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_20, fuzz_str_sz_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
	//fuzzer shim end}




    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char** table_result = nullptr;
    sqlite3_snapshot* snapshot = nullptr;
    void* aux_data = nullptr;
    int rows = 0, cols = 0;
    char* errmsg = nullptr;
    
    // 1. Open database
    int rc = sqlite3_open_v2(fuzz_str_1, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        cleanup:
        if (snapshot) sqlite3_snapshot_free(snapshot);
        if (table_result) sqlite3_free_table(table_result);
        if (stmt) sqlite3_finalize(stmt);
        if (errmsg) sqlite3_free(errmsg);
        if (aux_data) sqlite3_free(aux_data);
        if (db) sqlite3_close(db);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Setup trace
    sqlite3_trace_v2(db, fuzz_uint32_t_2,  id_000066_trace_callback, nullptr);
    
    // 3. Get and lock database mutex
    sqlite3_mutex* mutex = sqlite3_db_mutex(db);
    sqlite3_mutex_enter(mutex);
    sqlite3_mutex_leave(mutex); // Must leave after entering
    
    // 4. Create table and insert data using fuzzer input
    rc = sqlite3_exec(db, fuzz_str_3, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) goto cleanup;
    
    // Prepare INSERT statement
    const char* insert_sql = "INSERT INTO test VALUES(?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) goto cleanup;
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Bind parameters using fuzzer data
    if (param_count >= 2 && size >= 2) {
        // First parameter: integer from first byte
        int int_val = data[0];
        
        // Second parameter: text from remaining data (with null termination)
        size_t text_len = (size - 1 > 1000) ? 1000 : size - 1;
        char* text_buf = (char*)malloc(text_len + 1);
        if (text_buf) {
            memcpy(text_buf, data + 1, text_len);
            text_buf[text_len] = '\0';
            
            // Bind values
            sqlite3_bind_int(stmt, fuzz_int32_t_4, int_val);
            sqlite3_bind_text(stmt, fuzz_int32_t_5, text_buf, -1, SQLITE_TRANSIENT);
            
            // Execute statement
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                // Reset for potential reuse
                sqlite3_reset(stmt);
            }
            
            free(text_buf);
        }
    }
    
    // 5. Finalize the statement
    sqlite3_finalize(stmt);
    stmt = nullptr;
    
    // 6. Use get_table to query data
    rc = sqlite3_get_table(db, fuzz_str_6, &table_result, &rows, &cols, &errmsg);
    if (rc != SQLITE_OK) goto cleanup;
    
    // 7. Free the table result
    sqlite3_free_table(table_result);
    table_result = nullptr;
    
    // 8. Create custom function with auxdata
    rc = sqlite3_create_function_v2(
        db, fuzz_str_7, fuzz_int32_t_8, fuzz_int32_t_9, 
        nullptr,  id_000066_custom_function, nullptr, nullptr, nullptr
    );
    if (rc != SQLITE_OK) goto cleanup;
    
    // Use the custom function in a query
    rc = sqlite3_prepare_v2(db, "SELECT custom_double(5)", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 9. Setup commit hook
    sqlite3_commit_hook(db,  id_000066_commit_hook_callback, nullptr);
    
    // 10. Try to create and use snapshot (requires write-ahead logging)
    rc = sqlite3_exec(db, fuzz_str_10, nullptr, nullptr, &errmsg);
    if (rc == SQLITE_OK) {
        // Begin transaction for snapshot
        sqlite3_exec(db, fuzz_str_11, nullptr, nullptr, nullptr);
        
        // Insert another row for snapshot
        sqlite3_exec(db, fuzz_str_12, nullptr, nullptr, nullptr);
        
        // Try to open snapshot
        rc = sqlite3_snapshot_open(db, fuzz_str_13, snapshot);
        
        // Commit transaction
        sqlite3_exec(db, fuzz_str_14, nullptr, nullptr, nullptr);
        
        // Free snapshot if created
        if (snapshot) {
            sqlite3_snapshot_free(snapshot);
            snapshot = nullptr;
        }
    }
    
    // 11. Serialize the database
    sqlite3_int64 serial_size = 0;
    unsigned char* serial_data = sqlite3_serialize(db, fuzz_str_15, &serial_size, fuzz_uint32_t_16);
    if (serial_data) {
        // Write serialized data to output file
        if (serial_size > 0 && out_file) {
            fwrite(serial_data, 1, serial_size, out_file);
        }
        // Free the serialized data
        sqlite3_free(serial_data);
    }
    
    // 12. Set and get auxdata in a function context
    // This requires creating a scalar function that uses auxdata
    rc = sqlite3_create_function_v2(
        db, fuzz_str_17, fuzz_int32_t_18, fuzz_int32_t_19, 
        nullptr, [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            // Set auxdata
            int* data = new int(42);
            sqlite3_set_auxdata(ctx, 0, data, [](void* p) { delete (int*)p; });
            
            // Get auxdata
            void* retrieved = sqlite3_get_auxdata(ctx, 0);
            if (retrieved) {
                sqlite3_result_int(ctx, *(int*)retrieved);
            } else {
                sqlite3_result_int(ctx, 0);
            }
        }, 
        nullptr, nullptr, nullptr
    );
    
    if (rc == SQLITE_OK) {
        rc = sqlite3_prepare_v2(db, "SELECT test_aux(1)", -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // 13. Get parameter index by name
    rc = sqlite3_prepare_v2(db, "SELECT * FROM test WHERE id = :id", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        int idx = sqlite3_bind_parameter_index(stmt, fuzz_str_20);
        if (idx > 0) {
            sqlite3_bind_int(stmt, idx, fuzz_int32_t_21);
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Cleanup
    goto cleanup;
}