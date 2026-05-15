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
//<ID> 496
//<Prompt> []
/*<Combination>: [
*/
//<score> 261, nr_unique_branch: 104
//<Quality> {"density":9,"unique_branches":{"vdbeRecordCompareDebug":[[88262,9,88262,42,0,0,4,1]],"resolveAlias":[[105820,7,105820,23,0,0,4,1],[105826,9,105826,30,0,0,4,1],[18919,34,18919,55,2,0,4,1]],"sqlite3Pragma":[[137785,3,137785,34,0,0,4,0],[137800,9,137800,16,0,0,4,0],[139098,15,139098,33,0,0,4,0],[139105,13,139105,21,0,0,4,1],[139262,13,139262,25,0,0,4,0],[139262,29,139262,65,0,0,4,0],[139264,15,139264,34,0,0,4,0],[139270,36,139270,39,0,0,4,1],[14187,37,14187,58,24,0,4,1],[14187,38,14187,41,24,0,4,1]],"sqlite3VdbeExec":[[94492,11,94492,36,0,0,4,0],[94493,13,94493,71,0,0,4,0],[94498,13,94498,71,0,0,4,1]],"sqlite3ExprCodeTarget":[[112439,11,112439,17,0,0,4,0],[112440,13,112440,31,0,0,4,1]],"kahanBabuskaNeumaierStep":[[129182,7,129182,24,0,0,4,0]],"applyNumericAffinity":[[92550,7,92550,12,0,0,4,0]],"codeApplyAffinity":[[156067,17,156067,41,0,0,4,1],[156077,7,156077,10,0,0,4,0]],"sqlite3FkDelete":[[131619,32,131619,37,0,0,4,0],[131623,9,131623,28,0,0,4,0],[131624,11,131624,25,0,0,4,1],[131627,26,131627,40,0,0,4,1],[131630,11,131630,25,0,0,4,1]],"sqlite3ExprIfTrue":[[113530,5,113530,15,0,0,4,0],[113532,11,113532,44,0,0,4,1],[113570,5,113570,15,0,0,4,1],[14098,27,14098,28,32,0,4,1],[14098,27,14098,28,34,0,4,1],[14098,27,14098,28,35,0,4,1],[14098,27,14098,28,36,0,4,1],[14098,27,14098,28,37,0,4,1],[14098,27,14098,28,38,0,4,1],[14098,27,14098,28,39,0,4,0],[14098,27,14098,28,40,0,4,1],[14098,27,14098,28,41,0,4,1]],"sqlite3EndTable":[[122861,7,122861,16,0,0,4,0],[122863,9,122863,21,0,0,4,1],[123100,9,123100,20,0,0,4,1]],"incrAggFunctionDepth":[[105776,7,105776,10,0,0,4,1]],"yy_reduce":[[174151,7,174151,14,0,0,4,0],[174169,7,174169,14,0,0,4,0],[174172,7,174172,14,0,0,4,0],[174181,7,174181,14,0,0,4,0],[174184,7,174184,14,0,0,4,0],[174193,7,174193,14,0,0,4,0],[174230,7,174230,14,0,0,4,0],[14098,27,14098,28,340,0,4,0]],"selectInnerLoop":[[142484,13,142484,58,0,0,4,0],[142507,13,142507,43,0,0,4,0],[14098,27,14098,28,14,0,4,1]],"sqlite3ResolveSelfReference":[[107931,7,107931,12,0,0,4,0]],"pageFindSlot":[[71506,13,71506,28,0,0,4,0]],"codeAllEqualityTerms":[[156529,13,156529,69,0,0,4,1],[156532,13,156532,62,0,0,4,1]],"sqlite3ResolveOrderGroupBy":[[107362,9,107362,31,0,0,4,0],[107363,11,107363,47,0,0,4,1]],"sqlite3ExprIfFalse":[[113703,5,113703,15,0,0,4,0],[14098,27,14098,28,38,0,4,0]],"sqlite3CreateForeignKey":[[123750,7,123750,11,0,0,4,1],[123751,7,123751,18,0,0,4,0],[123754,9,123754,15,0,0,4,1],[123770,7,123770,13,0,0,4,1],[123776,7,123776,15,0,0,4,1],[123792,7,123792,18,0,0,4,0],[123814,7,123814,13,0,0,4,1],[123834,7,123834,21,0,0,4,1],[123838,7,123838,14,0,0,4,1],[19755,27,19755,72,1,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1],[19761,28,19761,67,3,0,4,1]],"sqlite3AddCheckConstraint":[[122049,7,122049,11,0,0,4,0],[122049,15,122049,31,0,0,4,0],[122050,7,122050,57,0,0,4,0],[122053,9,122053,33,0,0,4,1],[20397,30,20397,72,2,0,4,1],[20397,30,20397,72,3,0,4,1]],"sqlite3ExprNeedsNoAffinityChange":[[110699,7,110699,27,0,0,4,1],[110700,10,110700,25,0,0,4,1],[110700,29,110700,45,0,0,4,1],[110705,7,110705,22,0,0,4,1],[110707,5,110707,20,0,0,4,1],[110710,5,110710,18,0,0,4,1],[110713,5,110713,19,0,0,4,0],[110714,14,110714,25,0,0,4,0],[110714,29,110714,49,0,0,4,1],[110716,5,110716,17,0,0,4,1],[110719,5,110719,19,0,0,4,1],[110723,5,110723,12,0,0,4,1]],"resolveOrderGroupBy":[[107451,9,107451,41,0,0,4,0],[107455,11,107455,17,0,0,4,1],[107455,21,107455,32,0,0,4,1]],"generateSortTail":[[143036,9,143036,37,0,0,4,1],[143077,11,143077,36,0,0,4,0]],"sqlite3ExprCodeExprList":[[113342,41,113342,71,0,0,4,0],[113343,11,113343,38,0,0,4,0]],"markExprListImmutable":[[122725,7,122725,12,0,0,4,0]],"sqlite3FkClearTriggerCache":[[130879,34,130879,39,0,0,4,0]],"fkTriggerDelete":[[130857,7,130857,8,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_create_function","sqlite3_value_subtype","sqlite3_result_error_code","sqlite3_result_text","sqlite3_value_int","sqlite3_result_int","sqlite3_value_subtype","sqlite3_result_error_code","sqlite3_result_text","sqlite3_value_int","sqlite3_result_int","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_create_function","sqlite3_result_int","sqlite3_value_int","sqlite3_result_text","sqlite3_result_error_code","sqlite3_value_subtype","sqlite3_result_int","sqlite3_value_int","sqlite3_result_text","sqlite3_result_error_code","sqlite3_value_subtype","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;  // Empty input, nothing to process
    }

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    char *errmsg = nullptr;
    int rc;

    // 1. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    int fd_in = fuzz_fileno(in_file);
    
    // 2. Open a database (in-memory)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. Set client data (if available)
    const char *key = "fuzz_key";
    void *client_data = malloc(64);
    if (client_data) {
        // Simulate using client data - function may not exist in all versions
        // sqlite3_set_clientdata(db, key, client_data, free);
        free(client_data); // Clean up since we can't use the actual function
    }

    // 4. Create a custom function that uses sqlite3_result_error_code and sqlite3_value_subtype
    rc = sqlite3_create_function(
        db,
        "fuzz_func",
        2,  // nArg
        SQLITE_UTF8,
        nullptr,
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            // Check subtype of first argument
            unsigned int subtype = sqlite3_value_subtype(argv[0]);
            
            // Use sqlite3_result_error_code based on subtype
            if (subtype == 1) {
                sqlite3_result_error_code(ctx, SQLITE_ERROR);
                sqlite3_result_text(ctx, "Subtype error", -1, SQLITE_TRANSIENT);
            } else {
                int val = sqlite3_value_int(argv[0]);
                sqlite3_result_int(ctx, val * 2);
            }
        },
        nullptr,
        nullptr
    );

    // 5. Execute SQL commands from input data
    char *sql = (char *)malloc(size + 1);
    if (sql) {
        fread(sql, 1, size, in_file);
        sql[size] = '\0';
        
        // Try to execute as SQL
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK && errmsg) {
            sqlite3_free(errmsg);
        }
        free(sql);
    }

    // 6. Prepare and execute a statement with bound integer
    const char *select_sql = "SELECT fuzz_func(?, 10)";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind integer from input data (use first byte as integer)
        int bind_val = size > 0 ? data[0] : 42;
        rc = sqlite3_bind_int(stmt, 1, bind_val);
        
        // Step through results
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Process row if needed
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }

    // 7. Clean up
    assert_file_closed(&in_file);;
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}