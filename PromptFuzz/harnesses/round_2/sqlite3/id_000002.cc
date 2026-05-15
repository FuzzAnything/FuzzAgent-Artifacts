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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 1598, nr_unique_branch: 181
//<Quality> {"density":34,"unique_branches":{"moveToRightmost":[[75332,10,75332,38,0,0,4,1]],"sqlite3VdbeExplain":[[84511,30,84511,35,0,0,4,1],[84512,9,84512,14,0,0,4,1]],"sqlite3Insert":[[132475,7,132475,17,0,0,4,0],[132476,7,132476,17,0,0,4,1],[132522,7,132522,14,0,0,4,1],[132653,7,132653,17,0,0,4,0],[132653,21,132653,30,0,0,4,0],[132656,9,132656,21,0,0,4,1],[132675,9,132675,61,0,0,4,1],[132680,9,132680,38,0,0,4,1],[132687,7,132687,17,0,0,4,1],[132832,9,132832,16,0,0,4,1],[132844,15,132844,25,0,0,4,1]],"findBtree":[[81261,7,81261,11,0,0,4,1],[81276,7,81276,10,0,0,4,1]],"sqlite3DbStrDup":[[30581,7,30581,11,0,0,4,0]],"freeTempSpace":[[72641,7,72641,21,0,0,4,1]],"btreeBeginTrans":[[73368,15,73368,23,0,0,4,0],[73370,29,73370,34,0,0,4,1],[73464,11,73464,19,0,0,4,0]],"keywordCode":[[176298,9,176298,29,0,0,4,0]],"sqlite3DbNameToBtree":[[182415,13,182415,20,0,0,4,1]],"resolveExprStep":[[106659,24,106659,47,0,0,4,1]],"sqlite3IdListDelete":[[124853,7,124853,15,0,0,4,0]],"sqlite3JournalModename":[[137552,7,137552,23,0,0,4,1]],"sqlite3VdbeHalt":[[87270,7,87270,19,0,0,4,1],[87423,7,87423,19,0,0,4,1]],"sqlite3_result_int":[[89962,7,89962,14,0,0,4,1]],"sqlite3PagerOkToChangeJournalMode":[[63867,7,63867,44,0,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,27,14156,28,2,0,4,1]],"sqlite3BtreeInsert":[[79102,47,79102,72,0,0,4,1],[79251,22,79251,36,0,0,4,0]],"sqlite3_backup_finish":[[81753,7,81753,11,0,0,4,1],[81757,7,81757,17,0,0,4,0],[81762,7,81762,17,0,0,4,0],[81765,7,81765,20,0,0,4,1],[81779,8,81779,28,0,0,4,0],[81780,7,81780,17,0,0,4,0],[81787,7,81787,17,0,0,4,0]],"sqlite3GetToken":[[14098,27,14098,28,63,0,4,1],[14098,27,14098,28,64,0,4,0]],"sqlite3BtreeSetPageSize":[[72811,7,72811,17,0,0,4,1],[72812,7,72812,41,0,0,4,1],[72817,7,72817,20,0,0,4,0],[72817,24,72817,54,0,0,4,0],[72818,9,72818,35,0,0,4,0],[72821,9,72821,20,0,0,4,1],[72827,7,72827,11,0,0,4,1]],"sqlite3PagerSetJournalMode":[[63788,9,63788,40,0,0,4,1],[63793,7,63793,18,0,0,4,1],[57211,16,57211,29,1,0,4,0]],"sqlite3BtreeNext":[[76020,7,76020,33,0,0,4,1],[76024,7,76024,18,0,0,4,0]],"allocateBtreePage":[[14098,27,14098,28,2,0,4,1]],"sqlite3_backup_step":[[81497,7,81497,11,0,0,4,1],[81501,7,81501,17,0,0,4,0],[81506,7,81506,24,0,0,4,0],[81516,9,81516,19,0,0,4,0],[81516,23,81516,63,0,0,4,1],[81526,9,81526,22,0,0,4,0],[81526,26,81526,72,0,0,4,0],[81537,9,81537,26,0,0,4,0],[81537,30,81537,43,0,0,4,0],[81537,47,81537,75,0,0,4,1],[81542,9,81542,22,0,0,4,0],[81542,26,81542,43,0,0,4,0],[81543,9,81544,72,0,0,4,0],[81554,9,81554,22,0,0,4,0],[81555,10,81555,41,0,0,4,1],[81555,45,81555,76,0,0,4,0],[81556,9,81556,26,0,0,4,1],[81566,16,81566,23,0,0,4,0],[81566,40,81566,64,0,0,4,0],[81566,40,81566,64,0,0,4,1],[81566,68,81566,71,0,0,4,0],[81568,11,81568,36,0,0,4,0],[81571,13,81571,26,0,0,4,0],[81578,9,81578,22,0,0,4,0],[81581,11,81581,34,0,0,4,0],[81593,9,81593,24,0,0,4,0],[81594,11,81594,22,0,0,4,1],[81598,11,81598,24,0,0,4,1],[81598,28,81598,43,0,0,4,0],[81601,11,81601,24,0,0,4,0],[81602,13,81602,23,0,0,4,0],[81605,13,81605,44,0,0,4,1],[81609,11,81609,24,0,0,4,0],[81625,13,81625,29,0,0,4,1],[81636,13,81636,29,0,0,4,1],[81711,13,81711,26,0,0,4,0],[81712,13,81712,70,0,0,4,0],[81724,9,81724,20,0,0,4,0],[81731,9,81731,31,0,0,4,1],[81736,7,81736,17,0,0,4,0]],"sqlite3WhereBegin":[[166387,7,166387,18,0,0,4,0],[166388,9,166388,17,0,0,4,1],[166389,9,166389,46,0,0,4,1],[166518,7,166518,18,0,0,4,0]],"sqlite3PagerSetPagesize":[[60193,19,60193,50,0,0,4,1]],"sqlite3PagerIsMemdb":[[63311,10,63311,26,0,0,4,0]],"sqlite3PagerBegin":[[62362,28,62362,34,0,0,4,0]],"sqlite3Close":[[178854,7,178854,19,0,0,4,0],[178854,23,178854,43,0,0,4,1]],"sqlite3VdbeExec":[[94535,13,94535,19,0,0,4,1],[94550,7,94550,11,0,0,4,0],[94868,7,94868,8,0,0,4,0],[97736,11,97736,14,0,0,4,1],[97741,13,97741,25,0,0,4,1],[98589,7,98589,20,0,0,4,0],[100026,1,100026,20,0,0,4,0],[100051,7,100051,36,0,0,4,1],[100053,7,100053,49,0,0,4,1],[100061,7,100061,34,0,0,4,0],[100062,8,100062,37,0,0,4,0],[100068,7,100068,19,0,0,4,1],[100107,7,100107,9,0,0,4,1],[100115,7,100115,9,0,0,4,1]],"sqlite3_changes64":[[178714,7,178714,32,0,0,4,1]],"btreeLast":[[75372,7,75372,20,0,0,4,0],[75376,9,75376,22,0,0,4,0]],"sqlite3BtreeTableMoveto":[[75451,7,75451,33,0,0,4,0],[75451,37,75451,73,0,0,4,0],[75452,9,75452,32,0,0,4,1],[75456,9,75456,31,0,0,4,0],[75457,11,75457,44,0,0,4,0],[75529,11,75529,26,0,0,4,0],[75531,13,75531,20,0,0,4,1]],"checkReadTransaction":[[81301,7,81301,47,0,0,4,1]],"sqlite3Pragma":[[137702,9,137702,18,0,0,4,1],[137966,3,137966,28,0,0,4,0],[137970,9,137970,18,0,0,4,1],[137977,20,137977,62,0,0,4,0],[137978,13,137978,49,0,0,4,0],[137978,13,137978,49,0,0,4,1],[137980,11,137980,17,0,0,4,1],[137985,11,137985,39,0,0,4,1],[137991,9,137991,39,0,0,4,1],[137996,23,137996,28,0,0,4,0],[137996,23,137996,28,0,0,4,1],[137997,11,137997,26,0,0,4,0],[137997,11,137997,26,0,0,4,1],[137997,31,137997,38,0,0,4,0],[138929,3,138929,31,0,0,4,1]],"sqlite3ResetAllSchemasOfConnection":[[120803,12,120803,21,0,0,4,0],[120803,12,120803,21,0,0,4,1],[120805,9,120805,21,0,0,4,0],[120806,11,120806,29,0,0,4,0],[120816,7,120816,25,0,0,4,0]],"resolveP2Values":[[84881,9,84881,28,0,0,4,0]],"sqlite3_backup_init":[[81325,7,81325,36,0,0,4,1],[81325,38,81325,68,0,0,4,1],[81342,7,81342,22,0,0,4,1],[81353,9,81353,11,0,0,4,1],[81359,7,81359,8,0,0,4,0],[81367,9,81367,19,0,0,4,1],[81367,23,81367,34,0,0,4,1],[81368,9,81368,59,0,0,4,1],[81379,7,81379,8,0,0,4,0]],"sqlite3SchemaClear":[[126349,7,126349,45,0,0,4,1]],"sqlite3TableAffinity":[[131855,7,131855,17,0,0,4,1]],"checkActiveVdbeCnt":[[87126,11,87126,23,0,0,4,1]],"dropCell":[[76919,7,76919,22,0,0,4,1],[14098,27,14098,28,3,0,4,1]],"yy_reduce":[[174673,7,174673,15,0,0,4,0],[175071,7,175071,15,0,0,4,0]],"sqlite3VdbeMakeReady":[[86616,7,86616,17,0,0,4,0],[86616,21,86616,27,0,0,4,0]],"isFatalError":[[81394,11,81394,24,0,0,4,1]],"backupOnePage":[[81427,32,81427,45,0,0,4,0],[81427,49,81427,58,0,0,4,0],[81427,49,81427,58,0,0,4,1],[81430,9,81430,33,0,0,4,1],[81431,9,81431,74,0,0,4,0],[81432,9,81432,53,0,0,4,0],[81447,11,81447,18,0,0,4,0],[81447,11,81447,18,0,0,4,1],[81447,22,81447,32,0,0,4,0],[14676,20,14676,27,1,0,4,1]],"sqlite3Step":[[90263,11,90263,23,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_exec","sqlite3_close","sqlite3_wal_hook","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_zeroblob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_create_function","sqlite3_exec","sqlite3_wal_checkpoint","sqlite3_get_table","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_interrupt","sqlite3_errmsg16","sqlite3_free_table","sqlite3_free","sqlite3_close_v2"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_wal_hook","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_zeroblob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_create_function","sqlite3_exec","sqlite3_wal_checkpoint","sqlite3_get_table","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_interrupt","sqlite3_errmsg16","sqlite3_free_table","sqlite3_free","sqlite3_close_v2"],"visited":0}
/**/


// WAL hook callback function
static int  id_000002_wal_hook_callback(void* user_data, sqlite3* db, const char* db_name, int pages) {
    // Just return SQLITE_OK to continue normal operation
    return SQLITE_OK;
}

// Custom function to use sqlite3_context_db_handle
static void  id_000002_test_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Get the database handle from the context
    sqlite3* db = sqlite3_context_db_handle(context);
    if (db) {
        sqlite3_result_int(context, sqlite3_changes(db));
    } else {
        sqlite3_result_null(context);
    }
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_backup* backup = nullptr;
    char* err_msg = nullptr;
    char** pazResult = nullptr;
    int nRow = 0, nColumn = 0;
    int rc = SQLITE_OK;
    
    // Create a temporary database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable WAL mode
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Set WAL hook
    sqlite3_wal_hook(db,  id_000002_wal_hook_callback, nullptr);
    
    // Create a table with a blob column
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "name TEXT);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert data using the input
    const char* insert_sql = "INSERT INTO test_table (data, name) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind the input data as a blob
        rc = sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size > 1024 ? 1024 : size), SQLITE_STATIC);
        
        // Also insert a zero blob
        rc = sqlite3_bind_zeroblob(stmt, 1, 100);
        
        // Bind a text parameter
        const char* test_name = "fuzz_test";
        rc = sqlite3_bind_text(stmt, 2, test_name, -1, SQLITE_STATIC);
        
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Create another table for backup testing
    rc = sqlite3_exec(db, "CREATE TABLE backup_table (id INTEGER, content TEXT);", 
                     nullptr, nullptr, nullptr);
    
    // Insert some data into backup table
    rc = sqlite3_exec(db, "INSERT INTO backup_table VALUES (1, 'test1');", 
                     nullptr, nullptr, nullptr);
    rc = sqlite3_exec(db, "INSERT INTO backup_table VALUES (2, 'test2');", 
                     nullptr, nullptr, nullptr);
    
    // Create a user-defined function to use sqlite3_context_db_handle
    rc = sqlite3_create_function(db, "test_func", 0, SQLITE_UTF8, nullptr, 
                                 id_000002_test_function, nullptr, nullptr);
    
    // Use the custom function in a query
    rc = sqlite3_exec(db, "SELECT test_func();", nullptr, nullptr, nullptr);
    
    // Perform a WAL checkpoint
    rc = sqlite3_wal_checkpoint(db, "main");
    
    // Use sqlite3_get_table to retrieve data
    const char* select_sql = "SELECT * FROM test_table;";
    rc = sqlite3_get_table(db, select_sql, &pazResult, &nRow, &nColumn, &err_msg);
    
    if (rc == SQLITE_OK && pazResult) {
        // Process the table results
        for (int i = 0; i <= nRow; i++) {
            for (int j = 0; j < nColumn; j++) {
                const char* value = pazResult[i * nColumn + j];
                // Access the value (do nothing with it for fuzzing)
            }
        }
    }
    
    // Prepare a statement to read blob data
    const char* select_blob_sql = "SELECT data FROM test_table WHERE id = 1;";
    rc = sqlite3_prepare_v2(db, select_blob_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Use sqlite3_column_blob to get blob data
            const void* blob_data = sqlite3_column_blob(stmt, 0);
            int blob_size = sqlite3_column_bytes(stmt, 0);
            
            if (blob_data && blob_size > 0) {
                // Copy blob data to a temporary buffer (simulate usage)
                void* temp_buf = malloc(blob_size);
                if (temp_buf) {
                    memcpy(temp_buf, blob_data, blob_size);
                    free(temp_buf);
                }
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Create a backup of the database to another in-memory database
    sqlite3* db2 = nullptr;
    rc = sqlite3_open(":memory:", &db2);
    if (rc == SQLITE_OK && db2) {
        backup = sqlite3_backup_init(db2, "main", db, "main");
        if (backup) {
            sqlite3_backup_step(backup, -1);  // Copy entire database
            sqlite3_backup_finish(backup);
        }
        sqlite3_close(db2);
    }
    
    // Call sqlite3_interrupt (this might interrupt ongoing operations)
    sqlite3_interrupt(db);
    
    // Get error message in UTF-16 format if there was an error
    if (rc != SQLITE_OK) {
        const void* err_msg16 = sqlite3_errmsg16(db);
        // Error message is available in err_msg16 if needed
    }
    
    // Clean up
    if (pazResult) {
        sqlite3_free_table(pazResult);
    }
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Close the database
    if (db) {
        sqlite3_close_v2(db);
    }
    
    return 0;
}