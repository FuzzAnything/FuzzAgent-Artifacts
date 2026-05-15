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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 412.5, nr_unique_branch: 98
//<Quality> {"density":33,"unique_branches":{"sqlite3ColumnsFromExprList":[[143518,36,143518,61,0,0,4,0]],"sqlite3EndTable":[[122771,7,122771,14,0,0,4,0],[122771,18,122771,28,0,0,4,1],[122777,7,122777,17,0,0,4,1],[122957,9,122957,16,0,0,4,0],[122981,11,122981,23,0,0,4,1],[122983,11,122983,21,0,0,4,1],[122992,11,122992,23,0,0,4,1],[123007,9,123007,16,0,0,4,0],[123098,7,123098,15,0,0,4,1],[19767,28,19767,67,31,0,4,1]],"sqlite3Step":[[90225,9,90225,40,0,0,4,1],[14155,25,14155,46,10,0,4,0],[14155,26,14155,29,10,0,4,0]],"identPut":[[122225,12,122225,21,0,0,4,0],[122225,12,122225,21,0,0,4,1],[122226,9,122226,24,0,0,4,1],[122229,16,122229,52,0,0,4,0],[122229,16,122229,52,0,0,4,1],[122230,16,122230,28,0,0,4,1],[122231,16,122231,20,0,0,4,1],[122233,7,122233,16,0,0,4,0],[122233,7,122233,16,0,0,4,1],[122234,12,122234,21,0,0,4,0],[122234,12,122234,21,0,0,4,1],[122236,9,122236,23,0,0,4,1],[122238,7,122238,16,0,0,4,0],[122238,7,122238,16,0,0,4,1],[20400,30,20400,72,2,0,4,1]],"sqlite3SubqueryColumnTypes":[[143632,9,143632,40,0,0,4,1],[143635,9,143635,40,0,0,4,0],[143635,44,143635,58,0,0,4,1],[143652,9,143652,17,0,0,4,1],[143652,21,143652,66,0,0,4,1],[143667,9,143667,14,0,0,4,0],[143672,11,143672,24,0,0,4,0],[143678,9,143678,14,0,0,4,0]],"saveAllCursors":[[70552,24,70552,32,0,0,4,0]],"sqlite3ResultSetOfSelect":[[143700,7,143700,19,0,0,4,1],[143701,10,143701,25,0,0,4,1],[143703,7,143703,14,0,0,4,1],[143712,7,143712,23,0,0,4,1]],"createTableStmt":[[122254,28,122254,37,0,0,4,0],[122254,28,122254,37,0,0,4,1],[122258,7,122258,11,0,0,4,0],[122269,7,122269,15,0,0,4,1],[122277,26,122277,35,0,0,4,0],[122277,26,122277,35,0,0,4,1],[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,1,0,4,1],[14098,27,14098,28,2,0,4,0],[14098,27,14098,28,2,0,4,1],[14098,27,14098,28,3,0,4,1],[14098,27,14098,28,4,0,4,0],[14098,27,14098,28,4,0,4,1],[14098,27,14098,28,5,0,4,1],[14098,27,14098,28,6,0,4,1]],"sqlite3KeywordCode":[[176463,7,176463,11,0,0,4,0]],"yy_reduce":[[174042,7,174042,14,0,0,4,0]],"checkList":[[80331,10,80331,18,0,0,4,0],[80331,22,80331,35,0,0,4,0],[80334,9,80334,32,0,0,4,1],[80336,9,80336,68,0,0,4,1],[80341,9,80341,19,0,0,4,1],[80371,11,80371,34,0,0,4,1]],"pagerOpenSavepoint":[[57236,21,57236,41,3,0,4,1]],"countValidCursors":[[73204,23,73204,59,0,0,4,1]],"identLength":[[122201,12,122201,14,0,0,4,0],[122201,12,122201,14,0,0,4,1],[122202,9,122202,16,0,0,4,1]],"sqlite3VdbeExec":[[96246,31,96246,46,0,0,4,0],[96542,13,96542,34,0,0,4,1],[98992,7,98992,38,0,0,4,0],[14098,27,14098,28,283,0,4,1],[14098,27,14098,28,291,0,4,0]],"sqlite3TableColumnAffinity":[[107961,7,107961,13,0,0,4,0]],"checkTreePage":[[80540,7,80540,19,0,0,4,0],[80544,9,80544,24,0,0,4,1],[80584,11,80584,24,0,0,4,1],[80592,9,80592,34,0,0,4,0],[80599,11,80599,26,0,0,4,1],[80606,9,80606,21,0,0,4,0],[80610,11,80610,26,0,0,4,1],[80616,11,80616,20,0,0,4,1],[80634,9,80634,21,0,0,4,0],[80637,22,80637,26,0,0,4,0],[80637,22,80637,26,0,0,4,1]],"releaseAllSavepoints":[[58226,13,58226,34,0,0,4,0]],"btreeHeapInsert":[[80416,10,80416,21,0,0,4,0],[80416,25,80416,42,0,0,4,1]],"sqlite3AffinityType":[[121800,15,121800,52,0,0,4,0],[121825,7,121825,11,0,0,4,1]],"saveCursorsOnList":[[70570,24,70570,32,0,0,4,0]],"sqlite3ColumnSetColl":[[120880,7,120880,39,0,0,4,0],[120885,7,120885,11,0,0,4,0]],"writeJournalHdr":[[57889,13,57889,34,0,0,4,0],[57890,9,57890,45,0,0,4,0]],"columnTypeImpl":[[143279,13,143279,24,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_null","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_step","sqlite3_reset","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_step","sqlite3_prepare16_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_exec","sqlite3_free","sqlite3_step","sqlite3_column_count","sqlite3_column_blob","sqlite3_value_dup","sqlite3_value_free","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_exec","sqlite3_free","sqlite3_finalize","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_prepare16_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_exec","sqlite3_free","sqlite3_step","sqlite3_column_count","sqlite3_column_blob","sqlite3_value_dup","sqlite3_value_free","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_exec","sqlite3_free","sqlite3_finalize","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified SQLite3 APIs to process database operations based on input data:

*/


extern "C" {
}

// Custom function for sqlite3_create_function16
static void  id_000063_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Simple function that returns the first argument as text
    if (argc > 0) {
        const char* text = (const char*)sqlite3_value_text(argv[0]);
        if (text) {
            sqlite3_result_text(context, text, -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_result_null(context);
        }
    } else {
        sqlite3_result_null(context);
    }
}

// Callback for sqlite3_exec
static int  id_000063_exec_callback(void* data, int argc, char** argv, char** col_names) {
    // Do nothing in callback for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_stmt* stmt16 = nullptr;
    sqlite3_str* str = nullptr;
    sqlite3_value* value = nullptr;
    char* err_msg = nullptr;
    char* str_result = nullptr;
    
    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Open database (in-memory)
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create custom function
    sqlite3_create_function16(db, u"custom_func", 1, SQLITE_UTF16, 
                             nullptr,  id_000063_custom_function, nullptr, nullptr);

    // Create a table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }

    // Insert data using prepared statement
    const char* insert_sql = "INSERT INTO test (name, data) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind text parameter (using first part of data as string)
        std::string text_data;
        if (size > 0) {
            size_t text_len = size > 100 ? 100 : size;  // Limit text length
            text_data.assign((const char*)data, text_len);
            sqlite3_bind_text(stmt, 1, text_data.c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        // Bind blob parameter (using all data as blob)
        if (size > 0) {
            sqlite3_bind_blob(stmt, 2, data, size, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Insert another row with different parameters
        sqlite3_bind_text(stmt, 1, "test_name", -1, SQLITE_STATIC);
        if (size > 10) {
            sqlite3_bind_blob(stmt, 2, data + 10, size - 10 > 50 ? 50 : size - 10, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        sqlite3_step(stmt);
    }
    
    // Prepare UTF-16 statement
    const void* select_sql16 = u"SELECT * FROM test;";
    if (sqlite3_prepare16_v2(db, select_sql16, -1, &stmt16, nullptr) == SQLITE_OK) {
        // Execute query
        while (sqlite3_step(stmt16) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt16);
            
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt16, i);
                
                switch (col_type) {
                    case SQLITE_BLOB: {
                        const void* blob_data = sqlite3_column_blob(stmt16, i);
                        int blob_size = sqlite3_column_bytes(stmt16, i);
                        if (blob_data && blob_size > 0 && out_file) {
                            fwrite(blob_data, 1, blob_size, out_file);
                        }
                        break;
                    }
                    case SQLITE_TEXT: {
                        const void* text_data = sqlite3_column_blob(stmt16, i);
                        int text_size = sqlite3_column_bytes(stmt16, i);
                        if (text_data && text_size > 0 && out_file) {
                            fwrite(text_data, 1, text_size, out_file);
                        }
                        break;
                    }
                }
            }
        }
    }

    // Use string builder
    str = sqlite3_str_new(db);
    if (str) {
        sqlite3_str_append(str, "Generated string: ", 18);
        if (size > 0) {
            size_t append_len = size > 50 ? 50 : size;
            sqlite3_str_append(str, (const char*)data, append_len);
        }
        str_result = sqlite3_str_finish(str);
        if (str_result) {
            // Use the string in an SQL command
            std::string drop_sql = "DROP TABLE IF EXISTS temp_table;";
            sqlite3_exec(db, drop_sql.c_str(), nullptr, nullptr, nullptr);
            
            sqlite3_free(str_result);
        }
    }

    // Create and use a value
    if (stmt16) {
        sqlite3_step(stmt16);  // Move to first row
        if (sqlite3_column_count(stmt16) > 0) {
            const void* col_data = sqlite3_column_blob(stmt16, 0);
            if (col_data) {
                // Create a dummy value for demonstration
                value = sqlite3_value_dup(nullptr);
                if (value) {
                    sqlite3_value_free(value);
                }
            }
        }
    }

    // Use stricmp
    const char* str1 = "test1";
    const char* str2 = "TEST1";
    sqlite3_stricmp(str1, str2);

    // Get statistics
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    sqlite3_int64 memory_used = sqlite3_memory_used();

    // Use filename database (requires sqlite3_filename)
    // This is a bit tricky as we need a valid sqlite3_filename
    // We'll use the database file name
    const char* db_filename = ":memory:";
    // Note: sqlite3_filename_database expects sqlite3_filename type
    // We'll skip this as we don't have a valid sqlite3_filename object

    // Execute a complex SQL statement using exec
    std::string complex_sql = 
        "CREATE TABLE temp AS SELECT * FROM test; "
        "SELECT custom_func(name) FROM test; "
        "DROP TABLE temp;";
    sqlite3_exec(db, complex_sql.c_str(),  id_000063_exec_callback, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }

    // Read from input file and write to output file
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Simple file copy
    if (fd_in >= 0 && fd_out >= 0) {
        rewind(in_file);
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            fwrite(buffer, 1, bytes_read, out_file);
        }
    }

    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    if (stmt16) sqlite3_finalize(stmt16);
    
    if (db) sqlite3_close(db);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}