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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 10
//<Quality> {"density":5,"unique_branches":{"lou_getEmphClasses":[[5090,6,5090,12,0,0,4,1],[5092,9,5092,33,0,0,4,0],[5094,7,5094,12,0,0,4,0],[5094,7,5094,12,0,0,4,1],[5102,7,5102,14,0,0,4,1]],"lou_findTable":[[1069,6,1069,17,0,0,4,0]],"getTable":[[5217,27,5217,52,0,0,4,1]],"compileRule":[[2958,7,2958,20,0,0,4,0],[3066,7,3066,13,0,0,4,0]],"_lou_translate":[[1250,30,1250,48,0,0,4,0]]},"library_calls":["lou_logFile","lou_findTable","lou_getEmphClasses","lou_translateString","lou_freeEmphClasses","lou_freeTableFile","lou_logEnd","lou_free"],"critical_calls":["lou_logFile","lou_findTable","lou_getEmphClasses","lou_translateString","lou_freeEmphClasses","lou_freeTableFile","lou_logEnd","lou_free"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 2) {
	return 0;
	}
	

    // 1. Set up logging to a file
    lou_logFile(fuzz_str_1);

    // 2. Find a braille table using part of input as query
    // Use first min(100, size/2) bytes as query string
    size_t query_len = (size < 200) ? size / 2 : 100;
    std::vector<char> query(query_len + 1);
    for (size_t i = 0; i < query_len; i++) {
        query[i] = (char)(data[i] % 128); // Ensure ASCII range
    }
    query[query_len] = '\0';
    
    char *found_table = lou_findTable(query.data());
    
    // 3. Get emphasis classes if we found a table
    const char **emph_classes = nullptr;
    const char *table_to_use = "en-us-g2.ctb"; // Default fallback table
    if (found_table) {
        table_to_use = found_table;
    }
    
    emph_classes = lou_getEmphClasses(fuzz_str_2);
    
    // 4. Prepare input for translation
    // Convert input data to widechar string for translation
    // Use data from position query_len to end
    size_t translate_start = query_len;
    if (translate_start >= size) translate_start = 0;
    
    size_t input_len = size - translate_start;
    if (input_len > 0) {
        // Allocate buffers for translation
        std::vector<widechar> inbuf(input_len);
        std::vector<widechar> outbuf(input_len * 2); // Output might be larger
        
        // Convert bytes to widechars
        for (size_t i = 0; i < input_len; i++) {
            inbuf[i] = (widechar)data[translate_start + i];
        }
        
        int inlen = (int)input_len;
        int outlen = (int)outbuf.size();
        formtype *typeform = nullptr;
        char *spacing = nullptr;
        int mode = 0;
        
        // 5. Perform translation
        lou_translateString(fuzz_str_3, 
                           inbuf.data(), 
                           &inlen, 
                           outbuf.data(), 
                           &outlen, 
                           typeform, 
                           spacing, 
                           fuzz_int32_t_4);
    }
    
    // 6. Clean up resources
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // 7. End logging
    lou_logEnd();
    
    // Clean up any other resources
    lou_free();
    
    return 0;
}