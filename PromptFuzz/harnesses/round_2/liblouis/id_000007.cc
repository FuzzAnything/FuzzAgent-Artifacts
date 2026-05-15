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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"lou_setDataPath":[[63,6,63,18,0,0,4,0]],"lou_findTables":[[1118,6,1118,17,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_setDataPath","lou_listTables","lou_findTables","lou_checkTable","lou_hyphenate","lou_translatePrehyphenated","lou_freeTableFiles","lou_freeTableFiles","lou_setDataPath","lou_free"],"critical_calls":["lou_getDataPath","lou_setDataPath","lou_listTables","lou_findTables","lou_checkTable","lou_hyphenate","lou_translatePrehyphenated","lou_freeTableFiles","lou_freeTableFiles","lou_setDataPath","lou_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs with the given input:

*/


// Include necessary liblouis headers

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (size == 0 || data == nullptr) {
        return 0;
    }

    // Variables to track allocations
    char **tables = nullptr;
    char **found_tables = nullptr;
    char *old_data_path = nullptr;
    char *set_path_result = nullptr;
    std::vector<char> input_hyphens;
    std::vector<char> output_hyphens;
    std::vector<widechar> inbuf;
    std::vector<widechar> outbuf;
    std::vector<int> outputPos;
    std::vector<int> inputPos;
    std::vector<int> cursorPos;
    std::vector<char> spacing;
    formtype typeform = 0;
    
    // 1. Set data path using input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read a path from the input (first null-terminated string up to 256 bytes)
    char path_buffer[256] = {0};
    size_t path_len = 0;
    int c;
    while (path_len < sizeof(path_buffer) - 1 && (c = fgetc(in_file)) != EOF && c != 0) {
        path_buffer[path_len++] = static_cast<char>(c);
    }
    
    if (path_len > 0) {
        old_data_path = lou_getDataPath(); // Save old path
        set_path_result = lou_setDataPath(path_buffer);
    }
    
    // 2. List available tables
    tables = lou_listTables();
    
    // 3. Find tables based on query
    // Use next part of input as query (next null-terminated string or up to 128 bytes)
    char query_buffer[128] = {0};
    size_t query_len = 0;
    while (query_len < sizeof(query_buffer) - 1 && (c = fgetc(in_file)) != EOF && c != 0) {
        query_buffer[query_len++] = static_cast<char>(c);
    }
    
    if (query_len > 0) {
        found_tables = lou_findTables(query_buffer);
    }
    
    // 4. Check a table if we found any
    const char *table_to_use = nullptr;
    if (tables && tables[0]) {
        table_to_use = tables[0];
    } else if (found_tables && found_tables[0]) {
        table_to_use = found_tables[0];
    } else {
        // Use a default table if available
        table_to_use = "en-us-comp8.ctb";
    }
    
    int table_check_result = lou_checkTable(table_to_use);
    
    // 5. Read remaining data for translation input
    // Read as widechar data (2 bytes per character)
    size_t bytes_remaining = size - ftell(in_file);
    size_t max_widechars = bytes_remaining / sizeof(widechar);
    
    if (max_widechars > 0 && table_check_result == 0) { // Table is valid
        inbuf.resize(max_widechars);
        size_t read_count = fread(inbuf.data(), sizeof(widechar), max_widechars, in_file);
        inbuf.resize(read_count);
        
        if (read_count > 0) {
            int inlen = static_cast<int>(read_count);
            int outlen = inlen * 4; // Output buffer can be larger
            
            // Allocate output buffers
            outbuf.resize(outlen);
            outputPos.resize(outlen);
            inputPos.resize(inlen);
            cursorPos.resize(1); // Single cursor position
            spacing.resize(outlen);
            input_hyphens.resize(inlen);
            output_hyphens.resize(outlen);
            
            // Initialize with data from remaining input
            for (size_t i = 0; i < input_hyphens.size() && ftell(in_file) < size; i++) {
                input_hyphens[i] = static_cast<char>(fgetc(in_file));
            }
            
            // 6. Hyphenate the input
            int hyphen_result = lou_hyphenate(
                table_to_use,
                inbuf.data(),
                inlen,
                input_hyphens.data(),
                0 // mode
            );
            
            // 7. Translate with hyphenation information
            int translate_result = lou_translatePrehyphenated(
                table_to_use,
                inbuf.data(),
                &inlen,
                outbuf.data(),
                &outlen,
                &typeform,
                spacing.data(),
                outputPos.data(),
                inputPos.data(),
                cursorPos.data(),
                input_hyphens.data(),
                output_hyphens.data(),
                0 // mode
            );
            
            // Clean up translation-related buffers
            // (they will be automatically cleaned when vectors go out of scope)
        }
    }
    
    // Clean up resources
    assert_file_closed(&in_file);;
    
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    // Restore original data path if we changed it
    if (set_path_result) {
        lou_setDataPath(old_data_path);
    }
    
    if (old_data_path) {
        lou_free(); // Free the old data path string
    }
    
    assert_file_closed(&in_file);
	return 0;
}