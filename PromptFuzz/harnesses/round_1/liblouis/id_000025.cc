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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_findTables","lou_indexTables","lou_compileString","lou_getEmphClasses","lou_freeEmphClasses","lou_getTableInfo","lou_freeTableInfo","lou_hyphenate","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_findTables","lou_indexTables","lou_compileString","lou_getEmphClasses","lou_freeEmphClasses","lou_getTableInfo","lou_freeTableInfo","lou_hyphenate","lou_freeTableFiles","lou_free"],"visited":2}
/**/


// Include liblouis headers
extern "C" {
}

// Dummy log callback function
static void  id_000025_dummy_log_callback(logLevels level, const char *message) {
    // Do nothing - this is just to satisfy the API requirement
    (void)level;
    (void)message;
}

// Fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Early exit if input is too small for basic operations
    if (size < 2) {
        return 0;
    }

    // 1. Register a log callback
    lou_registerLogCallback( id_000025_dummy_log_callback);

    // 2. Parse input to extract components
    // We'll use the first byte to determine how to split the input
    size_t split_point = data[0] % size;
    if (split_point == 0) split_point = 1; // Ensure we have at least 1 byte for query

    // Create query string from first part of input
    std::string query((const char*)data + 1, split_point - 1);
    if (query.empty()) {
        query = "en-us-g2"; // Default query if no valid string
    }

    // Create table string from second part of input (for compilation)
    size_t table_string_len = size - split_point;
    const char* table_string = (const char*)data + split_point;

    // 3. Find tables using the query
    char **tables = lou_findTables(query.c_str());
    if (tables) {
        // 4. Index the found tables
        lou_indexTables((const char **)tables);

        // Use first table found (if any) for other operations
        const char *table_to_use = tables[0];
        if (table_to_use) {
            // 5. Compile a table string
            // Ensure the table string is null-terminated for safety
            std::vector<char> safe_table_string(table_string, table_string + table_string_len);
            safe_table_string.push_back('\0');
            
            lou_compileString(table_to_use, safe_table_string.data());

            // 6. Get emphasis classes
            const char **emph_classes = lou_getEmphClasses(table_to_use);
            if (emph_classes) {
                lou_freeEmphClasses(emph_classes);
            }

            // 7. Get table information
            // Use a key from the input if available, otherwise use default
            std::string info_key = "locale";
            if (table_string_len > 10) {
                info_key = std::string(table_string, std::min(table_string_len, (size_t)20));
            }
            char *table_info = lou_getTableInfo(table_to_use, info_key.c_str());
            if (table_info) {
                lou_freeTableInfo(table_info);
            }

            // 8. Hyphenate a test string using the table
            // Create a test widechar string from the input data
            std::vector<widechar> test_input;
            // Convert bytes to widechars (simple conversion)
            for (size_t i = 0; i < std::min(size, (size_t)100); i += 2) {
                widechar wc = (i + 1 < size) ? 
                    ((widechar)data[i] << 8) | data[i + 1] : 
                    (widechar)data[i];
                test_input.push_back(wc);
            }
            
            if (!test_input.empty()) {
                std::vector<char> hyphens(test_input.size());
                int mode = 0; // Default mode
                
                lou_hyphenate(table_to_use, 
                            test_input.data(), 
                            static_cast<int>(test_input.size()), 
                            hyphens.data(), 
                            fuzz_int32_t_1);
            }
        }

        // Free the tables list
        lou_freeTableFiles(tables);
    }

    // 9. Clean up liblouis resources
    lou_free();

    return 0;
}