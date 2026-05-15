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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 176, nr_unique_branch: 30
//<Quality> {"density":16,"unique_branches":{"sqlite3_transfer_bindings":[[91407,7,91407,29,0,0,4,0]],"sqlite3_db_filename":[[182444,7,182444,32,0,0,4,1],[182450,10,182450,13,0,0,4,0]],"sqlite3Prepare16":[[141211,7,141211,16,0,0,4,1],[141214,7,141214,32,0,0,4,1],[141214,34,141214,41,0,0,4,1],[141217,7,141217,16,0,0,4,1],[141225,7,141225,12,0,0,4,0],[141229,7,141229,13,0,0,4,0],[141229,17,141229,23,0,0,4,1]],"sqlite3_serialize":[[52964,7,52964,32,0,0,4,1],[52970,7,52970,17,0,0,4,1],[52973,7,52973,13,0,0,4,0],[52974,7,52974,12,0,0,4,1],[52975,7,52975,8,0,0,4,1],[52988,7,52988,13,0,0,4,1],[52991,8,52991,12,0,0,4,0],[52993,7,52993,9,0,0,4,1],[52995,7,52995,21,0,0,4,1],[52999,9,52999,15,0,0,4,0],[53000,9,53000,41,0,0,4,0]],"memdbFromDbSchema":[[52936,7,52936,42,0,0,4,0]],"sqlite3VdbeExec":[[100730,1,100730,18,0,0,4,0]],"sqlite3_complete16":[[177465,7,177465,9,0,0,4,1],[177470,7,177470,12,0,0,4,0]],"sqlite3VdbeMemSetStr":[[83157,18,83157,34,0,0,4,1]],"sqlite3_extended_result_codes":[[181627,7,181627,32,0,0,4,1],[181630,17,181630,22,0,0,4,0]],"sqlite3_clear_bindings":[[89629,12,89629,21,0,0,4,1],[89634,7,89634,17,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_extended_result_codes","sqlite3_create_collation_v2","sqlite3_malloc64","sqlite3_db_filename","sqlite3_prepare16_v2","sqlite3_step","sqlite3_clear_bindings","sqlite3_prepare16_v2","sqlite3_bind_int64","sqlite3_bind_text16","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_finalize","sqlite3_serialize","sqlite3_complete16","sqlite3_free","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_extended_result_codes","sqlite3_create_collation_v2","sqlite3_malloc64","sqlite3_db_filename","sqlite3_prepare16_v2","sqlite3_step","sqlite3_clear_bindings","sqlite3_prepare16_v2","sqlite3_bind_int64","sqlite3_bind_text16","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_finalize","sqlite3_serialize","sqlite3_complete16","sqlite3_free","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open database from input file
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Enable extended result codes
    sqlite3_extended_result_codes(db, 1);
    
    // Create a collation (UTF-16 version)
    rc = sqlite3_create_collation_v2(db, "CUSTOM_COLLATION", SQLITE_UTF16, nullptr,
        [](void *pArg, int n1, const void *s1, int n2, const void *s2) -> int {
            // Simple comparison based on length
            if (n1 < n2) return -1;
            if (n1 > n2) return 1;
            return memcmp(s1, s2, n1);
        }, nullptr);
    
    // Allocate memory using sqlite3_malloc64
    void *allocated_mem = sqlite3_malloc64(4096);
    if (allocated_mem) {
        memset(allocated_mem, 0, 4096);
    }
    
    // Get database filename (though :memory: won't have one)
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
    
    // Create a prepared statement
    sqlite3_stmt *stmt1 = nullptr;
    const char16_t *sql16 = u"CREATE TABLE IF NOT EXISTS test (id INTEGER, data TEXT);";
    rc = sqlite3_prepare16_v2(db, sql16, -1, &stmt1, nullptr);
    
    if (stmt1) {
        // Execute the statement
        while (sqlite3_step(stmt1) == SQLITE_ROW) {
            // Process rows if any
        }
        
        // Clear bindings on stmt1 (though none are bound)
        sqlite3_clear_bindings(stmt1);
        
        // Create another statement
        sqlite3_stmt *stmt2 = nullptr;
        const char16_t *sql16_2 = u"INSERT INTO test VALUES (?, ?);";
        rc = sqlite3_prepare16_v2(db, sql16_2, -1, &stmt2, nullptr);
        
        if (stmt2) {
            // Bind some values to stmt2
            sqlite3_bind_int64(stmt2, 1, 100);
            sqlite3_bind_text16(stmt2, 2, u"test data", -1, SQLITE_TRANSIENT);
            
            // Transfer bindings from stmt2 to stmt1 (though schemas differ)
            // Note: This will fail due to schema mismatch but we call it anyway
            rc = sqlite3_transfer_bindings(stmt2, stmt1);
            
            sqlite3_finalize(stmt2);
        }
        
        sqlite3_finalize(stmt1);
    }
    
    // Serialize the database
    sqlite3_int64 serialized_size = 0;
    unsigned int serialize_flags = SQLITE_SERIALIZE_NOCOPY;
    unsigned char *serialized = sqlite3_serialize(db, "main", &serialized_size, serialize_flags);
    
    // Check if SQL statement is complete (UTF-16)
    const char16_t *complete_sql = u"SELECT * FROM test;";
    int is_complete = sqlite3_complete16(complete_sql);
    
    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write serialized data if available
        if (serialized && serialized_size > 0) {
            fwrite(serialized, 1, serialized_size, out_file);
        }
        
        // Read from input file and write to output file
        rewind(in_file);
        char buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            fwrite(buffer, 1, bytes_read, out_file);
        }
        
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    if (serialized) {
        sqlite3_free(serialized);
    }
    
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
    
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}