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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 391.5, nr_unique_branch: 121
//<Quality> {"density":27,"unique_branches":{"sqlite3SrcListDup":[[109815,9,109815,31,0,0,4,0]],"applyNumericAffinity":[[92550,7,92550,12,0,0,4,1],[92551,7,92551,12,0,0,4,1],[92556,9,92556,19,0,0,4,0]],"analyzeFilterKeyword":[[176594,27,176594,46,0,0,4,1]],"applyAffinity":[[92598,13,92598,34,0,0,4,0]],"sqlite3ColumnExpr":[[120860,7,120860,21,0,0,4,1],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1],[14156,25,14156,46,3,0,4,1],[14156,26,14156,29,3,0,4,1]],"sqlite3_profile":[[179855,7,179855,19,0,0,4,1]],"btreePayloadToLocal":[[70957,7,70957,25,0,0,4,1],[70964,12,70964,35,0,0,4,0],[70964,12,70964,35,0,0,4,1]],"sqlite3VdbeMemSetStr":[[14098,27,14098,28,15,0,4,0]],"sqlite3GetInt32":[[35355,29,35355,41,0,0,4,0],[35360,12,35360,24,0,0,4,0]],"freeP4":[[85364,5,85364,16,0,0,4,0],[85365,11,85365,30,0,0,4,0]],"sqlite3Pragma":[[139163,17,139163,28,0,0,4,0],[139167,19,139167,29,0,0,4,0]],"yy_destructor":[[172761,5,172761,13,0,0,4,0]],"sqlite3ValueFromExpr":[[83704,10,83704,15,0,0,4,0]],"sqlite3ColumnDefault":[[151608,7,151608,18,0,0,4,0],[151617,9,151617,15,0,0,4,0]],"compileoptionusedFunc":[[128464,7,128464,63,0,0,4,0]],"sqlite3VdbeExec":[[99033,1,99033,14,0,0,4,0],[99041,7,99041,14,0,0,4,1],[99049,7,99049,9,0,0,4,1]],"resolveExprStep":[[106805,15,106805,19,0,0,4,0],[106807,17,106807,32,0,0,4,0]],"sqlite3WalkSelectFrom":[[105651,11,105651,30,0,0,4,0],[105652,11,105652,59,0,0,4,1]],"yy_reduce":[[174028,7,174028,15,0,0,4,0],[174450,13,174450,31,0,0,4,0],[175054,7,175054,15,0,0,4,0],[14098,27,14098,28,256,0,4,0]],"clearDatabasePage":[[79853,7,79853,15,0,0,4,0],[14098,27,14098,28,8,0,4,1]],"parserAddExprIdListTerm":[[170597,10,170597,20,0,0,4,1],[170597,24,170597,54,0,0,4,1]],"getToken":[[176536,7,176536,19,0,0,4,0]],"valueFromExpr":[[83550,10,83550,36,0,0,4,1],[83550,40,83550,51,0,0,4,0],[83550,40,83550,51,0,0,4,1],[83551,7,83551,22,0,0,4,1],[83559,7,83559,18,0,0,4,1],[83582,7,83582,20,0,0,4,1],[83590,7,83590,20,0,0,4,1],[83590,24,83590,36,0,0,4,0],[83592,9,83592,16,0,0,4,1],[83597,11,83597,18,0,0,4,1],[83600,10,83600,24,0,0,4,1],[83600,28,83600,40,0,0,4,0],[83600,46,83600,71,0,0,4,1],[83606,9,83606,53,0,0,4,0],[83611,9,83611,25,0,0,4,1],[18919,34,18919,55,13,0,4,1],[14098,27,14098,28,25,0,4,0],[14098,27,14098,28,26,0,4,1]],"openStatTable":[[117465,9,117465,63,0,0,4,1],[117483,11,117483,17,0,0,4,1],[117489,17,117489,39,0,0,4,1]],"trimFunc":[[128897,7,128897,47,0,0,4,1],[128901,7,128901,13,0,0,4,1],[128904,7,128904,14,0,0,4,0],[128904,7,128904,14,0,0,4,1],[128911,13,128911,56,0,0,4,1],[128915,30,128915,32,0,0,4,0],[128915,30,128915,32,0,0,4,1],[128918,9,128918,16,0,0,4,0],[128921,11,128921,20,0,0,4,1],[128925,32,128925,34,0,0,4,0],[128925,32,128925,34,0,0,4,1],[128932,7,128932,14,0,0,4,0],[128934,9,128934,18,0,0,4,0],[128935,14,128935,19,0,0,4,0],[128935,14,128935,19,0,0,4,1],[128937,18,128937,25,0,0,4,0],[128937,18,128937,25,0,0,4,1],[128939,15,128939,23,0,0,4,0],[128939,27,128939,57,0,0,4,0],[128939,27,128939,57,0,0,4,1],[128941,13,128941,21,0,0,4,0],[128941,13,128941,21,0,0,4,1],[128946,9,128946,18,0,0,4,1],[128957,9,128957,17,0,0,4,0],[128957,9,128957,17,0,0,4,1],[20329,7,20329,23,2,0,4,1],[20329,7,20329,23,3,0,4,1]],"selectExpander":[[147395,30,147395,53,0,0,4,0]],"exprProbability":[[106630,7,106630,22,0,0,4,0]],"sqlite3BtreeTransferRow":[[79349,7,79349,16,0,0,4,1],[79364,9,79364,33,0,0,4,0],[79369,9,79369,17,0,0,4,0],[79370,11,79370,42,0,0,4,1],[79380,13,79380,18,0,0,4,0],[79380,13,79380,18,0,0,4,1],[79388,13,79388,19,0,0,4,0],[79388,13,79388,19,0,0,4,1],[79392,15,79392,28,0,0,4,0],[79399,15,79399,28,0,0,4,0],[79399,32,79399,38,0,0,4,0],[79399,32,79399,38,0,0,4,1],[79401,11,79401,24,0,0,4,0],[79401,28,79401,34,0,0,4,0],[79401,28,79401,34,0,0,4,1],[79411,13,79411,21,0,0,4,0],[79418,13,79418,19,0,0,4,0],[79418,13,79418,19,0,0,4,1],[79418,23,79418,36,0,0,4,0],[14676,20,14676,27,7,0,4,0],[14676,20,14676,27,7,0,4,1],[14155,25,14155,46,13,0,4,0],[14155,26,14155,29,13,0,4,0],[69436,27,69436,44,15,0,4,1],[14676,20,14676,27,18,0,4,0],[14676,20,14676,27,18,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_trace","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_step","sqlite3_reset","sqlite3_prepare16_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_value_dup","sqlite3_value_dup","sqlite3_value_free","sqlite3_value_free","sqlite3_filename_database","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_trace","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_prepare16_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_value_dup","sqlite3_value_dup","sqlite3_value_free","sqlite3_value_free","sqlite3_filename_database","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_close","sqlite3_shutdown"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    sqlite3_initialize(); // Initialize SQLite library
    
    // Create a temporary database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set trace callback (simplified - no actual callback implementation)
    sqlite3_trace(db, nullptr, nullptr);
    
    // Set profile callback (simplified - no actual callback implementation)
    sqlite3_profile(db, nullptr, nullptr);
    
    // Create a table using the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read part of input data as SQL string (ensure null-terminated)
        size_t sql_len = size > 1024 ? 1024 : size;
        char *sql = (char *)malloc(sql_len + 1);
        fread(sql, 1, sql_len, in_file);
        sql[sql_len] = '\0';
        assert_file_closed(&in_file);;
        
        char *errmsg = nullptr;
        
        // Create a test table
        const char *create_table = 
            "CREATE TABLE IF NOT EXISTS test("
            "id INTEGER PRIMARY KEY,"
            "name TEXT,"
            "data BLOB);";
        
        rc = sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK && errmsg) {
            sqlite3_free(errmsg);
        }
        
        // Use the input data in an INSERT statement
        sqlite3_stmt *stmt = nullptr;
        const char *tail = nullptr;
        
        rc = sqlite3_prepare_v2(db, "INSERT INTO test(name, data) VALUES(?, ?)", 
                               -1, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            // Bind text parameter using part of input data
            size_t text_len = size > 50 ? 50 : size;
            sqlite3_bind_text(stmt, 1, (const char *)data, text_len, SQLITE_TRANSIENT);
            
            // Bind blob parameter using remaining input data
            size_t blob_len = size > text_len ? size - text_len : 0;
            if (blob_len > 0) {
                const void *blob_data = data + text_len;
                sqlite3_bind_blob(stmt, 2, blob_data, blob_len, SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_null(stmt, 2);
            }
            
            // Execute the statement
            rc = sqlite3_step(stmt);
            sqlite3_reset(stmt);
            
            // Also test prepare16_v2 with UTF-16 SQL
            sqlite3_stmt *stmt16 = nullptr;
            const void *tail16 = nullptr;
            
            // Convert simple ASCII SQL to UTF-16
            const char *simple_sql = "SELECT * FROM test";
            int simple_len = strlen(simple_sql);
            uint16_t *utf16_sql = (uint16_t *)malloc((simple_len + 1) * 2);
            for (int i = 0; i < simple_len; i++) {
                utf16_sql[i] = simple_sql[i];
            }
            utf16_sql[simple_len] = 0;
            
            rc = sqlite3_prepare16_v2(db, utf16_sql, -1, &stmt16, &tail16);
            if (rc == SQLITE_OK && stmt16) {
                while (sqlite3_step(stmt16) == SQLITE_ROW) {
                    // Test column retrieval functions
                    int col_count = sqlite3_column_count(stmt16);
                    for (int i = 0; i < col_count; i++) {
                        // Test column_blob and column_bytes
                        if (sqlite3_column_type(stmt16, i) == SQLITE_BLOB) {
                            const void *blob = sqlite3_column_blob(stmt16, i);
                            int blob_size = sqlite3_column_bytes(stmt16, i);
                            // Do something with blob data
                            (void)blob;
                            (void)blob_size;
                        }
                    }
                }
                sqlite3_finalize(stmt16);
            }
            free(utf16_sql);
            sqlite3_finalize(stmt);
        }
        
        // Use sqlite3_exec with input data as SQL (limited to safe operations)
        // Only execute if the SQL doesn't contain dangerous patterns
        int dangerous = 0;
        const char *dangerous_patterns[] = {"DROP", "DELETE", "UPDATE", "INSERT", nullptr};
        for (int i = 0; dangerous_patterns[i]; i++) {
            if (strstr(sql, dangerous_patterns[i])) {
                dangerous = 1;
                break;
            }
        }
        
        if (!dangerous) {
            rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
            if (rc != SQLITE_OK && errmsg) {
                sqlite3_free(errmsg);
            }
        }
        
        free(sql);
        
        // Test sqlite3_str_new and related string functions
        sqlite3_str *str_builder = sqlite3_str_new(db);
        if (str_builder) {
            sqlite3_str_append(str_builder, "Test string", 11);
            // Get the result
            char *str_result = sqlite3_str_finish(str_builder);
            if (str_result) {
                sqlite3_free(str_result);
            }
        }
        
        // Test sqlite3_value_dup - create a temporary value and duplicate it
        sqlite3_value *original_value = sqlite3_value_dup(nullptr); // NULL creates a null value
        if (original_value) {
            sqlite3_value *dup_value = sqlite3_value_dup(original_value);
            if (dup_value) {
                sqlite3_value_free(dup_value);
            }
            sqlite3_value_free(original_value);
        }
        
        // Test sqlite3_filename_database with a dummy filename
        const char *dummy_filename = ":memory:";
        const char *db_name = sqlite3_filename_database(dummy_filename);
        (void)db_name; // Suppress unused warning
        
        // Test sqlite3_stricmp
        int cmp_result = sqlite3_stricmp("test", "TEST");
        (void)cmp_result;
        
        // Get total changes
        sqlite3_int64 total_changes = sqlite3_total_changes64(db);
        (void)total_changes;
        
        // Get memory usage
        sqlite3_int64 mem_used = sqlite3_memory_used();
        (void)mem_used;
    }
    
    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	return 0;
}