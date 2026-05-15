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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 175, nr_unique_branch: 13
//<Quality> {"density":25,"unique_branches":{"sqlite3VdbeSorterWrite":[[103595,22,103595,26,0,0,4,1]],"sqlite3VdbeExpandSql":[[92134,11,92134,33,0,0,4,1],[92136,17,92136,52,0,0,4,1],[92138,17,92138,39,0,0,4,1],[92140,17,92140,38,0,0,4,0],[92144,13,92144,29,0,0,4,1],[92169,13,92169,29,0,0,4,1]],"sqlite3_str_vappendf":[[31512,20,31512,24,0,0,4,1]],"sqlite3PagerOpen":[[61176,22,61176,34,0,0,4,1]],"sqlite3VdbeRecordCompareWithSkip":[[88727,11,88727,26,0,0,4,0],[88732,14,88732,29,0,0,4,1]],"createCollation":[[180370,7,180370,25,0,0,4,0],[14098,27,14098,28,1,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_close","sqlite3_enable_load_extension","sqlite3_trace","sqlite3_collation_needed16","sqlite3_create_collation16","sqlite3_create_module","sqlite3_create_function","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_stmt_readonly","sqlite3_step","sqlite3_data_count","sqlite3_column_type","sqlite3_column_int","sqlite3_expired","sqlite3_reset","sqlite3_db_name","sqlite3_finalize","sqlite3_db_filename","sqlite3_uri_key","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_enable_load_extension","sqlite3_trace","sqlite3_collation_needed16","sqlite3_create_collation16","sqlite3_create_module","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_stmt_readonly","sqlite3_step","sqlite3_data_count","sqlite3_column_type","sqlite3_column_int","sqlite3_expired","sqlite3_reset","sqlite3_db_name","sqlite3_finalize","sqlite3_db_filename","sqlite3_uri_key","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Define custom collation callback
static int  id_000048_customCollation16(void* pArg, int n1, const void* s1, int n2, const void* s2) {
    // Simple byte-wise comparison
    int minLen = n1 < n2 ? n1 : n2;
    int cmp = memcmp(s1, s2, minLen);
    if (cmp == 0) return n1 - n2;
    return cmp;
}

// Define collation needed callback
static void  id_000048_collationNeeded16(void* pArg, sqlite3* db, int eTextRep, const void* zName) {
    // Create a simple collation when needed
    sqlite3_create_collation16(db, zName, eTextRep, nullptr,  id_000048_customCollation16);
}

// Define trace callback
static void  id_000048_traceCallback(void* pArg, const char* sql) {
    // Do nothing in fuzzing context
}

// Define aggregate step function
static void  id_000048_aggregateStep(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
        int* sum = static_cast<int*>(sqlite3_aggregate_context(ctx, sizeof(int)));
        if (sum) {
            *sum += sqlite3_value_int(argv[0]);
        }
    }
}

// Define aggregate final function
static void  id_000048_aggregateFinal(sqlite3_context* ctx) {
    int* sum = static_cast<int*>(sqlite3_aggregate_context(ctx, 0));
    if (sum) {
        sqlite3_result_int(ctx, *sum);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

// Define exec callback
static int  id_000048_execCallback(void* data, int argc, char** argv, char** colNames) {
    return 0; // Continue processing
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* errMsg = nullptr;
    int rc;
    
    // Create a temporary file for database
    const char* db_filename = "fuzz.db";
    
    // Open database with various flags
    rc = sqlite3_open_v2(db_filename, &db, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 
                        nullptr);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable load extension
    sqlite3_enable_load_extension(db, 1);
    
    // Set up trace
    sqlite3_trace(db,  id_000048_traceCallback, nullptr);
    
    // Set up collation needed callback
    sqlite3_collation_needed16(db, nullptr,  id_000048_collationNeeded16);
    
    // Create a custom collation
    const char16_t collationName[] = u"FUZZ_COLLATION";
    sqlite3_create_collation16(db, collationName, SQLITE_UTF16, 
                              reinterpret_cast<void*>(0x1234),  id_000048_customCollation16);
    
    // Create a simple virtual table module
    static sqlite3_module dummyModule = {0};
    sqlite3_create_module(db, "fuzz_module", &dummyModule, nullptr);
    
    // Create custom aggregate function
    sqlite3_create_function(db, "fuzz_agg", 1, SQLITE_UTF8, nullptr,
                           nullptr,  id_000048_aggregateStep,  id_000048_aggregateFinal);
    
    // Use input data as SQL command (truncate to valid C string)
    char* sql = static_cast<char*>(malloc(size + 1));
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, data, size);
    sql[size] = '\0';
    
    // Execute SQL directly
    rc = sqlite3_exec(db, sql,  id_000048_execCallback, nullptr, &errMsg);
    if (errMsg) {
        sqlite3_free(errMsg);
    }
    
    // Prepare a statement
    const char* test_sql = "SELECT fuzz_agg(1), ?";
    rc = sqlite3_prepare_v2(db, test_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind a parameter using input data
        if (size > 0) {
            sqlite3_bind_text(stmt, 1, reinterpret_cast<const char*>(data), 
                            size < 100 ? size : 100, SQLITE_TRANSIENT);
        }
        
        // Check if statement is read-only
        int isReadonly = sqlite3_stmt_readonly(stmt);
        
        // Step through statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column data
            int colCount = sqlite3_data_count(stmt);
            for (int i = 0; i < colCount; i++) {
                if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER) {
                    int val = sqlite3_column_int(stmt, i);
                    (void)val; // Use val to avoid unused warning
                }
            }
        }
        
        // Check if statement expired
        int isExpired = sqlite3_expired(stmt);
        
        // Reset statement
        sqlite3_reset(stmt);
        
        // Get database name
        const char* dbName = sqlite3_db_name(db, 0);
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Get database filename
    sqlite3_filename dbFile = sqlite3_db_filename(db, "main");
    if (dbFile) {
        // Get URI key
        const char* uriKey = sqlite3_uri_key(dbFile, 0);
        (void)uriKey; // Use to avoid unused warning
    }
    
    // Clean up
    free(sql);
    sqlite3_close(db);
    
    // Remove temporary file
    remove(db_filename);
    
    return 0;
}