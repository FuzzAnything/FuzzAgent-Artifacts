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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":2,"unique_branches":{},"library_calls":["lou_registerTableResolver","lou_findTables","lou_freeTableFiles","lou_checkTable","lou_compileString","lou_dotsToChar","lou_translateString","lou_free"],"critical_calls":["lou_registerTableResolver","lou_findTables","lou_freeTableFiles","lou_checkTable","lou_compileString","lou_dotsToChar","lou_translateString","lou_free"],"visited":0}
/**/


// Include liblouis headers

// Global variables for table resolver
static char**  id_000008_custom_table_resolver(const char* tableList, const char* base) {
    // Simple resolver that returns the tableList as the only entry
    static char* tableArray[2];
    static char* tableName = nullptr;
    
    if (tableName) {
        free(tableName);
    }
    
    tableName = strdup(tableList); // Copy the table name
    tableArray[0] = tableName;
    tableArray[1] = nullptr;
    
    return tableArray;
}

// Global table list
static std::vector<std::string>  id_000008_foundTables;

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 2) {
        return 0; // Need at least some data
    }
    
    // Register custom table resolver
    lou_registerTableResolver( id_000008_custom_table_resolver);
    
    // Step 1: Use input data as a table list query
    // Take first part of data as query string (null-terminated)
    size_t query_len = size < 100 ? size : 100;
    std::vector<char> query(query_len + 1);
    memcpy(query.data(), data, query_len);
    query[query_len] = '\0';
    
    // Find tables using the query
    char** tables = lou_findTables(query.data());
    if (tables) {
        for (int i = 0; tables[i] != nullptr; i++) {
             id_000008_foundTables.push_back(tables[i]);
        }
        lou_freeTableFiles(tables);
    }
    
    // Step 2: Check a table if we found any
    const char* tableToUse = "en-us-g2.ctb"; // Default table
    if (! id_000008_foundTables.empty()) {
        tableToUse =  id_000008_foundTables[0].c_str();
    }
    
    int check_result = lou_checkTable(fuzz_str_1);
    
    // Step 3: Compile a table string from input data
    // Use middle part of data as table string
    size_t compile_offset = query_len < size ? query_len : 0;
    size_t compile_len = size - compile_offset;
    if (compile_len > 0) {
        // Ensure null termination
        std::vector<char> table_string(compile_len + 1);
        size_t copy_len = compile_len < 1000 ? compile_len : 1000;
        memcpy(table_string.data(), data + compile_offset, copy_len);
        table_string[copy_len] = '\0';
        
        int compile_result = lou_compileString(tableToUse, table_string.data());
    }
    
    // Step 4: Use dotsToChar with remaining input data
    // Convert remaining data to widechar for processing
    size_t dots_offset = compile_offset + (compile_len > 0 ? (compile_len < 1000 ? compile_len : 1000) : 0);
    if (dots_offset >= size) {
        dots_offset = size / 2; // Fallback if we've used all data
    }
    
    size_t remaining = size - dots_offset;
    if (remaining >= 2) { // Need at least one widechar
        // Calculate number of widechars we can process
        size_t widechar_count = remaining / sizeof(widechar);
        if (widechar_count > 100) widechar_count = 100; // Limit
        
        if (widechar_count > 0) {
            std::vector<widechar> inbuf(widechar_count);
            std::vector<widechar> outbuf(widechar_count);
            
            // Copy data as widechars
            memcpy(inbuf.data(), data + dots_offset, widechar_count * sizeof(widechar));
            
            // Convert dots to characters
            int dots_result = lou_dotsToChar(fuzz_str_2, 
                                            inbuf.data(), 
                                            outbuf.data(), 
                                            widechar_count, 
                                            fuzz_int32_t_3); // mode 0
            
            // Step 5: Use translateString with the same data
            int inlen = widechar_count;
            int outlen = widechar_count * 2; // Allow for expansion
            std::vector<widechar> translate_outbuf(outlen);
            std::vector<char> spacing(outlen);
            
            int translate_result = lou_translateString(fuzz_str_4,
                                                      inbuf.data(),
                                                      &inlen,
                                                      translate_outbuf.data(),
                                                      &outlen,
                                                      nullptr, // typeform
                                                      spacing.data(),
                                                      fuzz_int32_t_5); // mode 0
        }
    }
    
    // Cleanup
    lou_free();
     id_000008_foundTables.clear();
    
    return 0;
}