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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 390, nr_unique_branch: 277
//<Quality> {"density":26,"unique_branches":{"sqlite3VtabSync":[[154626,29,154626,42,0,0,4,0],[154629,9,154629,14,0,0,4,0],[154629,18,154629,48,0,0,4,1]],"resolveP2Values":[[84895,9,84895,24,0,0,4,0],[84900,15,84900,25,0,0,4,0]],"sqlite3VdbeExec":[[93311,7,93311,28,0,0,4,1],[93655,1,93655,20,0,0,4,0],[98301,1,98301,16,0,0,4,0],[98306,7,98306,12,0,0,4,1],[98318,7,98318,34,0,0,4,0],[98323,7,98323,20,0,0,4,0],[100300,7,100300,9,0,0,4,1],[100328,1,100328,14,0,0,4,0],[100338,7,100338,15,0,0,4,1],[100345,7,100345,9,0,0,4,1],[100352,7,100352,11,0,0,4,0],[100459,1,100459,16,0,0,4,0],[100490,14,100490,20,0,0,4,0],[100490,14,100490,20,0,0,4,1],[100495,7,100495,9,0,0,4,1],[100499,7,100499,10,0,0,4,0],[14155,25,14155,46,39,0,4,0],[14155,26,14155,29,39,0,4,0],[14156,25,14156,46,802,0,4,1],[14156,26,14156,29,802,0,4,1],[92825,34,92825,60,810,0,4,1]],"sqlite3VtabDisconnect":[[153924,28,153924,35,0,0,4,0],[153925,9,153925,26,0,0,4,0]],"sqlite3VtabCallCreate":[[154430,18,154430,43,0,0,4,1],[154430,47,154430,73,0,0,4,1],[154439,7,154439,20,0,0,4,0],[154441,9,154441,22,0,0,4,0],[14155,25,14155,46,4,0,4,0],[14155,26,14155,29,4,0,4,0]],"sqlite3GetVTable":[[153840,29,153840,34,0,0,4,0],[153840,38,153840,51,0,0,4,1]],"cellSizePtrIdxLeaf":[[71214,9,71214,16,0,0,4,0]],"sqlite3FindInIndex":[[111103,7,111103,35,0,0,4,1],[111112,7,111112,15,0,0,4,0],[111119,9,111119,32,0,0,4,0],[111126,9,111126,21,0,0,4,1],[111132,7,111132,12,0,0,4,1]],"sqlite3ExprCodeGetColumnOfTable":[[18331,29,18331,57,3,0,4,0]],"sqlite3NestedParse":[[120445,7,120445,25,0,0,4,0]],"sqlite3StartTable":[[121418,7,121418,24,0,0,4,1]],"freeIndexInfo":[[161769,12,161769,35,0,0,4,0],[161769,12,161769,35,0,0,4,1]],"sqlite3_table_column_metadata":[[181571,15,181571,32,0,0,4,0],[181571,36,181571,74,0,0,4,1]],"whereShortCut":[[18331,29,18331,57,2,0,4,0]],"disconnectAllVtab":[[18331,29,18331,57,3,0,4,0]],"sqlite3VdbeChangeP4":[[85545,9,85545,19,0,0,4,0]],"freeP4":[[85372,5,85372,18,0,0,4,0],[85373,11,85373,30,0,0,4,0]],"sqlite3WhereMalloc":[[160571,7,160571,13,0,0,4,0]],"sqlite3VtabImportErrmsg":[[89356,7,89356,21,0,0,4,1]],"sqlite3WhereEnd":[[167022,9,167022,44,0,0,4,0],[167022,48,167022,66,0,0,4,0],[167026,63,167026,66,0,0,4,0],[167026,63,167026,66,0,0,4,1],[167030,13,167030,37,0,0,4,0],[167031,15,167031,27,0,0,4,1]],"viewGetColumnNames":[[18331,29,18331,57,1,0,4,0]],"sqlite3WhereCodeOneLoopStart":[[157085,8,157085,48,0,0,4,0],[157095,14,157095,27,0,0,4,0],[157095,14,157095,27,0,0,4,1],[157099,11,157099,35,0,0,4,0],[157100,13,157100,52,0,0,4,1],[157127,23,157127,45,0,0,4,1],[157132,9,157132,25,0,0,4,1],[157134,18,157134,34,0,0,4,1],[157138,14,157138,27,0,0,4,0],[157138,14,157138,27,0,0,4,1],[157140,11,157140,15,0,0,4,0],[157140,19,157140,48,0,0,4,0],[14156,25,14156,46,9,0,4,1],[14156,26,14156,29,9,0,4,1],[15284,24,15284,31,11,0,4,0]],"sqlite3CodeRhsOfIN":[[111261,7,111261,23,0,0,4,0],[111261,48,111261,67,0,0,4,0],[111367,9,111367,34,0,0,4,1],[111369,15,111369,40,0,0,4,1],[111372,9,111372,17,0,0,4,0],[111380,41,111380,44,0,0,4,0],[111380,41,111380,44,0,0,4,1],[111388,11,111388,19,0,0,4,0],[111388,23,111388,50,0,0,4,1],[111403,7,111403,15,0,0,4,0],[111406,7,111406,15,0,0,4,0],[18919,34,18919,55,2,0,4,1],[18935,29,18935,59,12,0,4,1],[18935,29,18935,59,15,0,4,1],[14155,25,14155,46,21,0,4,0],[14155,26,14155,29,21,0,4,0]],"sqlite3LocateTable":[[120605,29,120605,76,0,0,4,1],[18331,29,18331,57,6,0,4,0]],"sqlite3WhereExplainOneScan":[[155792,17,155792,46,0,0,4,1],[155823,15,155823,37,0,0,4,1],[155847,14,155847,45,0,0,4,0]],"sqlite3VtabUnlock":[[153857,7,153857,21,0,0,4,0],[153857,7,153857,21,0,0,4,1],[153859,9,153859,10,0,0,4,0]],"callFinaliser":[[154594,7,154594,18,0,0,4,0],[154597,14,154597,27,0,0,4,0],[154597,14,154597,27,0,0,4,1],[154600,11,154600,12,0,0,4,0],[154603,13,154603,14,0,0,4,1]],"btreeParseCellPtrIndex":[[71116,9,71116,23,0,0,4,0]],"vtabBestIndex":[[161802,7,161802,20,0,0,4,1],[161811,7,161811,34,0,0,4,1]],"sqlite3WalkSelect":[[105680,7,105680,11,0,0,4,0]],"fillInCell":[[76746,9,76746,12,0,0,4,0],[14098,27,14098,28,4,0,4,0]],"allocateIndexInfo":[[161596,32,161596,44,0,0,4,0],[161596,32,161596,44,0,0,4,1],[161598,9,161598,43,0,0,4,1],[161599,9,161599,39,0,0,4,1],[161605,9,161605,44,0,0,4,1],[161606,9,161606,36,0,0,4,1],[161611,9,161611,61,0,0,4,1],[161625,7,161625,15,0,0,4,1],[161681,7,161681,18,0,0,4,1],[161696,28,161696,40,0,0,4,0],[161696,28,161696,40,0,0,4,1],[161698,9,161698,38,0,0,4,1],[161702,9,161702,18,0,0,4,0],[161703,11,161703,43,0,0,4,0],[161708,9,161708,19,0,0,4,1],[161710,15,161710,37,0,0,4,1],[161728,11,161728,41,0,0,4,1],[161742,14,161742,24,0,0,4,1],[14098,27,14098,28,2,0,4,0],[14098,27,14098,28,3,0,4,1],[14098,27,14098,28,4,0,4,1],[14098,27,14098,28,5,0,4,0],[15284,24,15284,31,23,0,4,0]],"sqlite3VdbeHalt":[[21301,34,21301,49,13,0,4,0],[21301,53,21301,69,13,0,4,1]],"codeEqualityTerm":[[156248,7,156248,20,0,0,4,1],[156248,24,156248,37,0,0,4,1],[156250,13,156250,30,0,0,4,1],[156263,9,156263,49,0,0,4,1],[156274,14,156274,19,0,0,4,1],[156280,15,156280,30,0,0,4,0],[156280,15,156280,30,0,0,4,1],[156282,11,156282,38,0,0,4,0],[156286,9,156286,24,0,0,4,0],[156307,9,156307,35,0,0,4,1],[156311,26,156311,30,0,0,4,1],[156317,9,156317,28,0,0,4,0],[156320,9,156320,14,0,0,4,1],[156331,9,156331,12,0,0,4,0],[156334,17,156334,32,0,0,4,0],[156334,17,156334,32,0,0,4,1],[156335,13,156335,40,0,0,4,0],[156337,15,156337,36,0,0,4,1],[156340,24,156340,29,0,0,4,1],[156344,15,156344,21,0,0,4,0],[156346,31,156346,35,0,0,4,1],[156347,17,156347,22,0,0,4,1],[156362,11,156362,16,0,0,4,1],[14098,27,14098,28,29,0,4,1]],"whereLoopClearUnion":[[162612,9,162612,45,0,0,4,0],[162612,49,162612,67,0,0,4,1],[162616,15,162616,49,0,0,4,1]],"whereLoopAddVirtual":[[164614,7,164614,11,0,0,4,1],[164620,7,164620,53,0,0,4,1],[164631,7,164631,13,0,0,4,1],[164643,7,164643,20,0,0,4,0],[164643,25,164643,63,0,0,4,1],[164643,67,164643,70,0,0,4,0],[164651,9,164651,12,0,0,4,0],[164657,11,164657,23,0,0,4,0],[164665,12,164665,25,0,0,4,0],[164669,16,164669,29,0,0,4,0],[164669,16,164669,29,0,0,4,1],[164673,13,164673,24,0,0,4,1],[164676,11,164676,25,0,0,4,0],[164691,9,164691,22,0,0,4,0],[164691,26,164691,37,0,0,4,1],[164701,9,164701,22,0,0,4,0],[164701,26,164701,41,0,0,4,1],[164708,7,164708,26,0,0,4,1]],"whereInfoFree":[[162697,10,162697,28,0,0,4,0]],"sqlite3WhereAddScanStatus":[[155946,9,155946,42,0,0,4,1]],"sqlite3WhereRealloc":[[160581,7,160581,11,0,0,4,0],[160581,15,160581,19,0,0,4,1]],"sqlite3_stmt_scanstatus_v2":[[91842,15,91842,22,0,0,4,0]],"sqlite3_str_vappendf":[[31446,13,31446,21,0,0,4,0]],"sqlite3LogEstFromDouble":[[36157,7,36157,11,0,0,4,1],[36158,7,36158,20,0,0,4,1]],"whereLoopInsert":[[162990,7,162990,43,0,0,4,1]],"sqlite3RunParser":[[177037,28,177037,45,0,0,4,1]],"sqlite3DeleteColumnNames":[[18323,29,18323,57,1,0,4,1],[18323,29,18323,57,2,0,4,1]],"sqlite3VtabModuleUnref":[[153810,7,153810,26,0,0,4,1]],"sqlite3_declare_vtab":[[154462,7,154462,32,0,0,4,1],[154462,36,154462,51,0,0,4,1],[154468,7,154468,12,0,0,4,1],[154468,16,154468,31,0,0,4,1],[154486,7,154486,57,0,0,4,0],[154492,9,154492,20,0,0,4,0],[154503,11,154503,20,0,0,4,1],[154512,11,154512,15,0,0,4,1],[154528,7,154528,19,0,0,4,0],[14155,25,14155,46,7,0,4,0],[14155,26,14155,29,7,0,4,0],[14155,25,14155,46,8,0,4,0],[14155,26,14155,29,8,0,4,0],[18323,29,18323,57,9,0,4,0]],"selectExpander":[[147306,11,147306,21,0,0,4,1],[147310,11,147310,27,0,0,4,0],[147313,13,147313,52,0,0,4,1],[147326,13,147326,30,0,0,4,1],[18322,29,18322,57,16,0,4,1],[14155,25,14155,46,18,0,4,0],[14155,26,14155,29,18,0,4,0]],"whereLoopXfer":[[162668,7,162668,42,0,0,4,0]],"sqlite3VtabCallConnect":[[154348,7,154348,33,0,0,4,0]],"disableTerm":[[156020,10,156020,42,0,0,4,1]],"whereLoopAddAll":[[164898,24,164898,30,0,0,4,1],[18331,29,18331,57,9,0,4,0]],"isLimitTerm":[[164245,10,164245,56,0,0,4,1]],"growVTrans":[[154380,7,154380,34,0,0,4,0],[154385,9,154385,17,0,0,4,1]],"whereLoopAddVirtualOne":[[164298,12,164298,25,0,0,4,0],[164298,12,164298,25,0,0,4,1],[164301,9,164301,59,0,0,4,0],[164302,9,164302,41,0,0,4,0],[164302,9,164302,41,0,0,4,1],[164303,10,164303,22,0,0,4,0],[164323,7,164323,9,0,0,4,1],[164340,12,164340,25,0,0,4,0],[164340,12,164340,25,0,0,4,1],[164342,9,164342,45,0,0,4,0],[164342,9,164342,45,0,0,4,1],[164345,11,164345,29,0,0,4,1],[164346,11,164346,14,0,0,4,1],[164347,11,164347,24,0,0,4,1],[164348,11,164348,33,0,0,4,1],[164349,11,164349,30,0,0,4,1],[164362,11,164362,23,0,0,4,0],[164365,11,164365,25,0,0,4,0],[164366,13,164366,17,0,0,4,0],[164366,21,164366,40,0,0,4,0],[164372,13,164372,60,0,0,4,1],[164376,11,164376,45,0,0,4,1],[164378,17,164378,46,0,0,4,0],[164390,11,164390,29,0,0,4,1],[164408,12,164408,21,0,0,4,0],[164408,12,164408,21,0,0,4,1],[164409,9,164409,27,0,0,4,1],[164422,33,164422,58,0,0,4,1],[164430,7,164430,52,0,0,4,1],[164436,7,164436,28,0,0,4,1],[14098,27,14098,28,7,0,4,0],[14098,27,14098,28,8,0,4,0],[14098,27,14098,28,9,0,4,0],[14098,27,14098,28,10,0,4,1],[14098,27,14098,28,11,0,4,1],[14098,27,14098,28,12,0,4,1],[15284,24,15284,31,15,0,4,0]],"sqlite3VdbeFreeCursorNN":[[86714,5,86714,22,0,0,4,0]],"vtabCallConstructor":[[154223,26,154223,30,0,0,4,1],[154233,7,154233,19,0,0,4,1],[154238,7,154238,15,0,0,4,1],[154262,7,154262,23,0,0,4,1],[154265,7,154265,20,0,0,4,1],[154280,9,154280,26,0,0,4,1],[154296,19,154296,34,0,0,4,0],[154296,19,154296,34,0,0,4,1],[154301,18,154301,25,0,0,4,1],[154309,13,154309,20,0,0,4,1],[14155,25,14155,46,7,0,4,0],[14155,26,14155,29,7,0,4,0]],"sqlite3WhereBegin":[[166691,9,166691,49,0,0,4,0]],"sqlite3ViewGetColumnNames":[[123329,7,123329,17,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_close","sqlite3_create_module","sqlite3_close","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_table_column_metadata","sqlite3_prepare_v2","sqlite3_stmt_busy","sqlite3_step","sqlite3_stmt_scanstatus","sqlite3_reset","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_create_module","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_table_column_metadata","sqlite3_prepare_v2","sqlite3_stmt_busy","sqlite3_step","sqlite3_stmt_scanstatus","sqlite3_reset","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":3}
/**/


// Custom virtual table implementation to use sqlite3_vtab_in_first
struct DummyVTab {
    sqlite3_vtab base;
};

struct DummyCursor {
    sqlite3_vtab_cursor base;
    sqlite3_value* currentValue;
    int hasData;
};

static int  id_000008_dummyCreate(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    DummyVTab* pNew = (DummyVTab*)sqlite3_malloc(sizeof(DummyVTab));
    if (!pNew) return SQLITE_NOMEM;
    memset(pNew, 0, sizeof(DummyVTab));
    
    const char* createSQL = "CREATE TABLE dummy(x)";
    int rc = sqlite3_declare_vtab(db, createSQL);
    if (rc != SQLITE_OK) {
        sqlite3_free(pNew);
        return rc;
    }
    
    *ppVtab = &pNew->base;
    return SQLITE_OK;
}

static int  id_000008_dummyConnect(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    return  id_000008_dummyCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int  id_000008_dummyBestIndex(sqlite3_vtab* tab, sqlite3_index_info* pIdxInfo) {
    // Mark that we can handle IN constraints
    for (int i = 0; i < pIdxInfo->nConstraint; i++) {
        if (pIdxInfo->aConstraint[i].usable && pIdxInfo->aConstraint[i].op == SQLITE_INDEX_CONSTRAINT_EQ) {
            pIdxInfo->aConstraintUsage[i].argvIndex = 1;
            pIdxInfo->aConstraintUsage[i].omit = 1;
        }
    }
    return SQLITE_OK;
}

static int  id_000008_dummyOpen(sqlite3_vtab* pVtab, sqlite3_vtab_cursor** ppCursor) {
    DummyCursor* pCur = (DummyCursor*)sqlite3_malloc(sizeof(DummyCursor));
    if (!pCur) return SQLITE_NOMEM;
    memset(pCur, 0, sizeof(DummyCursor));
    pCur->currentValue = nullptr;
    pCur->hasData = 0;
    *ppCursor = &pCur->base;
    return SQLITE_OK;
}

static int  id_000008_dummyClose(sqlite3_vtab_cursor* cur) {
    DummyCursor* pCur = (DummyCursor*)cur;
    if (pCur->currentValue) {
        sqlite3_value_free(pCur->currentValue);
    }
    sqlite3_free(pCur);
    return SQLITE_OK;
}

static int  id_000008_dummyFilter(sqlite3_vtab_cursor* pVtabCursor, int idxNum, const char* idxStr, int argc, sqlite3_value** argv) {
    DummyCursor* pCur = (DummyCursor*)pVtabCursor;
    pCur->hasData = 0;
    
    // Try to use sqlite3_vtab_in_first if we have an IN constraint
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        // Try to get first value from IN list
        sqlite3_value* outVal = nullptr;
        int rc = sqlite3_vtab_in_first(argv[0], &outVal);
        if (rc == SQLITE_OK && outVal) {
            pCur->currentValue = outVal;
            pCur->hasData = 1;
        }
    }
    
    return SQLITE_OK;
}

static int  id_000008_dummyNext(sqlite3_vtab_cursor* cur) {
    DummyCursor* pCur = (DummyCursor*)cur;
    pCur->hasData = 0;
    return SQLITE_OK;
}

static int  id_000008_dummyEof(sqlite3_vtab_cursor* cur) {
    DummyCursor* pCur = (DummyCursor*)cur;
    return !pCur->hasData;
}

static int  id_000008_dummyColumn(sqlite3_vtab_cursor* cur, sqlite3_context* ctx, int i) {
    DummyCursor* pCur = (DummyCursor*)cur;
    if (pCur->currentValue) {
        sqlite3_result_value(ctx, pCur->currentValue);
    } else {
        sqlite3_result_null(ctx);
    }
    return SQLITE_OK;
}

static int  id_000008_dummyRowid(sqlite3_vtab_cursor* cur, sqlite3_int64* pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int  id_000008_dummyDestroy(sqlite3_vtab* pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int  id_000008_dummyDisconnect(sqlite3_vtab* pVtab) {
    return  id_000008_dummyDestroy(pVtab);
}

static sqlite3_module  id_000008_dummyModule = {
    0,              // iVersion
     id_000008_dummyCreate,    // xCreate
     id_000008_dummyConnect,   // xConnect
     id_000008_dummyBestIndex, // xBestIndex
     id_000008_dummyDestroy,   // xDestroy
     id_000008_dummyDisconnect,// xDisconnect
     id_000008_dummyOpen,      // xOpen
     id_000008_dummyClose,     // xClose
     id_000008_dummyFilter,    // xFilter
     id_000008_dummyNext,      // xNext
     id_000008_dummyEof,       // xEof
     id_000008_dummyColumn,    // xColumn
     id_000008_dummyRowid,     // xRowid
    nullptr,        // xUpdate
    nullptr,        // xBegin
    nullptr,        // xSync
    nullptr,        // xCommit
    nullptr,        // xRollback
    nullptr,        // xFindFunction
    nullptr,        // xRename
    nullptr,        // xSavepoint
    nullptr,        // xRelease
    nullptr         // xRollbackTo
};

// Custom function to test sqlite3_result_error and sqlite3_result_null
static void  id_000008_testFunction(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        int type = sqlite3_value_type(argv[0]);
        if (type == SQLITE_NULL) {
            sqlite3_result_null(ctx);
        } else {
            sqlite3_result_error(ctx, "Test error from custom function", -1);
        }
    } else {
        sqlite3_result_null(ctx);
    }
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<354) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_18, fuzz_str_sz_18, fdp);
	//fuzzer shim end}




    // Open input file from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc = 0;
    
    // Open in-memory database
    rc = sqlite3_open_v2(fuzz_str_1, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create custom function
    rc = sqlite3_create_function(db, fuzz_str_2, fuzz_int32_t_3, fuzz_int32_t_4, nullptr,  id_000008_testFunction, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create virtual table module
    rc = sqlite3_create_module(db, fuzz_str_5, & id_000008_dummyModule, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a test table using data from input
    const char* createTableSQL = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value INTEGER)";
    rc = sqlite3_exec(db, fuzz_str_6, nullptr, nullptr, nullptr);
    
    // Insert some data using prepared statement
    const char* insertSQL = "INSERT INTO test_table (name, value) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, fuzz_str_7, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Use first few bytes for name
        size_t name_len = size > 0 ? (data[0] % 50 + 1) : 10;
        if (name_len > size) name_len = size > 0 ? size : 1;
        
        char* name_buf = (char*)malloc(name_len + 1);
        if (name_buf) {
            memcpy(name_buf, data, name_len);
            name_buf[name_len] = '\0';
            
            sqlite3_bind_text(stmt, fuzz_int32_t_8, name_buf, -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, fuzz_int32_t_9, size);
            
            rc = sqlite3_step(stmt);
            sqlite3_reset(stmt);
            free(name_buf);
        }
    }
    
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Test sqlite3_table_column_metadata
    const char* dataType = nullptr;
    const char* collSeq = nullptr;
    int notNull = 0, primaryKey = 0, autoinc = 0;
    
    sqlite3_table_column_metadata(db, fuzz_str_10, fuzz_str_11, fuzz_str_12, 
                                  &dataType, &collSeq, &notNull, &primaryKey, &autoinc);
    
    // Create another statement to test busy and scanstatus
    const char* selectSQL = "SELECT * FROM test_table";
    rc = sqlite3_prepare_v2(db, fuzz_str_13, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Check if statement is busy (should be 0 before execution)
        int busy = sqlite3_stmt_busy(stmt);
        
        // Execute statement once
        rc = sqlite3_step(stmt);
        
        // Check scanstatus (requires SQLITE_ENABLE_STMT_SCANSTATUS)
        int scanStatus = 0;
        void* pOut = nullptr;
        sqlite3_stmt_scanstatus(stmt, fuzz_int32_t_14, fuzz_int32_t_15, pOut);
        
        sqlite3_reset(stmt);
    }
    
    // Test virtual table with IN constraint
    rc = sqlite3_exec(db, fuzz_str_16, nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK) {
        // Try to query with IN constraint
        const char* vtabSQL = "SELECT * FROM dummy_vtab WHERE x IN (1,2,3)";
        sqlite3_stmt* vtab_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, fuzz_str_17, -1, &vtab_stmt, nullptr);
        if (rc == SQLITE_OK && vtab_stmt) {
            rc = sqlite3_step(vtab_stmt);
            sqlite3_finalize(vtab_stmt);
        }
    }
    
    // Test custom function
    const char* funcSQL = "SELECT test_func(NULL)";
    sqlite3_stmt* func_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, fuzz_str_18, -1, &func_stmt, nullptr);
    if (rc == SQLITE_OK && func_stmt) {
        rc = sqlite3_step(func_stmt);
        sqlite3_finalize(func_stmt);
    }
    
    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}