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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 1
//<Quality> {"density":2,"unique_branches":{"analyzeTable":[[702,7,702,23,0,0,4,0]]},"library_calls":["lou_indexTables","lou_registerLogCallback","lou_registerTableResolver","lou_compileString","lou_getTable","lou_findTables","lou_freeTableFiles","lou_getTableInfo","lou_freeTableInfo","lou_translatePrehyphenated","lou_free"],"critical_calls":["lou_indexTables","lou_registerLogCallback","lou_registerTableResolver","lou_compileString","lou_getTable","lou_findTables","lou_freeTableFiles","lou_getTableInfo","lou_freeTableInfo","lou_translatePrehyphenated","lou_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to process output data:

*/


// Include liblouis headers (adjust paths as needed)

// Custom log callback
void  id_000002_log_callback(logLevels level, const char* message) {
    // Simple log callback - can be empty for fuzzing
}

// Table resolver function
char**  id_000002_table_resolver(const char* table, const char* base) {
    // Return NULL to use default resolver
    return nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<91) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Early return if data is too small
    if (size < 2) {
	return 0;
	}
	
    
    // Create temporary file for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Parse first bytes to determine parameters
    int param1 = data[0];
    int param2 = data[1];
    size_t data_offset = 2;
    
    // Set up table list - use a simple test table
    const char* tableList = "tests/tables/en-us-g2.ctb";
    
    // 1. Index tables
    const char* tables_to_index[] = {"en-us-g2.ctb", "tests/tables/test.ctb", nullptr};
    lou_indexTables(tables_to_index);
    
    // 2. Register log callback
    lou_registerLogCallback( id_000002_log_callback);
    
    // 3. Register table resolver
    lou_registerTableResolver( id_000002_table_resolver);
    
    // 4. Compile a table string (using part of input data as table definition)
    if (size - data_offset > 10) {
        char table_def[256];
        size_t copy_size = std::min(size - data_offset, (size_t)255);
        memcpy(table_def, data + data_offset, copy_size);
        table_def[copy_size] = '\0';
        lou_compileString("test", table_def);
        data_offset += copy_size;
    }
    
    // 5. Get table
    const void* table = lou_getTable(fuzz_str_1);
    if (!table) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Find tables
    char** found_tables = lou_findTables(fuzz_str_2);
    if (found_tables) {
        // Free the table list when done
        lou_freeTableFiles(found_tables);
    }
    
    // 7. Get table info
    char* table_info = lou_getTableInfo(fuzz_str_3, fuzz_str_4);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    // 8. Perform translation with hyphenation using remaining input data
    if (size - data_offset >= 4) {
        // Extract input parameters from data
        int inlen = std::min((int)(size - data_offset), 100);
        int outlen = 200;
        
        // Allocate buffers
        std::vector<widechar> inbuf(inlen);
        std::vector<widechar> outbuf(outlen);
        std::vector<int> outputPos(outlen);
        std::vector<int> inputPos(inlen);
        std::vector<char> inputHyphens(inlen);
        std::vector<char> outputHyphens(outlen);
        
        // Copy remaining data to input buffer
        size_t copy_size = std::min(size - data_offset, (size_t)inlen * sizeof(widechar));
        memcpy(inbuf.data(), data + data_offset, copy_size);
        
        // Fill remaining with zeros if needed
        if (copy_size < inlen * sizeof(widechar)) {
            memset(inbuf.data() + copy_size, 0, inlen * sizeof(widechar) - copy_size);
        }
        
        // Set up other parameters
        formtype* typeform = nullptr;
        char* spacing = nullptr;
        int cursorPos = 0;
        int mode = 0; // Default mode
        
        // Perform translation
        lou_translatePrehyphenated(
            fuzz_str_5,
            inbuf.data(),
            &inlen,
            outbuf.data(),
            &outlen,
            typeform,
            spacing,
            outputPos.data(),
            inputPos.data(),
            &cursorPos,
            inputHyphens.data(),
            outputHyphens.data(),
            fuzz_int32_t_6
        );
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    // Free liblouis resources
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}