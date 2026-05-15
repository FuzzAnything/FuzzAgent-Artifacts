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
//<ID> 850
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"resolveSubtable":[[4672,8,4672,17,0,0,4,0]]},"library_calls":["lou_setDataPath","lou_registerLogCallback","lou_readCharFromFile","lou_indexTables","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_logFile","lou_getDataPath","lou_free"],"critical_calls":["lou_setDataPath","lou_registerLogCallback","lou_readCharFromFile","lou_indexTables","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_logFile","lou_getDataPath","lou_free"],"visited":0}
/**/


extern "C" {
}

// Define log callback type
typedef void (*logcallback)(logLevels, const char *);

// Simple log callback for testing
static void  id_000039_test_log_callback(logLevels level, const char *message) {
    // Do nothing - just satisfy the API requirement
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<45) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if data is too small
    if (size < 4) {
	return 0;
	}
	
    
    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for logging
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Set data path (using first part of input as path)
    size_t path_len = data[0] % 64; // Limit path length
    if (path_len > size - 1) path_len = size - 1;
    
    std::string data_path;
    if (path_len > 0) {
        data_path.assign(reinterpret_cast<const char*>(&data[1]), path_len);
        lou_setDataPath(data_path.c_str());
    }
    
    // 2. Register log callback
    lou_registerLogCallback( id_000039_test_log_callback);
    
    // 3. Read character from file
    int mode = 0;
    int char_result = lou_readCharFromFile("input_file", &mode);
    (void)char_result; // Use result to avoid unused variable warning
    
    // 4. Index tables (using second part of input as table names)
    const char** tables = nullptr;
    if (size > path_len + 2) {
        size_t tables_start = 1 + path_len;
        size_t tables_len = data[tables_start] % 32;
        if (tables_len > 0 && tables_start + tables_len < size) {
            // Create a single table entry
            std::string table_name(reinterpret_cast<const char*>(&data[tables_start + 1]), 
                                   tables_len);
            const char* table_array[2] = {table_name.c_str(), nullptr};
            lou_indexTables(table_array);
        }
    }
    
    // 5. Get emphasis classes (using a default table)
    const char* test_table = "en-us-g2.ctb";
    const char** emph_classes = lou_getEmphClasses(fuzz_str_1);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // 6. Convert characters to dots (using remaining data as input)
    size_t conv_start = 1 + path_len;
    if (conv_start < size) {
        size_t conv_len = size - conv_start;
        if (conv_len > 0) {
            // Prepare input buffer
            std::vector<widechar> inbuf(conv_len);
            for (size_t i = 0; i < conv_len; i++) {
                inbuf[i] = static_cast<widechar>(data[conv_start + i]);
            }
            
            // Prepare output buffer
            std::vector<widechar> outbuf(conv_len * 2); // Ample space
            
            // Call charToDots
            int result = lou_charToDots(fuzz_str_2, inbuf.data(), 
                                       outbuf.data(), conv_len, 0);
            (void)result; // Use result to avoid unused variable warning
        }
    }
    
    // 7. Log to file
    lou_logFile(fuzz_str_3);
    
    // Get current data path (for API coverage)
    char* current_path = lou_getDataPath();
    (void)current_path;
    
    // Cleanup
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("output_file");
    remove("log_file.txt");
    remove("input_file");
    
    // Optional: Free library resources
    lou_free();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}