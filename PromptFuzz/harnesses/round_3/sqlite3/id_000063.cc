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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"balance_nonroot":[[78424,13,78424,28,0,0,4,0],[78588,34,78588,49,0,0,4,0],[78592,14,78592,20,0,0,4,0],[78592,14,78592,20,0,0,4,1]]},"library_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_trace","sqlite3_complete16","sqlite3_str_new","sqlite3_errmsg16","sqlite3_str_appendall","sqlite3_str_appendall","sqlite3_str_appendall","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_stmt_status","sqlite3_column_text","sqlite3_finalize","sqlite3_str_finish","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_trace","sqlite3_complete16","sqlite3_str_new","sqlite3_errmsg16","sqlite3_str_appendall","sqlite3_str_appendall","sqlite3_str_appendall","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_stmt_status","sqlite3_column_text","sqlite3_finalize","sqlite3_str_finish","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Trace callback function
static void  id_000063_trace_callback(void* user_data, const char* sql) {
    // Do nothing in fuzzing - just satisfy API requirement
    (void)user_data;
    (void)sql;
}

// Exec callback function
static int  id_000063_exec_callback(void* user_data, int argc, char** argv, char** col_names) {
    // Do nothing in fuzzing - just satisfy API requirement
    (void)user_data;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<293) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_2, fuzz_char_array_size_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	
		const char * fuzzer_var_sqlite3_str_appendall_7_1 = fuzz_char_array_2;//fuzzer shim end}




    // 1. Check if SQLite is thread-safe
    int thread_safe = sqlite3_threadsafe();
    (void)thread_safe; // Use variable to avoid unused warning
    
    // 2. Open a database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        return 0; // Early exit if we can't open database
    }
    
    // 3. Set up trace callback
    sqlite3_trace(db,  id_000063_trace_callback, nullptr);
    
    // 4. Check if input contains a complete SQL statement (UTF-16)
    // Convert data to UTF-16 for sqlite3_complete16
    // We need to ensure we have an even number of bytes
    size_t utf16_size = (size / 2) * 2;
    if (utf16_size > 0) {
        // Create a buffer with null terminator
        void* utf16_data = malloc(utf16_size + 2);
        if (utf16_data) {
            memcpy(utf16_data, data, utf16_size);
            // Add null terminator (two zero bytes for UTF-16)
            ((char*)utf16_data)[utf16_size] = 0;
            ((char*)utf16_data)[utf16_size + 1] = 0;
            
            int is_complete = sqlite3_complete16(utf16_data);
            (void)is_complete; // Use variable
            
            free(utf16_data);
        }
    }
    
    // 5. Create a string builder
    sqlite3_str* str_builder = sqlite3_str_new(db);
    if (str_builder) {
        // 6. Get error message (UTF-16) and append to string builder
        const void* err_msg = sqlite3_errmsg16(db);
        if (err_msg) {
            // Convert UTF-16 to UTF-8 for appendall (simplified approach)
            // In real code, you'd need proper conversion
            char temp[256] = {0};
            sqlite3_str_appendall(str_builder, fuzz_str_1);
            sqlite3_str_appendall(str_builder, fuzzer_var_sqlite3_str_appendall_7_1);
        }
        
        // 7. Append input data as string (if we have at least some data)
        if (size > 0) {
            // Create a null-terminated copy of the data
            char* data_str = (char*)malloc(size + 1);
            if (data_str) {
                memcpy(data_str, data, size);
                data_str[size] = 0;
                sqlite3_str_appendall(str_builder, data_str);
                free(data_str);
            }
        }
        
        // 8. Execute SQL using the input data as SQL command
        if (size > 0) {
            // Create a null-terminated SQL string from input
            char* sql = (char*)malloc(size + 1);
            if (sql) {
                memcpy(sql, data, size);
                sql[size] = 0;
                
                char* err_msg_exec = nullptr;
                rc = sqlite3_exec(db, sql,  id_000063_exec_callback, nullptr, &err_msg_exec);
                
                if (err_msg_exec) {
                    sqlite3_free(err_msg_exec);
                }
                free(sql);
            }
        }
        
        // 9. Prepare a statement
        sqlite3_stmt* stmt = nullptr;
        const char* tail = nullptr;
        const char* simple_sql = "SELECT 1";
        rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt, &tail);
        
        if (rc == SQLITE_OK && stmt) {
            // 10. Bind a zeroblob with size from input
            sqlite3_uint64 blob_size = 0;
            if (size >= sizeof(blob_size)) {
                memcpy(&blob_size, data, sizeof(blob_size));
                // Limit size to avoid excessive memory usage
                blob_size = blob_size % 1024;
            }
            sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_3, fuzz_uint64_t_4);
            
            // 11. Step through the statement
            rc = sqlite3_step(stmt);
            
            // 12. Get statement status
            int status = sqlite3_stmt_status(stmt, fuzz_int32_t_5, fuzz_int32_t_6);
            (void)status; // Use variable
            
            // 13. Get column text if available
            if (rc == SQLITE_ROW) {
                const unsigned char* col_text = sqlite3_column_text(stmt, fuzz_int32_t_7);
                (void)col_text; // Use variable
            }
            
            // 14. Finalize the statement
            sqlite3_finalize(stmt);
        }
        
        // 15. Finish the string builder and free it
        char* finished_str = sqlite3_str_finish(str_builder);
        if (finished_str) {
            // Write to output file as per instructions
            FILE* out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(finished_str, 1, strlen(finished_str), out_file);
                assert_file_closed(&out_file);;
            }
            sqlite3_free(finished_str);
        }
    }
    
    // 16. Use fmemopen to read input data as file (as per instruction 4)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Get file descriptor (as per instruction 5)
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use variable
        
        // Read a small amount to satisfy the requirement
        char buf[16];
        fread(buf, 1, sizeof(buf) < size ? sizeof(buf) : size, in_file);
        assert_file_closed(&in_file);;
    }
    
    // 17. Clean up
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}