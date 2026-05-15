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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 123.5, nr_unique_branch: 107
//<Quality> {"density":13,"unique_branches":{"pagerOpenWalIfPresent":[[59772,11,59772,16,0,0,4,0],[59776,13,59776,15,0,0,4,1],[59777,13,59777,21,0,0,4,0]],"sqliteDefaultBusyCallback":[[179306,7,179306,21,0,0,4,1]],"doWalCallbacks":[[90198,11,90198,19,0,0,4,0],[90198,23,90198,39,0,0,4,0],[90198,43,90198,56,0,0,4,0]],"walFrames":[[68187,7,68187,64,0,0,4,1],[68194,7,68194,44,0,0,4,1],[68203,7,68203,16,0,0,4,0],[68203,7,68203,16,0,0,4,1],[68211,9,68211,23,0,0,4,0],[68225,9,68225,22,0,0,4,1],[68236,9,68236,25,0,0,4,0],[68238,11,68238,13,0,0,4,1],[68241,7,68241,32,0,0,4,1],[68255,16,68255,17,0,0,4,0],[68255,16,68255,17,0,0,4,1],[68262,9,68262,15,0,0,4,1],[68282,16,68282,24,0,0,4,0],[68282,28,68282,40,0,0,4,0],[68282,28,68282,40,0,0,4,1],[68284,9,68284,11,0,0,4,1],[68291,7,68291,15,0,0,4,0],[68291,19,68291,33,0,0,4,1],[68310,7,68310,15,0,0,4,0],[68310,19,68310,48,0,0,4,0],[68312,9,68312,34,0,0,4,1],[68325,9,68325,14,0,0,4,0],[68335,7,68335,15,0,0,4,0],[68335,19,68335,41,0,0,4,0],[68335,19,68335,41,0,0,4,1],[68335,45,68335,63,0,0,4,1],[68350,16,68350,17,0,0,4,0],[68350,16,68350,17,0,0,4,1],[68350,21,68350,34,0,0,4,0],[68351,9,68351,41,0,0,4,1],[68356,10,68356,23,0,0,4,0],[68356,27,68356,35,0,0,4,1],[68362,7,68362,20,0,0,4,0],[68368,9,68368,17,0,0,4,0],[68373,9,68373,17,0,0,4,0],[14098,27,14098,28,26,0,4,0],[14098,27,14098,28,27,0,4,1]],"walTryBeginRead":[[67229,7,67229,14,0,0,4,1],[67271,7,67271,14,0,0,4,1],[67271,18,67271,66,0,0,4,1],[67315,7,67315,22,0,0,4,1],[67319,12,67319,25,0,0,4,0],[67319,12,67319,25,0,0,4,1],[67321,9,67321,29,0,0,4,0],[67321,33,67321,50,0,0,4,0],[67321,33,67321,50,0,0,4,1],[67327,7,67327,43,0,0,4,0],[67328,8,67328,26,0,0,4,0],[67328,8,67328,26,0,0,4,1],[67328,30,67328,36,0,0,4,0],[67330,14,67330,27,0,0,4,0],[67332,11,67332,24,0,0,4,0],[67343,7,67343,13,0,0,4,1],[67349,7,67349,9,0,0,4,1],[67388,7,67388,51,0,0,4,1],[67389,7,67389,73,0,0,4,1]],"walChecksumBytes":[[65093,7,65093,10,0,0,4,0],[65111,13,65111,24,0,0,4,0],[65129,13,65129,23,0,0,4,0],[65129,13,65129,23,0,0,4,1]],"pagerPagecount":[[59721,7,59721,15,0,0,4,1]],"pagerWalFrames":[[59615,16,59615,17,0,0,4,0],[59615,21,59615,30,0,0,4,0],[59615,21,59615,30,0,0,4,1],[59621,7,59621,15,0,0,4,0],[59628,18,59628,34,0,0,4,0],[59628,18,59628,34,0,0,4,1],[59629,11,59629,29,0,0,4,0],[59640,7,59640,21,0,0,4,0],[59640,7,59640,21,0,0,4,1],[59644,7,59644,20,0,0,4,0],[59644,24,59644,39,0,0,4,1]],"walEncodeFrame":[[65209,7,65209,24,0,0,4,0]],"walWriteOneFrame":[[68090,7,68090,9,0,0,4,1]],"walWriteToLog":[[68060,7,68060,28,0,0,4,1]],"walHashGet":[[65405,7,65405,18,0,0,4,0],[65407,9,65407,17,0,0,4,0]],"sqlite3PagerCommitPhaseOne":[[62908,11,62908,19,0,0,4,1],[62920,11,62920,24,0,0,4,0],[63085,24,63085,36,0,0,4,1],[57263,25,57263,39,6,0,4,0],[14155,25,14155,46,7,0,4,0],[14155,26,14155,29,7,0,4,0]],"sqlite3WalDefaultHook":[[180006,7,180006,32,0,0,4,1]],"openDatabase":[[180841,13,180841,40,0,0,4,0]],"walIndexAppend":[[65532,7,65532,20,0,0,4,0],[65543,9,65543,15,0,0,4,0],[65543,9,65543,15,0,0,4,1],[65555,9,65555,26,0,0,4,1],[65562,30,65562,46,0,0,4,0],[65562,30,65562,46,0,0,4,1],[65563,11,65563,26,0,0,4,1]],"walRestartLog":[[67994,7,67994,24,0,0,4,0],[67994,7,67994,24,0,0,4,1],[67997,9,67997,27,0,0,4,1],[68023,13,68023,26,0,0,4,1],[14098,27,14098,28,10,0,4,1],[14098,27,14098,28,11,0,4,1],[14098,27,14098,28,12,0,4,0]],"sqlite3PagerExclusiveLock":[[62837,9,62837,23,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_busy_timeout","sqlite3_wal_autocheckpoint","sqlite3_collation_needed","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_busy_timeout","sqlite3_wal_autocheckpoint","sqlite3_collation_needed","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"visited":1}
/**/


// Custom callback for collation_needed
static void  id_000019_collation_needed_callback(void* pCtx, sqlite3* db, int eTextRep, const char* zName) {
    sqlite3_context* context = static_cast<sqlite3_context*>(pCtx);
    if (context) {
        // Use sqlite3_result_text16 with some dummy data
        const void* dummy_text = u"Dummy collation result";
        sqlite3_result_text16(context, dummy_text, -1, SQLITE_TRANSIENT);
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read database content from input
    FILE* db_file = fopen("fuzz_temp.db", "wb");
    if (!db_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, db_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&db_file);;
    
    // Step 2: Open database using sqlite3_open_v2
    sqlite3* db = nullptr;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX;
    int rc = sqlite3_open_v2("fuzz_temp.db", &db, flags, nullptr);
    
    if (rc != SQLITE_OK || !db) {
        remove("fuzz_temp.db");
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Set busy timeout
    sqlite3_busy_timeout(db, 5000); // 5 second timeout
    
    // Step 4: Configure WAL autocheckpoint
    sqlite3_wal_autocheckpoint(db, 1000); // Checkpoint after 1000 pages
    
    // Step 5: Register collation needed callback
    sqlite3_context* dummy_context = nullptr;
    sqlite3_collation_needed(db, dummy_context,  id_000019_collation_needed_callback);
    
    // Step 6: Create a simple table and trigger collation
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER, name TEXT);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Insert some data using the input
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO fuzz_test (id, name) VALUES (?, ?);";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use first 4 bytes as integer if available
        int id_value = (size >= 4) ? *(reinterpret_cast<const int*>(data)) : 1;
        sqlite3_bind_int(stmt, 1, id_value);
        
        // Use remaining data as text (truncate to reasonable length)
        size_t text_len = (size > 4) ? (size - 4) : 0;
        text_len = (text_len > 1000) ? 1000 : text_len;
        
        if (text_len > 0) {
            const char* text_data = reinterpret_cast<const char*>(data + 4);
            sqlite3_bind_text(stmt, 2, text_data, text_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_text(stmt, 2, "default", 7, SQLITE_STATIC);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Step 7: Query with collation to trigger callback
    const char* query_sql = 
        "SELECT name FROM fuzz_test ORDER BY name COLLATE unknown_collation;";
    sqlite3_exec(db, query_sql, nullptr, nullptr, nullptr);
    
    // Step 8: Cleanup
    sqlite3_close(db);
    remove("fuzz_temp.db");
    
    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	return 0;
}