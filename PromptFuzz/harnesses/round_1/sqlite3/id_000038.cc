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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 263.5, nr_unique_branch: 31
//<Quality> {"density":31,"unique_branches":{"execSql":[[153284,7,153284,22,0,0,4,1],[153285,7,153285,9,0,0,4,0]],"autoVacuumCommit":[[73912,7,73912,23,0,0,4,1]],"getPageMMap":[[62080,9,62080,22,0,0,4,1],[57263,25,57263,39,5,0,4,0]],"sqlite3_uri_int64":[[182371,7,182371,8,0,0,4,1]],"lockBtree":[[73132,11,73132,44,0,0,4,1]],"sqlite3_backup_step":[[81516,23,81516,63,0,0,4,0],[81526,9,81526,22,0,0,4,1],[81598,28,81598,43,0,0,4,1],[81601,11,81601,24,0,0,4,1],[81609,11,81609,24,0,0,4,1]],"sqlite3BtreeGetAutoVacuum":[[72960,5,72960,26,0,0,4,1]],"sqlite3VdbeMemTranslate":[[34047,7,34047,14,11,0,4,0],[34049,12,34049,22,11,0,4,0],[34049,26,34049,45,11,0,4,0],[34049,26,34049,45,11,0,4,1],[34052,9,34052,15,11,0,4,0],[34052,9,34052,15,11,0,4,1],[34053,12,34053,34,11,0,4,1],[34054,12,34054,34,11,0,4,1]],"sqlite3RunVacuum":[[153517,7,153517,20,0,0,4,0]],"sqlite3InitOne":[[140523,32,140523,45,0,0,4,0],[140524,11,140524,44,0,0,4,1],[140608,7,140608,23,0,0,4,0]],"isAuxiliaryVtabOperator":[[18333,5,18333,23,6,0,4,1]],"corruptSchema":[[140258,13,140258,43,0,0,4,0]],"pager_playback_one_page":[[58786,18,58786,52,0,0,4,0]],"jsonAppendString":[[203032,15,203032,22,0,0,4,0]],"pagerBeginReadTransaction":[[57219,22,57219,38,2,0,4,0]],"sqlite3BtreeSetAutoVacuum":[[72941,23,72941,28,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_close","sqlite3_open_v2","sqlite3_close","sqlite3_close","sqlite3_trace_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_double","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_error_offset","sqlite3_uri_int64","sqlite3_db_filename","sqlite3_next_stmt","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_open_v2","sqlite3_trace_v2","sqlite3_free","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_double","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_error_offset","sqlite3_uri_int64","sqlite3_db_filename","sqlite3_next_stmt","sqlite3_close","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	

    // 1. Write input data to a file (treat as SQLite database output)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create database files
    const char* input_db_name = "input_file";
    const char* output_db_name = "output_file";
    
    // Write input data to file
    FILE *db_file = fopen(input_db_name, "wb");
    if (db_file) {
        uint8_t buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            fwrite(buffer, 1, bytes_read, db_file);
        }
        assert_file_closed(&db_file);;
    }
    assert_file_closed(&in_file);;

    // 3. Open databases
    sqlite3 *input_db = nullptr, *output_db = nullptr;
    int rc = sqlite3_open_v2(input_db_name, &input_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(input_db);
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    rc = sqlite3_open_v2(output_db_name, &output_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(input_db);
        sqlite3_close(output_db);
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 4. Setup trace callback
    auto trace_callback = [](unsigned int mask, void* ctx, void* p, void* x) -> int {
        return 0;
    };
    sqlite3_trace_v2(input_db, SQLITE_TRACE_STMT, trace_callback, nullptr);

    // 5. Execute SQL commands from input data (treat as SQL statements)
    // Extract potential SQL statements from the data
    std::vector<std::string> sql_commands;
    size_t pos = 0;
    while (pos < size && pos < 1024) { // Limit to first 1KB for safety
        size_t end = pos;
        while (end < size && data[end] != ';' && (end - pos) < 256) end++;
        if (end < size && data[end] == ';') {
            sql_commands.push_back(std::string(data + pos, data + end + 1));
            pos = end + 1;
        } else {
            break;
        }
    }

    // 6. Execute each SQL command
    for (const auto& sql : sql_commands) {
        char* errmsg = nullptr;
        sqlite3_exec(input_db, sql.c_str(), nullptr, nullptr, &errmsg);
        if (errmsg) sqlite3_free(errmsg);
    }

    // 7. Prepare and execute statements
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    
    // Prepare a simple statement
    const char* create_table = "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT, value REAL)";
    rc = sqlite3_prepare_v2(input_db, create_table, -1, &stmt, &tail);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // 8. Insert data with bindings
    const char* insert_sql = "INSERT INTO test VALUES(?, ?, ?)";
    rc = sqlite3_prepare_v2(input_db, insert_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK) {
        // Use data bytes for binding values
        int int_val = size > 0 ? data[0] : 0;
        double double_val = size > 1 ? data[1] / 255.0 : 0.5;
        
        sqlite3_bind_int(stmt, 1, int_val);
        sqlite3_bind_text(stmt, 2, "test", -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, double_val);
        
        sqlite3_step(stmt);
        
        // Get statement status
        int status = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        (void)status;
        
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    }

    // 9. Query data
    const char* select_sql = "SELECT * FROM test";
    rc = sqlite3_prepare_v2(input_db, select_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cols = sqlite3_data_count(stmt);
            if (cols > 2) {
                double val = sqlite3_column_double(stmt, 2);
                (void)val;
            }
        }
        sqlite3_finalize(stmt);
    }

    // 10. String comparison
    const char* str1 = "test1";
    const char* str2 = "test2";
    int cmp = sqlite3_strnicmp(str1, str2, 5);
    (void)cmp;

    // 11. Check memory usage
    sqlite3_int64 memory_highwater = sqlite3_memory_highwater(1);
    (void)memory_highwater;

    // 12. Backup database
    sqlite3_backup* backup = sqlite3_backup_init(output_db, "main", input_db, "main");
    if (backup) {
        sqlite3_backup_step(backup, 5); // Backup 5 pages
        sqlite3_backup_finish(backup);
    }

    // 13. Check error offset (if any error)
    int error_offset = sqlite3_error_offset(input_db);
    (void)error_offset;

    // 14. Use URI function
    const char* uri_param = "cache_size";
    sqlite3_int64 cache_size = sqlite3_uri_int64(sqlite3_db_filename(input_db, "main"), uri_param, -1);
    (void)cache_size;

    // 15. Iterate through all statements
    sqlite3_stmt* next_stmt = nullptr;
    while ((next_stmt = sqlite3_next_stmt(input_db, next_stmt)) != nullptr) {
        // Do nothing, just iterating
    }

    // 16. Cleanup
    sqlite3_close(input_db);
    sqlite3_close(output_db);

    // 17. Remove temporary files
    remove(input_db_name);
    remove(output_db_name);

    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}