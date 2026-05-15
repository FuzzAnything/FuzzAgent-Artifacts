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
//<ID> 1105
//<Prompt> []
/*<Combination>: [
*/
//<score> 49, nr_unique_branch: 33
//<Quality> {"density":7,"unique_branches":{"compileRule":[[2912,7,2912,51,0,0,4,0],[2913,7,2913,47,0,0,4,0],[2955,2,2955,19,0,0,4,0],[2961,7,2961,28,0,0,4,0],[3068,3,3068,18,0,0,4,0],[3827,8,3827,76,0,0,4,0],[3852,9,3852,11,0,0,4,0],[4075,8,4075,62,0,0,4,0],[4078,11,4078,52,0,0,4,1],[4080,9,4080,27,0,0,4,0],[4113,8,4113,49,0,0,4,0],[4223,8,4223,49,0,0,4,0],[4234,9,4234,14,0,0,4,0],[4237,38,4237,69,0,0,4,0],[4302,8,4302,43,0,0,4,0],[4303,8,4303,44,0,0,4,0]],"_lou_pattern_compile":[[1203,6,1203,63,0,0,4,0]],"pattern_compile_2":[[1049,8,1049,79,0,0,4,0],[1060,8,1060,40,0,0,4,0]],"compileGrouping":[[2359,6,2359,44,0,0,4,0],[2359,6,2359,44,0,0,4,1],[2360,6,2360,42,0,0,4,0],[2360,6,2360,42,0,0,4,1],[2361,6,2361,49,0,0,4,0],[2361,6,2361,49,0,0,4,1],[2362,14,2362,34,0,0,4,0],[2362,14,2362,34,0,0,4,1],[2362,38,2362,63,0,0,4,0],[2363,6,2363,27,0,0,4,0]],"parseDots":[[1462,4,1462,12,0,0,4,0]],"pattern_compile_expression":[[607,6,607,29,0,0,4,0],[681,7,682,41,0,0,4,0],[834,7,834,30,0,0,4,0]]},"library_calls":["lou_setDataPath","lou_compileString","lou_checkTable","lou_checkTable","lou_getTableInfo","lou_freeTableInfo","lou_getTableInfo","lou_freeTableInfo","lou_hyphenate","lou_dotsToChar","lou_free"],"critical_calls":["lou_setDataPath","lou_compileString","lou_checkTable","lou_checkTable","lou_getTableInfo","lou_freeTableInfo","lou_getTableInfo","lou_freeTableInfo","lou_hyphenate","lou_dotsToChar","lou_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<128) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    // Ensure minimum data size for processing
    if (size < 10) {
	return 0;
	}
	
    
    // Initialize liblouis
    lou_setDataPath(nullptr);
    
    // Split data into parts for different uses
    size_t table_data_size = size / 3;
    size_t string_data_size = size / 3;
    size_t buffer_data_size = size - 2 * (size / 3);
    
    // 1. lou_compileString - compile a table from input data
    // Use first part as table string (null-terminated)
    char *table_string = static_cast<char*>(malloc(table_data_size + 1));
    if (!table_string) {
	return 0;
	}
	
    memcpy(table_string, data, table_data_size);
    table_string[table_data_size] = '\0';
    
    // Try to compile using a fixed table list name
    const char *table_list = "en-us-g2.ctb";
    int compile_result = lou_compileString(table_list, table_string);
    // Result is ignored as compilation may fail with fuzzed input
    
    // 2. lou_checkTable - check if table is valid
    int check_result1 = lou_checkTable(fuzz_str_1);
    int check_result2 = lou_checkTable(fuzz_str_2);
    
    // 3. lou_getTableInfo / lou_freeTableInfo - get table metadata
    char *table_info = lou_getTableInfo(fuzz_str_3, fuzz_str_4);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    table_info = lou_getTableInfo(fuzz_str_5, fuzz_str_6);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    // 4. lou_hyphenate - hyphenate input text
    // Use second part as widechar text to hyphenate
    if (string_data_size >= sizeof(widechar)) {
        // Convert bytes to widechars (simple cast for fuzzing)
        size_t widechar_count = string_data_size / sizeof(widechar);
        widechar *wide_buf = static_cast<widechar*>(malloc(string_data_size));
        if (wide_buf) {
            memcpy(wide_buf, data + table_data_size, string_data_size);
            
            // Allocate hyphens array
            char *hyphens = static_cast<char*>(malloc(widechar_count));
            if (hyphens) {
                // Hyphenate the input
                int hyphen_result = lou_hyphenate(
                    fuzz_str_7, 
                    wide_buf, 
                    static_cast<int>(widechar_count), 
                    hyphens, 
                    fuzz_int32_t_8
                );
                // Result is ignored
                free(hyphens);
            }
            free(wide_buf);
        }
    }
    
    // 5. lou_dotsToChar - convert braille dots to characters
    // Use third part as braille dots data
    if (buffer_data_size >= 2 * sizeof(widechar)) {
        size_t dots_count = buffer_data_size / sizeof(widechar);
        widechar *dots_buf = static_cast<widechar*>(malloc(buffer_data_size));
        widechar *chars_buf = static_cast<widechar*>(malloc(buffer_data_size));
        
        if (dots_buf && chars_buf) {
            memcpy(dots_buf, data + table_data_size + string_data_size, buffer_data_size);
            
            // Convert dots to characters
            int dots_result = lou_dotsToChar(
                fuzz_str_9,
                dots_buf,
                chars_buf,
                static_cast<int>(dots_count),
                fuzz_int32_t_10
            );
            // Result is ignored
        }
        
        free(dots_buf);
        free(chars_buf);
    }
    
    // 6. lou_free - cleanup liblouis resources
    lou_free();
    
    // Free allocated resources
    free(table_string);
    
    return 0;
}