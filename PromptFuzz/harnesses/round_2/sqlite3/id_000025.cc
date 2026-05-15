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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 181.5, nr_unique_branch: 20
//<Quality> {"density":33,"unique_branches":{"sqlite3ExprCodeRunJustOnce":[[113184,7,113184,15,0,0,4,1]],"sqlite3_bind_value":[[91288,5,91288,21,0,0,4,1],[91301,5,91301,12,0,0,4,0]],"sqlite3Insert":[[132834,16,132834,30,0,0,4,1],[132835,11,132835,26,0,0,4,0]],"sqlite3_collation_needed":[[181307,7,181307,32,0,0,4,1]],"sqlite3VdbeExec":[[95867,30,95867,39,0,0,4,1],[97603,7,97603,47,0,0,4,0],[97611,9,97611,31,0,0,4,0],[14098,27,14098,28,470,0,4,0],[14098,27,14098,28,567,0,4,1]],"sqlite3ExprCodeFactorable":[[113291,7,113291,28,0,0,4,0],[113291,32,113291,67,0,0,4,0]],"btreeNext":[[75986,9,75986,21,0,0,4,0],[75988,11,75988,13,0,0,4,1],[75998,13,75998,35,0,0,4,1],[75999,9,75999,22,0,0,4,0]],"moveToLeftmost":[[75307,27,75307,55,0,0,4,0]],"sqlite3BtreeNext":[[76024,7,76024,18,0,0,4,1]],"sqlite3_value_type":[[89779,9,89779,31,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_keyword_count","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_prepare_v2","sqlite3_bind_value","sqlite3_step","sqlite3_value_free","sqlite3_finalize","sqlite3_reset","sqlite3_clear_bindings","sqlite3_bind_zeroblob64","sqlite3_prepare_v2","sqlite3_transfer_bindings","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_collation_needed","sqlite3_errstr","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_create_function_v2","sqlite3_aggregate_context","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_result_int64","sqlite3_result_error_nomem","sqlite3_aggregate_context","sqlite3_result_int64","sqlite3_result_error_nomem","sqlite3_exec","sqlite3_free","sqlite3_create_module","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_keyword_count","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_prepare_v2","sqlite3_bind_value","sqlite3_step","sqlite3_value_free","sqlite3_finalize","sqlite3_reset","sqlite3_clear_bindings","sqlite3_bind_zeroblob64","sqlite3_prepare_v2","sqlite3_transfer_bindings","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_collation_needed","sqlite3_errstr","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_create_function_v2","sqlite3_result_error_nomem","sqlite3_result_int64","sqlite3_aggregate_context","sqlite3_result_error_nomem","sqlite3_result_int64","sqlite3_aggregate_context","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_exec","sqlite3_free","sqlite3_create_module","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create an in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }

    // Get keyword count
    int keyword_count = sqlite3_keyword_count();

    // Prepare a simple table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB, value INTEGER);";
    char *errmsg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }

    // Prepare INSERT statement
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test (data, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind blob from input data
        rc = sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size > 1000 ? 1000 : size), 
                               SQLITE_TRANSIENT);
        
        // Bind integer from keyword count
        rc = sqlite3_bind_int(stmt, 2, keyword_count);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
            // Get column value
            sqlite3_value *col_value = sqlite3_column_value(stmt, 0);
            if (col_value) {
                // Duplicate the value
                sqlite3_value *dup_value = sqlite3_value_dup(col_value);
                if (dup_value) {
                    // Prepare another statement
                    sqlite3_stmt *stmt2 = nullptr;
                    const char *select_sql = "SELECT * FROM test WHERE id = ?";
                    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr);
                    if (rc == SQLITE_OK && stmt2) {
                        // Bind the duplicated value
                        rc = sqlite3_bind_value(stmt2, 1, dup_value);
                        
                        // Step through the statement
                        rc = sqlite3_step(stmt2);
                        
                        // Clean up the duplicated value
                        sqlite3_value_free(dup_value);
                        sqlite3_finalize(stmt2);
                    }
                }
            }
        }
        
        // Reset the statement and clear bindings
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        
        // Bind zeroblob64
        rc = sqlite3_bind_zeroblob64(stmt, 1, static_cast<sqlite3_uint64>(size));
        
        // Transfer bindings to a new statement
        sqlite3_stmt *stmt3 = nullptr;
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt3, nullptr);
        if (rc == SQLITE_OK && stmt3) {
            rc = sqlite3_transfer_bindings(stmt, stmt3);
            if (rc == SQLITE_OK) {
                rc = sqlite3_step(stmt3);
            }
            sqlite3_finalize(stmt3);
        }
        
        sqlite3_finalize(stmt);
    }

    // Test collation needed callback
    sqlite3_collation_needed(db, nullptr, 
        [](void *, sqlite3 *, int, const char *) {
            // Dummy callback
            return;
        });

    // Get error string for SQLITE_OK
    const char *err_str = sqlite3_errstr(SQLITE_OK);

    // Prepare a statement with bind_text
    sqlite3_stmt *stmt4 = nullptr;
    const char *text_sql = "INSERT INTO test (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, text_sql, -1, &stmt4, nullptr);
    if (rc == SQLITE_OK && stmt4) {
        // Convert part of input data to string for binding
        size_t text_len = size > 100 ? 100 : size;
        char *text_buf = static_cast<char*>(malloc(text_len + 1));
        if (text_buf) {
            memcpy(text_buf, data, text_len);
            text_buf[text_len] = '\0';
            
            rc = sqlite3_bind_text(stmt4, 1, text_buf, -1, SQLITE_TRANSIENT);
            rc = sqlite3_step(stmt4);
            
            free(text_buf);
        }
        sqlite3_finalize(stmt4);
    }

    // Create a simple aggregate function
    struct AggContext {
        sqlite3_int64 sum;
    };
    
    rc = sqlite3_create_function_v2(
        db, "my_sum", 1, SQLITE_UTF8, nullptr,
        nullptr, // xFunc (scalar version)
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            // xStep for aggregate
            void *agg_ctx = sqlite3_aggregate_context(ctx, sizeof(AggContext));
            if (agg_ctx) {
                AggContext *context = static_cast<AggContext*>(agg_ctx);
                if (argc > 0) {
                    sqlite3_int64 val = sqlite3_value_int64(argv[0]);
                    context->sum += val;
                }
            }
        },
        [](sqlite3_context *ctx) {
            // xFinal for aggregate
            void *agg_ctx = sqlite3_aggregate_context(ctx, sizeof(AggContext));
            if (agg_ctx) {
                AggContext *context = static_cast<AggContext*>(agg_ctx);
                sqlite3_result_int64(ctx, context->sum);
            } else {
                sqlite3_result_error_nomem(ctx);
            }
        },
        nullptr // xDestroy
    );

    // Test the aggregate function
    const char *agg_sql = "SELECT my_sum(id) FROM test;";
    rc = sqlite3_exec(db, agg_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }

    // Create a dummy module
    static sqlite3_module dummy_module = {0};
    rc = sqlite3_create_module(db, "dummy_module", &dummy_module, nullptr);

    // Clean up
    sqlite3_close(db);
    
    return 0;
}