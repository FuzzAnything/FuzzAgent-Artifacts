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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 108, nr_unique_branch: 4
//<Quality> {"density":27,"unique_branches":{"unixFileControl":[[41907,11,41907,46,0,0,4,0]],"sqlite3InvokeBusyHandler":[[179343,29,179343,39,0,0,4,0],[179345,7,179345,12,0,0,4,0]],"sqlite3BtreeSetPageSize":[[72817,24,72817,54,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_soft_heap_limit64","sqlite3_busy_handler","sqlite3_threadsafe","sqlite3_close","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_close","sqlite3_bind_parameter_count","sqlite3_bind_int64","sqlite3_bind_text","sqlite3_step","sqlite3_column_count","sqlite3_column_decltype","sqlite3_column_type","sqlite3_column_int64","sqlite3_column_text","sqlite3_finalize","sqlite3_open","sqlite3_close","sqlite3_backup_init","sqlite3_close","sqlite3_close","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_soft_heap_limit64","sqlite3_busy_handler","sqlite3_threadsafe","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_parameter_count","sqlite3_bind_int64","sqlite3_bind_text","sqlite3_step","sqlite3_column_count","sqlite3_column_decltype","sqlite3_column_type","sqlite3_column_int64","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Busy handler callback
static int  id_000005_busy_handler_callback(void *data, int count) {
    (void)data;
    (void)count;
    return 0; // Return 0 to abort the operation
}

// Callback for sqlite3_exec
static int  id_000005_exec_callback(void *data, int argc, char **argv, char **colNames) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)colNames;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 8) {
        return 0;
    }

    sqlite3 *db = nullptr;
    sqlite3 *db2 = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_backup *backup = nullptr;
    char *err_msg = nullptr;
    int rc = 0;

    // 1. Open first database
    rc = sqlite3_open("input_file", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        if (db) sqlite3_close(db);
        assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Set soft heap limit (use a value derived from input)
    sqlite3_int64 heap_limit = static_cast<sqlite3_int64>(data[0]) * 1024 * 1024;
    sqlite3_soft_heap_limit64(heap_limit);

    // 3. Set busy handler
    sqlite3_busy_handler(db,  id_000005_busy_handler_callback, nullptr);

    // 4. Check if SQLite is threadsafe
    int is_threadsafe = sqlite3_threadsafe();

    // 5. Execute SQL commands from input data
    // First, write input data to a file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Read SQL commands from the input file (simplified - just use first part as SQL)
    size_t sql_size = size > 1024 ? 1024 : size;
    char *sql = (char *)malloc(sql_size + 1);
    if (!sql) {
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    fseek(in_file, 0, SEEK_SET);
    size_t read_bytes = fread(sql, 1, sql_size, in_file);
    sql[read_bytes] = '\0';
    
    rc = sqlite3_exec(db, sql,  id_000005_exec_callback, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    free(sql);
    assert_file_closed(&in_file);;

    // 6. Prepare a statement
    const char *query = "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT); "
                        "INSERT INTO test VALUES(?, ?); "
                        "SELECT * FROM test;";
    const char *tail = nullptr;
    rc = sqlite3_prepare_v3(db, query, -1, 0, &stmt, &tail);
    if (rc != SQLITE_OK || stmt == nullptr) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 7. Bind parameters to the prepared statement
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count >= 1) {
        // Use data[1] and data[2] for int64 parameter
        sqlite3_int64 int_val = static_cast<sqlite3_int64>(data[1]) << 32 | data[2];
        sqlite3_bind_int64(stmt, 1, int_val);
    }
    
    if (param_count >= 2) {
        // Use a portion of input data for text parameter
        size_t text_len = size > 100 ? 100 : size;
        char *text_data = (char *)malloc(text_len + 1);
        if (text_data) {
            memcpy(text_data, data, text_len);
            text_data[text_len] = '\0';
            sqlite3_bind_text(stmt, 2, text_data, -1, SQLITE_TRANSIENT);
            free(text_data);
        }
    }

    // 8. Step through the prepared statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Get column information (for demonstration)
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            const char *decl_type = sqlite3_column_decltype(stmt, i);
            int col_type = sqlite3_column_type(stmt, i);
            
            if (col_type == SQLITE_INTEGER) {
                sqlite3_int64 int_val = sqlite3_column_int64(stmt, i);
                (void)int_val;
            } else if (col_type == SQLITE_TEXT) {
                const unsigned char *text_val = sqlite3_column_text(stmt, i);
                (void)text_val;
            }
        }
    }

    // 9. Finalize the statement
    sqlite3_finalize(stmt);

    // 10. Open second database for backup
    rc = sqlite3_open("output_file", &db2);
    if (rc != SQLITE_OK || db2 == nullptr) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 11. Initialize backup
    backup = sqlite3_backup_init(db2, "main", db, "main");
    if (!backup) {
        sqlite3_close(db2);
        sqlite3_close(db);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 12. Perform backup in steps
    do {
        rc = sqlite3_backup_step(backup, 5); // Backup 5 pages at a time
    } while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

    // 13. Finish backup
    sqlite3_backup_finish(backup);

    // 14. Close databases
    sqlite3_close(db2);
    sqlite3_close(db);

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}