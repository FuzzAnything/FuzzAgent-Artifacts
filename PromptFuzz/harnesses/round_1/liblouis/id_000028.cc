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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 1
//<Quality> {"density":2,"unique_branches":{"getAChar":[[304,13,304,41,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_logFile","lou_readCharFromFile","lou_compileString","lou_listTables","lou_freeTableFiles","lou_getTable","lou_getTypeformForEmphClass","lou_charToDots","lou_backTranslateString","lou_logPrint","lou_logEnd","lou_free"],"critical_calls":["lou_registerLogCallback","lou_logFile","lou_readCharFromFile","lou_compileString","lou_listTables","lou_freeTableFiles","lou_getTable","lou_getTypeformForEmphClass","lou_charToDots","lou_backTranslateString","lou_logPrint","lou_logEnd","lou_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Create input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file for logging
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Register log callback first
    auto log_callback = [](logLevels level, const char *msg) {
        // Empty callback for fuzzing
    };
    lou_registerLogCallback(log_callback);
    
    // 4. Set log file
    lou_logFile(fuzz_str_1);
    
    // 5. Read character from input file using lou_readCharFromFile
    int read_mode = 0;
    int char_read = lou_readCharFromFile("input_file", &read_mode);
    // Note: We'll simulate reading from our actual file handle
    // by reading first few bytes as table string
    
    // 6. Process input data to extract table list string
    // First null-terminated string will be treated as table list
    std::string tableList;
    if (size > 0) {
        const char* table_start = reinterpret_cast<const char*>(data);
        size_t table_len = strnlen(table_start, size);
        if (table_len < size) {
            tableList.assign(table_start, table_len);
        }
    }
    
    // 7. If we have a valid table list, compile it
    if (!tableList.empty()) {
        lou_compileString(tableList.c_str(), tableList.c_str());
    }
    
    // 8. List available tables
    char **tables = lou_listTables();
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // 9. Get table handle
    if (!tableList.empty()) {
        const void *table_handle = lou_getTable(tableList.c_str());
        (void)table_handle; // Use to avoid unused variable warning
    }
    
    // 10. Get typeform for emphasis class
    // Use a fixed emphasis class for fuzzing
    if (!tableList.empty()) {
        const char *emphClass = "emphasis";
        formtype typeform = lou_getTypeformForEmphClass(tableList.c_str(), fuzz_str_2);
        (void)typeform; // Use to avoid unused variable warning
    }
    
    // 11. Convert input to dots using charToDots
    // Use the remaining data after table string as input characters
    if (!tableList.empty() && size > tableList.size() + 1) {
        size_t data_offset = tableList.size() + 1;
        size_t chars_count = (size - data_offset) / sizeof(widechar);
        
        if (chars_count > 0) {
            std::vector<widechar> in_buf(chars_count);
            std::vector<widechar> out_buf(chars_count * 2); // Allocate enough space
            
            // Copy remaining data as wide characters
            memcpy(in_buf.data(), data + data_offset, chars_count * sizeof(widechar));
            
            lou_charToDots(tableList.c_str(), in_buf.data(), out_buf.data(), 
                          chars_count, 0);
        }
    }
    
    // 12. Perform back translation
    if (!tableList.empty() && size > tableList.size() + 1) {
        size_t data_offset = tableList.size() + 1;
        size_t chars_count = (size - data_offset) / sizeof(widechar);
        
        if (chars_count > 0) {
            std::vector<widechar> in_buf(chars_count);
            std::vector<widechar> out_buf(chars_count * 4); // Allocate enough space
            int inlen = chars_count;
            int outlen = chars_count * 4;
            char spacing = 0;
            formtype typeform = 0;
            
            // Copy remaining data as wide characters
            memcpy(in_buf.data(), data + data_offset, chars_count * sizeof(widechar));
            
            lou_backTranslateString(tableList.c_str(), in_buf.data(), &inlen,
                                   out_buf.data(), &outlen, &typeform, &spacing, 0);
        }
    }
    
    // 13. Print some log message
    lou_logPrint("Fuzzing iteration completed");
    
    // 14. Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 15. End logging
    lou_logEnd();
    
    // 16. Free liblouis resources
    lou_free();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}