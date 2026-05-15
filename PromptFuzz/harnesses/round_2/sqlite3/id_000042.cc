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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 770, nr_unique_branch: 309
//<Quality> {"density":20,"unique_branches":{"pcache1RemoveFromHash":[[55090,30,55090,42,0,0,4,0]],"btreeComputeFreeSpace":[[71861,7,71861,11,0,0,4,0],[71863,9,71863,15,0,0,4,1],[71870,11,71870,23,0,0,4,1],[71877,11,71877,26,0,0,4,0],[71880,9,71880,15,0,0,4,1],[71884,9,71884,41,0,0,4,1]],"sqlite3TableLock":[[120230,7,120230,13,0,0,4,0]],"ptrmapPut":[[70808,7,70808,11,0,0,4,1],[70815,7,70815,13,0,0,4,1],[70821,7,70821,20,0,0,4,1],[70825,7,70825,51,0,0,4,1],[70833,7,70833,15,0,0,4,1],[70840,7,70840,29,0,0,4,0],[70843,9,70843,22,0,0,4,0]],"pagerStress":[[61067,9,61067,35,0,0,4,0]],"btreeHeapInsert":[[80416,10,80416,21,0,0,4,1]],"insertCellFast":[[77110,9,77110,31,0,0,4,0],[77116,11,77116,14,0,0,4,1]],"pageReinit":[[72217,7,72217,20,0,0,4,1]],"unixFileControl":[[41914,11,41914,21,0,0,4,0],[41919,11,41919,22,0,0,4,1],[41919,26,41919,54,0,0,4,0],[41919,58,41919,77,0,0,4,0],[41921,13,41921,30,0,0,4,1]],"pagerFreeMapHdrs":[[60539,32,60539,33,0,0,4,0]],"allocateSpace":[[71618,7,71618,22,0,0,4,0],[71622,9,71622,11,0,0,4,1],[14676,20,14676,27,11,0,4,0]],"sqlite3_step":[[90374,9,90374,22,0,0,4,0],[90385,11,90385,28,0,0,4,0]],"btreeCreateTable":[[79673,7,79673,22,0,0,4,0],[79689,9,79689,37,0,0,4,1],[79697,12,79697,35,0,0,4,0],[79697,12,79697,35,0,0,4,1],[79698,9,79698,36,0,0,4,1],[79708,9,79708,22,0,0,4,1],[79712,9,79712,27,0,0,4,1],[79768,9,79768,11,0,0,4,1],[14156,25,14156,46,17,0,4,1],[14156,26,14156,29,17,0,4,1]],"unixMapfile":[[43113,7,43113,23,0,0,4,1],[43115,7,43115,13,0,0,4,0],[43115,7,43115,13,0,0,4,1],[43117,9,43117,34,0,0,4,1],[43122,7,43122,28,0,0,4,1],[43127,7,43127,26,0,0,4,0]],"fcntlSizeHint":[[41792,7,41792,27,0,0,4,0],[41792,31,41792,52,0,0,4,0],[41794,9,41794,26,0,0,4,0],[41795,11,41795,44,0,0,4,1]],"sqlite3Analyze":[[118706,7,118706,43,0,0,4,0]],"balance_deeper":[[78668,7,78668,20,0,0,4,1],[78675,7,78675,9,0,0,4,0]],"pagerAcquireMapPage":[[60487,7,60487,28,0,0,4,1],[60495,9,60495,13,0,0,4,1]],"sqlite3PcacheFetchStress":[[54006,9,54006,12,0,0,4,1],[54006,30,54006,58,0,0,4,0],[54010,9,54010,13,0,0,4,0],[54011,35,54011,38,0,0,4,0],[54011,42,54011,51,0,0,4,0],[54011,42,54011,51,0,0,4,1]],"unixRead":[[41233,7,41233,29,0,0,4,0],[41234,9,41234,38,0,0,4,0]],"ptrmapPutOvflPtr":[[71320,7,71320,11,0,0,4,1],[71323,7,71323,32,0,0,4,1]],"ptrmapPageno":[[70781,7,70781,13,0,0,4,1],[70785,7,70785,29,0,0,4,1]],"getPageMMap":[[62060,24,62060,30,0,0,4,0],[62060,24,62060,30,0,0,4,1],[62061,8,62061,36,0,0,4,1],[62061,40,62061,68,0,0,4,0],[62061,40,62061,68,0,0,4,1],[62070,7,62070,14,0,0,4,0],[62070,7,62070,14,0,0,4,1],[62070,18,62070,25,0,0,4,1],[62078,7,62078,14,0,0,4,0],[62078,7,62078,14,0,0,4,1],[62085,7,62085,14,0,0,4,0],[62085,7,62085,14,0,0,4,1],[62085,18,62085,27,0,0,4,0],[62090,9,62090,22,0,0,4,0],[62090,26,62090,31,0,0,4,0],[62090,26,62090,31,0,0,4,1],[62091,11,62091,38,0,0,4,0],[62094,11,62094,17,0,0,4,0],[62094,11,62094,17,0,0,4,1],[62099,11,62099,14,0,0,4,0],[62105,9,62105,22,0,0,4,1],[57263,25,57263,39,5,0,4,1]],"sqlite3Reprepare":[[141111,7,141111,9,0,0,4,0],[141112,9,141112,25,0,0,4,1]],"codeAttach":[[119604,7,119604,43,0,0,4,0]],"sqlite3PagerSharedLock":[[57219,22,57219,38,27,0,4,0]],"pager_write":[[62504,11,62504,45,0,0,4,1]],"balance":[[78764,13,78764,26,0,0,4,1],[78785,11,78785,24,0,0,4,1],[78788,11,78788,24,0,0,4,1],[78858,11,78858,24,0,0,4,1]],"setGetterMethod":[[57219,22,57219,38,1,0,4,0]],"sqlite3PcacheMove":[[54222,33,54222,59,0,0,4,1]],"balance_nonroot":[[77951,9,77951,26,0,0,4,0],[77953,11,77953,13,0,0,4,1]],"sqlite3BtreeInsert":[[79217,9,79217,11,0,0,4,0]],"backupOnePage":[[81427,32,81427,45,0,0,4,1],[81432,9,81432,53,0,0,4,1],[14676,20,14676,27,1,0,4,0]],"sqlite3AnalysisLoad":[[119191,45,119191,46,0,0,4,0],[119218,45,119218,46,0,0,4,0],[119220,9,119220,24,0,0,4,0]],"zeroJournalHdr":[[57826,7,57826,25,0,0,4,0],[57830,9,57830,19,0,0,4,1],[57830,23,57830,32,0,0,4,1],[57836,9,57836,22,0,0,4,0],[57836,26,57836,41,0,0,4,0],[57846,9,57846,22,0,0,4,0],[57846,26,57846,34,0,0,4,1]],"sqlite3VdbeDisplayComment":[[85708,9,85708,38,0,0,4,0],[85752,58,85752,68,0,0,4,0]],"unixUnfetch":[[43191,7,43191,8,0,0,4,0],[43191,7,43191,8,0,0,4,1]],"sqlite3VdbeExec":[[94931,1,94931,15,0,0,4,0],[94938,7,94938,17,0,0,4,0],[94942,9,94942,31,0,0,4,0],[94944,11,94944,25,0,0,4,0],[94944,11,94944,25,0,0,4,1],[94945,13,94945,25,0,0,4,0],[94987,7,94987,25,0,0,4,0],[98657,7,98657,9,0,0,4,0],[98724,9,98724,11,0,0,4,0],[100134,7,100134,9,0,0,4,0],[14098,27,14098,28,282,0,4,1],[14098,27,14098,28,283,0,4,0],[14098,27,14098,28,284,0,4,1],[14098,27,14098,28,285,0,4,1],[14098,27,14098,28,286,0,4,1],[14098,27,14098,28,287,0,4,1],[14098,27,14098,28,288,0,4,1],[14098,27,14098,28,289,0,4,1],[14098,27,14098,28,290,0,4,1],[14098,27,14098,28,291,0,4,1],[14098,27,14098,28,292,0,4,1],[14098,27,14098,28,293,0,4,1]],"btreeHeapPull":[[80425,7,80425,24,0,0,4,1],[80431,10,80431,29,0,0,4,1]],"unixFetch":[[43154,7,43154,25,0,0,4,0],[43155,9,43155,27,0,0,4,0],[43155,9,43155,27,0,0,4,1],[43157,11,43157,24,0,0,4,1],[43159,9,43159,35,0,0,4,0],[43159,9,43159,35,0,0,4,1]],"sqlite3PagerCommitPhaseTwo":[[63134,7,63134,28,0,0,4,1]],"sqlite3VdbeEnter":[[19560,29,19560,59,2,0,4,1]],"backupTruncateFile":[[81468,7,81468,20,0,0,4,0],[81468,24,81468,38,0,0,4,0]],"lockTable":[[120203,24,120203,37,0,0,4,1],[120204,25,120204,39,0,0,4,1],[120204,43,120204,54,0,0,4,0],[120204,43,120204,54,0,0,4,1]],"unixUnmapfile":[[42995,7,42995,22,0,0,4,0]],"sqlite3VdbeDisplayP4":[[85883,5,85883,20,0,0,4,0]],"sqlite3BtreeIntegrityCheck":[[80797,7,80797,18,0,0,4,0],[80807,7,80807,16,0,0,4,0],[80808,9,80808,24,0,0,4,1],[80819,15,80819,51,0,0,4,1],[80832,9,80832,24,0,0,4,0],[80832,28,80832,38,0,0,4,0],[80832,42,80832,51,0,0,4,1],[80843,7,80843,16,0,0,4,0],[80844,14,80844,31,0,0,4,0],[80844,14,80844,31,0,0,4,1],[80844,35,80844,47,0,0,4,0],[80853,11,80853,43,0,0,4,1],[80857,11,80857,43,0,0,4,0],[80858,11,80858,35,0,0,4,0],[80858,11,80858,35,0,0,4,1],[80858,39,80858,54,0,0,4,1]],"vdbeLeave":[[19560,29,19560,59,1,0,4,1]],"sqlite3RunVacuum":[[153451,7,153451,20,0,0,4,0],[153484,7,153484,20,0,0,4,0],[153495,8,153495,16,0,0,4,0],[153495,20,153495,77,0,0,4,1],[153594,9,153594,22,0,0,4,0],[153629,7,153629,10,0,0,4,1]],"hasSharedCacheTableLock":[[69993,7,69993,14,0,0,4,0],[69993,19,69993,27,0,0,4,1],[69993,31,69993,72,0,0,4,1],[70001,7,70001,14,0,0,4,0],[70004,47,70004,48,0,0,4,0],[70004,47,70004,48,0,0,4,1],[70006,11,70006,28,0,0,4,0],[70006,11,70006,28,0,0,4,1],[70007,13,70007,18,0,0,4,1],[70026,10,70026,29,0,0,4,1],[70026,34,70026,58,0,0,4,0],[70026,34,70026,58,0,0,4,1],[70026,62,70026,78,0,0,4,0],[70026,62,70026,78,0,0,4,1],[70027,9,70027,32,0,0,4,1]],"sqlite3PagerDontWrite":[[62690,7,62690,24,0,0,4,0],[62690,28,62690,52,0,0,4,0],[62690,28,62690,52,0,0,4,1],[62690,56,62690,77,0,0,4,0],[14098,27,14098,28,4,0,4,0]],"sqlite3Pragma":[[138204,9,138204,22,0,0,4,0]],"execSql":[[153267,7,153267,20,0,0,4,0]],"sqlite3PagerClearCache":[[63902,7,63902,26,0,0,4,0]],"pager_write_pagelist":[[60877,33,60877,67,0,0,4,1]],"pager_end_transaction":[[58500,15,58500,62,0,0,4,0],[58501,11,58501,32,0,0,4,1],[58505,13,58505,26,0,0,4,0],[58505,30,58505,46,0,0,4,0],[58515,15,58515,61,0,0,4,0],[58518,35,58518,43,0,0,4,1],[58518,45,58518,61,0,0,4,1]],"allocateCursor":[[92489,9,92489,25,0,0,4,0]],"sqlite3PagerSetJournalMode":[[63811,35,63811,48,0,0,4,0],[63811,52,63811,66,0,0,4,0],[63811,52,63811,66,0,0,4,1],[63821,11,63821,39,0,0,4,1],[63827,13,63827,30,0,0,4,0],[63830,13,63830,41,0,0,4,0],[63834,13,63834,26,0,0,4,0],[63834,13,63834,26,0,0,4,1],[63837,13,63837,26,0,0,4,0],[63837,13,63837,26,0,0,4,1],[63837,30,63837,49,0,0,4,1],[63839,19,63839,36,0,0,4,0],[63844,47,63844,78,0,0,4,0]],"unixRemapfile":[[43040,7,43040,12,0,0,4,0],[43040,7,43040,12,0,0,4,1],[43050,9,43050,22,0,0,4,0],[43050,9,43050,22,0,0,4,1],[43070,9,43070,25,0,0,4,1],[43070,29,43070,36,0,0,4,1],[43076,7,43076,14,0,0,4,0],[43076,7,43076,14,0,0,4,1],[43080,7,43080,23,0,0,4,1]],"syncJournal":[[60706,32,60706,77,0,0,4,1],[60783,11,60783,17,0,0,4,0],[60783,21,60783,54,0,0,4,0],[60786,13,60786,26,0,0,4,1],[57236,21,57236,41,2,0,4,1]],"sqlite3BtreeDelete":[[79479,7,79479,21,0,0,4,0],[79479,25,79479,53,0,0,4,1]],"writeJournalHdr":[[57918,25,57918,72,0,0,4,0]],"xferOptimization":[[134525,7,134525,30,0,0,4,1]],"sqlite3BtreeCopyFile":[[81903,7,81903,20,0,0,4,0],[81906,9,81906,28,0,0,4,0],[81907,9,81907,11,0,0,4,1],[81931,7,81931,20,0,0,4,1]],"defragmentPage":[[71383,7,71383,33,0,0,4,0],[71385,9,71385,27,0,0,4,1],[71386,9,71386,14,0,0,4,0],[71388,11,71388,30,0,0,4,1],[71389,11,71389,20,0,0,4,0],[71395,13,71395,23,0,0,4,1],[71398,13,71398,19,0,0,4,1],[71404,19,71404,38,0,0,4,1],[71411,38,71411,48,0,0,4,0],[71411,38,71411,48,0,0,4,1],[71413,15,71413,23,0,0,4,0],[71413,15,71413,23,0,0,4,1],[71414,20,71414,29,0,0,4,1]],"sqlite3PagerUnrefNotNull":[[62192,7,62192,30,0,0,4,0]],"pcache1Rekey":[[55612,10,55612,22,0,0,4,0]],"pager_truncate":[[59093,26,59093,46,0,0,4,1]],"sqlite3VdbeHalt":[[87408,11,87408,43,0,0,4,1]],"sqlite3InitOne":[[140474,7,140474,54,0,0,4,1],[140523,11,140523,28,0,0,4,0],[140523,32,140523,45,0,0,4,1]],"sqlite3PagerCommitPhaseOne":[[63077,11,63077,18,0,0,4,1]],"unixTruncate":[[41686,9,41686,29,0,0,4,0],[41686,33,41686,41,0,0,4,1],[41696,9,41696,30,0,0,4,0]],"allocateBtreePage":[[76195,9,76195,11,0,0,4,0],[76406,9,76406,24,0,0,4,0],[76415,11,76415,24,0,0,4,0],[76419,11,76419,13,0,0,4,1],[76421,11,76421,40,0,0,4,1],[69381,34,69381,71,40,0,4,0]],"checkTreePage":[[80560,18,80560,22,0,0,4,0],[80560,26,80560,39,0,0,4,0],[80568,9,80568,25,0,0,4,1],[80568,29,80568,44,0,0,4,1],[80576,9,80576,33,0,0,4,1],[80583,9,80583,22,0,0,4,0],[80584,11,80584,24,0,0,4,0],[80584,11,80584,71,0,0,4,1],[80592,9,80592,34,0,0,4,1],[80606,9,80606,21,0,0,4,1],[80683,12,80683,34,0,0,4,0],[80684,11,80684,33,0,0,4,1]],"sqlite3_backup_step":[[81625,13,81625,29,0,0,4,0],[81628,15,81628,52,0,0,4,1],[81636,13,81636,29,0,0,4,0],[81667,34,81667,47,0,0,4,0],[81667,51,81667,70,0,0,4,0],[81667,51,81667,70,0,0,4,1],[81668,17,81668,39,0,0,4,0],[81671,19,81671,32,0,0,4,0],[81677,15,81677,28,0,0,4,0],[81685,13,81685,26,0,0,4,0],[81685,30,81685,39,0,0,4,1],[81697,15,81697,28,0,0,4,0],[81702,15,81702,28,0,0,4,0],[14676,20,14676,27,31,0,4,1]],"checkList":[[80331,10,80331,18,0,0,4,1],[80380,7,80380,8,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_trace","sqlite3_profile","sqlite3_exec","sqlite3_exec","sqlite3_free","sqlite3_total_changes64","sqlite3_stricmp","sqlite3_value_dup","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_value_bytes","sqlite3_finalize","sqlite3_value_free","sqlite3_memory_used","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_step","sqlite3_finalize","sqlite3_total_changes64","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_trace","sqlite3_profile","sqlite3_exec","sqlite3_exec","sqlite3_free","sqlite3_total_changes64","sqlite3_stricmp","sqlite3_value_dup","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_value_bytes","sqlite3_finalize","sqlite3_value_free","sqlite3_memory_used","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_total_changes64","sqlite3_close","sqlite3_shutdown"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs:

*/


extern "C" {
// Include SQLite3 headers
}

// Custom callback for sqlite3_profile
static void  id_000042_profileCallback(void* context, const char* sql, sqlite3_uint64 elapsed) {
    // Log profiling information
    FILE* out_file = fopen("output_file", "ab");
    if (out_file) {
        fprintf(out_file, "SQL: %s, Time: %llu ns\n", sql, elapsed);
        assert_file_closed(&out_file);;
    }
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Create a temporary database file
    const char* db_filename = "fuzz_temp.db";
    sqlite3* db = nullptr;
    
    // Open database
    if (sqlite3_open(db_filename, &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Enable tracing to capture SQL statements
    sqlite3_trace(db, [](void*, const char* sql) {
        // Simple trace callback
    }, nullptr);
    
    // Set profile callback to measure execution time
    sqlite3_profile(db,  id_000042_profileCallback, nullptr);
    
    // Create a simple table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value BLOB"
        ");";
    
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Use input data as SQL commands (with safety checks)
    if (size > 0 && data != nullptr) {
        // Create a null-terminated copy of the input for safer handling
        char* sql_input = static_cast<char*>(malloc(size + 1));
        if (sql_input) {
            memcpy(sql_input, data, size);
            sql_input[size] = '\0';
            
            // Execute the SQL statement from input
            char* err_msg = nullptr;
            int rc = sqlite3_exec(db, sql_input, nullptr, nullptr, &err_msg);
            
            if (err_msg) {
                // Log error to output file
                FILE* out_file = fopen("output_file", "ab");
                if (out_file) {
                    fprintf(out_file, "SQL Error: %s\n", err_msg);
                    assert_file_closed(&out_file);;
                }
                sqlite3_free(err_msg);
            }
            
            free(sql_input);
            
            // Use sqlite3_changes64 to get number of changes
            sqlite3_int64 changes = sqlite3_total_changes64(db);
            
            // Log changes to output file
            FILE* out_file = fopen("output_file", "ab");
            if (out_file) {
                fprintf(out_file, "Changes: %lld\n", changes);
                assert_file_closed(&out_file);;
            }
        }
    }
    
    // Use sqlite3_strnicmp for string comparison
    const char* str1 = "Hello";
    const char* str2 = "hello";
    int cmp_result = sqlite3_stricmp(str1, str2);  // Using stricmp instead of strnicmp
    FILE* out_file = fopen("output_file", "ab");
    if (out_file) {
        fprintf(out_file, "String comparison result: %d\n", cmp_result);
        assert_file_closed(&out_file);;
    }
    
    // Create and use sqlite3_value
    sqlite3_value* test_value = sqlite3_value_dup(nullptr);  // Create a null value
    if (test_value) {
        // Bind some data to the value (simulating use)
        int value_bytes = 0;
        // We need to use the value somehow - let's create a prepared statement
        sqlite3_stmt* stmt = nullptr;
        const char* select_sql = "SELECT name FROM test_table LIMIT 1";
        
        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                // Get the value from the first column
                const void* blob_data = sqlite3_column_blob(stmt, 0);
                int data_size = sqlite3_column_bytes(stmt, 0);
                
                // Use sqlite3_value_bytes
                value_bytes = sqlite3_value_bytes(test_value);
                
                FILE* out_file = fopen("output_file", "ab");
                if (out_file) {
                    fprintf(out_file, "Value bytes: %d, Column bytes: %d\n", 
                            value_bytes, data_size);
                    assert_file_closed(&out_file);;
                }
            }
            sqlite3_finalize(stmt);
        }
        
        sqlite3_value_free(test_value);
    }
    
    // Get memory usage
    sqlite3_int64 memory_used = sqlite3_memory_used();
    out_file = fopen("output_file", "ab");
    if (out_file) {
        fprintf(out_file, "Memory used: %lld bytes\n", memory_used);
        assert_file_closed(&out_file);;
    }
    
    // Insert some test data using the input
    sqlite3_stmt* insert_stmt = nullptr;
    const char* insert_sql = "INSERT INTO test_table (name, value) VALUES (?, ?)";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr) == SQLITE_OK) {
        // Bind parameters using input data
        sqlite3_bind_text(insert_stmt, 1, "test_name", -1, SQLITE_TRANSIENT);
        
        if (size > 0) {
            sqlite3_bind_blob(insert_stmt, 2, data, 
                             static_cast<int>(size > 100 ? 100 : size),  // Limit size
                             SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(insert_stmt, 2);
        }
        
        // Execute insert
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
        
        // Get changes again
        sqlite3_int64 new_changes = sqlite3_total_changes64(db);
        out_file = fopen("output_file", "ab");
        if (out_file) {
            fprintf(out_file, "New changes after insert: %lld\n", new_changes);
            assert_file_closed(&out_file);;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    // Remove temporary file
    remove(db_filename);
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	return 0;
}