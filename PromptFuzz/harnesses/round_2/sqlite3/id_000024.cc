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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 720, nr_unique_branch: 201
//<Quality> {"density":16,"unique_branches":{"sqlite3BtreeSetAutoVacuum":[[72937,7,72937,46,0,0,4,0],[72937,50,72937,76,0,0,4,0],[72937,51,72937,53,0,0,4,0],[72940,23,72940,25,0,0,4,0]],"sqlite3ExprDeleteNN":[[18919,34,18919,55,6,0,4,0]],"vdbeColumnFromOverflow":[[92934,9,92934,24,0,0,4,1],[92940,9,92940,27,0,0,4,1],[92941,9,92941,27,0,0,4,1],[92942,9,92942,41,0,0,4,1],[92943,9,92943,41,0,0,4,1],[92944,9,92944,60,0,0,4,1],[92963,9,92963,12,0,0,4,0]],"invalidateAllOverflowCache":[[70313,23,70313,24,0,0,4,1]],"pageInsertArray":[[77372,7,77372,19,0,0,4,0],[77372,7,77372,19,0,0,4,1],[77373,30,77373,49,0,0,4,1],[77380,10,77380,21,0,0,4,0],[77380,25,77380,36,0,0,4,0],[77381,11,77381,30,0,0,4,1],[77391,9,77391,49,0,0,4,0],[77391,9,77391,49,0,0,4,1],[77392,9,77392,46,0,0,4,1],[77402,9,77402,16,0,0,4,0],[14155,25,14155,46,1,0,4,0],[14155,26,14155,29,1,0,4,0]],"moveToRoot":[[75280,13,75280,25,0,0,4,0],[75282,9,75282,23,0,0,4,1]],"yy_reduce":[[174775,7,174775,15,0,0,4,0],[175214,7,175214,15,0,0,4,0],[175337,7,175337,15,0,0,4,0],[175343,7,175343,15,0,0,4,0],[175402,7,175402,15,0,0,4,0],[175426,7,175426,15,0,0,4,0],[175429,7,175429,27,0,0,4,0]],"cachedCellSize":[[77248,7,77248,19,0,0,4,1]],"balance":[[78793,13,78793,29,0,0,4,1],[78835,15,78835,20,0,0,4,1],[78860,7,78860,12,0,0,4,0]],"sqlite3Reindex":[[125638,7,125638,16,0,0,4,1],[125641,33,125641,45,0,0,4,0],[125645,9,125645,15,0,0,4,1],[125647,9,125647,14,0,0,4,1],[125655,7,125655,12,0,0,4,1],[125657,7,125657,11,0,0,4,1],[125660,7,125660,11,0,0,4,1],[125667,7,125667,13,0,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1]],"sqlite3RunParser":[[176986,17,176986,35,0,0,4,0]],"sqlite3_next_stmt":[[91507,7,91507,15,0,0,4,0]],"btreeNext":[[75992,11,75992,25,0,0,4,1],[75998,13,75998,35,0,0,4,0]],"sqlite3WindowOffsetExpr":[[168445,7,168445,38,0,0,4,1]],"balance_nonroot":[[77906,7,77906,18,0,0,4,1],[77923,7,77923,10,0,0,4,0],[77937,7,77937,51,0,0,4,0],[77944,9,77944,22,0,0,4,0],[77947,9,77947,11,0,0,4,1],[77951,9,77951,26,0,0,4,1],[77959,9,77959,17,0,0,4,0],[78012,7,78012,18,0,0,4,1],[78039,12,78039,18,0,0,4,0],[78039,12,78039,18,0,0,4,1],[78051,9,78051,43,0,0,4,1],[78074,9,78074,26,0,0,4,0],[78080,16,78080,23,0,0,4,1],[78085,16,78085,33,0,0,4,0],[78085,16,78085,33,0,0,4,1],[78092,12,78092,24,0,0,4,1],[78101,9,78101,17,0,0,4,1],[78152,14,78152,20,0,0,4,0],[78152,14,78152,20,0,0,4,1],[78156,9,78156,10,0,0,4,1],[78159,9,78159,18,0,0,4,1],[78166,14,78166,28,0,0,4,0],[78166,14,78166,28,0,0,4,1],[78172,12,78172,15,0,0,4,0],[78172,12,78172,15,0,0,4,1],[78174,12,78174,32,0,0,4,1],[78193,12,78193,29,0,0,4,1],[78207,9,78207,27,0,0,4,0],[78226,14,78226,17,0,0,4,1],[78277,12,78277,15,0,0,4,0],[78277,12,78277,15,0,0,4,1],[78279,9,78279,15,0,0,4,0],[78284,11,78284,77,0,0,4,1],[78289,11,78289,13,0,0,4,1],[78321,12,78321,18,0,0,4,0],[78321,12,78321,18,0,0,4,1],[78326,12,78326,20,0,0,4,1],[78372,7,78372,32,0,0,4,1],[78436,12,78436,20,0,0,4,1],[78517,17,78517,23,0,0,4,0],[78517,17,78517,23,0,0,4,1],[78518,15,78518,18,0,0,4,1],[78522,9,78522,20,0,0,4,1],[78523,9,78523,13,0,0,4,0],[78538,11,78538,17,0,0,4,0],[78548,11,78548,13,0,0,4,1],[78562,7,78562,13,0,0,4,0],[78562,17,78562,34,0,0,4,0],[78562,38,78562,73,0,0,4,1],[78604,15,78604,21,0,0,4,1],[78624,12,78624,18,0,0,4,0],[78624,12,78624,18,0,0,4,1],[78627,12,78627,18,0,0,4,0],[78627,12,78627,18,0,0,4,1],[14156,25,14156,46,17,0,4,1],[14156,26,14156,29,17,0,4,1],[69436,27,69436,44,33,0,4,1],[69436,27,69436,44,43,0,4,1]],"analyzeFilterKeyword":[[176594,7,176594,23,0,0,4,1]],"keywordCode":[[14098,27,14098,28,75,0,4,0],[14098,27,14098,28,107,0,4,0],[14098,27,14098,28,119,0,4,0],[14098,27,14098,28,124,0,4,0],[14098,27,14098,28,125,0,4,0],[14098,27,14098,28,133,0,4,0]],"yy_destructor":[[172746,5,172746,13,0,0,4,0],[172747,5,172747,13,0,0,4,0],[172763,5,172763,13,0,0,4,0],[172804,5,172804,13,0,0,4,0]],"assertParentIndex":[[20092,21,20092,52,1,0,4,0]],"sqlite3Pragma":[[138034,3,138034,27,0,0,4,0],[138037,9,138037,16,0,0,4,1],[138049,11,138049,24,0,0,4,0],[138049,11,138049,24,0,0,4,1],[138049,29,138049,37,0,0,4,0],[14187,37,14187,58,65,0,4,1],[14187,38,14187,41,65,0,4,1]],"sqlite3BtreeCommitPhaseOne":[[74014,9,74014,24,0,0,4,0],[74016,11,74016,24,0,0,4,1]],"moveToParent":[[14098,27,14098,28,1,0,4,1]],"analyzeWindowKeyword":[[176583,7,176583,15,0,0,4,1]],"sqlite3BtreeBeginTrans":[[73504,34,73504,43,0,0,4,0]],"finalDbSize":[[73842,7,73842,30,0,0,4,1],[73845,38,73845,61,0,0,4,1],[69381,34,69381,71,4,0,4,1]],"sqlite3ResolveExprNames":[[107802,10,107802,23,0,0,4,0]],"writeJournalHdr":[[57881,7,57881,29,0,0,4,1]],"getAutoVacuum":[[137375,7,137375,35,0,0,4,1],[137376,7,137376,35,0,0,4,1],[137377,7,137377,42,0,0,4,1],[137379,16,137379,20,0,0,4,0],[137379,22,137379,26,0,0,4,0]],"ptrmapPageno":[[70781,7,70781,13,0,0,4,0]],"analyzeOverKeyword":[[176587,7,176587,23,0,0,4,0],[176587,7,176587,23,0,0,4,1],[176589,9,176589,17,0,0,4,1],[176589,21,176589,29,0,0,4,0],[176589,21,176589,29,0,0,4,1]],"sqlite3WindowUnlinkFromSelect":[[168402,7,168402,16,0,0,4,1]],"sqlite3WindowAlloc":[[168477,7,168477,15,0,0,4,0],[168496,8,168496,26,0,0,4,1],[168497,8,168497,28,0,0,4,1],[168504,7,168504,14,0,0,4,1],[168508,7,168508,18,0,0,4,0],[17754,41,17754,71,7,0,4,1]],"lookupName":[[106361,9,106361,38,0,0,4,0],[106362,9,106362,16,0,0,4,0],[106366,16,106366,31,0,0,4,0],[106366,16,106366,31,0,0,4,1],[106368,13,106368,47,0,0,4,0],[106369,13,106369,42,0,0,4,1]],"sqlite3WindowAssemble":[[168535,7,168535,11,0,0,4,0],[168538,9,168538,14,0,0,4,0]],"sqlite3_value_subtype":[[89676,11,89676,38,0,0,4,1]],"lockBtree":[[73171,7,73171,24,0,0,4,1]],"sqlite3BtreeUpdateMeta":[[80087,9,80087,31,0,0,4,0]],"sqlite3WindowDelete":[[168413,7,168413,8,0,0,4,0]],"editPage":[[77515,7,77515,16,0,0,4,1],[77521,7,77521,24,0,0,4,1],[77528,7,77528,19,0,0,4,1],[77532,7,77532,16,0,0,4,1],[77546,12,77546,28,0,0,4,0],[77546,12,77546,28,0,0,4,1],[77548,9,77548,17,0,0,4,0],[77548,21,77548,31,0,0,4,0],[77550,11,77550,22,0,0,4,1],[77555,11,77558,8,0,0,4,1],[77565,7,77568,4,0,0,4,1],[77577,12,77577,18,0,0,4,0],[77577,22,77577,33,0,0,4,1],[14156,25,14156,46,4,0,4,1],[14156,26,14156,29,4,0,4,1]],"sqlite3VdbeAddOpList":[[85123,65,85123,74,0,0,4,1]],"resolveSelectStep":[[107618,9,107618,49,0,0,4,0]],"getToken":[[176540,7,176540,38,0,0,4,0]],"autoVacuumCommit":[[73912,7,73912,23,0,0,4,0],[73920,38,73920,62,0,0,4,1],[73930,9,73930,26,0,0,4,1],[73952,9,73952,19,0,0,4,1],[73953,9,73953,19,0,0,4,1],[73956,22,73956,32,0,0,4,1],[73959,10,73959,25,0,0,4,1],[73959,29,73959,42,0,0,4,0],[73959,47,73959,54,0,0,4,1],[73969,9,73969,22,0,0,4,1],[69381,34,69381,71,4,0,4,1]],"sqlite3WindowAttach":[[168590,7,168590,8,0,0,4,0],[168597,9,168597,33,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_complete","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_prepare_v2","sqlite3_next_stmt","sqlite3_finalize","sqlite3_exec","sqlite3_autovacuum_pages","sqlite3_finalize","sqlite3_next_stmt","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_complete","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_prepare_v2","sqlite3_next_stmt","sqlite3_finalize","sqlite3_exec","sqlite3_autovacuum_pages","sqlite3_finalize","sqlite3_next_stmt","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" {
}

// Callback function for sqlite3_autovacuum_pages
static unsigned int  id_000024_autovacuum_callback(void *pCtx, const char *zSchema, 
                                      unsigned int iPage, 
                                      unsigned int nFree, 
                                      unsigned int nTotal) {
    return 1;  // Recommended number of pages to vacuum
}

// Callback for freeing autovacuum context
static void  id_000024_autovacuum_free(void *pCtx) {
    // Nothing to free in this simple example
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize sqlite3
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt *next_stmt = nullptr;
    int rc;
    
    // Create an in-memory database for testing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // 1. Check if the input contains a complete SQL statement
    if (size > 0) {
        // Ensure null termination for sqlite3_complete
        char *sql_str = (char *)malloc(size + 1);
        if (sql_str) {
            memcpy(sql_str, data, size);
            sql_str[size] = '\0';
            
            // Use sqlite3_complete to check if it's a complete SQL statement
            int is_complete = sqlite3_complete(sql_str);
            
            // Try to prepare the statement if it appears complete
            if (is_complete) {
                rc = sqlite3_prepare_v2(db, sql_str, -1, &stmt, nullptr);
                if (rc == SQLITE_OK && stmt != nullptr) {
                    // Execute the statement
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // Process rows if any
                    }
                    
                    // Reset the statement for potential reuse
                    sqlite3_reset(stmt);
                    
                    // 2. Get value subtype from the statement's first column
                    if (sqlite3_column_count(stmt) > 0) {
                        sqlite3_value *value = sqlite3_column_value(stmt, 0);
                        if (value) {
                            unsigned int subtype = sqlite3_value_subtype(value);
                            // subtype can be used or ignored
                        }
                    }
                    
                    // 3. Use sqlite3_next_stmt to iterate through statements
                    // Create another statement to have multiple
                    sqlite3_stmt *stmt2 = nullptr;
                    rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt2, nullptr);
                    if (rc == SQLITE_OK) {
                        // Iterate through all statements
                        stmt = nullptr;
                        while ((stmt = sqlite3_next_stmt(db, stmt)) != nullptr) {
                            // Do something with each statement
                        }
                        sqlite3_finalize(stmt2);
                    }
                }
            }
            
            free(sql_str);
        }
    }
    
    // 4. Set up autovacuum pages callback
    // First enable auto-vacuum mode
    rc = sqlite3_exec(db, "PRAGMA auto_vacuum = 1", nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK) {
        // Set the autovacuum pages callback
        rc = sqlite3_autovacuum_pages(db,  id_000024_autovacuum_callback, nullptr,  id_000024_autovacuum_free);
    }
    
    // Finalize any remaining statements
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Clean up any remaining statements using sqlite3_next_stmt
    stmt = nullptr;
    while ((stmt = sqlite3_next_stmt(db, stmt)) != nullptr) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Close database
    sqlite3_close(db);
    
    // 5. Shutdown sqlite3
    sqlite3_shutdown();
    
    return 0;
}