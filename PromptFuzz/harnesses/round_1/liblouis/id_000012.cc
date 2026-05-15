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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["lou_logFile","lou_registerTableResolver","lou_checkTable","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_backTranslateString","lou_translate","lou_translatePrehyphenated","lou_dotsToChar","lou_readCharFromFile","lou_free"],"critical_calls":["lou_logFile","lou_registerTableResolver","lou_checkTable","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_backTranslateString","lou_translate","lou_translatePrehyphenated","lou_dotsToChar","lou_readCharFromFile","lou_free"],"visited":0}
/**/


// Custom table resolver function
static char ** id_000012_custom_table_resolver(const char *table, const char *base) {
    static char *result[2] = {nullptr, nullptr};
    
    // Free previous allocations if any
    if (result[0]) {
        free(result[0]);
        result[0] = nullptr;
    }
    
    // Create a dummy table path
    if (table) {
        result[0] = strdup("test.ctb");
    }
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<129) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	
    
    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set log file
    lou_logFile(fuzz_str_1);
    
    // Register custom table resolver
    lou_registerTableResolver( id_000012_custom_table_resolver);
    
    // Use a default table list for testing
    const char *tableList = "en-us-g2.ctb";
    
    // First, try to read the table list from the input data
    // We'll use the first portion of data as potential table list
    size_t tableListLen = size > 100 ? 100 : size;
    std::string potentialTableList;
    for (size_t i = 0; i < tableListLen && data[i] != 0; i++) {
        potentialTableList.push_back(static_cast<char>(data[i]));
    }
    
    if (!potentialTableList.empty()) {
        tableList = potentialTableList.c_str();
    }
    
    // Check if table is valid
    if (lou_checkTable(fuzz_str_2) != 0) {
        // Use default if invalid
        tableList = "en-us-g2.ctb";
    }
    
    // Get emphasis classes
    const char **emphClasses = lou_getEmphClasses(fuzz_str_3);
    if (emphClasses) {
        lou_freeEmphClasses(emphClasses);
    }
    
    // Get table handle
    const void *tableHandle = lou_getTable(fuzz_str_4);
    (void)tableHandle; // Use to avoid unused variable warning
    
    // Calculate remaining data for processing
    size_t remainingSize = size - (potentialTableList.size() + 1);
    if (remainingSize > size) remainingSize = 0;
    
    // Convert remaining data to widechars for processing
    std::vector<widechar> inbuf;
    size_t widecharCount = remainingSize / sizeof(widechar);
    
    if (widecharCount > 0) {
        inbuf.resize(widecharCount);
        memcpy(inbuf.data(), data + potentialTableList.size() + 1, 
               widecharCount * sizeof(widechar));
        
        // Prepare for translation operations
        int inlen = static_cast<int>(widecharCount);
        int outlen = inlen * 2; // Allocate double for safety
        std::vector<widechar> outbuf(outlen);
        std::vector<formtype> typeform(inlen, 0);
        std::vector<char> spacing(inlen, 0);
        std::vector<int> outputPos(inlen, 0);
        std::vector<int> inputPos(inlen, 0);
        int cursorPos = 0;
        
        // Mode for translation
        int mode = 0;
        
        // 1. Try back translation
        int backInlen = inlen;
        int backOutlen = outlen;
        lou_backTranslateString(fuzz_str_5, inbuf.data(), &backInlen, 
                               outbuf.data(), &backOutlen, 
                               typeform.data(), spacing.data(), mode);
        
        // 2. Try regular translation
        int transInlen = inlen;
        int transOutlen = outlen;
        lou_translate(fuzz_str_6, inbuf.data(), &transInlen, 
                     outbuf.data(), &transOutlen,
                     typeform.data(), spacing.data(),
                     outputPos.data(), inputPos.data(), &cursorPos, fuzz_int32_t_7);
        
        // 3. Try prehyphenated translation (with dummy hyphens)
        std::vector<char> inputHyphens(inlen, 0);
        std::vector<char> outputHyphens(outlen, 0);
        int prehyphenInlen = inlen;
        int prehyphenOutlen = outlen;
        lou_translatePrehyphenated(fuzz_str_8, inbuf.data(), &prehyphenInlen,
                                  outbuf.data(), &prehyphenOutlen,
                                  typeform.data(), spacing.data(),
                                  outputPos.data(), inputPos.data(), &cursorPos,
                                  inputHyphens.data(), outputHyphens.data(), mode);
        
        // 4. Try dots to char conversion on first few characters
        if (inlen > 0) {
            size_t dotsLength = inlen > 10 ? 10 : inlen;
            std::vector<widechar> dotsBuf(dotsLength);
            lou_dotsToChar(fuzz_str_9, inbuf.data(), dotsBuf.data(), 
                          static_cast<int>(dotsLength), fuzz_int32_t_10);
        }
    }
    
    // Try to read a character from the input file using lou_readCharFromFile
    // We'll write the input data to a temporary file first
    FILE *temp_file = fopen("temp_input.txt", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        int fileMode = 0;
        lou_readCharFromFile("temp_input.txt", &fileMode);
        
        // Clean up temp file
        remove("temp_input.txt");
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    remove("output_file");
    remove("louis_log.txt");
    
    // Free louis resources
    lou_free();
    
    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}