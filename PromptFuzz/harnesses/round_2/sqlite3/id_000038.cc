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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 2250, nr_unique_branch: 298
//<Quality> {"density":25,"unique_branches":{"sqlite3LeaveMutexAndCloseZombie":[[178968,11,178968,15,0,0,4,1]],"defragmentPage":[[71383,7,71383,33,0,0,4,1],[71424,7,71424,14,0,0,4,0],[71428,14,71428,21,0,0,4,0],[71428,14,71428,21,0,0,4,1],[71437,11,71437,23,0,0,4,1],[71443,11,71443,26,0,0,4,1],[71443,30,71443,48,0,0,4,1],[71457,7,71457,48,0,0,4,1],[14098,27,14098,28,17,0,4,1],[14098,27,14098,28,18,0,4,1],[14098,27,14098,28,21,0,4,0],[14098,27,14098,28,21,0,4,1],[14098,27,14098,28,22,0,4,0],[14098,27,14098,28,22,0,4,1]],"indexMightHelpWithOrderBy":[[163621,9,163621,29,0,0,4,1],[163626,15,163626,47,0,0,4,1]],"exprStructSize":[[18919,34,18919,55,1,0,4,0]],"exprCommute":[[158549,7,158549,34,0,0,4,1],[158550,7,158550,35,0,0,4,1],[158551,7,158552,71,0,0,4,1],[158557,7,158557,23,0,0,4,1]],"sqlite3CreateIndex":[[124120,11,124120,23,0,0,4,1],[124335,9,124335,21,0,0,4,0],[124421,22,124421,49,0,0,4,1],[124564,13,124564,31,0,0,4,1]],"sqlite3VdbeExec":[[94440,11,94440,35,0,0,4,0],[94480,11,94480,38,0,0,4,0],[95019,7,95019,34,0,0,4,1],[95546,9,95546,24,0,0,4,1],[97079,11,97079,24,0,0,4,0],[97080,13,97080,28,0,0,4,0],[97531,13,97531,40,0,0,4,0],[98885,1,98885,14,0,0,4,0],[99988,1,99988,19,0,0,4,0],[100002,7,100002,9,0,0,4,1],[100007,35,100007,38,0,0,4,0],[100007,35,100007,38,0,0,4,1]],"btreeNext":[[70664,4,70664,33,2,0,4,1]],"vdbeColumnFromOverflow":[[92943,9,92943,41,0,0,4,0],[92946,11,92946,26,0,0,4,0]],"cellSizePtrTableLeaf":[[71276,7,71276,22,0,0,4,0],[71277,7,71277,22,0,0,4,0],[71278,7,71278,22,0,0,4,0],[71279,7,71279,22,0,0,4,0],[71280,7,71280,22,0,0,4,0],[71281,7,71281,22,0,0,4,0],[71282,7,71282,22,0,0,4,0]],"sqlite3SrcListShiftJoinType":[[125241,7,125241,8,0,0,4,1]],"sqlite3GetTempRange":[[114850,7,114850,14,0,0,4,0]],"sqlite3VdbeSorterInit":[[102833,35,102833,68,0,0,4,0]],"randomBlob":[[128006,7,128006,8,0,0,4,1]],"putVarint64":[[35605,7,35605,34,0,0,4,0],[35608,14,35608,18,0,0,4,0],[35608,14,35608,18,0,0,4,1]],"sqlite3BtreeInsert":[[76679,7,76679,35,22,0,4,0],[14098,27,14098,28,23,0,4,0]],"cellSizePtrNoPayload":[[71245,29,71245,39,0,0,4,1]],"cellSizePtrIdxLeaf":[[71220,9,71220,30,0,0,4,1]],"sqlite3AffinityType":[[121803,13,121803,36,0,0,4,1],[121803,40,121803,60,0,0,4,1],[121847,9,121847,14,0,0,4,0]],"getOverflowPage":[[74724,42,74724,53,0,0,4,1],[74732,7,74732,13,0,0,4,0]],"sqlite3VdbeRecordCompareWithSkip":[[88708,17,88708,31,0,0,4,0],[88753,11,88753,25,0,0,4,0],[88783,14,88783,36,0,0,4,1],[88787,11,88787,25,0,0,4,0],[88805,15,88805,20,0,0,4,0],[88813,13,88813,27,0,0,4,0],[88813,13,88813,27,0,0,4,1],[88813,31,88813,46,0,0,4,0],[88832,9,88832,26,0,0,4,1],[88835,9,88835,27,0,0,4,1],[88837,9,88837,31,0,0,4,1]],"sqlite3BtreeDelete":[[79525,7,79525,19,0,0,4,0],[79528,9,79528,11,0,0,4,1],[79558,7,79558,19,0,0,4,0],[79564,9,79564,23,0,0,4,1],[79568,9,79568,33,0,0,4,1],[79574,9,79574,31,0,0,4,1],[79580,9,79580,22,0,0,4,0],[79584,9,79584,11,0,0,4,1],[79604,7,79604,57,0,0,4,0],[79611,24,79611,46,0,0,4,0],[79614,12,79614,34,0,0,4,1],[76679,7,76679,35,11,0,4,0]],"sqlite3VdbeSorterWrite":[[103595,7,103595,10,0,0,4,1],[103597,13,103597,17,0,0,4,1],[103655,14,103655,25,0,0,4,0]],"selectExpander":[[147374,27,147374,54,0,0,4,0],[147395,12,147395,26,0,0,4,1],[147395,30,147395,53,0,0,4,1],[147412,13,147412,27,0,0,4,0],[147582,15,147582,21,0,0,4,0]],"vdbeRecordCompareInt":[[88895,5,88895,11,0,0,4,0],[88923,5,88923,11,0,0,4,0],[88936,5,88936,11,0,0,4,0],[88947,13,88947,18,0,0,4,0],[14098,27,14098,28,2,0,4,0],[14098,27,14098,28,4,0,4,1]],"out2Prerelease":[[23230,3,23230,38,2,0,4,0]],"sqlite3ExprIsInteger":[[110631,5,110631,19,0,0,4,0],[110633,11,110633,45,0,0,4,1]],"sqlite3MemCompare":[[88506,7,88506,30,0,0,4,0],[88520,11,88520,34,0,0,4,0]],"btreeParseCellPtr":[[71041,9,71041,16,0,0,4,0],[71043,11,71043,18,0,0,4,0],[71045,13,71045,20,0,0,4,0],[71047,15,71047,22,0,0,4,0],[71049,17,71049,24,0,0,4,0],[71051,19,71051,26,0,0,4,0],[71053,21,71053,28,0,0,4,0]],"sqlite3OpenTempDatabase":[[125345,7,125345,24,0,0,4,0],[125345,28,125345,44,0,0,4,0],[125356,9,125356,22,0,0,4,1],[125364,9,125364,75,0,0,4,1]],"patternCompare":[[128082,37,128082,46,17,0,4,1],[128082,37,128082,46,21,0,4,1]],"clearDatabasePage":[[79851,9,79851,22,0,0,4,1],[76679,7,76679,35,6,0,4,0]],"sumStep":[[129239,12,129239,29,0,0,4,1]],"lookupName":[[106333,9,106333,51,0,0,4,1]],"clearCellOverflow":[[76618,7,76618,45,0,0,4,1],[76630,10,76630,17,0,0,4,0],[76630,10,76630,17,0,0,4,1],[76633,9,76633,19,0,0,4,1],[76633,23,76633,51,0,0,4,1],[76639,9,76639,14,0,0,4,0],[76639,9,76639,14,0,0,4,1],[76641,11,76641,13,0,0,4,1],[76644,11,76644,16,0,0,4,0],[76644,11,76644,16,0,0,4,1],[76644,20,76644,65,0,0,4,0],[76645,9,76645,52,0,0,4,1],[76662,9,76662,14,0,0,4,0],[76665,9,76665,11,0,0,4,1],[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,1,0,4,1],[14098,27,14098,28,2,0,4,1]],"balance_nonroot":[[78199,13,78199,30,0,0,4,1],[78562,38,78562,73,0,0,4,0],[78604,15,78604,21,0,0,4,0],[14098,27,14098,28,42,0,4,1]],"yy_reduce":[[174008,7,174008,14,0,0,4,0],[174067,7,174067,30,0,0,4,0],[174067,34,174067,86,0,0,4,1],[174128,7,174128,14,0,0,4,0],[174131,7,174131,8,0,0,4,0],[174382,7,174382,15,0,0,4,0],[174394,7,174394,15,0,0,4,0],[174516,7,174516,15,0,0,4,0],[174887,7,174887,15,0,0,4,0],[14098,27,14098,28,25,0,4,1],[14098,27,14098,28,275,0,4,0],[14098,27,14098,28,304,0,4,1],[14098,27,14098,28,348,0,4,0]],"termIsEquivalence":[[159353,7,159353,27,0,0,4,1],[159354,7,159354,23,0,0,4,1],[159358,7,159358,17,0,0,4,0],[159358,7,159358,17,0,0,4,1],[159359,8,159359,33,0,0,4,0],[159364,7,159364,29,0,0,4,0],[18919,34,18919,55,4,0,4,1]],"openSubJournal":[[60933,9,60933,54,0,0,4,0]],"isLikeOrGlob":[[158648,13,158648,26,0,0,4,1],[158652,7,158652,8,0,0,4,1],[158656,29,158656,37,0,0,4,1],[158669,10,158669,15,0,0,4,1],[158669,20,158669,25,0,0,4,0],[158669,29,158669,40,0,0,4,0],[158673,22,158673,30,0,0,4,0],[158673,34,158673,45,0,0,4,1],[158713,17,158713,23,0,0,4,0],[158713,27,158713,39,0,0,4,1]],"sqlite3VdbeDisplayComment":[[85729,17,85729,50,0,0,4,0]],"sqlite3VdbeFindCompare":[[14098,27,14098,28,4,0,4,0]],"editPage":[[77555,11,77558,8,0,0,4,0],[77565,7,77568,4,0,0,4,0],[77591,7,77591,13,0,0,4,1]],"sqlite3ExprIfFalse":[[18923,29,18923,77,52,0,4,0]],"sqlite3ResolveSelfReference":[[107930,7,107930,61,0,0,4,0]],"btreePageLookup":[[72089,7,72089,14,0,0,4,0]],"sumFinalize":[[129301,12,129301,20,0,0,4,1]],"sqlite3StartTable":[[18322,29,18322,57,13,0,4,0]],"codeAttach":[[119613,7,119613,49,0,0,4,0]],"sqlite3GenerateIndexKey":[[127371,7,127371,13,0,0,4,0],[127371,18,127371,35,0,0,4,1],[127371,39,127371,60,0,0,4,1],[127373,9,127373,15,0,0,4,0],[127374,9,127374,47,0,0,4,0],[127375,9,127375,37,0,0,4,0]],"sqlite3CreateView":[[123137,7,123137,11,0,0,4,0]],"sqlite3BtreePrevious":[[76108,7,76108,27,0,0,4,0]],"yy_destructor":[[172749,5,172749,13,0,0,4,0],[172760,5,172760,13,0,0,4,0],[172796,5,172796,13,0,0,4,0]],"sqlite3BtreeSetPageSize":[[72817,24,72817,54,0,0,4,1]],"balance":[[78792,13,78792,43,0,0,4,1]],"sqlite3CompareAffinity":[[108273,31,108273,51,0,0,4,0],[18155,38,18155,63,3,0,4,1],[18155,38,18155,63,4,0,4,0],[18155,38,18155,63,4,0,4,1]],"sqlite3BtreeIndexMoveto":[[75703,7,75703,24,0,0,4,1],[75781,12,75781,67,0,0,4,0],[14098,27,14098,28,12,0,4,0],[14098,27,14098,28,12,0,4,1]],"isAuxiliaryVtabOperator":[[18333,5,18333,23,8,0,4,0],[18333,27,18333,61,8,0,4,1]],"rebuildPage":[[77290,30,77290,49,0,0,4,0],[77299,11,77299,40,0,0,4,1],[77314,9,77314,16,0,0,4,1],[77315,9,77315,28,0,0,4,0],[77315,9,77315,28,0,0,4,1],[14823,57,14823,78,3,0,4,0]],"vdbeRecordCompareString":[[88985,7,88985,21,0,0,4,0],[88986,9,88986,22,0,0,4,0],[88986,9,88986,22,0,0,4,1],[88988,11,88988,26,0,0,4,0],[88992,13,88992,34,0,0,4,0]],"exprDup":[[109580,11,109580,30,0,0,4,0]],"sqlite3Utf8Read":[[34065,7,34065,14,0,0,4,0],[34067,12,34067,33,0,0,4,0],[34067,12,34067,33,0,0,4,1],[34070,9,34070,15,0,0,4,0],[34070,9,34070,15,0,0,4,1],[34071,12,34071,34,0,0,4,0],[34071,12,34071,34,0,0,4,1],[34072,12,34072,34,0,0,4,1]],"resolveAsName":[[107122,7,107122,20,0,0,4,1]],"computeLimitRegisters":[[143783,11,143783,15,0,0,4,0]],"cellSizePtr":[[71160,7,71160,18,0,0,4,1],[71170,7,71170,29,0,0,4,0],[71178,9,71178,30,0,0,4,1]],"sqlite3GenerateConstraintChecks":[[133835,9,133835,25,0,0,4,1]],"sqlite3Pragma":[[138946,9,138946,19,0,0,4,0],[138953,9,138953,15,0,0,4,1],[138974,11,138974,17,0,0,4,1],[138990,13,138990,20,0,0,4,1],[138996,11,138996,18,0,0,4,1],[139000,11,139000,18,0,0,4,1],[139004,13,139004,20,0,0,4,1],[139040,13,139040,20,0,0,4,1],[139588,3,139588,30,0,0,4,0],[139589,16,139589,23,0,0,4,1],[139591,9,139591,15,0,0,4,1]],"sqlite3AddPrimaryKey":[[121975,7,121975,40,0,0,4,0]],"sqlite3BtreeIntegrityCheck":[[80759,7,80759,18,0,0,4,1],[80777,7,80777,24,0,0,4,0]],"sqlite3GetVarint":[[35693,7,35693,16,0,0,4,1],[35717,7,35717,16,0,0,4,1],[35739,7,35739,16,0,0,4,1],[35755,7,35755,16,0,0,4,1],[35772,7,35772,16,0,0,4,1]],"sqlite3CodeVerifySchemaAtToplevel":[[125385,25,125385,31,0,0,4,0]],"btreePrevious":[[76074,7,76074,19,0,0,4,0],[76077,9,76077,11,0,0,4,1],[76080,12,76080,23,0,0,4,1],[76092,9,76092,22,0,0,4,1]],"sqlite3ClearOnOrUsing":[[109350,7,109350,11,0,0,4,1],[109352,13,109352,19,0,0,4,0]],"lengthFunc":[[127535,5,127535,24,0,0,4,0],[127555,5,127555,12,0,0,4,0]],"wherePathSatisfiesOrderBy":[[165044,11,165044,33,0,0,4,0],[165044,37,165044,63,0,0,4,0],[165181,17,165181,39,0,0,4,0],[165181,43,165181,69,0,0,4,0]],"sqlite3SrcListAppendFromTerm":[[125107,29,125107,42,0,0,4,0],[125109,8,125109,21,0,0,4,0]],"resolveP2Values":[[84878,9,84878,27,0,0,4,0]],"pageFindSlot":[[14098,27,14098,28,3,0,4,0]],"exprAnalyze":[[159590,18,159590,54,0,0,4,1],[159607,9,159607,55,0,0,4,0],[159608,9,159608,25,0,0,4,0],[159614,11,159614,31,0,0,4,0],[159617,13,159617,29,0,0,4,1],[159622,13,159622,22,0,0,4,1],[159625,13,159625,22,0,0,4,1],[159629,13,159629,44,0,0,4,0],[159629,13,159629,44,0,0,4,1],[159762,7,159762,64,0,0,4,1],[14098,27,14098,28,22,0,4,1]],"allocateCursor":[[92476,7,92476,21,0,0,4,0]],"avgFinalize":[[129318,7,129318,8,0,0,4,1]],"sqlite3Checkpoint":[[180177,19,180177,37,0,0,4,0],[14098,27,14098,28,3,0,4,0]],"keywordCode":[[14098,27,14098,28,34,0,4,0]],"populateCellCache":[[77224,10,77224,13,0,0,4,0],[77224,10,77224,13,0,0,4,1],[77226,9,77226,23,0,0,4,0],[77226,9,77226,23,0,0,4,1]],"freeSpace":[[71678,26,71678,39,0,0,4,1]],"sqlite3WalkExprNN":[[14098,27,14098,28,1,0,4,0]],"pageInsertArray":[[77380,25,77380,36,0,0,4,1],[77380,41,77380,78,0,0,4,0],[77381,11,77381,30,0,0,4,0]],"sqlite3BtreeCheckpoint":[[80929,7,80929,8,0,0,4,1]],"contextMalloc":[[127899,7,127899,44,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_busy_timeout","sqlite3_create_function","sqlite3_mprintf","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_stmt_readonly","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_reset","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_msize","sqlite3_free","sqlite3_mutex_alloc","sqlite3_mutex_enter","sqlite3_mutex_leave","sqlite3_mutex_free","sqlite3_finalize","sqlite3_close_v2","sqlite3_shutdown"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_busy_timeout","sqlite3_create_function","sqlite3_mprintf","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_stmt_readonly","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_reset","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_msize","sqlite3_free","sqlite3_mutex_alloc","sqlite3_mutex_enter","sqlite3_mutex_leave","sqlite3_mutex_free","sqlite3_finalize","sqlite3_close_v2","sqlite3_shutdown"],"visited":0}
/**/


// Custom function implementation for sqlite3_create_function
static void  id_000038_custom_function(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    if (argc == 1) {
        const char* input = (const char*)sqlite3_value_text(argv[0]);
        if (input) {
            int len = sqlite3_value_bytes(argv[0]);
            sqlite3_result_text(ctx, input, len, SQLITE_TRANSIENT);
        } else {
            sqlite3_result_null(ctx);
        }
    } else {
        sqlite3_result_error(ctx, "Invalid number of arguments", -1);
    }
}

// Rollback hook callback
static void  id_000038_rollback_hook_callback(void* data) {
    // Do nothing, just a placeholder
}

// Exec callback
static int  id_000038_exec_callback(void* data, int argc, char** argv, char** colNames) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create input file from data
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

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_str* str_obj = nullptr;
    sqlite3_mutex* mutex = nullptr;
    char* errmsg = nullptr;
    
    // 1. Open database (in-memory)
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Set rollback hook
    sqlite3_rollback_hook(db,  id_000038_rollback_hook_callback, nullptr);
    
    // 3. Set busy timeout
    sqlite3_busy_timeout(db, 1000);
    
    // 4. Create custom function
    rc = sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, 
                                 nullptr,  id_000038_custom_function, nullptr, nullptr);
    
    // 5. Prepare mprintf string
    // Extract a substring from data for mprintf (ensure null termination)
    size_t mprintf_len = size < 100 ? size : 100;
    char* temp_buf = (char*)malloc(mprintf_len + 1);
    if (temp_buf) {
        memcpy(temp_buf, data, mprintf_len);
        temp_buf[mprintf_len] = '\0';
        char* mprintf_result = sqlite3_mprintf("Test: %s", temp_buf);
        if (mprintf_result) {
            sqlite3_free(mprintf_result);
        }
        free(temp_buf);
    }
    
    // 6. Execute SQL using sqlite3_exec
    // Read SQL from input file
    fseek(in_file, 0, SEEK_SET);
    char sql_buf[1024];
    size_t read_size = fread(sql_buf, 1, sizeof(sql_buf) - 1, in_file);
    if (read_size > 0) {
        sql_buf[read_size] = '\0';
        rc = sqlite3_exec(db, sql_buf,  id_000038_exec_callback, nullptr, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
        }
    }
    
    // 7. Execute another SQL statement using sqlite3_exec
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT)", 
                       id_000038_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // 8. Prepare a statement with prepare_v2
    const char* sql = "INSERT INTO test VALUES(?, ?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind some values
        sqlite3_bind_int(stmt, 1, 1);
        sqlite3_bind_text(stmt, 2, "test", 4, SQLITE_STATIC);
        
        // 9. Check if statement is readonly
        int is_readonly = sqlite3_stmt_readonly(stmt);
        (void)is_readonly; // Suppress unused warning
        
        // 10. Step through the statement
        rc = sqlite3_step(stmt);
        
        // 11. Reset the statement
        sqlite3_reset(stmt);
        
        // Prepare and execute a SELECT statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        rc = sqlite3_prepare_v2(db, "SELECT * FROM test", -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // 12. Get column text
                const unsigned char* text = sqlite3_column_text(stmt, 1);
                if (text) {
                    // Write to output file
                    fwrite(text, 1, strlen((const char*)text), out_file);
                }
            }
            
            // 13. Reset again
            sqlite3_reset(stmt);
        }
    }
    
    // 14. Create sqlite3_str object
    str_obj = sqlite3_str_new(db);
    if (str_obj) {
        // 15. Append to the string
        sqlite3_str_append(str_obj, "Appended string", 15);
        
        // 16. Finish the string
        char* str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            // 17. Get memory size
            sqlite3_uint64 mem_size = sqlite3_msize(str_result);
            (void)mem_size; // Suppress unused warning
            
            // 18. Free the string
            sqlite3_free(str_result);
        }
    }
    
    // 19. Allocate mutex
    mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
    if (mutex) {
        // 20. Enter mutex
        sqlite3_mutex_enter(mutex);
        
        // 21. Leave mutex
        sqlite3_mutex_leave(mutex);
        
        // 22. Free mutex
        sqlite3_mutex_free(mutex);
    }
    
    // 23. Finalize statement if still exists
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // 24. Close database with close_v2
    sqlite3_close_v2(db);
    
    // 25. Shutdown SQLite
    sqlite3_shutdown();
    
    // Cleanup file descriptors
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}