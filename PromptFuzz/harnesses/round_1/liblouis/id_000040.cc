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
//<ID> 943
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"lou_getEmphClasses":[[5092,9,5092,33,0,0,4,1]],"lou_getTypeformForEmphClass":[[5264,7,5264,55,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_listTables","lou_findTables","lou_findTables","lou_dotsToChar","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_compileString","lou_hyphenate","lou_freeTableFiles","lou_freeTableFiles","lou_logEnd","lou_free"],"critical_calls":["lou_registerLogCallback","lou_listTables","lou_findTables","lou_dotsToChar","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_compileString","lou_hyphenate","lou_freeTableFiles","lou_freeTableFiles","lou_logEnd","lou_free"],"visited":0}
/**/


// Include necessary liblouis headers

// Custom log callback function
static void  id_000040_fuzzLogCallback(logLevels level, const char* message) {
    // Minimal logging during fuzzing to avoid noise
    (void)level;
    (void)message;
}

// Helper function to safely extract strings from fuzzer input
static std::string  id_000040_extractString(const uint8_t*& data, size_t& size) {
    if (size == 0) return "";
    
    // Find null terminator or use available bytes
    size_t str_len = 0;
    while (str_len < size && data[str_len] != 0) {
        str_len++;
    }
    
    std::string result(reinterpret_cast<const char*>(data), str_len);
    
    // Advance data pointer (skip null terminator if present)
    if (str_len < size) {
        data += str_len + 1;
        size -= str_len + 1;
    } else {
        data += str_len;
        size = 0;
    }
    
    return result;
}

// Helper function to extract widechar array from fuzzer input
static std::vector<widechar>  id_000040_extractWidecharArray(const uint8_t*& data, size_t& size, size_t max_chars) {
    std::vector<widechar> result;
    
    // Each widechar is 2 bytes, but we'll interpret the data byte by byte
    // to avoid alignment issues
    size_t chars_to_read = std::min(max_chars, size / 2);
    
    for (size_t i = 0; i < chars_to_read; i++) {
        widechar wc = 0;
        if (i * 2 + 1 < size) {
            wc = (data[i * 2] << 8) | data[i * 2 + 1];
        } else if (i * 2 < size) {
            wc = data[i * 2] << 8;
        }
        result.push_back(wc);
    }
    
    if (chars_to_read > 0) {
        data += chars_to_read * 2;
        size -= chars_to_read * 2;
    }
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Initialize data pointers for extraction
    const uint8_t* original_data = data;
    size_t original_size = size;
    
    // 1. Register log callback first
    lou_registerLogCallback( id_000040_fuzzLogCallback);
    
    // 2. Extract table query and compilation string from input
    std::string table_query =  id_000040_extractString(data, size);
    std::string compile_string =  id_000040_extractString(data, size);
    
    // 3. List all available tables
    char** all_tables = lou_listTables();
    
    // 4. Find tables based on query (use empty string if no query extracted)
    char** found_tables = nullptr;
    if (!table_query.empty()) {
        found_tables = lou_findTables(table_query.c_str());
    } else {
        // Use a simple default query if no data available
        found_tables = lou_findTables(fuzz_str_1);
    }
    
    // 5. Try to use the first found table for operations
    const char* table_to_use = nullptr;
    if (found_tables && found_tables[0]) {
        table_to_use = found_tables[0];
    } else if (all_tables && all_tables[0]) {
        table_to_use = all_tables[0];
    }
    
    if (table_to_use) {
        // 6. Extract widechar data for dotsToChar operation
        std::vector<widechar> inbuf =  id_000040_extractWidecharArray(data, size, 100);
        std::vector<widechar> outbuf(inbuf.size() * 2); // Allocate enough space
        
        if (!inbuf.empty()) {
            // 7. Convert dots to characters
            int dots_result = lou_dotsToChar(table_to_use, 
                                            inbuf.data(), 
                                            outbuf.data(), 
                                            static_cast<int>(inbuf.size()), 
                                            fuzz_int32_t_2);
            (void)dots_result; // Result intentionally ignored for fuzzing
        }
        
        // 8. Get emphasis classes for the table
        const char** emph_classes = lou_getEmphClasses(table_to_use);
        if (emph_classes) {
            // 9. Get typeform for the first emphasis class if available
            if (emph_classes[0]) {
                formtype typeform = lou_getTypeformForEmphClass(table_to_use, emph_classes[0]);
                (void)typeform; // Result intentionally ignored for fuzzing
            }
            
            // 10. Free emphasis classes
            lou_freeEmphClasses(emph_classes);
        }
        
        // 11. Try to compile a string as a table
        if (!compile_string.empty()) {
            int compile_result = lou_compileString(table_to_use, compile_string.c_str());
            (void)compile_result; // Result intentionally ignored for fuzzing
        }
        
        // 12. Hyphenate using remaining data
        if (size > 0) {
            // Use remaining data as widechar input for hyphenation
            std::vector<widechar> hyphen_input =  id_000040_extractWidecharArray(data, size, 50);
            if (!hyphen_input.empty()) {
                std::vector<char> hyphens(hyphen_input.size() + 1);
                int hyphen_result = lou_hyphenate(table_to_use,
                                                hyphen_input.data(),
                                                static_cast<int>(hyphen_input.size()),
                                                hyphens.data(),
                                                fuzz_int32_t_3);
                (void)hyphen_result; // Result intentionally ignored for fuzzing
            }
        }
    }
    
    // 13. Clean up allocated resources
    if (all_tables) {
        lou_freeTableFiles(all_tables);
    }
    
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    // 14. End logging
    lou_logEnd();
    
    // Reset global state if needed
    lou_free();
    
    return 0;
}