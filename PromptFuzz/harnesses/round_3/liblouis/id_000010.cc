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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 5
//<Quality> {"density":2,"unique_branches":{"lou_findTables":[[1118,6,1118,17,0,0,4,1],[1124,7,1124,19,0,0,4,0],[1131,6,1131,13,0,0,4,0],[1135,27,1135,28,0,0,4,0],[1135,27,1135,28,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_readCharFromFile","lou_indexTables","lou_findTables","lou_findTable","lou_translateString","lou_freeTableFiles","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerLogCallback","lou_readCharFromFile","lou_indexTables","lou_findTables","lou_findTable","lou_translateString","lou_freeTableFiles","lou_freeTableFile","lou_free"],"visited":1}
/**/


// Include liblouis headers
extern "C" {
}

// Custom log callback function
static void  id_000010_log_callback(logLevels level, const char* message) {
    // Simple callback that does nothing - just to satisfy API requirement
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return for insufficient data
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Register log callback
    lou_registerLogCallback( id_000010_log_callback);
    
    // 2. Create a temporary file with the input data
    FILE* temp_file = fmemopen((void*)data, size, "rb");
    if (!temp_file) {
        assert_file_closed(&temp_file);
	return 0;
    }
    
    // 3. Read character from file using lou_readCharFromFile
    int mode = 0;
    int char_result = lou_readCharFromFile("input_file", &mode);
    (void)char_result; // Result may be unused
    
    // 4. Write data to actual file for lou_readCharFromFile
    FILE* input_file = fopen("input_file", "wb");
    if (input_file) {
        fwrite(data, 1, size, input_file);
        assert_file_closed(&input_file);;
    }
    
    // 5. Index tables - create a null-terminated array of table names
    const char* tables_to_index[] = {"en-us-g2.ctb", "en-ueb-g2.ctb", nullptr};
    lou_indexTables(tables_to_index);
    
    // 6. Find tables based on query
    // Use first few bytes as query string (ensure null termination)
    size_t query_len = size > 100 ? 100 : size;
    std::vector<char> query_buf(query_len + 1);
    memcpy(query_buf.data(), data, query_len);
    query_buf[query_len] = '\0';
    
    char** found_tables = lou_findTables(query_buf.data());
    
    // 7. Find a specific table
    char* specific_table = lou_findTable(fuzz_str_1);
    
    // 8. Prepare translation buffers
    // Convert input data to widechar format for translation
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count < 1) {
        widechar_count = 1;
    }
    
    std::vector<widechar> inbuf(widechar_count);
    std::vector<widechar> outbuf(widechar_count * 2); // Output may be larger
    
    // Copy data to input buffer (as widechars)
    size_t copy_size = size < widechar_count * sizeof(widechar) ? 
                      size : widechar_count * sizeof(widechar);
    memcpy(inbuf.data(), data, copy_size);
    
    // 9. Perform translation using lou_translateString
    int inlen = static_cast<int>(widechar_count);
    int outlen = static_cast<int>(outbuf.size());
    formtype* typeform = nullptr;
    char* spacing = nullptr;
    
    // Use found table or default
    const char* table_to_use = specific_table ? specific_table : "en-us-g2.ctb";
    
    int translate_result = lou_translateString(
        table_to_use,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform,
        spacing,
        fuzz_int32_t_2
    );
    (void)translate_result; // Result may be unused
    
    // 10. Clean up resources
    if (temp_file) {
        assert_file_closed(&temp_file);;
    }
    
    // Remove temporary file
    remove("input_file");
    
    // Free table resources
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    if (specific_table) {
        lou_freeTableFile(specific_table);
    }
    
    // Free any internal liblouis resources
    lou_free();
    
    assert_file_closed(&input_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}