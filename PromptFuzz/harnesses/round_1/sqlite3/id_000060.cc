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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_open_v2","sqlite3_close","sqlite3_close","sqlite3_extended_result_codes","sqlite3_extended_result_codes","sqlite3_libversion_number","sqlite3_malloc64","sqlite3_create_collation_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_malloc64","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_free","sqlite3_finalize","sqlite3_complete16","sqlite3_db_filename","sqlite3_serialize","sqlite3_free","sqlite3_profile","sqlite3_malloc64","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_free","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_open_v2","sqlite3_extended_result_codes","sqlite3_extended_result_codes","sqlite3_libversion_number","sqlite3_malloc64","sqlite3_create_collation_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_malloc64","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_free","sqlite3_finalize","sqlite3_complete16","sqlite3_db_filename","sqlite3_serialize","sqlite3_free","sqlite3_profile","sqlite3_malloc64","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_free","sqlite3_close","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use in a safe way if needed

    // Open two database connections
    sqlite3 *db1 = nullptr;
    sqlite3 *db2 = nullptr;
    int rc1 = sqlite3_open(":memory:", &db1);
    int rc2 = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    
    if (rc1 != SQLITE_OK || rc2 != SQLITE_OK) {
        if (db1) sqlite3_close(db1);
        if (db2) sqlite3_close(db2);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Enable extended result codes
    sqlite3_extended_result_codes(db1, 1);
    sqlite3_extended_result_codes(db2, 1);

    // Get SQLite version
    int version = sqlite3_libversion_number();
    (void)version;

    // Allocate memory using sqlite3_malloc64
    size_t alloc_size = size > 1024 ? 1024 : size;
    void *allocated_mem = sqlite3_malloc64(alloc_size);
    if (allocated_mem && size > 0) {
        // Copy some input data to allocated memory
        size_t copy_size = alloc_size < size ? alloc_size : size;
        memcpy(allocated_mem, data, copy_size);
    }

    // Create a custom collation
    auto collation_func = [](void *pArg, int n1, const void *s1, int n2, const void *s2) -> int {
        // Simple byte-wise comparison
        const unsigned char *str1 = static_cast<const unsigned char*>(s1);
        const unsigned char *str2 = static_cast<const unsigned char*>(s2);
        int min_len = n1 < n2 ? n1 : n2;
        for (int i = 0; i < min_len; i++) {
            if (str1[i] != str2[i]) {
                return str1[i] - str2[i];
            }
        }
        return n1 - n2;
    };
    
    auto destroy_func = [](void *pArg) {
        // Cleanup function for collation
    };
    
    sqlite3_create_collation_v2(db1, "fuzz_collation", SQLITE_UTF8, 
                               allocated_mem, collation_func, destroy_func);

    // Create tables and execute SQL
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data TEXT);";
    char *errmsg = nullptr;
    
    sqlite3_exec(db1, create_table_sql, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }

    // Prepare and bind a statement using input data
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO fuzz_test (data) VALUES (?);";
    const char *tail = nullptr;
    
    if (sqlite3_prepare_v2(db1, insert_sql, -1, &stmt, &tail) == SQLITE_OK) {
        // Convert input data to null-terminated string for binding
        size_t bind_size = size > 255 ? 255 : size;
        char *bind_str = static_cast<char*>(sqlite3_malloc64(bind_size + 1));
        if (bind_str && size > 0) {
            size_t copy_len = bind_size < size ? bind_size : size;
            memcpy(bind_str, data, copy_len);
            bind_str[copy_len] = '\0';
            
            // Bind the text parameter
            sqlite3_bind_text(stmt, 1, bind_str, -1, SQLITE_STATIC);
            
            // Execute the statement
            int step_rc = sqlite3_step(stmt);
            (void)step_rc;
            
            // Reset statement for potential reuse
            sqlite3_reset(stmt);
            
            sqlite3_free(bind_str);
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    // Check if SQL is complete (using UTF-16)
    const void *test_sql16 = u"SELECT * FROM fuzz_test;";
    int is_complete = sqlite3_complete16(test_sql16);
    (void)is_complete;

    // Get database filename
    sqlite3_filename db_filename = sqlite3_db_filename(db1, "main");
    (void)db_filename;

    // Serialize the database
    sqlite3_int64 serialize_size = 0;
    unsigned char *serialized_data = sqlite3_serialize(db1, "main", &serialize_size, 0);
    if (serialized_data) {
        // Use the serialized data (could write to file or process)
        // For fuzzing, we just acknowledge it exists
        sqlite3_free(serialized_data);
    }

    // Set up profile callback
    auto profile_func = [](void *pArg, const char *sql, sqlite3_uint64 elapsed) {
        // Profile callback - could log or process timing info
    };
    sqlite3_profile(db1, profile_func, nullptr);

    // Execute more SQL using input data
    if (size > 0) {
        // Create a null-terminated string from input data
        size_t sql_size = size > 1023 ? 1023 : size;
        char *sql_str = static_cast<char*>(sqlite3_malloc64(sql_size + 1));
        if (sql_str) {
            memcpy(sql_str, data, sql_size);
            sql_str[sql_size] = '\0';
            
            // Try to execute as SQL if it looks reasonable
            if (sql_size > 10) {
                sqlite3_exec(db2, sql_str, nullptr, nullptr, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                }
            }
            
            sqlite3_free(sql_str);
        }
    }

    // Cleanup
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
    
    // Close database connections
    sqlite3_close(db1);
    sqlite3_close(db2);
    
    // Close the input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}