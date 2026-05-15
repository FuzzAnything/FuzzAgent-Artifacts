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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 0
//<Quality> {"density":35,"unique_branches":{},"library_calls":["sqlite3_open_v2","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_free","sqlite3_open","sqlite3_hard_heap_limit64","sqlite3_soft_heap_limit","sqlite3_busy_timeout","sqlite3_trace_v2","sqlite3_malloc","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_bind_text","sqlite3_free","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_column_int64","sqlite3_finalize","sqlite3_strnicmp","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_free"],"critical_calls":["sqlite3_open_v2","sqlite3_open","sqlite3_hard_heap_limit64","sqlite3_soft_heap_limit","sqlite3_busy_timeout","sqlite3_trace_v2","sqlite3_malloc","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_bind_text","sqlite3_free","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_reset","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_column_int64","sqlite3_finalize","sqlite3_strnicmp","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_free"],"visited":0}
/**/


extern "C" {
}

// Trace callback for sqlite3_trace_v2
static int  id_000072_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    FILE* trace_file = (FILE*)context;
    if (trace_file) {
        // Just write a simple marker to indicate trace was called
        static const char marker[] = "TRACE\n";
        fwrite(marker, 1, sizeof(marker) - 1, trace_file);
    }
    return 0;
}

// Callback for sqlite3_exec
static int  id_000072_exec_callback(void* data, int argc, char** argv, char** colNames) {
    FILE* out_file = (FILE*)data;
    if (out_file && argc > 0 && argv && argv[0]) {
        fwrite(argv[0], 1, strlen(argv[0]), out_file);
        fputc('\n', out_file);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create input file from the data
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

    sqlite3* source_db = nullptr;
    sqlite3* dest_db = nullptr;
    sqlite3_backup* backup = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* err_msg = nullptr;
    int rc = 0;

    // Open source database
    rc = sqlite3_open_v2(":memory:", &source_db, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 
                        nullptr);
    if (rc != SQLITE_OK) {
        cleanup:
        if (stmt) sqlite3_finalize(stmt);
        if (backup) sqlite3_backup_finish(backup);
        if (source_db) sqlite3_close(source_db);
        if (dest_db) sqlite3_close(dest_db);
        if (err_msg) sqlite3_free(err_msg);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Open destination database
    rc = sqlite3_open("output_file.db", &dest_db);
    if (rc != SQLITE_OK) goto cleanup;

    // Set heap limits
    sqlite3_int64 hard_limit = 1024 * 1024; // 1MB
    sqlite3_hard_heap_limit64(hard_limit);
    sqlite3_soft_heap_limit(512 * 1024); // 512KB

    // Set busy timeout
    sqlite3_busy_timeout(source_db, 1000); // 1 second

    // Set trace
    sqlite3_trace_v2(source_db, SQLITE_TRACE_STMT,  id_000072_trace_callback, out_file);

    // Create a table using input data
    char* sql_create = (char*)sqlite3_malloc(size + 100);
    if (!sql_create) goto cleanup;
    
    // Use first part of input for table name and schema
    size_t sql_len = size > 50 ? 50 : size;
    char table_name[51];
    memcpy(table_name, data, sql_len);
    table_name[sql_len] = 0;
    
    // Replace non-alphanumeric characters with underscores
    for (size_t i = 0; i < sql_len; i++) {
        if (!isalnum(table_name[i])) table_name[i] = '_';
    }
    
    snprintf(sql_create, size + 100, 
             "CREATE TABLE IF NOT EXISTS %s (id INTEGER PRIMARY KEY, data TEXT, value REAL);", 
             table_name);
    
    rc = sqlite3_exec(source_db, sql_create, nullptr, nullptr, &err_msg);
    sqlite3_free(sql_create);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }

    // Prepare INSERT statement
    const char* insert_sql = "INSERT INTO test_table (data, value) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(source_db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) goto cleanup;

    // Bind text data from input
    size_t text_len = size > 100 ? 100 : size;
    char* text_data = (char*)sqlite3_malloc(text_len + 1);
    if (text_data) {
        memcpy(text_data, data, text_len);
        text_data[text_len] = 0;
        sqlite3_bind_text(stmt, 1, text_data, -1, SQLITE_TRANSIENT);
        sqlite3_free(text_data);
    }

    // Bind integer and double values derived from input
    int int_val = 0;
    double double_val = 0.0;
    if (size >= sizeof(int)) {
        memcpy(&int_val, data, sizeof(int));
    }
    if (size >= sizeof(int) + sizeof(double)) {
        memcpy(&double_val, data + sizeof(int), sizeof(double));
    }
    
    sqlite3_bind_int(stmt, 2, int_val);
    sqlite3_bind_double(stmt, 3, double_val);

    // Execute the prepared statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
        // Statement executed successfully
    }

    // Reset statement for potential reuse
    sqlite3_reset(stmt);

    // Get statement status
    int status = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);

    // Get data count (should be 0 for INSERT, but we check anyway)
    int data_count = sqlite3_data_count(stmt);

    // Finalize the statement
    sqlite3_finalize(stmt);
    stmt = nullptr;

    // Execute a SELECT query using sqlite3_exec with callback
    const char* select_sql = "SELECT data FROM test_table";
    rc = sqlite3_exec(source_db, select_sql,  id_000072_exec_callback, out_file, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }

    // Prepare and execute another statement to test column functions
    const char* select2_sql = "SELECT value FROM test_table";
    rc = sqlite3_prepare_v2(source_db, select2_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            double col_double = sqlite3_column_double(stmt, 0);
            sqlite3_int64 col_int64 = sqlite3_column_int64(stmt, 0);
            
            // Write column values to output file
            char buf[100];
            int len = snprintf(buf, sizeof(buf), "Double: %f, Int64: %lld\n", 
                              col_double, col_int64);
            if (len > 0) {
                fwrite(buf, 1, len, out_file);
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Use strnicmp to compare strings from input
    if (size >= 10) {
        const char* str1 = (const char*)data;
        const char* str2 = (const char*)data + (size > 5 ? 5 : 0);
        size_t cmp_len = size > 5 ? 5 : size;
        int cmp_result = sqlite3_strnicmp(str1, str2, cmp_len);
        
        char cmp_buf[50];
        int cmp_len_written = snprintf(cmp_buf, sizeof(cmp_buf), 
                                      "strnicmp result: %d\n", cmp_result);
        if (cmp_len_written > 0) {
            fwrite(cmp_buf, 1, cmp_len_written, out_file);
        }
    }

    // Initialize backup from source to destination
    backup = sqlite3_backup_init(dest_db, "main", source_db, "main");
    if (backup) {
        // Perform backup in steps
        while ((rc = sqlite3_backup_step(backup, 5)) == SQLITE_OK) {
            // Continue stepping
        }
        
        // Finish backup
        sqlite3_backup_finish(backup);
        backup = nullptr;
    }

    // Check memory highwater mark
    sqlite3_int64 highwater = sqlite3_memory_highwater(1);
    char hw_buf[100];
    int hw_len = snprintf(hw_buf, sizeof(hw_buf), 
                         "Memory highwater: %lld\n", highwater);
    if (hw_len > 0) {
        fwrite(hw_buf, 1, hw_len, out_file);
    }

    // Release memory
    sqlite3_release_memory(1024 * 1024); // Try to release 1MB

    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    if (backup) sqlite3_backup_finish(backup);
    if (source_db) sqlite3_close(source_db);
    if (dest_db) sqlite3_close(dest_db);
    if (err_msg) sqlite3_free(err_msg);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file.db");
	return 0;
}