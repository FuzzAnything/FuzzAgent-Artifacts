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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"lou_getTableInfo":[[1155,7,1155,15,0,0,4,0],[1156,8,1156,22,0,0,4,0],[1156,8,1156,22,0,0,4,1],[1156,26,1156,52,0,0,4,1],[1157,9,1157,38,0,0,4,0]]},"library_calls":["lou_listTables","lou_checkTable","lou_freeTableFiles","lou_getTableInfo","lou_freeTableFiles","lou_hyphenate","lou_getTypeformForEmphClass","lou_translate","lou_readCharFromFile","lou_logEnd","lou_freeTableFiles"],"critical_calls":["lou_listTables","lou_checkTable","lou_getTableInfo","lou_hyphenate","lou_getTypeformForEmphClass","lou_translate","lou_readCharFromFile","lou_logEnd","lou_freeTableFiles"],"visited":0}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Early exit if not enough data for basic operations
    if (size < sizeof(widechar) * 2) {
	return 0;
	}
	
    
    // Step 1: Get list of available tables
    char **tables_list = lou_listTables();
    if (!tables_list) {
	return 0;
	}
	
    
    // Find first valid table
    const char *table_to_use = nullptr;
    for (int i = 0; tables_list[i] != nullptr; i++) {
        if (lou_checkTable(tables_list[i]) == 0) {
            table_to_use = tables_list[i];
            break;
        }
    }
    
    if (!table_to_use) {
        lou_freeTableFiles(tables_list);
        return 0;
    }
    
    // Step 2: Get table information
    char *table_info = lou_getTableInfo(table_to_use, fuzz_str_1);
    if (table_info) {
        // Use the table info (could be used for logging or decisions)
        free(table_info);
    }
    
    // Step 3: Convert input data to widechar buffer for liblouis
    // We'll use the first part of data as input text
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count < 2) {
        lou_freeTableFiles(tables_list);
        return 0;
    }
    
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));
    
    // Ensure null termination for safety
    if (widechar_count > 0) {
        inbuf[widechar_count - 1] = 0;
    }
    
    int inlen = static_cast<int>(widechar_count - 1); // Exclude null terminator
    
    // Step 4: Perform hyphenation
    std::vector<char> hyphens(inlen);
    int hyphen_result = lou_hyphenate(table_to_use, inbuf.data(), inlen, 
                                      hyphens.data(), fuzz_int32_t_2);
    
    // Step 5: Get typeform for emphasis class
    // We'll use a simple emphasis class name from the remaining data
    size_t emph_offset = widechar_count * sizeof(widechar);
    if (size > emph_offset + 1) {
        size_t emph_len = std::min(size - emph_offset, (size_t)20);
        std::string emph_class(reinterpret_cast<const char*>(data + emph_offset), 
                               emph_len);
        // Ensure null termination
        emph_class.push_back('\0');
        
        formtype typeform = lou_getTypeformForEmphClass(table_to_use, 
                                                        emph_class.c_str());
        (void)typeform; // Use typeform to avoid unused variable warning
    }
    
    // Step 6: Perform translation
    // Allocate output buffer (typically larger than input)
    int outlen = inlen * 4; // Conservative estimate
    std::vector<widechar> outbuf(outlen);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(inlen);
    std::vector<int> cursorPos(inlen);
    std::vector<char> spacing(inlen);
    formtype typeform_array[1] = {0};
    
    int trans_inlen = inlen;
    int trans_outlen = outlen;
    
    int translate_result = lou_translate(
        table_to_use,
        inbuf.data(),
        &trans_inlen,
        outbuf.data(),
        &trans_outlen,
        typeform_array,
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        fuzz_int32_t_3
    );
    
    // Step 7: Create a FILE* from input data for additional processing
    // (if needed by other functions not in our list)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Could use the file descriptor if needed
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Read a character from the file (simulating usage)
        int mode = 0;
        int char_result = lou_readCharFromFile("dummy_table", &mode);
        (void)char_result;
        
        assert_file_closed(&in_file);;
    }
    
    // Step 8: Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use out_fd to avoid unused variable warning
        
        // Write translation result if successful
        if (translate_result > 0 && trans_outlen > 0) {
            fwrite(outbuf.data(), sizeof(widechar), trans_outlen, out_file);
        }
        
        assert_file_closed(&out_file);;
    }
    
    // Step 9: End logging
    lou_logEnd();
    
    // Step 10: Clean up resources
    lou_freeTableFiles(tables_list);
    
    // The fuzzer should always return 0
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}