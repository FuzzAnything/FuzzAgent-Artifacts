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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 1785, nr_unique_branch: 249
//<Quality> {"density":21,"unique_branches":{"sqlite3OomClear":[[30672,7,30672,23,0,0,4,1]],"hasSharedCacheTableLock":[[69982,7,69982,28,0,0,4,0]],"sqlite3SchemaMutexHeld":[[69739,10,69739,24,0,0,4,0]],"sqlite3VdbeVerifyAbortable":[[85069,7,85069,24,0,0,4,0]],"whereLoopAddBtree":[[164094,24,164094,25,0,0,4,1]],"sqlite3VdbeLeave":[[19563,29,19563,37,1,0,4,0]],"codeInteger":[[111883,7,111883,33,0,0,4,0],[111886,9,111886,16,0,0,4,1]],"sqlite3VdbeMemSetZeroBlob":[[82847,7,82847,10,0,0,4,1]],"sqlite3LocateTable":[[120580,7,120580,47,0,0,4,1]],"sqlite3PagerCheckpoint":[[63923,7,63923,22,0,0,4,0],[63923,26,63923,68,0,0,4,1],[63935,7,63935,19,0,0,4,1]],"sqlite3_value_blob":[[89648,7,89648,36,0,0,4,0],[89649,9,89649,33,0,0,4,1],[89654,12,89654,16,0,0,4,0],[89654,12,89654,16,0,0,4,1],[23623,26,23623,47,3,0,4,1]],"sqlite3BtreeHoldsAllMutexes":[[69708,14,69708,25,0,0,4,1]],"valueToText":[[83268,7,83268,39,0,0,4,0],[83271,9,83271,51,0,0,4,0],[83271,9,83271,51,0,0,4,1],[83274,9,83274,40,0,0,4,1],[23623,25,23623,78,3,0,4,1],[23623,26,23623,47,3,0,4,1]],"sqlite3BtreeCursor":[[74471,7,74471,18,0,0,4,1]],"sqlite3VdbeMemNulTerminate":[[82383,7,82383,50,0,0,4,0],[82383,7,82383,50,0,0,4,1],[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,1,0,4,1],[14098,27,14098,28,2,0,4,1]],"sqlite3GenerateConstraintChecks":[[133669,7,133669,13,0,0,4,0],[133669,17,133669,23,0,0,4,0],[133674,9,133674,34,0,0,4,1],[133676,15,133676,34,0,0,4,0],[133681,9,133681,16,0,0,4,1],[133703,9,133703,28,0,0,4,1],[133712,9,133712,17,0,0,4,1],[133729,7,133729,14,0,0,4,1],[133733,7,133733,23,0,0,4,1],[133734,7,133734,20,0,0,4,0],[133735,7,133735,19,0,0,4,1],[133742,7,133742,22,0,0,4,1],[133790,7,133790,21,0,0,4,1],[133795,7,133795,21,0,0,4,1],[133802,9,133802,16,0,0,4,1],[133804,15,133804,21,0,0,4,1],[14098,27,14098,28,62,0,4,1],[14098,27,14098,28,63,0,4,0],[14098,27,14098,28,64,0,4,1]],"sqlite3_wal_checkpoint_v2":[[180088,7,180088,32,0,0,4,1],[180092,7,180092,12,0,0,4,1],[180093,7,180093,13,0,0,4,1],[180099,7,180099,38,0,0,4,1],[180099,42,180099,74,0,0,4,1],[180106,7,180106,10,0,0,4,0],[180106,14,180106,20,0,0,4,0],[180111,7,180111,12,0,0,4,1],[180123,7,180123,25,0,0,4,0]],"sqlite3_reset":[[89602,7,89602,15,0,0,4,1],[89557,8,89557,26,2,0,4,1]],"sqlite3_errmsg16":[[180284,7,180284,10,0,0,4,1],[180287,7,180287,38,0,0,4,1],[180291,7,180291,23,0,0,4,1],[180295,9,180295,13,0,0,4,0]],"sqlite3VdbeEnter":[[19563,29,19563,37,1,0,4,0]],"applyAffinity":[[92603,13,92603,38,0,0,4,1]],"sqlite3VdbeUsesBtree":[[85979,15,85979,54,0,0,4,1]],"sqlite3VdbeCheckMemInvariants":[[82036,42,82036,48,0,0,4,1]],"sqlite3TableLock":[[120231,7,120231,54,0,0,4,0]],"sqlite3ErrorFinish":[[34556,7,34556,15,0,0,4,0]],"sqlite3BtreeClose":[[72683,7,72683,19,0,0,4,0]],"sqlite3ReadSchema":[[140699,15,140699,32,0,0,4,0]],"sqlite3ParseUri":[[180579,8,180579,33,0,0,4,1],[180770,9,180770,15,0,0,4,1],[180773,9,180773,13,0,0,4,0],[13868,32,13868,71,3,0,4,1]],"sqlite3BtreeLeave":[[69622,7,69622,18,0,0,4,1]],"indexMightHelpWithOrderBy":[[163617,7,163617,44,0,0,4,0]],"sqlite3Prepare":[[140961,7,140961,25,0,0,4,1]],"sqlite3BtreeBeginTrans":[[73502,7,73502,18,0,0,4,1],[73503,7,73503,29,0,0,4,0],[73503,7,73503,29,0,0,4,1],[73504,8,73504,30,0,0,4,0],[73504,8,73504,30,0,0,4,1],[73504,34,73504,43,0,0,4,1],[73509,7,73509,21,0,0,4,0],[73512,7,73512,13,0,0,4,1]],"sqlite3BtreeEnter":[[69570,7,69570,19,0,0,4,0]],"sqlite3OpenTable":[[131691,7,131691,33,0,0,4,1]],"getCellInfo":[[74568,7,74568,26,0,0,4,1]],"sqlite3Error":[[34568,19,34568,27,0,0,4,0]],"sqlite3ExprCodeTarget":[[112502,5,112502,20,0,0,4,0]],"sqlite3ExprCodeGetColumnOfTable":[[111983,7,111983,13,0,0,4,0]],"btreeEnterAll":[[69670,14,69670,25,0,0,4,1]],"sqlite3BtreeOpen":[[72287,37,72287,69,0,0,4,0],[72325,37,72325,66,0,0,4,1],[72475,9,72475,20,0,0,4,1],[72498,7,72498,18,0,0,4,1],[72550,7,72550,16,0,0,4,1]],"sqlite3ValueText":[[83306,7,83306,12,0,0,4,0],[83310,63,83310,77,0,0,4,1]],"sqlite3BtreeHoldsMutex":[[69644,11,69644,25,0,0,4,0]],"sqlite3FinishCoding":[[120380,9,120380,27,0,0,4,1]],"sqlite3ValueNew":[[83343,7,83343,8,0,0,4,0]],"sqlite3BtreeConnectionCount":[[14098,27,14098,28,1,0,4,1]],"sqlite3VdbeMemExpandBlob":[[82358,7,82358,15,0,0,4,0],[82358,7,82358,15,0,0,4,1],[82359,9,82359,36,0,0,4,1],[82362,7,82362,41,0,0,4,1],[14098,27,14098,28,1,0,4,1]],"sqlite3_bind_zeroblob":[[91312,7,91312,20,0,0,4,0]],"sqlite3WhereBegin":[[166546,9,166546,25,0,0,4,1]],"sqlite3GetToken":[[176764,5,176764,18,0,0,4,0],[176771,11,176771,20,0,0,4,1],[176778,11,176778,20,0,0,4,1],[176783,12,176783,21,0,0,4,1],[176783,25,176783,34,0,0,4,1],[14098,27,14098,28,62,0,4,1],[14098,27,14098,28,63,0,4,0],[14098,27,14098,28,64,0,4,1],[14098,27,14098,28,65,0,4,1],[14098,27,14098,28,66,0,4,1],[14098,27,14098,28,67,0,4,1],[14098,27,14098,28,68,0,4,1],[14098,27,14098,28,69,0,4,1],[14098,27,14098,28,70,0,4,1],[14098,27,14098,28,71,0,4,1],[14098,27,14098,28,72,0,4,1],[20400,30,20400,72,76,0,4,0],[20400,30,20400,72,76,0,4,1],[176500,20,176500,65,84,0,4,1]],"sqlite3GetInt32":[[35374,46,35374,50,0,0,4,1]],"querySharedCacheTableLock":[[70094,7,70094,19,0,0,4,0]],"sqlite3_get_table_cb":[[149899,7,149899,17,0,0,4,0],[149899,21,149899,28,0,0,4,0],[149904,7,149904,34,0,0,4,1],[149915,7,149915,17,0,0,4,0],[149917,14,149917,20,0,0,4,0],[149917,14,149917,20,0,0,4,1],[149919,11,149919,15,0,0,4,1],[149933,7,149933,14,0,0,4,0],[149934,14,149934,20,0,0,4,0],[149934,14,149934,20,0,0,4,1],[149935,11,149935,21,0,0,4,1],[149940,13,149940,17,0,0,4,1]],"sqlite3Checkpoint":[[180176,12,180176,21,0,0,4,0],[180176,12,180176,21,0,0,4,1],[180176,25,180176,38,0,0,4,0],[180177,9,180177,15,0,0,4,0],[180177,9,180177,15,0,0,4,1],[180177,19,180177,37,0,0,4,1],[180181,11,180181,26,0,0,4,1],[180188,11,180188,24,0,0,4,0],[180188,28,180188,33,0,0,4,1],[14098,27,14098,28,2,0,4,1],[14098,27,14098,28,3,0,4,1]],"btreeBeginTrans":[[73446,11,73446,22,0,0,4,1]],"sqlite3VdbeMemGrow":[[82202,7,82202,23,0,0,4,0],[82202,27,82202,36,0,0,4,0],[82202,40,82202,62,0,0,4,0],[82203,9,82203,17,0,0,4,0],[82224,7,82224,16,0,0,4,0],[82224,20,82224,27,0,0,4,1],[14098,27,14098,28,2,0,4,0],[14098,27,14098,28,2,0,4,0]],"sqlite3_get_table":[[149976,7,149976,32,0,0,4,1],[149976,36,149976,48,0,0,4,1],[149979,7,149979,15,0,0,4,0],[149980,7,149980,12,0,0,4,0],[149981,7,149981,15,0,0,4,0],[149989,7,149989,22,0,0,4,1],[149997,7,149997,30,0,0,4,1],[150010,7,150010,20,0,0,4,1],[150014,7,150014,27,0,0,4,0],[150017,9,150017,17,0,0,4,1],[150025,7,150025,15,0,0,4,0],[150026,7,150026,12,0,0,4,0]],"sqlite3BtreeLeaveAll":[[69690,7,69690,27,0,0,4,1]],"sqlite3_free_table":[[150036,7,150036,15,0,0,4,0],[150041,14,150041,17,0,0,4,0],[150041,14,150041,17,0,0,4,1],[150041,29,150041,40,0,0,4,0]],"wherePathSolver":[[165425,7,165425,26,0,0,4,0],[165715,7,165715,23,0,0,4,1]],"vdbeMemAddTerminator":[[82307,7,82307,45,0,0,4,1]],"sqlite3ValueBytes":[[83956,7,83956,30,0,0,4,1],[83959,7,83959,30,0,0,4,1],[83962,7,83962,31,0,0,4,0],[83963,9,83963,28,0,0,4,1]],"sqlite3VdbeMemTooBig":[[82968,9,82968,28,0,0,4,0]],"sqlite3ValueFree":[[83940,7,83940,9,0,0,4,1]],"sqlite3DbRealloc":[[30527,11,30527,34,0,0,4,0]],"sqlite3VdbeAddOpList":[[85114,7,85114,33,0,0,4,0],[85114,37,85114,56,0,0,4,1]],"sqlite3ValueSetStr":[[83933,7,83933,8,0,0,4,0]],"sqlite3BtreeEnterAll":[[69678,7,69678,27,0,0,4,1]],"sqlite3VdbeExec":[[94247,1,94247,18,0,0,4,0],[94249,7,94249,33,0,0,4,1],[95016,1,95016,16,0,0,4,0],[95019,7,95019,34,0,0,4,0],[95761,11,95761,33,0,0,4,0],[95763,13,95763,18,0,0,4,0],[95764,15,95764,45,0,0,4,1],[97641,7,97641,13,0,0,4,0],[97643,9,97643,19,0,0,4,1],[101074,1,101074,18,0,0,4,0],[19564,29,19564,37,4,0,4,1],[19564,29,19564,37,889,0,4,1]],"sqlite3OpenTableAndIndices":[[134333,13,134333,41,0,0,4,1]],"sqlite3VdbeMemTranslate":[[34120,7,34120,29,0,0,4,1],[34141,7,34141,30,0,0,4,1],[34172,7,34172,29,0,0,4,0],[34173,9,34173,35,0,0,4,0],[34175,14,34175,23,0,0,4,0],[34175,14,34175,23,0,0,4,1],[34047,7,34047,14,9,0,4,1],[33995,7,33995,16,10,0,4,0]],"sqlite3ErrorWithMsg":[[34631,7,34631,17,0,0,4,1],[34633,13,34633,21,0,0,4,1],[34633,25,34633,60,0,0,4,0]],"sqlite3_interrupt":[[179430,7,179430,32,0,0,4,1]],"sqlite3VdbeNoopComment":[[85613,7,85613,8,0,0,4,0]],"sqlite3_bind_int64":[[91206,7,91206,20,0,0,4,0]],"columnTypeImpl":[[143269,13,143269,19,0,0,4,0]],"sqlite3BtreeInsert":[[79113,15,79113,21,0,0,4,1]],"sqlite3ExprAlloc":[[108871,9,108871,23,0,0,4,1],[108871,27,108871,39,0,0,4,1],[108872,14,108872,52,0,0,4,1],[108883,11,108883,20,0,0,4,0],[108884,45,108884,51,0,0,4,0]],"sqlite3BtreeCheckpoint":[[80929,7,80929,8,0,0,4,0],[80932,9,80932,39,0,0,4,1]],"whereScanInit":[[160767,9,160767,37,0,0,4,1],[160769,15,160769,25,0,0,4,1],[160772,15,160772,31,0,0,4,1]],"sqlite3VdbeReset":[[87531,7,87531,15,0,0,4,1]],"sqlite3Insert":[[132532,15,132532,19,0,0,4,1],[132533,15,132533,29,0,0,4,0],[132937,9,132937,21,0,0,4,0],[132939,11,132939,23,0,0,4,1],[132941,17,132941,24,0,0,4,1],[132945,13,132945,30,0,0,4,1],[132955,11,132955,22,0,0,4,0],[132957,13,132957,23,0,0,4,0]],"sqlite3HaltConstraint":[[125480,7,125480,24,0,0,4,0]],"yy_reduce":[[174716,7,174716,15,0,0,4,0],[174719,7,174719,23,0,0,4,0]],"sqlite3RowidConstraint":[[125533,7,125533,21,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_wal_hook","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_interrupt","sqlite3_wal_checkpoint","sqlite3_get_table","sqlite3_errmsg16","sqlite3_free_table","sqlite3_free","sqlite3_finalize","sqlite3_context_db_handle","sqlite3_context_db_handle","sqlite3_close_v2"],"critical_calls":["sqlite3_open","sqlite3_wal_hook","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_interrupt","sqlite3_wal_checkpoint","sqlite3_get_table","sqlite3_errmsg16","sqlite3_free_table","sqlite3_free","sqlite3_finalize","sqlite3_context_db_handle","sqlite3_context_db_handle","sqlite3_close_v2"],"visited":0}
/**/


// WAL hook callback function
static int  id_000001_wal_hook_callback(void* pArg, sqlite3* db, const char* db_name, int pages) {
    // Just return 0 to continue normally
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	 // Need minimum data
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char** pazResult = nullptr;
    int nRow = 0, nColumn = 0;
    char* pzErrmsg = nullptr;
    
    // Create a temporary database file using input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    assert_file_closed(&in_file);;
    
    // Open an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Set WAL hook
    sqlite3_wal_hook(db,  id_000001_wal_hook_callback, nullptr);
    
    // Create a simple table
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS test ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "name TEXT);";
    
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &pzErrmsg);
    if (rc != SQLITE_OK && pzErrmsg) {
        sqlite3_free(pzErrmsg);
    }
    
    // 2. Prepare and execute a statement with bind_zeroblob
    const char* insert_sql = "INSERT INTO test (id, data, name) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind parameters
        sqlite3_bind_int(stmt, 1, 1);
        
        // Use zeroblob for data column
        int zeroblob_size = size % 1024; // Limit size
        sqlite3_bind_zeroblob(stmt, 2, zeroblob_size);
        
        // Bind name
        const char* name = "test_name";
        sqlite3_bind_text(stmt, 3, name, -1, SQLITE_STATIC);
        
        // Execute
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // 3. Query and get blob data
        const char* select_sql = "SELECT data FROM test WHERE id = 1;";
        sqlite3_stmt* select_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
        if (rc == SQLITE_OK) {
            if (sqlite3_step(select_stmt) == SQLITE_ROW) {
                // Get blob data
                const void* blob = sqlite3_column_blob(select_stmt, 0);
                int blob_size = sqlite3_column_bytes(select_stmt, 0);
                // We could process blob here if needed
            }
            sqlite3_finalize(select_stmt);
        }
        
        // 4. Interrupt the database
        sqlite3_interrupt(db);
        
        // 5. Perform WAL checkpoint (in-memory db, but we call it anyway)
        sqlite3_wal_checkpoint(db, "main");
        
        // 6. Use sqlite3_get_table
        const char* table_sql = "SELECT * FROM test;";
        rc = sqlite3_get_table(db, table_sql, &pazResult, &nRow, &nColumn, &pzErrmsg);
        
        // 8. Use sqlite3_errmsg16
        const void* err_msg = sqlite3_errmsg16(db);
        // Could log error message if needed
        
        // Clean up table results
        if (pazResult) {
            sqlite3_free_table(pazResult);
        }
        
        // Clean up error message
        if (pzErrmsg) {
            sqlite3_free(pzErrmsg);
        }
    }
    
    // Clean up statement
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // 7. Try to get database handle from context (simulate with null context)
    // Note: sqlite3_context_db_handle requires a valid context, which we don't have
    // We'll create a dummy scenario
    sqlite3_context* dummy_context = nullptr;
    // We can't actually call sqlite3_context_db_handle without a real context
    
    // Create a simple function to demonstrate context usage
    static auto simple_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        // Get database handle from context
        sqlite3* db_handle = sqlite3_context_db_handle(ctx);
        if (db_handle) {
            // We could use db_handle here
        }
    };
    // Note: We would need to register this function with sqlite3_create_function
    // but that API isn't in the provided list
    
    // Close database
    if (db) {
        sqlite3_close_v2(db);
    }
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}