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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 341, nr_unique_branch: 171
//<Quality> {"density":11,"unique_branches":{"sqlite3_create_function16":[[179705,7,179705,32,0,0,4,1],[179705,36,179705,52,0,0,4,1]],"sqlite3FinishCoding":[[120288,9,120288,25,0,0,4,0]],"sqlite3PagerSetPagesize":[[60205,11,60205,16,0,0,4,0],[60212,9,60212,22,0,0,4,1],[60216,9,60216,22,0,0,4,1],[60228,7,60228,20,0,0,4,1]],"pagerAddPageToRollbackJournal":[[62424,7,62424,20,0,0,4,0]],"pcache1FetchStage2":[[55405,7,55405,12,0,0,4,1]],"sqlite3_errmsg":[[180234,7,180234,10,0,0,4,1],[180237,7,180237,38,0,0,4,1],[180241,7,180241,23,0,0,4,1],[180245,9,180245,20,0,0,4,0],[180247,9,180247,13,0,0,4,1],[14098,27,14098,28,4,0,4,1]],"sqlite3VdbeMemGrow":[[82215,7,82215,23,0,0,4,0]],"sqlite3BtreeCloseCursor":[[74508,7,74508,13,0,0,4,1]],"createFunctionApi":[[179632,15,179632,28,0,0,4,0]],"memjrnlWrite":[[105262,15,105262,20,0,0,4,0]],"pcache1Alloc":[[54846,9,54846,10,0,0,4,1]],"sqlite3VdbeExec":[[95810,9,95810,55,0,0,4,0],[96572,7,96572,9,0,0,4,0],[99101,7,99101,9,0,0,4,0],[99138,7,99138,11,0,0,4,0],[99141,9,99141,25,0,0,4,0],[99206,11,99206,24,0,0,4,1],[99207,11,99207,24,0,0,4,1],[99217,7,99217,9,0,0,4,0],[99219,9,99219,25,0,0,4,0],[99327,7,99327,14,0,0,4,1],[99329,13,99329,15,0,0,4,0],[101197,7,101197,23,0,0,4,0],[101215,24,101215,46,0,0,4,1],[101224,7,101224,29,0,0,4,0]],"allocateBtreePage":[[76401,9,76401,11,0,0,4,0],[76429,9,76429,11,0,0,4,0]],"sqlite3BtreeInsert":[[79204,9,79204,11,0,0,4,0]],"apiHandleError":[[30684,7,30684,23,0,0,4,0]],"openDatabase":[[181009,7,181009,23,0,0,4,0],[181104,12,181104,25,0,0,4,1],[181111,7,181111,20,0,0,4,1],[181136,7,181136,9,0,0,4,0],[181152,7,181152,30,0,0,4,0]],"readJournalHdr":[[58041,16,58041,43,0,0,4,0],[58046,9,58046,57,0,0,4,0]],"sqlite3OomFault":[[30645,7,30645,26,0,0,4,0],[30645,7,30645,26,0,0,4,1],[30645,30,30645,50,0,0,4,0],[30647,9,30647,24,0,0,4,0],[30647,9,30647,24,0,0,4,1],[30651,9,30651,19,0,0,4,0],[30651,9,30651,19,0,0,4,1],[30655,43,30655,49,0,0,4,0],[30655,43,30655,49,0,0,4,1]],"strAccumFinishRealloc":[[31769,7,31769,12,0,0,4,1]],"checkOom":[[80186,7,80186,22,0,0,4,0]],"allocateTempSpace":[[72610,7,72610,24,0,0,4,0]],"sqlite3StrAccumSetError":[[30832,7,30832,17,0,0,4,0],[30833,7,30833,28,0,0,4,1]],"sqlite3PagerOpen":[[61291,7,61291,12,0,0,4,0],[14098,27,14098,28,37,0,4,0]],"vdbeFreeOpArray":[[85387,7,85387,10,0,0,4,1]],"pager_open_journal":[[62251,9,62251,30,0,0,4,0]],"sqlite3_errcode":[[180319,14,180319,30,0,0,4,0]],"sqlite3PcacheSetPageSize":[[53909,9,53909,16,0,0,4,0]],"constInsert":[[146101,7,146101,24,0,0,4,0]],"sqlite3RunParser":[[177022,7,177022,23,0,0,4,0],[177025,52,177025,75,0,0,4,0],[177026,9,177026,27,0,0,4,0]],"sqlite3ExprListAppendNew":[[109928,7,109928,15,0,0,4,0]],"sqlite3_hard_heap_limit64":[[29956,7,29956,9,0,0,4,1],[29960,7,29960,11,0,0,4,0],[29962,9,29962,30,0,0,4,0]],"btreeGetUnusedPage":[[72191,7,72191,20,0,0,4,1]],"sqlite3SystemError":[[34591,7,34591,29,0,0,4,0]],"selectExpander":[[147550,17,147550,24,0,0,4,0]],"sqlite3_sleep":[[181613,7,181613,14,0,0,4,1],[181618,30,181618,34,0,0,4,1]],"sqlite3DbMallocZero":[[30413,7,30413,8,0,0,4,1]],"sqlite3_free_filename":[[182315,7,182315,11,0,0,4,0]],"sqlite3VdbeGetOp":[[85650,7,85650,26,0,0,4,0]],"sqlite3KeyInfoUnref":[[142821,7,142821,8,0,0,4,1]],"setupLookaside":[[178394,9,178394,15,0,0,4,1],[178418,7,178418,13,0,0,4,1]],"sqlite3FindFunction":[[126293,7,126293,67,0,0,4,1]],"sqlite3RegisterPerConnectionBuiltinFunctions":[[129637,7,129637,23,0,0,4,0]],"btreeEndTransaction":[[74043,32,74043,47,0,0,4,0]],"sqlite3BtreeTripAllCursors":[[74173,33,74173,34,0,0,4,0],[74174,11,74174,20,0,0,4,1]],"sqlite3MallocAlarm":[[30033,7,30033,29,0,0,4,0]],"sqlite3CreateFunc":[[179565,7,179565,9,0,0,4,0]],"sqlite3BtreeIntegrityCheck":[[80787,7,80787,21,0,0,4,0],[80871,7,80871,21,0,0,4,1]],"sqlite3InitOne":[[140599,9,140599,22,0,0,4,1],[140602,9,140602,22,0,0,4,1],[140613,7,140613,20,0,0,4,1],[140613,25,140613,57,0,0,4,1],[140640,9,140640,25,0,0,4,0]],"sqlite3WhereBegin":[[166342,7,166342,23,0,0,4,0],[166898,7,166898,13,0,0,4,1]],"sqlite3IoerrnomemError":[[14098,27,14098,28,1,0,4,1]],"sqlite3KeyInfoAlloc":[[142803,7,142803,8,0,0,4,1]],"sqlite3DbReallocOrFree":[[30565,7,30565,12,0,0,4,0]],"sqlite3Realloc":[[30351,11,30351,27,0,0,4,1]],"sqlite3_exec":[[134949,24,134949,32,0,0,4,0],[134951,9,134951,21,0,0,4,1]],"btreeCreateTable":[[79786,9,79786,11,0,0,4,0]],"sqlite3KeyInfoFromExprList":[[142878,7,142878,12,0,0,4,1]],"growOp3":[[84202,7,84202,24,0,0,4,0]],"sqlite3DbMallocRawNN":[[30469,15,30469,31,0,0,4,0]],"sqlite3VMPrintf":[[31907,7,31907,33,0,0,4,0]],"getPageNormal":[[61958,7,61958,15,0,0,4,0],[61961,9,61961,22,0,0,4,1],[61962,9,61962,17,0,0,4,0],[62036,7,62036,10,0,0,4,1]],"sqlite3ExprAlloc":[[108878,7,108878,11,0,0,4,1]],"propagateConstants":[[146297,9,146297,17,0,0,4,1]],"sqlite3ApiExit":[[30711,7,30711,23,0,0,4,0]],"sqlite3LockAndPrepare":[[141077,26,141077,42,0,0,4,0]],"sqlite3Step":[[90340,7,90340,49,0,0,4,0],[90342,9,90342,51,0,0,4,0]],"sqlite3PagerRollback":[[63193,9,63193,15,0,0,4,1]],"dbMallocRawFinish":[[30425,7,30425,9,0,0,4,0]],"sqlite3BitvecCreate":[[53247,7,53247,8,0,0,4,1]],"sqlite3VdbeSetColName":[[86835,7,86835,26,0,0,4,0]],"sqlite3Utf16to8":[[34385,7,34385,23,0,0,4,1]],"sqlite3Select":[[148556,15,148556,27,0,0,4,0],[149187,9,149187,18,0,0,4,0]],"growOpArray":[[84167,7,84167,11,0,0,4,1],[84172,11,84172,15,0,0,4,1]],"pager_write":[[62500,11,62500,24,0,0,4,0]],"sqlite3VdbeMemSetStr":[[83126,9,83126,25,0,0,4,1],[83129,20,83129,33,0,0,4,0],[83129,37,83129,60,0,0,4,0],[83129,37,83129,60,0,0,4,1],[83162,9,83162,64,0,0,4,0]],"sqlite3ExprSetErrorOffset":[[108834,7,108834,15,0,0,4,0]],"pcache1AllocPage":[[54937,9,54937,15,0,0,4,0]],"sqlite3VdbeChangeP4":[[85522,7,85522,23,0,0,4,0],[85523,9,85523,19,0,0,4,0]],"pcache1ResizeHash":[[55034,7,55034,12,0,0,4,1]],"sqlite3NomemError":[[14098,27,14098,28,1,0,4,1]],"mallocWithAlarm":[[30057,7,30057,28,0,0,4,1],[30064,13,30064,44,0,0,4,0]],"sqlite3MallocZero":[[30399,7,30399,8,0,0,4,1]],"sqlite3OomClear":[[30672,7,30672,23,0,0,4,0],[30672,27,30672,43,0,0,4,0],[30672,27,30672,43,0,0,4,1],[17405,21,17405,43,2,0,4,0],[17405,21,17405,43,2,0,4,1]],"sqlite3PcacheFetchStress":[[53993,7,53993,25,0,0,4,0]],"countValidCursors":[[73204,10,73204,19,0,0,4,1],[73204,23,73204,59,0,0,4,0],[73205,9,73205,35,0,0,4,1]],"sqlite3VdbeHalt":[[87262,7,87262,23,0,0,4,0],[87281,24,87281,41,0,0,4,0],[87301,11,87301,23,0,0,4,1],[87301,27,87301,48,0,0,4,0],[87302,14,87302,31,0,0,4,0],[87302,56,87302,74,0,0,4,0],[87302,56,87302,74,0,0,4,1],[87429,7,87429,23,0,0,4,0]],"fillInCell":[[76853,11,76853,13,0,0,4,0]],"findCollSeqEntry":[[125954,9,125954,14,0,0,4,1]],"downgradeAllSharedCacheTableLocks":[[70259,7,70259,22,0,0,4,0],[70263,27,70263,32,0,0,4,1]],"createCollation":[[180412,7,180412,15,0,0,4,0]],"sqlite3VdbeSetNumCols":[[86810,7,86810,21,0,0,4,0]],"pcache1Create":[[55281,9,55281,25,0,0,4,0]],"sqlite3Prepare":[[141011,7,141011,23,0,0,4,0],[141024,9,141024,23,0,0,4,1]]},"library_calls":["sqlite3_hard_heap_limit64","sqlite3_libversion_number","sqlite3_open_v2","sqlite3_close","sqlite3_create_function16","sqlite3_close","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_sleep","sqlite3_close"],"critical_calls":["sqlite3_hard_heap_limit64","sqlite3_libversion_number","sqlite3_open_v2","sqlite3_create_function16","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_sleep","sqlite3_close"],"visited":2}
/**/


// Custom function implementation
static void  id_000014_custom_function(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        // Get encoding of first argument
        int encoding = sqlite3_value_encoding(argv[0]);
        sqlite3_result_int(ctx, encoding);
    } else {
        sqlite3_result_int(ctx, -1);
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize resources
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    FILE* in_file = nullptr;
    
    // Set hard heap limit
    sqlite3_int64 heap_limit = size > 0 ? (sqlite3_int64)(data[0] * 1024) : 1024;
    sqlite3_hard_heap_limit64(heap_limit);
    
    // Get library version
    int version = sqlite3_libversion_number();
    
    // Create in-memory database
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create custom function using UTF-16 name
    const void* func_name = L"my_func";
    rc = sqlite3_create_function16(db, func_name, 1, SQLITE_UTF8, nullptr, 
                                    id_000014_custom_function, nullptr, nullptr);
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a table using input data
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK && size > 0) {
        // Prepare INSERT statement
        const char* insert_sql = "INSERT INTO test (data) VALUES (?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // Bind input data as blob
            if (size > 0) {
                rc = sqlite3_bind_blob64(stmt, 1, data, size, SQLITE_STATIC);
                if (rc == SQLITE_OK) {
                    sqlite3_step(stmt);
                }
            }
            sqlite3_finalize(stmt);
        }
        
        // Use custom function in a query
        const char* query_sql = "SELECT my_func(data) FROM test;";
        rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                // Function executed successfully
            }
            sqlite3_finalize(stmt);
        }
    }
    
    // Create a FILE* from input data if we have enough data
    if (size > 4) {
        in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            // Read some bytes to simulate usage
            char buffer[16];
            if (fread(buffer, 1, size > 16 ? 16 : size, in_file) > 0) {
                // Data was read
            }
            assert_file_closed(&in_file);;
        }
    }
    
    // Perform a sleep operation based on input size
    int sleep_ms = size > 1 ? (data[1] % 100) : 1;  // Max 100ms
    if (sleep_ms > 0) {
        sqlite3_sleep(sleep_ms);
    }
    
    // Clean up
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}