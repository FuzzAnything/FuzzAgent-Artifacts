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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 1033.5, nr_unique_branch: 221
//<Quality> {"density":39,"unique_branches":{"generateSortTail":[[142976,7,142976,24,0,0,4,1],[142993,7,142993,24,0,0,4,0],[142994,9,142994,23,0,0,4,1],[143009,7,143009,44,0,0,4,0],[143012,9,143012,26,0,0,4,1],[143017,9,143017,17,0,0,4,1],[143032,30,143032,39,0,0,4,0],[143032,30,143032,39,0,0,4,1],[143036,9,143036,37,0,0,4,0],[143069,20,143069,24,0,0,4,0],[143069,20,143069,24,0,0,4,1],[143077,11,143077,36,0,0,4,1],[143088,5,143088,19,0,0,4,1],[143089,5,143089,22,0,0,4,1],[143097,5,143097,17,0,0,4,1],[143104,5,143104,17,0,0,4,1],[143109,5,143109,20,0,0,4,1],[143120,5,143120,12,0,0,4,0],[143124,11,143124,28,0,0,4,0],[143132,7,143132,15,0,0,4,1],[143143,7,143143,44,0,0,4,0],[143149,7,143149,23,0,0,4,1],[16756,62,16756,63,1,0,4,1],[14098,27,14098,28,33,0,4,0],[14098,27,14098,28,34,0,4,1]],"sqlite3Select":[[149197,11,149197,46,0,0,4,1],[149207,35,149207,52,0,0,4,1],[149800,7,149800,21,0,0,4,0]],"sqlite3_create_collation16":[[181282,7,181282,32,0,0,4,1],[181282,36,181282,44,0,0,4,1],[181287,7,181287,13,0,0,4,0]],"whereSortingCost":[[165350,7,165350,16,0,0,4,1],[165360,7,165360,48,0,0,4,1],[165368,13,165368,55,0,0,4,1]],"sqlite3VdbeSorterRowkey":[[104519,7,104519,47,0,0,4,1]],"sqlite3LogEstAdd":[[36121,9,36121,15,0,0,4,0]],"sqlite3VdbeExec":[[95138,9,95138,20,0,0,4,0],[95139,11,95139,39,0,0,4,0],[95139,43,95139,59,0,0,4,0],[96732,1,96732,19,0,0,4,0],[96738,7,96738,13,0,0,4,1],[96743,7,96743,9,0,0,4,1],[96781,1,96781,19,0,0,4,0],[96787,7,96787,13,0,0,4,1],[98159,1,98159,19,0,0,4,0],[98168,7,98168,9,0,0,4,1],[98434,1,98434,19,0,0,4,0],[98548,1,98548,19,0,0,4,0],[98668,1,98668,21,0,0,4,0],[98680,7,98680,9,0,0,4,1],[98682,7,98682,9,0,0,4,1],[92437,21,92437,52,660,0,4,0],[23623,26,23623,47,678,0,4,1]],"sqlite3TableAffinityStr":[[131787,13,131787,17,0,0,4,1]],"createCollation":[[180370,7,180370,25,0,0,4,0],[14098,27,14098,28,1,0,4,0]],"vdbeSorterJoinThread":[[102928,7,102928,21,0,0,4,1]],"sqlite3GetCollSeq":[[126051,7,126051,9,0,0,4,0],[126054,7,126054,9,0,0,4,1],[126054,13,126054,21,0,0,4,0],[126061,7,126061,8,0,0,4,0],[126061,12,126061,20,0,0,4,1],[126065,7,126065,11,0,0,4,1]],"allocateCursor":[[92473,8,92473,31,0,0,4,1],[92505,7,92505,30,0,0,4,1]],"sqlite3ExprCompare":[[113880,9,113880,27,0,0,4,0],[113880,31,113880,78,0,0,4,0],[113935,11,113935,35,0,0,4,1],[113936,11,113936,27,0,0,4,1],[113937,11,113937,24,0,0,4,0],[113937,28,113937,50,0,0,4,1]],"callCollNeeded":[[125861,7,125861,22,0,0,4,1],[125868,7,125868,24,0,0,4,0],[125873,9,125873,18,0,0,4,0]],"sqlite3VdbeSorterNext":[[104454,7,104454,23,0,0,4,1],[104475,9,104475,33,0,0,4,1],[104476,10,104476,29,0,0,4,1]],"sqlite3VdbeFreeCursorNN":[[86703,11,86703,24,0,0,4,0],[86704,5,86704,24,0,0,4,0],[86708,5,86708,23,0,0,4,1]],"sqlite3ExprNNCollSeq":[[108252,7,108252,11,0,0,4,1]],"sqlite3VdbeScanStatusRange":[[85199,11,85199,20,0,0,4,0],[85201,13,85201,37,0,0,4,1]],"vdbeSorterRecordFree":[[102850,18,102850,19,0,0,4,1]],"sqlite3VdbeSorterInit":[[102761,7,102761,30,0,0,4,1],[102761,34,102761,67,0,0,4,1],[102784,7,102784,17,0,0,4,1],[102791,9,102791,15,0,0,4,1],[102801,14,102801,30,0,0,4,0],[102801,14,102801,30,0,0,4,1],[102806,9,102806,33,0,0,4,0],[102812,11,102812,20,0,0,4,0],[102824,11,102824,46,0,0,4,0],[102828,13,102828,35,0,0,4,1],[102832,9,102832,31,0,0,4,0],[102833,10,102833,31,0,0,4,1],[102833,35,102833,68,0,0,4,1],[14676,20,14676,27,6,0,4,0],[14679,20,14679,27,7,0,4,1]],"sqlite3VdbeSorterClose":[[103081,7,103081,14,0,0,4,0]],"pushOntoSorter":[[142028,7,142028,17,0,0,4,0],[142036,12,142036,28,0,0,4,1],[142039,46,142039,57,0,0,4,0],[142040,7,142040,11,0,0,4,1],[142043,7,142043,20,0,0,4,1],[142046,7,142046,15,0,0,4,1],[142089,7,142089,13,0,0,4,1],[142114,7,142114,19,0,0,4,0],[142117,7,142117,44,0,0,4,0],[142124,7,142124,12,0,0,4,1]],"vdbeMergeEngineFree":[[103015,7,103015,14,0,0,4,1]],"indexMightHelpWithOrderBy":[[163618,13,163618,26,0,0,4,1],[163622,11,163622,27,0,0,4,1],[163623,17,163623,35,0,0,4,0],[163623,17,163623,35,0,0,4,1],[163624,13,163624,49,0,0,4,1]],"sqlite3ExprCodeExprList":[[113342,9,113342,37,0,0,4,0],[113342,41,113342,71,0,0,4,1]],"vdbeSorterRowkey":[[104490,7,104490,23,0,0,4,1]],"sqlite3VdbeSorterWrite":[[103595,7,103595,10,0,0,4,0],[103595,14,103595,18,0,0,4,1],[103597,13,103597,17,0,0,4,0],[103597,21,103597,31,0,0,4,1],[103623,7,103623,25,0,0,4,0],[103624,9,103624,30,0,0,4,0],[103625,16,103625,32,0,0,4,1],[103632,9,103632,15,0,0,4,1],[103641,7,103641,30,0,0,4,0],[103645,7,103645,28,0,0,4,0],[103648,9,103648,30,0,0,4,1],[103669,9,103669,28,0,0,4,1],[21031,18,21031,25,2,0,4,0],[21031,18,21031,25,2,0,4,1]],"sqlite3VdbeAllocUnpackedRecord":[[88145,7,88145,9,0,0,4,1]],"sqlite3_db_readonly":[[182460,7,182460,32,0,0,4,1],[182466,10,182466,13,0,0,4,0]],"sqlite3VdbeScanStatusCounters":[[85225,24,85225,29,0,0,4,0],[85227,11,85227,42,0,0,4,0],[85230,9,85230,14,0,0,4,0],[85231,11,85231,21,0,0,4,0],[85232,11,85232,22,0,0,4,0],[21575,33,21575,68,1,0,4,0]],"sqlite3ExprCollSeq":[[108207,9,108207,23,0,0,4,0]],"vdbeSorterJoinAll":[[102971,27,102971,31,0,0,4,0],[102971,27,102971,31,0,0,4,1],[102974,9,102974,22,0,0,4,0]],"wherePathSatisfiesOrderBy":[[165015,16,165015,31,0,0,4,1],[165173,18,165173,23,0,0,4,1],[165183,17,165183,42,0,0,4,0],[165200,13,165200,20,0,0,4,1],[165215,13,165215,20,0,0,4,1],[165222,13,165222,20,0,0,4,1],[165230,15,165230,19,0,0,4,0],[165237,11,165237,26,0,0,4,1],[165244,9,165244,24,0,0,4,1],[165259,7,165259,20,0,0,4,1],[165260,7,165260,23,0,0,4,0],[165261,23,165261,26,0,0,4,1],[14098,27,14098,28,61,0,4,0]],"vdbeSortSubtaskCleanup":[[102865,7,102865,26,0,0,4,1],[102873,7,102873,22,0,0,4,1],[102876,7,102876,23,0,0,4,1]],"sqlite3VdbeSorterReset":[[103049,7,103049,23,0,0,4,1],[103057,12,103057,28,0,0,4,0],[103057,12,103057,28,0,0,4,1],[103062,7,103062,31,0,0,4,1]],"sqlite3_system_errno":[[180334,10,180334,12,0,0,4,0]],"sqlite3_backup_pagecount":[[81817,7,81817,11,0,0,4,1]],"resolveAsName":[[107127,11,107127,45,0,0,4,1]],"vdbeSortAllocUnpacked":[[103146,7,103146,26,0,0,4,0],[103148,9,103148,28,0,0,4,1]],"sqlite3VdbeIncrWriteCounter":[[84806,8,84806,36,0,0,4,1]],"makeSorterRecord":[[141976,7,141976,30,0,0,4,1]],"sqlite3TableAffinity":[[131865,7,131865,8,0,0,4,1]],"vdbeSorterGetCompare":[[103202,7,103202,39,0,0,4,1],[103204,13,103204,42,0,0,4,1]],"sqlite3VdbeSorterRewind":[[104406,7,104406,26,0,0,4,0],[104407,9,104407,28,0,0,4,0]],"sqlite3_bind_pointer":[[91231,7,91231,20,0,0,4,1],[91234,13,91234,24,0,0,4,0]],"sqlite3ExprSkipCollateAndLikely":[[18919,34,18919,55,1,0,4,1],[18919,34,18919,55,2,0,4,0]],"selectInnerLoop":[[142426,16,142426,34,0,0,4,1],[142427,7,142427,15,0,0,4,1],[142437,9,142437,14,0,0,4,0],[142439,11,142439,51,0,0,4,1],[142473,9,142473,14,0,0,4,0],[142473,18,142473,32,0,0,4,0],[142473,36,142473,55,0,0,4,0],[142473,59,142473,75,0,0,4,0],[142482,28,142482,52,0,0,4,0],[142482,28,142482,52,0,0,4,1],[142484,13,142484,58,0,0,4,1],[142506,16,142506,31,0,0,4,0],[142506,16,142506,31,0,0,4,1],[142507,13,142507,43,0,0,4,1],[142714,11,142714,16,0,0,4,0],[142791,7,142791,15,0,0,4,1],[14098,27,14098,28,14,0,4,0],[14098,27,14098,28,15,0,4,1],[14098,27,14098,28,16,0,4,1],[14098,27,14098,28,17,0,4,1],[14098,27,14098,28,18,0,4,0]],"vdbeSorterSort":[[103222,7,103222,20,0,0,4,1],[103228,10,103228,11,0,0,4,0],[103228,10,103228,11,0,0,4,1],[103230,9,103230,23,0,0,4,0],[103231,11,103231,33,0,0,4,0],[103242,14,103242,22,0,0,4,1],[103251,12,103251,23,0,0,4,0],[103251,12,103251,23,0,0,4,1],[103252,9,103252,20,0,0,4,0],[103252,9,103252,20,0,0,4,1],[103253,9,103253,10,0,0,4,1]],"vdbeUnbind":[[91102,7,91102,31,0,0,4,0]],"sqlite3_result_blob64":[[89923,7,89923,14,0,0,4,1],[89929,7,89929,19,0,0,4,1]],"sqlite3_collation_needed16":[[181328,7,181328,32,0,0,4,1]],"wherePathSolver":[[165514,29,165514,47,0,0,4,0],[165515,15,165515,38,0,0,4,0],[165739,17,165739,44,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_open","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_close","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_create_collation16","sqlite3_collation_needed16","sqlite3_db_readonly","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_step","sqlite3_column_type","sqlite3_finalize","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_system_errno","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_create_collation16","sqlite3_collation_needed16","sqlite3_db_readonly","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_step","sqlite3_column_type","sqlite3_finalize","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_system_errno","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"visited":1}
/**/


// Forward declarations for callback functions
static int  id_000028_myCollation(void* pArg, int n1, const void* s1, int n2, const void* s2);
static void  id_000028_myCollationNeeded(void* pArg, sqlite3* db, int eTextRep, const void* name);
static void  id_000028_myDestructor(void* ptr);
static void  id_000028_myResultBlobFunction(sqlite3_context* ctx, int argc, sqlite3_value** argv);

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3* db2 = nullptr;
    sqlite3_backup* backup = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* errMsg = nullptr;
    int rc;
    
    // 1. Create in-memory databases
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
	return 0;
	}
	
    
    rc = sqlite3_open(":memory:", &db2);
    if (rc != SQLITE_OK || !db2) {
        sqlite3_close(db);
        return 0;
    }
    
    // 2. Use input data to create a table with blob data
    // Create a table with a blob column
    rc = sqlite3_exec(db, 
        "CREATE TABLE test(data BLOB);"
        "CREATE TABLE source(data BLOB);", 
        nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(db2);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert the input data as blob
    rc = sqlite3_prepare_v2(db, "INSERT INTO test(data) VALUES(?)", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size > 1024 ? 1024 : size), 
                         SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Also insert into source table for backup
    rc = sqlite3_prepare_v2(db, "INSERT INTO source(data) VALUES(?)", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size > 512 ? 512 : size), 
                         SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 3. Use sqlite3_create_collation16 to create a custom collation
    // We'll use the first few bytes of input to determine collation behavior
    static int collationArg = 0;
    if (size > 0) collationArg = data[0];
    
    rc = sqlite3_create_collation16(db, 
        reinterpret_cast<const void*>(u"MY_COLLATION"),
        SQLITE_UTF16,
        &collationArg,
         id_000028_myCollation);
    
    // 4. Set up collation needed callback using sqlite3_collation_needed16
    rc = sqlite3_collation_needed16(db, nullptr,  id_000028_myCollationNeeded);
    
    // 5. Use sqlite3_db_readonly to check database status
    int isReadOnly = sqlite3_db_readonly(db, "main");
    
    // 6. Create a backup and use sqlite3_backup_pagecount
    backup = sqlite3_backup_init(db2, "main", db, "main");
    if (backup) {
        int pageCount = sqlite3_backup_pagecount(backup);
        
        // Perform the backup
        sqlite3_backup_step(backup, -1); // Backup all pages
        sqlite3_backup_finish(backup);
        backup = nullptr;
    }
    
    // 7. Prepare a statement and use sqlite3_bind_pointer
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Create a dummy pointer to bind
        static int dummyData = 42;
        void* ptr = &dummyData;
        
        // sqlite3_bind_pointer - note: this requires a pointer type name
        rc = sqlite3_bind_pointer(stmt, 1, ptr, "dummy_type",  id_000028_myDestructor);
        
        // Execute and use sqlite3_column_type
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int colType = sqlite3_column_type(stmt, 0);
            // Do something with colType
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 8. Create a function that uses sqlite3_result_blob64
    rc = sqlite3_create_function(db, "my_blob_func", 0, SQLITE_UTF8, 
                                 nullptr,  id_000028_myResultBlobFunction, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        // Call the function
        rc = sqlite3_prepare_v2(db, "SELECT my_blob_func()", -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // 9. Check system error using sqlite3_system_errno
    int systemErrno = sqlite3_system_errno(db);
    
    // 10. Test the collation in a query (this will trigger collation_needed if not found)
    rc = sqlite3_prepare_v2(db, 
        "SELECT data FROM test ORDER BY data COLLATE MY_COLLATION", 
        -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    if (backup) sqlite3_backup_finish(backup);
    sqlite3_close(db2);
    sqlite3_close(db);
    
    return 0;
}

// Collation function implementation
static int  id_000028_myCollation(void* pArg, int n1, const void* s1, int n2, const void* s2) {
    // Simple byte-by-byte comparison
    int len = n1 < n2 ? n1 : n2;
    int cmp = memcmp(s1, s2, len);
    if (cmp == 0) {
        return n1 - n2;
    }
    return cmp;
}

// Collation needed callback
static void  id_000028_myCollationNeeded(void* pArg, sqlite3* db, int eTextRep, const void* name) {
    // Create the collation when needed
    static int dummy = 0;
    sqlite3_create_collation16(db, name, eTextRep, &dummy,  id_000028_myCollation);
}

// Pointer destructor for sqlite3_bind_pointer
static void  id_000028_myDestructor(void* ptr) {
    // Nothing to do for our dummy pointer
}

// Function that uses sqlite3_result_blob64
static void  id_000028_myResultBlobFunction(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // Create a dummy blob of 100 bytes
    static unsigned char dummyBlob[100];
    for (int i = 0; i < 100; i++) {
        dummyBlob[i] = static_cast<unsigned char>(i);
    }
    
    // Use sqlite3_result_blob64
    sqlite3_result_blob64(ctx, dummyBlob, 100, SQLITE_TRANSIENT);
}