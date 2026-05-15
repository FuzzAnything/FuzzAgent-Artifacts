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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 11
//<Quality> {"density":19,"unique_branches":{"sqlite3_autovacuum_pages":[[179976,7,179976,32,0,0,4,1],[179982,7,179982,24,0,0,4,1]],"sqlite3_enable_load_extension":[[136425,7,136425,32,0,0,4,1],[136428,7,136428,12,0,0,4,0]],"sqlite3VdbeExec":[[14098,27,14098,28,387,0,4,0],[14098,27,14098,28,388,0,4,0],[14098,27,14098,28,389,0,4,0],[14098,27,14098,28,390,0,4,0],[14098,27,14098,28,391,0,4,0]],"sqlite3_is_interrupted":[[179446,7,179446,32,0,0,4,1]],"columnName":[[90971,9,90971,17,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_enable_load_extension","sqlite3_create_collation_v2","sqlite3_autovacuum_pages","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_table_name16","sqlite3_finalize","sqlite3_create_function","sqlite3_exec","sqlite3_vsnprintf","sqlite3_is_interrupted","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_enable_load_extension","sqlite3_create_collation_v2","sqlite3_autovacuum_pages","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_table_name16","sqlite3_finalize","sqlite3_create_function","sqlite3_exec","sqlite3_vsnprintf","sqlite3_is_interrupted","sqlite3_close"],"visited":0}
/**/


// Custom collation comparison function
static int  id_000015_customCollationFunc(void* pArg, int n1, const void* p1, int n2, const void* p2) {
    // Simple byte-by-byte comparison
    int minLen = n1 < n2 ? n1 : n2;
    int cmp = memcmp(p1, p2, minLen);
    if (cmp == 0) {
        return n1 - n2;
    }
    return cmp;
}

// Custom collation destroy function
static void  id_000015_customCollationDestroy(void* pArg) {
    // Nothing to destroy in this simple case
}

// Autovacuum callback function
static unsigned int  id_000015_autovacuumCallback(void* pArg, const char* zDb, 
                                       unsigned int nPage, unsigned int nFree, 
                                       unsigned int nDirty) {
    // Return a simple value based on input
    return nPage > 100 ? 10 : 5;
}

// User-defined function for testing set_auxdata
static void  id_000015_testFunction(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Set auxdata if we have a string argument
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_TEXT) {
        const char* str = (const char*)sqlite3_value_text(argv[0]);
        if (str) {
            char* aux = strdup(str);
            sqlite3_set_auxdata(context, 0, aux, free);
        }
    }
    sqlite3_result_int(context, 0);
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<137) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Open an in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 1. Enable load extension
    sqlite3_enable_load_extension(db, fuzz_int32_t_1);
    
    // 2. Create custom collation using input data as collation name
    if (size > 0) {
        // Use first 50 bytes or less as collation name
        size_t nameLen = size > 50 ? 50 : size;
        char* collationName = (char*)malloc(nameLen + 1);
        if (collationName) {
            memcpy(collationName, data, nameLen);
            collationName[nameLen] = '\0';
            
            // Use sqlite3_create_collation (v2 version)
            sqlite3_create_collation_v2(
                db, 
                collationName, 
                fuzz_int32_t_2, 
                nullptr, 
                 id_000015_customCollationFunc, 
                 id_000015_customCollationDestroy
            );
            free(collationName);
        }
    }
    
    // 3. Set up autovacuum pages callback
    sqlite3_autovacuum_pages(db,  id_000015_autovacuumCallback, nullptr, nullptr);
    
    // 4. Create a table and insert data
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER, name TEXT);";
    sqlite3_exec(db, fuzz_str_3, nullptr, nullptr, nullptr);
    
    // Insert some data using input
    sqlite3_stmt* stmt = nullptr;
    const char* insertSQL = "INSERT INTO test_table (id, name) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use first 4 bytes as integer if available
        int idValue = size >= 4 ? *(int*)data : 1;
        sqlite3_bind_int(stmt, fuzz_int32_t_4, idValue);
        
        // Use part of input as string
        if (size > 4) {
            size_t strLen = size - 4 > 100 ? 100 : size - 4;
            sqlite3_bind_text(stmt, fuzz_int32_t_5, (const char*)data + 4, strLen, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_text(stmt, fuzz_int32_t_6, "test", 4, SQLITE_TRANSIENT);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // 5. Query data and use sqlite3_column_table_name16
    const char* selectSQL = "SELECT * FROM test_table;";
    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get table name (16-bit version)
            const void* tableName = sqlite3_column_table_name16(stmt, fuzz_int32_t_7);
            // Can't do much with it in fuzzing context, but API is called
        }
        sqlite3_finalize(stmt);
    }
    
    // 6. Create and use a user-defined function to test set_auxdata
    sqlite3_create_function(
        db, 
        fuzz_str_8, 
        fuzz_int32_t_9, 
        fuzz_int32_t_10, 
        nullptr, 
         id_000015_testFunction, 
        nullptr, 
        nullptr
    );
    
    // Call the function
    sqlite3_exec(db, fuzz_str_11, nullptr, nullptr, nullptr);
    
    // 7. Use sqlite3_vsnprintf with input data
    if (size > 0) {
        char buffer[256];
        // Use input as format string (limited to safe characters)
        char format[100];
        size_t formatLen = size > 99 ? 99 : size;
        memcpy(format, data, formatLen);
        format[formatLen] = '\0';
        
        // Replace any % characters to avoid format string vulnerabilities
        for (size_t i = 0; i < formatLen; i++) {
            if (format[i] == '%') format[i] = '_';
        }
        
        sqlite3_vsnprintf(sizeof(buffer), buffer, format, nullptr);
        
        // Write result to output file
        fwrite(buffer, 1, strlen(buffer), out_file);
    }
    
    // 8. Check if interrupted
    sqlite3_is_interrupted(db);
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}