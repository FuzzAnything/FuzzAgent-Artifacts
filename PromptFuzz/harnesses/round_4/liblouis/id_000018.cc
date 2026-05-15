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
#include <liblouis/liblouis.h>
}
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 6
//<Quality> {"density":5,"unique_branches":{"compileTable":[[5047,32,5047,41,0,0,4,1],[5048,9,5048,52,0,0,4,1],[5052,7,5052,23,0,0,4,1],[5068,7,5068,23,0,0,4,1]],"getTable":[[5217,27,5217,52,0,0,4,1],[5221,8,5221,35,0,0,4,1]]},"library_calls":["lou_version","lou_listTables","lou_getEmphClasses","lou_hyphenate","lou_logFile","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"critical_calls":["lou_version","lou_listTables","lou_getEmphClasses","lou_hyphenate","lou_logFile","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"visited":0}
/*Here's a fuzz driver that uses the specified liblouis APIs according to your instructions:

*/


// Include necessary headers for liblouis
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 1: Get liblouis version
    const char *version = lou_version();
    if (version) {
        // Could log this, but for fuzzing we just verify it's non-null
    }
    
    // Step 2: Create a temporary file from input data for lou_logFile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: List available tables
    char **tables = lou_listTables();
    if (!tables) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use the first table if available
    const char *table_to_use = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        table_to_use = tables[i];
        break;
    }
    
    // If we found a table, use it for further operations
    if (table_to_use) {
        // Step 4: Get emphasis classes for the table
        const char **emph_classes = lou_getEmphClasses(table_to_use);
        
        // Step 5: Set up hyphenation test
        // We need to use the input data for hyphenation
        // Convert the input data to widechars for lou_hyphenate
        std::vector<widechar> inbuf(size);
        for (size_t i = 0; i < size && i < inbuf.size(); i++) {
            inbuf[i] = static_cast<widechar>(data[i] & 0xFF);
        }
        
        std::vector<char> hyphens(inbuf.size());
        
        // Call lou_hyphenate with the input data
        int result = lou_hyphenate(table_to_use, 
                                   inbuf.data(), 
                                   static_cast<int>(inbuf.size()), 
                                   hyphens.data(), 
                                   fuzz_int32_t_1);  // mode = 0
        
        // Step 6: Set log file (we'll log to a temporary file)
        lou_logFile(fuzz_str_2);
        
        // Step 7: Free emphasis classes if they were allocated
        if (emph_classes) {
            lou_freeEmphClasses(emph_classes);
        }
    }
    
    // Step 8: Clean up
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    assert_file_closed(&in_file);;
    
    // Step 9: Call lou_free to clean up any internal allocations
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}