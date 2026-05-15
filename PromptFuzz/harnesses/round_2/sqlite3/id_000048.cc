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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 528, nr_unique_branch: 115
//<Quality> {"density":32,"unique_branches":{"sqlite3BtreeTransferRow":[[79337,7,79337,31,0,0,4,0],[79337,7,79337,31,0,0,4,1],[79342,7,79342,25,0,0,4,0],[79342,7,79342,25,0,0,4,1],[79345,7,79345,36,0,0,4,1],[79349,7,79349,16,0,0,4,0],[79349,20,79349,46,0,0,4,0]],"vdbeColumnFromOverflow":[[92944,9,92944,60,0,0,4,0]],"btreeOverwriteContent":[[78895,9,78895,59,0,0,4,0],[78897,11,78897,13,0,0,4,1]],"isLikeOrGlob":[[158656,53,158656,61,0,0,4,1],[158712,15,158712,23,0,0,4,1],[158721,15,158721,22,0,0,4,0]],"sqlite3WhereCodeOneLoopStart":[[14098,27,14098,28,127,0,4,0]],"sqlite3BtreeDelete":[[79568,9,79568,33,0,0,4,0],[79614,12,79614,34,0,0,4,0]],"countOfViewOptimization":[[148371,7,148371,33,0,0,4,0]],"isSimpleCount":[[146739,7,146739,33,0,0,4,0]],"vdbeRecordCompareInt":[[88939,5,88939,12,0,0,4,0],[88949,13,88949,29,0,0,4,0]],"analysisLoader":[[118838,7,118838,14,0,0,4,1],[118838,18,118838,28,0,0,4,1],[118838,32,118838,42,0,0,4,1],[118842,7,118842,16,0,0,4,1],[118845,7,118845,17,0,0,4,0],[118854,7,118854,13,0,0,4,1]],"vdbeSorterSort":[[103253,9,103253,10,0,0,4,0]],"openSubJournal":[[60928,7,60928,14,0,0,4,1]],"exprNodeIsConstant":[[110340,5,110340,25,0,0,4,0],[14098,27,14098,28,16,0,4,0]],"sqlite3VdbeExec":[[97944,1,97944,16,0,0,4,0],[97954,10,97954,17,0,0,4,0],[97954,10,97954,17,0,0,4,1],[97956,7,97956,20,0,0,4,1],[98378,7,98378,16,0,0,4,0],[98380,9,98380,12,0,0,4,1]],"freePage2":[[76534,9,76534,43,0,0,4,1]],"allocateSpace":[[71598,39,71598,49,0,0,4,1],[14098,27,14098,28,5,0,4,0],[14098,27,14098,28,6,0,4,0],[14098,27,14098,28,7,0,4,0],[14676,20,14676,27,11,0,4,1]],"sqlite3VdbeRecordCompareWithSkip":[[88706,11,88706,26,0,0,4,0],[88707,14,88707,29,0,0,4,1],[88718,19,88718,26,0,0,4,0]],"xferCompatibleIndex":[[134383,7,134383,36,0,0,4,1],[134383,40,134383,69,0,0,4,1],[134386,7,134386,36,0,0,4,1],[134389,12,134389,27,0,0,4,0],[134389,12,134389,27,0,0,4,1],[134390,9,134390,46,0,0,4,1],[134393,9,134393,35,0,0,4,1],[134400,9,134400,50,0,0,4,1],[134403,9,134403,61,0,0,4,1],[134407,7,134407,75,0,0,4,1]],"allocateBtreePage":[[76364,23,76364,55,0,0,4,1]],"defragmentPage":[[71389,11,71389,20,0,0,4,1],[71389,25,71389,40,0,0,4,0],[71389,25,71389,40,0,0,4,1],[71389,44,71389,61,0,0,4,0],[71398,13,71398,19,0,0,4,0],[71399,15,71399,30,0,0,4,1],[71401,15,71401,38,0,0,4,1],[71414,20,71414,29,0,0,4,0],[14098,27,14098,28,17,0,4,0]],"editPage":[[77528,7,77528,19,0,0,4,0],[77538,9,77541,6,0,0,4,0]],"wherePathSolver":[[165598,16,165598,33,0,0,4,0],[165599,20,165599,34,0,0,4,1],[165600,24,165600,39,0,0,4,0],[165600,43,165600,68,0,0,4,0],[14098,27,14098,28,6,0,4,0]],"decodeIntArray":[[118773,12,118773,14,0,0,4,0],[118773,12,118773,14,0,0,4,1],[118773,18,118773,24,0,0,4,0],[118775,12,118775,25,0,0,4,0],[118775,12,118775,25,0,0,4,1],[118775,29,118775,35,0,0,4,0],[118788,9,118788,16,0,0,4,1],[118797,12,118797,16,0,0,4,1]],"wherePathSatisfiesOrderBy":[[14098,27,14098,28,61,0,4,1]],"sqlite3VdbeIdxRowid":[[14098,27,14098,28,8,0,4,0]],"xferOptimization":[[134615,31,134615,39,0,0,4,0],[134619,31,134619,38,0,0,4,0],[134620,11,134620,49,0,0,4,0],[134622,9,134622,19,0,0,4,1],[134625,9,134625,38,0,0,4,0],[134625,42,134625,71,0,0,4,1],[134703,9,134703,24,0,0,4,1],[134713,15,134713,31,0,0,4,1],[134748,31,134748,39,0,0,4,0],[134751,11,134751,49,0,0,4,0],[134762,9,134762,37,0,0,4,0],[134777,16,134777,34,0,0,4,0],[134777,16,134777,34,0,0,4,1],[134779,13,134779,53,0,0,4,1],[134781,11,134781,30,0,0,4,0],[134789,9,134789,61,0,0,4,1],[18627,31,18627,54,19,0,4,0],[14155,25,14155,46,58,0,4,0],[14155,26,14155,29,58,0,4,0]],"pageFreeArray":[[77459,13,77459,57,0,0,4,0],[77460,20,77460,27,0,0,4,0],[77460,20,77460,27,0,0,4,1]],"execSql":[[153276,9,153276,16,0,0,4,1]],"indexCellCompare":[[14098,27,14098,28,2,0,4,0]],"btreeGetHasContent":[[70416,10,70416,11,0,0,4,0],[70416,16,70416,41,0,0,4,1],[70416,45,70416,78,0,0,4,0]],"balance_nonroot":[[78373,22,78373,31,0,0,4,1]],"sqlite3BtreeInsert":[[79189,7,79189,30,0,0,4,0],[79192,9,79192,16,0,0,4,1],[79193,33,79193,54,0,0,4,1],[69436,27,69436,44,16,0,4,0],[69436,27,69436,44,16,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_trace","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_text","sqlite3_stricmp","sqlite3_finalize","sqlite3_prepare16_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_new","sqlite3_str_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_finalize","sqlite3_filename_database","sqlite3_memory_used","sqlite3_total_changes64","sqlite3_value_free","sqlite3_str_finish","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_trace","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_text","sqlite3_stricmp","sqlite3_finalize","sqlite3_prepare16_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_new","sqlite3_str_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_finalize","sqlite3_filename_database","sqlite3_memory_used","sqlite3_total_changes64","sqlite3_value_free","sqlite3_str_finish","sqlite3_close","sqlite3_shutdown"],"visited":1}
/*Looking at the APIs provided, I can see they can work together to achieve a specific event: **loading SQLite3 database backup/snapshot data from a byte stream and executing queries on it**. Here's the step-by-step implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt *stmt16 = nullptr;
    sqlite3_str *str_obj = nullptr;
    sqlite3_value *value_dup = nullptr;
    char *errmsg = nullptr;
    int rc = 0;
    
    // 1. Initialize SQLite
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
	return 0;
	}
	
    
    // 2. Create input file from byte stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd to avoid unused variable warning
    
    // 3. Open database (in-memory for fuzzing safety)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set up trace and profile callbacks
    sqlite3_trace(db, [](void*, const char* sql){ /* Trace callback */ }, nullptr);
    sqlite3_profile(db, [](void*, const char*, sqlite3_uint64 time){ /* Profile callback */ }, nullptr);
    
    // 5. Create a temporary table and load data
    const char* create_table = "CREATE TABLE fuzz_data (id INTEGER PRIMARY KEY, content BLOB, text_val TEXT);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // 6. Insert fuzzed data into table
    rc = sqlite3_prepare_v2(db, "INSERT INTO fuzz_data (content, text_val) VALUES (?, ?)", -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind blob from input data
        rc = sqlite3_bind_blob(stmt, 1, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
        
        // Create text from first part of input for binding
        int text_len = (int)(size > 512 ? 512 : size);
        char* temp_text = (char*)malloc(text_len + 1);
        if (temp_text) {
            memcpy(temp_text, data, text_len);
            temp_text[text_len] = '\0';
            rc = sqlite3_bind_text(stmt, 2, temp_text, -1, SQLITE_TRANSIENT);
            free(temp_text);
        }
        
        if (rc == SQLITE_OK) {
            rc = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 7. Execute SQL from remaining data (as string if possible)
    if (size > 0) {
        char* sql_str = (char*)malloc(size + 1);
        if (sql_str) {
            memcpy(sql_str, data, size);
            sql_str[size] = '\0';
            
            // Execute any valid SQL commands in the input
            rc = sqlite3_exec(db, sql_str, nullptr, nullptr, &errmsg);
            if (rc != SQLITE_OK && errmsg) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
            free(sql_str);
        }
    }
    
    // 8. Query the inserted data
    rc = sqlite3_prepare_v2(db, "SELECT content, text_val FROM fuzz_data", -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Get blob column
            const void* blob = sqlite3_column_blob(stmt, 0);
            int blob_size = sqlite3_column_bytes(stmt, 0);
            (void)blob;
            (void)blob_size;
            
            // Get text column and use stricmp
            const unsigned char* text = sqlite3_column_text(stmt, 1);
            if (text) {
                // Demonstrate stricmp usage
                int cmp = sqlite3_stricmp((const char*)text, "test");
                (void)cmp;
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 9. Prepare and execute UTF-16 statement
    const void* sql16 = u"SELECT * FROM fuzz_data";
    rc = sqlite3_prepare16_v2(db, sql16, -1, &stmt16, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt16);
        sqlite3_finalize(stmt16);
        stmt16 = nullptr;
    }
    
    // 10. Create sqlite3_str object
    str_obj = sqlite3_str_new(db);
    if (str_obj) {
        // Just create and immediately reset
        sqlite3_str_reset(str_obj);
    }
    
    // 11. Create and duplicate a value
    rc = sqlite3_prepare_v2(db, "SELECT text_val FROM fuzz_data LIMIT 1", -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            sqlite3_value* value = sqlite3_column_value(stmt, 0);
            if (value) {
                value_dup = sqlite3_value_dup(value);
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 12. Demonstrate sqlite3_filename_database usage
    const char* filename = "input_file";
    const char* db_name = sqlite3_filename_database((sqlite3_filename)filename);
    (void)db_name;
    
    // 13. Get memory usage
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;
    
    // 14. Get total changes
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    (void)total_changes;
    
    // Cleanup
    if (value_dup) {
        sqlite3_value_free(value_dup);
    }
    
    if (str_obj) {
        sqlite3_str_finish(str_obj);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}