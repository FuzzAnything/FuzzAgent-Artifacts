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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 8
//<Quality> {"density":20,"unique_branches":{"whereLoopFindLesser":[[162832,9,162832,50,0,0,4,1]],"sqlite3RootPageMoved":[[123387,37,123387,42,0,0,4,0],[123389,9,123389,26,0,0,4,0],[123389,9,123389,26,0,0,4,1]],"saveCursorsOnList":[[70582,11,70582,12,0,0,4,0]],"btreeDropTable":[[79960,9,79960,28,0,0,4,0],[79966,11,79966,24,0,0,4,1]],"freePage2":[[76534,9,76534,43,0,0,4,1]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_encoding","sqlite3_set_clientdata","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_vtab_config","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_encoding","sqlite3_set_clientdata","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_vtab_config","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<119) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    // 1. Get SQLite library version
    const char *version = sqlite3_libversion();
    
    // 2. Create a temporary file for database
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Open a database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Set some database limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // 5. Set collation needed callback (16-bit)
    struct CollationContext {
        int called;
    } collationCtx = {0};
    
    auto collationNeeded = [](void *pCtx, sqlite3 *db, int eTextRep, const void *name) {
        CollationContext *ctx = static_cast<CollationContext*>(pCtx);
        ctx->called = 1;
    };
    sqlite3_collation_needed16(db, &collationCtx, collationNeeded);
    
    // 6. Create a simple table
    const char *createTable = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, fuzz_str_7, nullptr, nullptr, nullptr);
    
    // 7. Use input data as SQL statements (with proper null termination)
    if (size > 0) {
        // Create null-terminated copy of input data
        char *sql = static_cast<char*>(malloc(size + 1));
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            
            // Execute SQL from input
            char *errmsg = nullptr;
            rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
            
            if (errmsg) {
                sqlite3_free(errmsg);
            }
            
            // 8. Prepare statement from input
            sqlite3_stmt *stmt = nullptr;
            const char *tail = nullptr;
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
            
            if (stmt) {
                // 9. Check if statement is EXPLAIN
                int isExplain = sqlite3_stmt_isexplain(stmt);
                
                // 10. Step through the statement
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                    // Get value encoding if we have columns
                    int colCount = sqlite3_column_count(stmt);
                    if (colCount > 0) {
                        // Create a value to check encoding
                        sqlite3_value *value = sqlite3_column_value(stmt, fuzz_int32_t_8);
                        if (value) {
                            int encoding = sqlite3_value_encoding(value);
                            // Use encoding value to avoid unused variable warning
                            (void)encoding;
                        }
                    }
                }
                
                // 11. Set client data
                struct ClientData {
                    int marker;
                } clientData = {42};
                
                sqlite3_set_clientdata(db, fuzz_str_9, &clientData, nullptr);
                
                // 12. Finalize statement
                sqlite3_finalize(stmt);
            }
            
            free(sql);
        }
    }
    
    // 13. Get WAL filename (for memory database, this will likely be nullptr)
    sqlite3_filename dbname = sqlite3_db_filename(db, fuzz_str_10);
    if (dbname) {
        const char *wal = sqlite3_filename_wal(dbname);
        (void)wal; // Use to avoid unused variable warning
    }
    
    // 14. Configure virtual table
    sqlite3_vtab_config(db, fuzz_int32_t_11);
    
    // 15. Close database
    sqlite3_close(db);
    
    // 16. Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}