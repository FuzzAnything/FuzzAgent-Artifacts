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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 488, nr_unique_branch: 139
//<Quality> {"density":16,"unique_branches":{"sqlite3PcacheFetchStress":[[53995,7,53995,53,0,0,4,1]],"sqlite3VdbeMemZeroTerminateIfAble":[[82271,7,82271,71,0,0,4,0]],"sqlite3GenerateIndexKey":[[127374,9,127374,47,0,0,4,1]],"checkList":[[80341,9,80341,19,0,0,4,0],[80344,11,80344,34,0,0,4,1],[80348,11,80348,40,0,0,4,1],[80353,18,80353,26,0,0,4,0],[80353,18,80353,26,0,0,4,1],[80356,15,80356,38,0,0,4,1]],"sqlite3Utf8Read":[[34072,12,34072,34,0,0,4,0]],"freePage2":[[76475,7,76475,15,0,0,4,1],[76558,13,76558,18,0,0,4,1],[76574,7,76574,15,0,0,4,0],[76574,19,76574,72,0,0,4,1],[76587,7,76587,12,0,0,4,1]],"sqlite3InitOne":[[140523,11,140523,28,0,0,4,0],[140523,32,140523,45,0,0,4,1]],"sqlite3VdbeExec":[[94945,13,94945,25,0,0,4,1],[95355,15,95355,35,0,0,4,0],[95359,9,95359,39,0,0,4,0],[95908,7,95908,14,0,0,4,0],[99141,9,99141,25,0,0,4,1],[14098,27,14098,28,290,0,4,0]],"editPage":[[14676,20,14676,27,5,0,4,0]],"sqlite3VdbeAssertMayAbort":[[84777,11,84777,27,0,0,4,0]],"sqlite3PagerLookup":[[62171,7,62171,15,0,0,4,0]],"pcache1Rekey":[[55612,10,55612,22,0,0,4,0]],"removeElementGivenHash":[[36576,7,36576,13,0,0,4,0],[36578,9,36578,28,0,0,4,0]],"sqlite3PcacheMakeDirty":[[54139,9,54139,31,0,0,4,1]],"sqlite3PagerSetJournalMode":[[63834,13,63834,26,0,0,4,1],[63837,13,63837,26,0,0,4,1]],"sqlite3PCachePercentDirty":[[54452,30,54452,36,0,0,4,0],[54452,30,54452,36,0,0,4,1],[54453,10,54453,16,0,0,4,0]],"sqlite3Pragma":[[18627,31,18627,54,255,0,4,1]],"sqlite3PagerCommitPhaseOne":[[63069,11,63069,44,0,0,4,0],[63073,13,63073,26,0,0,4,1]],"unixRead":[[41234,9,41234,38,0,0,4,1]],"clearDatabasePage":[[14098,27,14098,28,8,0,4,0]],"decodeIntArray":[[118788,9,118788,16,0,0,4,0]],"pagerFlushOnCommit":[[58409,7,58409,14,0,0,4,1]],"readJournalHdr":[[58041,16,58041,43,0,0,4,0],[58046,9,58046,57,0,0,4,0]],"statGet":[[118120,9,118120,22,0,0,4,1],[118121,14,118121,26,0,0,4,0],[118121,14,118121,26,0,0,4,1],[118124,11,118124,18,0,0,4,1]],"strHash":[[36437,10,36437,38,0,0,4,0],[36437,10,36437,38,0,0,4,1]],"statPush":[[117973,7,117973,17,0,0,4,0],[117973,7,117973,17,0,0,4,1],[117975,14,117975,23,0,0,4,0],[117975,14,117975,23,0,0,4,1],[117984,14,117984,21,0,0,4,0],[117984,14,117984,21,0,0,4,1],[117987,18,117987,27,0,0,4,0],[117987,18,117987,27,0,0,4,1],[118026,7,118026,16,0,0,4,1]],"sqlite3PagerDontWrite":[[14098,27,14098,28,4,0,4,1]],"sqlite3PagerSync":[[62810,24,62810,39,0,0,4,1]],"statInit":[[117693,7,117693,11,0,0,4,1]],"pcache1Unpin":[[55576,24,55576,59,0,0,4,0]],"rehash":[[36490,7,36490,59,0,0,4,1],[36493,7,36493,27,0,0,4,1],[36508,7,36508,16,0,0,4,1],[36513,36,36513,40,0,0,4,0],[36513,36,36513,40,0,0,4,1]],"insertElement":[[36457,7,36457,13,0,0,4,0],[36458,13,36458,26,0,0,4,0],[36458,13,36458,26,0,0,4,1],[36464,7,36464,12,0,0,4,0],[36467,9,36467,20,0,0,4,0],[36467,9,36467,20,0,0,4,1]],"sqlite3RefillIndex":[[123914,7,123914,21,0,0,4,1],[123939,7,123939,20,0,0,4,0],[123942,42,123942,58,0,0,4,1],[18627,31,18627,54,14,0,4,0]],"pcache1TruncateUnsafe":[[14109,23,14109,24,4,0,4,1]],"sqlite3BtreeRowCountEst":[[75931,12,75931,25,0,0,4,0],[75931,12,75931,25,0,0,4,1],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1]],"xferOptimization":[[134620,11,134620,49,0,0,4,1],[134751,11,134751,49,0,0,4,1]],"pager_truncate":[[59094,11,59094,30,0,0,4,1],[59096,17,59096,46,0,0,4,0],[14098,27,14098,28,6,0,4,0],[14098,27,14098,28,6,0,4,1],[14098,27,14098,28,7,0,4,0],[14098,27,14098,28,7,0,4,1]],"pager_incr_changecounter":[[62748,7,62748,31,0,0,4,1]],"sqlite3NoTempsInRange":[[114923,7,114923,26,0,0,4,0],[114924,7,114924,51,0,0,4,0],[114925,7,114925,33,0,0,4,1]],"findElementWithHash":[[36536,7,36536,13,0,0,4,0]],"btreePageLookup":[[72089,7,72089,14,0,0,4,1]],"pcache1FetchStage2":[[55369,9,55369,34,0,0,4,0]],"sqlite3BtreeTransferRow":[[79349,20,79349,46,0,0,4,1],[79369,9,79369,17,0,0,4,1]],"sqlite3ValueBytes":[[83959,34,83959,50,0,0,4,1]],"reindexTable":[[125573,30,125573,36,0,0,4,0],[125574,11,125574,19,0,0,4,0]],"xferCompatibleIndex":[[134386,7,134386,36,0,0,4,0]],"pagerAcquireMapPage":[[60487,7,60487,28,0,0,4,0]],"sqlite3BtreeDelete":[[79505,9,79506,78,0,0,4,1],[79507,9,79507,24,0,0,4,1],[79564,9,79564,23,0,0,4,0],[79566,11,79566,13,0,0,4,1],[79622,9,79622,20,0,0,4,0],[79627,11,79627,33,0,0,4,0],[79627,11,79627,33,0,0,4,1]],"btreeGetHasContent":[[70416,45,70416,78,0,0,4,1]],"analysisLoader":[[118845,7,118845,17,0,0,4,1],[118847,13,118847,48,0,0,4,1],[118854,7,118854,13,0,0,4,0],[118870,9,118870,33,0,0,4,0]],"vdbeColumnFromOverflow":[[92975,9,92975,17,0,0,4,0],[92975,21,92975,42,0,0,4,1]],"sqlite3DefaultRowEst":[[124690,7,124690,11,0,0,4,0]],"clearCellOverflow":[[76644,20,76644,65,0,0,4,1],[76662,9,76662,14,0,0,4,1]],"sqlite3AtoF":[[34954,30,34954,37,0,0,4,1],[34955,9,34955,17,0,0,4,0]],"sqlite3Atoi64":[[35223,30,35223,40,0,0,4,1]],"sqlite3HashInsert":[[36641,7,36641,20,0,0,4,0],[36641,24,36641,48,0,0,4,0],[36641,24,36641,48,0,0,4,1],[36642,9,36642,32,0,0,4,0],[36647,21,36647,27,0,0,4,0]],"pager_playback":[[59338,11,59338,24,0,0,4,1],[59341,13,59341,28,0,0,4,0]],"schemaIsValid":[[140728,9,140728,51,0,0,4,1],[140749,9,140749,26,0,0,4,1]],"sqlite3AnalysisLoad":[[119220,9,119220,24,0,0,4,1]],"pager_playback_one_page":[[58762,7,58762,14,0,0,4,0],[58772,23,58772,61,0,0,4,0]]},"library_calls":["sqlite3_os_init","sqlite3_open16","sqlite3_rollback_hook","sqlite3_global_recover","sqlite3_close_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_bind_parameter_name","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_strglob","sqlite3_next_stmt","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_os_init","sqlite3_open16","sqlite3_rollback_hook","sqlite3_global_recover","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_bind_parameter_name","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_strglob","sqlite3_next_stmt","sqlite3_finalize","sqlite3_close_v2"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimal input size
    if (size < 1) {
        return 0;
    }

    // Initialize SQLite OS interface
    sqlite3_os_init();

    // Open a temporary in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open16(L":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Set up rollback hook
    int rollback_data = 42;
    sqlite3_rollback_hook(db, [](void *p) {
        // Rollback callback - do nothing
        (void)p;
    }, &rollback_data);

    // Try to recover from global memory issues
    sqlite3_global_recover();

    // Prepare a statement with the input data as SQL
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    
    // Convert input data to null-terminated string for SQL
    char *sql_str = (char *)malloc(size + 1);
    if (!sql_str) {
        sqlite3_close_v2(db);
        return 0;
    }
    memcpy(sql_str, data, size);
    sql_str[size] = '\0';

    // Execute SQL directly
    char *errmsg = nullptr;
    sqlite3_exec(db, sql_str, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }

    // Prepare a separate statement
    const char *sample_sql = "SELECT ?1, ?2, ?3";
    if (sqlite3_prepare_v2(db, sample_sql, -1, &stmt, &tail) == SQLITE_OK) {
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // Bind parameters using input data
        if (param_count > 0) {
            // Use first byte as integer value for first parameter
            sqlite3_bind_int(stmt, 1, data[0] % 100);
            
            // Get parameter name for second parameter
            const char *param_name = sqlite3_bind_parameter_name(stmt, 2);
            (void)param_name; // Avoid unused variable warning
            
            // Bind more parameters if available
            if (param_count > 2) {
                sqlite3_bind_int(stmt, 3, size % 100);
            }
        }
        
        // Execute the prepared statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows if needed
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    // Use strglob for pattern matching on the SQL string
    int glob_result = sqlite3_strglob("SELECT *", sql_str);
    (void)glob_result; // Avoid unused variable warning

    // Iterate through all statements in the database
    sqlite3_stmt *next_stmt = nullptr;
    while ((next_stmt = sqlite3_next_stmt(db, next_stmt)) != nullptr) {
        // Clean up any remaining statements
        sqlite3_finalize(next_stmt);
    }

    // Free allocated resources
    free(sql_str);
    
    // Close the database
    sqlite3_close_v2(db);

    return 0;
}