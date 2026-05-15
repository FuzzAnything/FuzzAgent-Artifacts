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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 496, nr_unique_branch: 91
//<Quality> {"density":16,"unique_branches":{"sqlite3_mutex_leave":[[28682,7,28682,8,0,0,4,1]],"sqlite3_commit_hook":[[179875,7,179875,32,0,0,4,1]],"sqlite3_mutex_enter":[[28656,7,28656,8,0,0,4,1]],"sqlite3PagerLockingMode":[[63740,19,63740,36,0,0,4,0],[63740,40,63740,75,0,0,4,0]],"databaseIsUnmoved":[[60558,7,60558,26,0,0,4,0]],"memdbClose":[[52409,7,52409,16,0,0,4,1],[52433,7,52433,17,0,0,4,0],[52434,9,52434,51,0,0,4,0]],"memdbUnlock":[[52626,7,52626,26,0,0,4,0],[52626,7,52626,26,0,0,4,1],[52630,7,52630,32,0,0,4,1],[52635,9,52635,40,0,0,4,1]],"sqlite3_stmt_scanstatus_reset":[[91996,13,91996,17,0,0,4,0],[91996,21,91996,30,0,0,4,0],[91996,21,91996,30,0,0,4,1]],"sqlite3PagerFilename":[[63495,40,63495,68,0,0,4,0]],"attachFunc":[[119357,9,119357,16,0,0,4,1],[119359,9,119359,22,0,0,4,0],[119361,11,119361,21,0,0,4,0],[119374,9,119374,11,0,0,4,1],[119471,9,119471,25,0,0,4,1],[119346,31,119346,53,2,0,4,0],[14155,25,14155,46,5,0,4,0],[14155,26,14155,29,5,0,4,0]],"memdbOpen":[[52753,7,52753,15,0,0,4,1],[52802,9,52802,13,0,0,4,1],[52810,7,52810,19,0,0,4,0]],"sqlite3WalHeapMemory":[[68622,11,68622,15,0,0,4,1]],"sqlite3_deserialize":[[53043,7,53043,32,0,0,4,1],[53046,7,53046,13,0,0,4,1],[53047,7,53047,14,0,0,4,1],[53051,7,53051,17,0,0,4,1],[53054,7,53054,12,0,0,4,0],[53054,16,53054,22,0,0,4,1],[53059,7,53059,14,0,0,4,1],[53065,7,53065,9,0,0,4,1],[53070,7,53070,22,0,0,4,1],[53075,7,53075,11,0,0,4,1],[53084,9,53084,54,0,0,4,0],[53093,7,53093,12,0,0,4,1],[14098,27,14098,28,4,0,4,1]],"sqlite3_mutex_free":[[28645,7,28645,8,0,0,4,1]],"sqlite3PagerWalSupported":[[63957,61,63957,78,0,0,4,1]],"sqlite3_unlock_notify":[[182811,7,182811,32,0,0,4,1],[182816,7,182816,17,0,0,4,1],[182822,13,182822,39,0,0,4,0],[182845,32,182845,34,0,0,4,1]],"sqlite3_vfs_find":[[26014,9,26014,37,0,0,4,1]],"memdbFileControl":[[52665,7,52665,31,0,0,4,1],[52669,7,52669,34,0,0,4,1]],"sqlite3BtreeOpen":[[72502,11,72502,37,0,0,4,0],[72502,41,72502,55,0,0,4,1]],"sqlite3BtreeSchema":[[80975,7,80975,20,0,0,4,1]],"sqlite3PagerOpenWal":[[64044,9,64044,42,0,0,4,0]],"sqlite3_file_control":[[181655,9,181655,38,0,0,4,0]],"sqlite3_stmt_scanstatus_v2":[[91842,7,91842,11,0,0,4,1],[91842,15,91842,22,0,0,4,1],[91843,10,91843,45,0,0,4,1],[91844,10,91844,46,0,0,4,1],[91850,7,91850,16,0,0,4,1],[91857,7,91857,14,0,0,4,1],[91869,7,91869,38,0,0,4,1],[91875,16,91875,28,0,0,4,0],[91875,16,91875,28,0,0,4,1],[91877,11,91877,23,0,0,4,1],[91883,7,91883,20,0,0,4,0]],"sqlite3PagerOpen":[[61476,22,61476,27,0,0,4,0]],"sqlite3_bind_parameter_name":[[91357,7,91357,11,0,0,4,1]],"sqlite3_compileoption_used":[[182616,7,182616,18,0,0,4,1],[182624,7,182624,49,0,0,4,1],[182629,12,182629,18,0,0,4,0],[182629,12,182629,18,0,0,4,1],[182630,9,182630,57,0,0,4,1]],"memdbFromDbSchema":[[52935,7,52935,9,0,0,4,1],[52936,7,52936,42,0,0,4,1],[52939,7,52939,24,0,0,4,1]],"sqlite3VdbeExec":[[96133,15,96133,32,0,0,4,0],[96133,36,96133,52,0,0,4,1]],"memdbLock":[[52569,7,52569,26,0,0,4,1],[52576,7,52576,31,0,0,4,1],[52580,7,52580,30,0,0,4,0],[52582,13,52582,25,0,0,4,1],[52590,7,52590,32,0,0,4,1],[52591,7,52591,31,0,0,4,1],[52603,7,52603,14,0,0,4,1],[52615,7,52615,20,0,0,4,0]],"memdbRead":[[52457,7,52457,23,0,0,4,0],[52457,7,52457,23,0,0,4,1],[52459,9,52459,20,0,0,4,0],[52459,9,52459,20,0,0,4,1]]},"library_calls":["sqlite3_compileoption_used","sqlite3_open","sqlite3_close","sqlite3_close","sqlite3_deserialize","sqlite3_commit_hook","sqlite3_prepare_v3","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_parameter_name","sqlite3_stmt_scanstatus_reset","sqlite3_stmt_scanstatus_v2","sqlite3_unlock_notify","sqlite3_finalize","sqlite3_os_end","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_compileoption_used","sqlite3_open","sqlite3_deserialize","sqlite3_commit_hook","sqlite3_prepare_v3","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_parameter_name","sqlite3_stmt_scanstatus_reset","sqlite3_stmt_scanstatus_v2","sqlite3_unlock_notify","sqlite3_finalize","sqlite3_os_end","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to deserialize a database, check compile options, set up commit hooks, and perform various operations:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Check compile options
    const char *opt_name = "ENABLE_JSON1";
    int compile_option_used = sqlite3_compileoption_used(opt_name);
    (void)compile_option_used;  // Use the result
    
    // 2. Deserialize database from input data
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare buffer for deserialization
    unsigned char *db_data = (unsigned char*)malloc(size);
    if (!db_data) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(db_data, data, size);
    
    // Try to deserialize the data as a database
    const char *zSchema = "main";
    sqlite3_int64 szDb = (sqlite3_int64)size;
    sqlite3_int64 szBuf = (sqlite3_int64)size * 2;
    unsigned int mFlags = SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE;
    
    int deserialize_rc = sqlite3_deserialize(db, zSchema, db_data, szDb, szBuf, mFlags);
    // Don't free db_data here - sqlite3_deserialize takes ownership when SQLITE_DESERIALIZE_FREEONCLOSE is set
    
    // 3. Set commit hook
    struct CommitContext {
        int count;
    };
    CommitContext ctx = {0};
    
    auto commit_hook_callback = [](void *pCtx) -> int {
        CommitContext *ctx = static_cast<CommitContext*>(pCtx);
        ctx->count++;
        return 0;  // Return 0 to allow commit, non-zero to rollback
    };
    
    void *old_commit_hook = sqlite3_commit_hook(db, commit_hook_callback, &ctx);
    (void)old_commit_hook;  // Use the result
    
    // 4. Prepare a statement and test parameter binding
    sqlite3_stmt *pStmt = nullptr;
    const char *sql = "SELECT ?1, ?2, ?3";
    rc = sqlite3_prepare_v3(db, sql, -1, 0, &pStmt, nullptr);
    
    if (rc == SQLITE_OK && pStmt) {
        // Bind parameters
        sqlite3_bind_int(pStmt, 1, 42);
        sqlite3_bind_text(pStmt, 2, "test", -1, SQLITE_STATIC);
        
        // Get parameter name for parameter 1
        const char *param_name = sqlite3_bind_parameter_name(pStmt, 1);
        (void)param_name;  // Use the result
        
        // 5. Use virtual table APIs (create dummy structures)
        struct DummyIndexInfo {
            int nConstraint;
            int nOrderBy;
            unsigned int distinct;
        };
        
        DummyIndexInfo dummy_info = {0};
        // Note: We can't actually call sqlite3_vtab_distinct without a real index_info
        // This is just to demonstrate the pattern
        sqlite3_index_info* fake_info = nullptr;
        // We can't properly create sqlite3_index_info as it's an internal struct
        // So we'll skip actually calling sqlite3_vtab_distinct
        
        // 6. Statement scan status
        if (size > 100) {
            // Use some data to simulate scan status
            int idx = data[0] % 10;
            int iScanStatusOp = data[1] % 5;
            int flags = 0;
            int scanStatusOut;
            
            // Reset scan status first
            sqlite3_stmt_scanstatus_reset(pStmt);
            
            // Get scan status
            sqlite3_stmt_scanstatus_v2(pStmt, idx, iScanStatusOp, flags, &scanStatusOut);
        }
        
        // 7. Test unlock notify (simulate by calling with null args)
        auto notify_callback = [](void **apArg, int nArg) {
            // Do nothing in callback
        };
        
        int unlock_result = sqlite3_unlock_notify(db, notify_callback, nullptr);
        (void)unlock_result;  // Use the result
        
        sqlite3_finalize(pStmt);
    }
    
    // 8. Call os_end (cleanup OS-specific resources)
    int os_end_result = sqlite3_os_end();
    (void)os_end_result;  // Use the result
    
    // 9. Perform a transaction to trigger commit hook
    if (deserialize_rc == SQLITE_OK) {
        sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER)", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    }
    
    // 10. Cleanup
    sqlite3_close(db);
    
    return 0;
}