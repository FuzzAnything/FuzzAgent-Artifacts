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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 143, nr_unique_branch: 24
//<Quality> {"density":13,"unique_branches":{"multiSelectValues":[[144098,9,144098,16,0,0,4,1],[144100,9,144100,21,0,0,4,0],[144100,9,144100,21,0,0,4,1],[144107,10,144107,11,0,0,4,0],[144107,10,144107,11,0,0,4,1],[144109,9,144109,18,0,0,4,1],[16754,56,16754,57,1,0,4,1]],"sqlite3Insert":[[132398,56,132398,74,0,0,4,1],[132577,18,132577,32,0,0,4,1],[132857,11,132857,33,0,0,4,0]],"sqlite3VdbeMemSetPointer":[[82907,20,82907,26,0,0,4,0],[82911,16,82911,27,0,0,4,1]],"sqlite3VdbeMemShallowCopy":[[83030,7,83030,35,0,0,4,1]],"parserDoubleLinkSelect":[[170515,11,170515,43,0,0,4,1]],"multiSelect":[[144194,7,144194,34,0,0,4,0],[144196,9,144196,14,0,0,4,0]],"sqlite3VdbeCheckMemInvariants":[[82003,9,82003,68,0,0,4,0]],"yy_reduce":[[174340,7,174340,14,0,0,4,0],[174345,7,174345,13,0,0,4,0],[14155,25,14155,46,55,0,4,0],[14155,26,14155,29,55,0,4,0]],"sqlite3_bind_pointer":[[91231,7,91231,20,0,0,4,0]],"sqlite3_soft_heap_limit64":[[29920,7,29920,10,0,0,4,0],[29924,48,29924,52,0,0,4,0]]},"library_calls":["sqlite3_soft_heap_limit64","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_pointer","sqlite3_step","sqlite3_column_table_name16","sqlite3_column_value","sqlite3_value_text","sqlite3_file_control","sqlite3_complete16","sqlite3_changes","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_soft_heap_limit64","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_step","sqlite3_column_table_name16","sqlite3_column_value","sqlite3_value_text","sqlite3_file_control","sqlite3_complete16","sqlite3_changes","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum input size for basic operations
    if (size < 16) {
	return 0;
	}
	

    // 1. Set soft heap limit using first 8 bytes as limit value
    sqlite3_int64 heap_limit = 0;
    if (size >= sizeof(sqlite3_int64)) {
        memcpy(&heap_limit, data, sizeof(sqlite3_int64));
        data += sizeof(sqlite3_int64);
        size -= sizeof(sqlite3_int64);
        sqlite3_soft_heap_limit64(heap_limit);
    }

    // 2. Create in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // 3. Create a simple table and insert data
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO test (name) VALUES ('test1'), ('test2');";
    sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);

    // 4. Prepare a statement for binding
    sqlite3_stmt *stmt = nullptr;
    const char *select_sql = "SELECT * FROM test WHERE id = ?;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 5. Use sqlite3_bind_pointer with a dummy object
    struct Dummy { int x; } dummy = {42};
    const char *pointer_type = "dummy_pointer";
    sqlite3_bind_pointer(stmt, 1, &dummy, pointer_type, nullptr);

    // 6. Execute statement and get column info
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // 7. Use sqlite3_column_table_name16 on first column
        const void *table_name16 = sqlite3_column_table_name16(stmt, 0);
        
        // 8. Get sqlite3_value and extract text
        sqlite3_value *value = sqlite3_column_value(stmt, 0);
        const unsigned char *text = sqlite3_value_text(value);
        
        // Use the text to avoid unused variable warning
        if (text != nullptr) {
            // Do nothing, just using the API
        }
    }

    // 9. Use sqlite3_file_control on main database
    int file_control_result;
    // SQLITE_FCNTL_LOCKSTATE = 1 (common operation)
    sqlite3_file_control(db, "main", 1, &file_control_result);

    // 10. Use remaining input data as UTF-16 SQL to check completeness
    if (size >= 2) {  // Need at least 2 bytes for UTF-16
        // Ensure even number of bytes for UTF-16
        size_t utf16_size = (size / 2) * 2;
        char *utf16_sql = static_cast<char*>(malloc(utf16_size + 2));
        if (utf16_sql) {
            memcpy(utf16_sql, data, utf16_size);
            // Add null terminator (two null bytes for UTF-16)
            utf16_sql[utf16_size] = 0;
            utf16_sql[utf16_size + 1] = 0;
            
            int is_complete = sqlite3_complete16(utf16_sql);
            // Use the result to avoid unused variable warning
            if (is_complete) {
                // Do nothing
            }
            
            free(utf16_sql);
        }
    }

    // 11. Get number of changes
    int changes = sqlite3_changes(db);
    // Use changes to avoid unused variable warning
    if (changes >= 0) {
        // Do nothing
    }

    // 12. Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}