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
//<ID> 799
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 31
//<Quality> {"density":2,"unique_branches":{"back_updatePositions":[[1012,6,1012,20,0,0,4,1],[1012,24,1012,46,0,0,4,0],[1012,50,1012,84,0,0,4,0]],"getTable":[[5200,9,5200,26,0,0,4,0]],"resolveSubtable":[[4672,8,4672,17,0,0,4,0]],"_lou_backTranslate":[[250,6,250,23,0,0,4,0],[251,15,251,31,0,0,4,0],[251,15,251,31,0,0,4,1],[252,6,252,23,0,0,4,0],[353,6,353,22,0,0,4,0],[356,15,356,25,0,0,4,0],[356,15,356,25,0,0,4,1],[357,8,357,30,0,0,4,0],[358,12,358,34,0,0,4,0],[358,12,358,34,0,0,4,1],[359,10,359,21,0,0,4,0],[359,10,359,21,0,0,4,1],[359,25,359,41,0,0,4,0],[360,26,360,35,0,0,4,1],[365,7,365,17,0,0,4,1],[366,10,366,26,0,0,4,0],[366,10,366,26,0,0,4,1],[368,6,368,23,0,0,4,0],[369,15,369,25,0,0,4,0],[369,15,369,25,0,0,4,1],[370,8,370,25,0,0,4,1],[372,13,372,40,0,0,4,0],[372,13,372,40,0,0,4,1],[377,6,377,23,0,0,4,0],[377,27,377,43,0,0,4,0],[378,7,378,24,0,0,4,0]]},"library_calls":["lou_getDataPath","lou_setDataPath","lou_listTables","lou_freeTableFiles","lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_backTranslate","lou_freeTableFile","lou_logEnd","lou_free"],"critical_calls":["lou_getDataPath","lou_setDataPath","lou_listTables","lou_freeTableFiles","lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_backTranslate","lou_freeTableFile","lou_logEnd","lou_free"],"visited":0}
/**/


// Include liblouis headers (assuming they're available)
extern "C" {
}

// Helper function to read null-terminated string from fuzzer input
const char*  id_000034_read_string(const uint8_t*& data, size_t& size, size_t max_len) {
    if (size == 0) return nullptr;
    
    size_t len = 0;
    while (len < size && len < max_len && data[len] != 0) {
        len++;
    }
    
    char* str = static_cast<char*>(malloc(len + 1));
    if (!str) return nullptr;
    
    memcpy(str, data, len);
    str[len] = '\0';
    
    // Move data pointer forward
    data += (len < size) ? len + 1 : len; // +1 to skip null terminator if present
    size -= (len < size) ? len + 1 : len;
    
    return str;
}

// Helper function to read widechar array from fuzzer input
std::vector<widechar>  id_000034_read_widechar_array(const uint8_t*& data, size_t& size, size_t max_chars) {
    std::vector<widechar> result;
    
    // Each widechar is 2 bytes
    size_t max_bytes = max_chars * sizeof(widechar);
    size_t bytes_to_read = (size < max_bytes) ? size : max_bytes;
    
    // Ensure we read complete widechars
    bytes_to_read = bytes_to_read - (bytes_to_read % sizeof(widechar));
    
    if (bytes_to_read == 0) return result;
    
    result.resize(bytes_to_read / sizeof(widechar));
    memcpy(result.data(), data, bytes_to_read);
    
    data += bytes_to_read;
    size -= bytes_to_read;
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables
    const uint8_t* original_data = data;
    size_t original_size = size;
    
    // 1. Get and set data path using first string from input
    const char* data_path =  id_000034_read_string(data, size, 256);
    if (data_path) {
        char* old_path = lou_getDataPath();
        char* new_path = lou_setDataPath(data_path);
        free(const_cast<char*>(data_path));
    }
    
    // 2. List available tables
    char** tables = lou_listTables();
    if (tables) {
        // Use table list for further operations
        lou_freeTableFiles(tables);
    }
    
    // 3. Find tables using query from input
    const char* table_query =  id_000034_read_string(data, size, 256);
    if (table_query) {
        char** found_tables = lou_findTables(table_query);
        if (found_tables) {
            lou_freeTableFiles(found_tables);
        }
        free(const_cast<char*>(table_query));
    }
    
    // 4. Get emphasis classes for a table
    const char* table_for_emphasis =  id_000034_read_string(data, size, 256);
    if (table_for_emphasis) {
        const char** emph_classes = lou_getEmphClasses(table_for_emphasis);
        if (emph_classes) {
            lou_freeEmphClasses(emph_classes);
        }
        free(const_cast<char*>(table_for_emphasis));
    }
    
    // 5. Perform charToDots conversion
    std::vector<widechar> inbuf1 =  id_000034_read_widechar_array(data, size, 100);
    if (!inbuf1.empty()) {
        std::vector<widechar> outbuf1(inbuf1.size());
        const char* table1 = "en-us-comp8.ctb"; // Default table
        lou_charToDots(table1, inbuf1.data(), outbuf1.data(), inbuf1.size(), 0);
    }
    
    // 6. Perform dotsToChar conversion
    std::vector<widechar> inbuf2 =  id_000034_read_widechar_array(data, size, 100);
    if (!inbuf2.empty()) {
        std::vector<widechar> outbuf2(inbuf2.size());
        const char* table2 = "en-us-comp8.ctb"; // Default table
        lou_dotsToChar(table2, inbuf2.data(), outbuf2.data(), inbuf2.size(), 0);
    }
    
    // 7. Perform hyphenation
    std::vector<widechar> inbuf3 =  id_000034_read_widechar_array(data, size, 100);
    if (!inbuf3.empty()) {
        std::vector<char> hyphens(inbuf3.size());
        const char* table3 = "en-us-g2.ctb"; // Grade 2 table
        lou_hyphenate(table3, inbuf3.data(), inbuf3.size(), hyphens.data(), 0);
    }
    
    // 8. Perform back translation
    std::vector<widechar> inbuf4 =  id_000034_read_widechar_array(data, size, 100);
    if (!inbuf4.empty()) {
        int inlen4 = inbuf4.size();
        int outlen4 = inbuf4.size() * 2; // Reserve enough space
        std::vector<widechar> outbuf4(outlen4);
        
        // Optional arrays
        std::vector<int> outputPos(outlen4);
        std::vector<int> inputPos(inbuf4.size());
        int cursorPos = 0;
        
        const char* table4 = "en-us-comp8.ctb";
        lou_backTranslate(table4, inbuf4.data(), &inlen4, outbuf4.data(), &outlen4, 
                         nullptr, nullptr, outputPos.data(), inputPos.data(), &cursorPos, 0);
    }
    
    // 9. Free any remaining table file allocations
    // Create a dummy table file pointer from remaining data
    if (size > 0) {
        char* dummy_table_file = static_cast<char*>(malloc(size + 1));
        if (dummy_table_file) {
            memcpy(dummy_table_file, data, size);
            dummy_table_file[size] = '\0';
            lou_freeTableFile(dummy_table_file);
        }
    }
    
    // 10. End logging
    lou_logEnd();
    
    // 11. Clean up global state
    lou_free();
    
    return 0;
}