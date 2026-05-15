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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 340, nr_unique_branch: 71
//<Quality> {"density":17,"unique_branches":{"sqlite3MarkAllShadowTablesOf":[[122668,7,122668,14,0,0,4,1],[122670,7,122670,32,0,0,4,0],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1]],"growVTrans":[[154380,7,154380,34,0,0,4,0],[154385,9,154385,17,0,0,4,1]],"sqlite3StartTable":[[121418,7,121418,24,0,0,4,1]],"sqlite3VtabModuleUnref":[[153810,7,153810,26,0,0,4,1]],"sqlite3VtabDisconnect":[[153924,28,153924,35,0,0,4,0],[153925,9,153925,26,0,0,4,0]],"vtabCallConstructor":[[154223,26,154223,30,0,0,4,1],[154233,7,154233,19,0,0,4,1],[154238,7,154238,15,0,0,4,1],[154262,7,154262,23,0,0,4,1],[154265,7,154265,20,0,0,4,1],[154280,9,154280,26,0,0,4,1],[154296,19,154296,34,0,0,4,0],[154296,19,154296,34,0,0,4,1],[154301,18,154301,25,0,0,4,0],[154301,18,154301,25,0,0,4,1],[154302,15,154302,57,0,0,4,1],[154309,13,154309,20,0,0,4,1],[14155,25,14155,46,7,0,4,0],[14155,26,14155,29,7,0,4,0]],"callFinaliser":[[154594,7,154594,18,0,0,4,0],[154597,14,154597,27,0,0,4,0],[154597,14,154597,27,0,0,4,1],[154600,11,154600,12,0,0,4,0],[154603,13,154603,14,0,0,4,1]],"sqlite3VtabCallCreate":[[154430,7,154430,14,0,0,4,1],[154430,18,154430,43,0,0,4,1],[154430,47,154430,73,0,0,4,1],[154439,7,154439,20,0,0,4,0],[154441,9,154441,22,0,0,4,0],[14155,25,14155,46,4,0,4,0],[14155,26,14155,29,4,0,4,0]],"sqlite3GetVTable":[[153840,29,153840,34,0,0,4,0],[153840,38,153840,51,0,0,4,1]],"sqlite3RunParser":[[177037,28,177037,45,0,0,4,1]],"sqlite3_declare_vtab":[[154462,7,154462,32,0,0,4,1],[154462,36,154462,51,0,0,4,1],[154468,7,154468,12,0,0,4,1],[154468,16,154468,31,0,0,4,1],[154486,7,154486,57,0,0,4,0],[154492,9,154492,20,0,0,4,0],[154503,11,154503,20,0,0,4,1],[154512,11,154512,15,0,0,4,1],[154528,7,154528,19,0,0,4,0],[14155,25,14155,46,7,0,4,0],[14155,26,14155,29,7,0,4,0],[14155,25,14155,46,8,0,4,0],[14155,26,14155,29,8,0,4,0],[18323,29,18323,57,9,0,4,0]],"readSuperJournal":[[57747,7,57747,13,0,0,4,1],[57748,7,57748,58,0,0,4,1],[57749,7,57749,63,0,0,4,1],[57750,7,57750,39,0,0,4,0]],"sqlite3VtabUnlock":[[153857,7,153857,21,0,0,4,0],[153857,7,153857,21,0,0,4,1],[153859,9,153859,10,0,0,4,0]],"sqlite3VtabSync":[[154626,29,154626,42,0,0,4,0],[154629,9,154629,14,0,0,4,0],[154629,18,154629,48,0,0,4,1]],"sqlite3DeleteColumnNames":[[18323,29,18323,57,1,0,4,1],[18323,29,18323,57,2,0,4,1]],"sqlite3NestedParse":[[120445,7,120445,25,0,0,4,0]],"sqlite3VdbeExec":[[96129,9,96129,18,0,0,4,0],[100300,7,100300,9,0,0,4,1]],"sqlite3VdbeHalt":[[21301,34,21301,49,13,0,4,0],[21301,53,21301,69,13,0,4,1]],"disconnectAllVtab":[[18331,29,18331,57,3,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_shutdown","sqlite3_exec","sqlite3_exec","sqlite3_create_module_v2","sqlite3_exec","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_bind_text64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_total_changes64","sqlite3_wal_checkpoint_v2","sqlite3_exec","sqlite3_exec","sqlite3_snapshot_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_exec","sqlite3_exec","sqlite3_create_module_v2","sqlite3_exec","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_bind_text64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_total_changes64","sqlite3_wal_checkpoint_v2","sqlite3_exec","sqlite3_exec","sqlite3_snapshot_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2","sqlite3_shutdown"],"visited":0}
/**/


// Custom virtual table implementation to use vtab_distinct
struct dummy_vtab {
    sqlite3_vtab base;
};

struct dummy_cursor {
    sqlite3_vtab_cursor base;
    int rowid;
};

static int  id_000012_dummy_connect(sqlite3* db, void* pAux, int argc, const char* const* argv,
                         sqlite3_vtab** ppVtab, char** pzErr) {
    dummy_vtab* pNew = new dummy_vtab();
    *ppVtab = &pNew->base;
    
    const char* create = "CREATE TABLE x(id INTEGER)";
    int rc = sqlite3_declare_vtab(db, create);
    return rc;
}

static int  id_000012_dummy_bestindex(sqlite3_vtab* tab, sqlite3_index_info* pIdxInfo) {
    // Use vtab_distinct API
    int distinct = sqlite3_vtab_distinct(pIdxInfo);
    (void)distinct; // Use variable to avoid warning
    return SQLITE_OK;
}

static int  id_000012_dummy_disconnect(sqlite3_vtab* pVtab) {
    delete (dummy_vtab*)pVtab;
    return SQLITE_OK;
}

// Simple stubs for required virtual table methods
static int  id_000012_dummy_open(sqlite3_vtab* pVtab, sqlite3_vtab_cursor** ppCursor) {
    dummy_cursor* pCur = new dummy_cursor();
    pCur->rowid = 0;
    *ppCursor = &pCur->base;
    return SQLITE_OK;
}

static int  id_000012_dummy_close(sqlite3_vtab_cursor* cur) {
    delete (dummy_cursor*)cur;
    return SQLITE_OK;
}

static int  id_000012_dummy_filter(sqlite3_vtab_cursor* cur, int idxNum, const char* idxStr,
                        int argc, sqlite3_value** argv) {
    dummy_cursor* pCur = (dummy_cursor*)cur;
    pCur->rowid = 0;
    return SQLITE_OK;
}

static int  id_000012_dummy_next(sqlite3_vtab_cursor* cur) {
    dummy_cursor* pCur = (dummy_cursor*)cur;
    pCur->rowid++;
    return SQLITE_OK;
}

static int  id_000012_dummy_eof(sqlite3_vtab_cursor* cur) {
    dummy_cursor* pCur = (dummy_cursor*)cur;
    return pCur->rowid >= 1 ? 1 : 0;
}

static int  id_000012_dummy_column(sqlite3_vtab_cursor* cur, sqlite3_context* ctx, int i) {
    sqlite3_result_int(ctx, 42);
    return SQLITE_OK;
}

static int  id_000012_dummy_rowid(sqlite3_vtab_cursor* cur, sqlite3_int64* pRowid) {
    dummy_cursor* pCur = (dummy_cursor*)cur;
    *pRowid = pCur->rowid;
    return SQLITE_OK;
}

// Custom function to use result_zeroblob and value_text16
static void  id_000012_custom_function(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    if (argc >= 1) {
        // Use value_text16 API
        const void* text16 = sqlite3_value_text16(argv[0]);
        if (text16) {
            // Use result_zeroblob API
            sqlite3_result_zeroblob(ctx, 100);
        } else {
            sqlite3_result_null(ctx);
        }
    } else {
        sqlite3_result_null(ctx);
    }
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<273) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_snapshot* snapshot = nullptr;
    int rc = SQLITE_OK;
    
    // Open in-memory database
    rc = sqlite3_open_v2(fuzz_str_1, &db, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                        nullptr);
    if (rc != SQLITE_OK || !db) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Enable WAL mode for checkpointing
    sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);
    
    // Create a table
    const char* create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data TEXT);";
    rc = sqlite3_exec(db, fuzz_str_3, nullptr, nullptr, nullptr);
    
    // Register custom virtual table module
    static sqlite3_module dummy_module = {
        0,                          // iVersion
         id_000012_dummy_connect,              // xCreate/xConnect
         id_000012_dummy_connect,              // xConnect
         id_000012_dummy_bestindex,            // xBestIndex
         id_000012_dummy_disconnect,           // xDisconnect
         id_000012_dummy_disconnect,           // xDestroy
         id_000012_dummy_open,                 // xOpen
         id_000012_dummy_close,                // xClose
         id_000012_dummy_filter,               // xFilter
         id_000012_dummy_next,                 // xNext
         id_000012_dummy_eof,                  // xEof
         id_000012_dummy_column,               // xColumn
         id_000012_dummy_rowid,                // xRowid
        nullptr,                    // xUpdate
        nullptr,                    // xBegin
        nullptr,                    // xSync
        nullptr,                    // xCommit
        nullptr,                    // xRollback
        nullptr,                    // xFindFunction
        nullptr,                    // xRename
        nullptr,                    // xSavepoint
        nullptr,                    // xRelease
        nullptr                     // xRollbackTo
    };
    
    rc = sqlite3_create_module_v2(db, fuzz_str_4, &dummy_module, nullptr, nullptr);
    
    // Create virtual table
    if (rc == SQLITE_OK) {
        sqlite3_exec(db, fuzz_str_5, 
                    nullptr, nullptr, nullptr);
    }
    
    // Register custom function
    rc = sqlite3_create_function_v2(db, fuzz_str_6, fuzz_int32_t_7, fuzz_int32_t_8, nullptr,
                                    id_000012_custom_function, nullptr, nullptr, nullptr);
    
    // Prepare insert statement
    const char* insert_sql = "INSERT INTO test (id, data) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind parameters using input data
        if (size > 0) {
            // Use bind_text64 API
            const char* text_data = reinterpret_cast<const char*>(data);
            size_t text_len = size > 1000 ? 1000 : size; // Limit size
            rc = sqlite3_bind_text64(stmt, fuzz_int32_t_9, text_data, text_len, 
                                    SQLITE_TRANSIENT, SQLITE_UTF8);
            
            // Bind an integer
            int id_value = size % 100;
            sqlite3_bind_int(stmt, fuzz_int32_t_10, id_value);
            
            // Execute statement
            rc = sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
    }
    
    // Use total_changes64 API
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    (void)total_changes; // Use variable to avoid warning
    
    // Perform WAL checkpoint
    rc = sqlite3_wal_checkpoint_v2(db, fuzz_str_11, fuzz_int32_t_12, nullptr, nullptr);
    
    // Try to open a snapshot (requires write transaction)
    rc = sqlite3_exec(db, fuzz_str_13, nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK) {
        // Insert more data
        sqlite3_exec(db, fuzz_str_14, nullptr, nullptr, nullptr);
        
        // Open snapshot
        rc = sqlite3_snapshot_open(db, fuzz_str_15, snapshot);
        
        // Rollback transaction
        sqlite3_exec(db, fuzz_str_16, nullptr, nullptr, nullptr);
    }
    
    // Use the custom function
    if (size > 0) {
        const char* select_sql = "SELECT custom_func(?);";
        sqlite3_stmt* func_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, select_sql, -1, &func_stmt, nullptr);
        
        if (rc == SQLITE_OK && func_stmt) {
            // Bind input data as parameter
            const char* input_text = reinterpret_cast<const char*>(data);
            size_t input_len = size > 500 ? 500 : size;
            sqlite3_bind_text(func_stmt, fuzz_int32_t_17, input_text, input_len, SQLITE_TRANSIENT);
            
            // Execute function
            if (sqlite3_step(func_stmt) == SQLITE_ROW) {
                // Function executed successfully
            }
            sqlite3_finalize(func_stmt);
        }
    }
    
    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close_v2(db);
    }
    
    sqlite3_shutdown();
    return 0;
}