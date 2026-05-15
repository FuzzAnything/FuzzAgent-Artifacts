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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_findTables","lou_indexTables","lou_getTableInfo","lou_freeTableInfo","lou_getEmphClasses","lou_freeEmphClasses","lou_compileString","lou_hyphenate","lou_freeTableFiles","lou_logEnd"],"critical_calls":["lou_registerLogCallback","lou_findTables","lou_indexTables","lou_getTableInfo","lou_freeTableInfo","lou_getEmphClasses","lou_freeEmphClasses","lou_compileString","lou_hyphenate","lou_freeTableFiles","lou_logEnd"],"visited":1}
/**/



extern "C" {

// Log callback function
static void test_log_callback(logLevels level, const char *message) {
    // Empty callback for fuzzing
}

int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<15) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Early return if we don't have enough data for basic operations
    if (size < 4) {
        return 0;
    }

    // Step 1: Register log callback
    lou_registerLogCallback(test_log_callback);

    // Step 2: Create a string from input data (ensuring null termination)
    // We'll use part of the data as a query string for table finding
    size_t query_len = size > 100 ? 100 : size;
    char *query = static_cast<char*>(malloc(query_len + 1));
    if (!query) {
        return 0;
    }
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    // Use a portion of data as table name/query
    size_t table_query_len = size > 50 ? 50 : size / 2;
    char *table_query = static_cast<char*>(malloc(table_query_len + 1));
    if (!table_query) {
        free(query);
        return 0;
    }
    memcpy(table_query, data, table_query_len);
    table_query[table_query_len] = '\0';

    // Step 3: Find tables using the query
    char **found_tables = lou_findTables(table_query);
    
    if (found_tables) {
        // Step 4: Index the found tables
        lou_indexTables(const_cast<const char**>(found_tables));

        // Try to use the first found table if available
        if (found_tables[0]) {
            const char *table_name = found_tables[0];
            
            // Step 5: Get table information
            char *table_info = lou_getTableInfo(table_name, fuzz_str_1);
            if (table_info) {
                lou_freeTableInfo(table_info);
            }
            
            // Step 6: Get emphasis classes
            const char **emph_classes = lou_getEmphClasses(table_name);
            if (emph_classes) {
                lou_freeEmphClasses(emph_classes);
            }

            // Step 7: Compile a string using the table
            // Use a portion of data as the string to compile
            size_t compile_str_len = size > 200 ? 200 : size / 4;
            char *compile_str = static_cast<char*>(malloc(compile_str_len + 1));
            if (compile_str) {
                memcpy(compile_str, data + (size - compile_str_len > 0 ? size - compile_str_len : 0), 
                       compile_str_len > size ? size : compile_str_len);
                compile_str[compile_str_len] = '\0';
                
                int compile_result = lou_compileString(table_name, compile_str);
                // Result is ignored as we're just testing the API
                
                free(compile_str);
            }

            // Step 8: Hyphenate using the table
            // Convert input data to widechar for hyphenation
            if (size >= sizeof(widechar)) {
                size_t widechar_len = size / sizeof(widechar);
                if (widechar_len > 0) {
                    widechar *wide_buf = static_cast<widechar*>(malloc(widechar_len * sizeof(widechar)));
                    char *hyphens = static_cast<char*>(malloc(widechar_len));
                    
                    if (wide_buf && hyphens) {
                        // Copy and convert data to widechar (simplified conversion)
                        for (size_t i = 0; i < widechar_len && i * sizeof(widechar) < size; i++) {
                            wide_buf[i] = static_cast<widechar>(data[i * sizeof(widechar) % 256]);
                        }
                        
                        // Call hyphenate function
                        int hyphen_result = lou_hyphenate(table_name, wide_buf, 
                                                         static_cast<int>(widechar_len), 
                                                         hyphens, fuzz_int32_t_2);
                        // Result is ignored as we're just testing the API
                        
                        free(wide_buf);
                        free(hyphens);
                    } else {
                        if (wide_buf) free(wide_buf);
                        if (hyphens) free(hyphens);
                    }
                }
            }
        }

        // Free the found tables
        lou_freeTableFiles(found_tables);
    }

    // Step 9: Clean up
    free(query);
    free(table_query);
    
    // Additional cleanup (optional but good practice)
    lou_logEnd();
    
    return 0;
}

}  // extern "C"