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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"lou_setDataPath":[[63,6,63,18,0,0,4,0]]},"library_calls":["lou_getDataPath","lou_setDataPath","lou_findTables","lou_setDataPath","lou_freeTableFiles","lou_setDataPath","lou_freeTableFiles","lou_setDataPath","lou_dotsToChar","lou_backTranslateString","lou_hyphenate","lou_freeTableFiles","lou_setDataPath","lou_free"],"critical_calls":["lou_getDataPath","lou_setDataPath","lou_findTables","lou_dotsToChar","lou_backTranslateString","lou_hyphenate","lou_freeTableFiles","lou_setDataPath","lou_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Set up data path
    char *original_path = lou_getDataPath();
    char *temp_path = strdup("/tmp");
    char *set_result = lou_setDataPath(temp_path);
    free(temp_path);
    
    // Find available tables
    char **tables = lou_findTables(fuzz_str_1);
    if (!tables) {
        lou_setDataPath(original_path);
        return 0;
    }
    
    // Use first available table
    const char *tableList = tables[0];
    if (!tableList) {
        lou_freeTableFiles(tables);
        lou_setDataPath(original_path);
        return 0;
    }
    
    // Check if we have enough data for basic operations
    if (size < sizeof(widechar) * 2) {
        lou_freeTableFiles(tables);
        lou_setDataPath(original_path);
        return 0;
    }
    
    // Convert input to widechar array
    size_t widechar_count = size / sizeof(widechar);
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, size);
    
    // Prepare variables for various operations
    int inlen = static_cast<int>(widechar_count);
    std::vector<widechar> outbuf(widechar_count * 2);
    int outlen = static_cast<int>(outbuf.size());
    formtype *typeform = nullptr;
    char *spacing = nullptr;
    char *hyphens = new char[widechar_count + 1];
    memset(hyphens, 0, widechar_count + 1);
    
    // 1. Call lou_dotsToChar
    // This converts braille dots to characters
    std::vector<widechar> dotsToChar_out(widechar_count);
    lou_dotsToChar(tableList, inbuf.data(), dotsToChar_out.data(), 
                   static_cast<int>(widechar_count), fuzz_int32_t_2);
    
    // 2. Call lou_backTranslateString
    // This backtranslates from braille to text
    int back_inlen = inlen;
    int back_outlen = outlen;
    lou_backTranslateString(tableList, inbuf.data(), &back_inlen, 
                           outbuf.data(), &back_outlen, typeform, spacing, fuzz_int32_t_3);
    
    // 3. Call lou_hyphenate
    // This adds hyphenation information
    lou_hyphenate(tableList, inbuf.data(), inlen, hyphens, fuzz_int32_t_4);
    
    // Clean up
    delete[] hyphens;
    lou_freeTableFiles(tables);
    
    // Restore original data path
    lou_setDataPath(original_path);
    
    // Additional cleanup
    lou_free();
    
    return 0;
}