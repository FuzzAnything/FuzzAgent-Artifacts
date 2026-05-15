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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 178.5, nr_unique_branch: 20
//<Quality> {"density":21,"unique_branches":{"sqlite3BtreeCopyFile":[[81931,7,81931,20,0,0,4,1]],"sqlite3BtreeDelete":[[79548,7,79548,9,0,0,4,0]],"unixFileControl":[[41921,13,41921,30,0,0,4,0]],"sqlite3PagerSharedLock":[[61689,9,61689,22,0,0,4,0]],"sqlite3RunVacuum":[[153594,9,153594,22,0,0,4,0]],"allocateBtreePage":[[76195,9,76195,11,0,0,4,0]],"backupOnePage":[[81427,32,81427,45,0,0,4,1],[81432,9,81432,53,0,0,4,1]],"sqlite3_trace":[[179797,16,179797,22,0,0,4,1]],"sqlite3BtreeUpdateMeta":[[80084,7,80084,20,0,0,4,1]],"sqlite3VdbeExec":[[96381,7,96381,9,0,0,4,0],[98087,7,98087,9,0,0,4,0]],"sqlite3BtreeInsert":[[79217,9,79217,11,0,0,4,0]],"hasHotJournal":[[61601,15,61601,28,0,0,4,1],[61611,21,61611,40,0,0,4,1]],"sqlite3_backup_step":[[81712,13,81712,70,0,0,4,1]],"balance":[[78785,11,78785,24,0,0,4,1],[78788,11,78788,24,0,0,4,1]],"unixMapfile":[[43127,7,43127,26,0,0,4,1]],"sqlite3PagerClearCache":[[63902,7,63902,26,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_db_filename","sqlite3_uri_key","sqlite3_enable_load_extension","sqlite3_create_module","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_expired","sqlite3_collation_needed16","sqlite3_trace","sqlite3_step","sqlite3_column_int","sqlite3_reset","sqlite3_finalize","sqlite3_aggregate_context","sqlite3_aggregate_context","sqlite3_aggregate_context","sqlite3_aggregate_count","sqlite3_result_int","sqlite3_result_int","sqlite3_aggregate_context","sqlite3_aggregate_count","sqlite3_result_int","sqlite3_result_int","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_db_filename","sqlite3_uri_key","sqlite3_enable_load_extension","sqlite3_create_module","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_expired","sqlite3_collation_needed16","sqlite3_trace","sqlite3_step","sqlite3_column_int","sqlite3_reset","sqlite3_finalize","sqlite3_aggregate_context","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_result_int","sqlite3_aggregate_count","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_result_int","sqlite3_aggregate_count","sqlite3_aggregate_context","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create temporary input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Variables for database operations
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    char *errmsg = nullptr;
    int rc;
    
    // 1. Open database with URI filename
    rc = sqlite3_open_v2("file:test.db?cache=shared", &db, 
                         SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, 
                         nullptr);
    if (rc != SQLITE_OK) {
        // Cleanup and return
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("test.db");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Get database filename
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
    
    // 3. Query URI parameter (if URI format is used)
    if (db_filename) {
        const char *cache_param = sqlite3_uri_key(db_filename, 1);
        (void)cache_param; // Use variable to avoid warning
    }
    
    // 4. Enable load extension
    sqlite3_enable_load_extension(db, 1);
    
    // 5. Create a simple virtual table module
    sqlite3_module module = {};
    module.xCreate = nullptr;
    module.xConnect = nullptr;
    module.xBestIndex = nullptr;
    module.xDisconnect = nullptr;
    module.xDestroy = nullptr;
    module.xOpen = nullptr;
    module.xClose = nullptr;
    module.xFilter = nullptr;
    module.xNext = nullptr;
    module.xEof = nullptr;
    module.xColumn = nullptr;
    module.xRowid = nullptr;
    module.xUpdate = nullptr;
    module.xBegin = nullptr;
    module.xSync = nullptr;
    module.xCommit = nullptr;
    module.xRollback = nullptr;
    module.xFindFunction = nullptr;
    module.xRename = nullptr;
    module.xSavepoint = nullptr;
    module.xRelease = nullptr;
    module.xRollbackTo = nullptr;
    
    rc = sqlite3_create_module(db, "fuzz_module", &module, nullptr);
    
    // 6. Execute SQL using fuzzer data as SQL command (with size limit)
    if (size > 0 && data[0] < 128) { // Basic sanity check
        char *sql = (char*)malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
            free(sql);
            if (errmsg) sqlite3_free(errmsg);
        }
    }
    
    // 7. Prepare a statement
    rc = sqlite3_prepare_v2(db, "SELECT 1, 2, 3", -1, &stmt, &tail);
    
    if (stmt) {
        // 8. Check if statement is readonly
        int is_readonly = sqlite3_stmt_readonly(stmt);
        
        // 9. Check if statement expired
        int is_expired = sqlite3_expired(stmt);
        
        // 10. Set collation needed callback
        sqlite3_collation_needed16(db, nullptr, nullptr);
        
        // 11. Set trace callback
        void *old_trace = sqlite3_trace(db, nullptr, nullptr);
        
        // 12. Get statement scan status (if supported)
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        int scanstatus_rc = sqlite3_stmt_scanstatus_v2(stmt, 0, 0, 0, nullptr);
        #endif
        
        // 13. Step through statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // 17. Get integer column
            int col_value = sqlite3_column_int(stmt, 0);
            (void)col_value; // Use variable
        }
        
        // 14. Reset statement
        sqlite3_reset(stmt);
        
        // 15. Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Create and use aggregate context to test sqlite3_aggregate_count
    // This requires creating an aggregate function
    struct AggregateContext {
        int count;
    };
    
    auto step_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        void *agg_ctx = sqlite3_aggregate_context(ctx, sizeof(AggregateContext));
        if (agg_ctx) {
            AggregateContext* ac = static_cast<AggregateContext*>(agg_ctx);
            ac->count++;
        }
    };
    
    auto final_func = [](sqlite3_context* ctx) {
        void *agg_ctx = sqlite3_aggregate_context(ctx, 0);
        if (agg_ctx) {
            AggregateContext* ac = static_cast<AggregateContext*>(agg_ctx);
            // 4. Get aggregate count
            int agg_count = sqlite3_aggregate_count(ctx);
            (void)agg_count; // Use variable
            sqlite3_result_int(ctx, ac->count);
        } else {
            sqlite3_result_int(ctx, 0);
        }
    };
    
    // Register aggregate function
    rc = sqlite3_create_function(db, "fuzz_agg", 1, SQLITE_UTF8, nullptr,
                                 nullptr, step_func, final_func);
    
    // Use the aggregate function
    if (rc == SQLITE_OK) {
        rc = sqlite3_prepare_v2(db, "SELECT fuzz_agg(1)", -1, &stmt, nullptr);
        if (stmt) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // 18. Close database
    sqlite3_close(db);
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("test.db");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}