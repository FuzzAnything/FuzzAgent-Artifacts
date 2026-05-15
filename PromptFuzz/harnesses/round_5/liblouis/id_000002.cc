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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 1, nr_unique_branch: 0
//<Quality> {"density":2,"unique_branches":{},"library_calls":["lou_charSize","lou_getTypeformForEmphClass","lou_charToDots","lou_findTable","lou_freeTableFile","lou_free"],"critical_calls":["lou_charSize","lou_getTypeformForEmphClass","lou_charToDots","lou_findTable","lou_freeTableFile","lou_free"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize output buffer for dots
    const int max_length = size / sizeof(widechar);
    if (max_length == 0) {
        return 0;
    }
    
    widechar *outbuf = (widechar *)malloc(max_length * sizeof(widechar));
    if (!outbuf) {
        return 0;
    }
    
    // Create input buffer from data
    widechar *inbuf = (widechar *)malloc(max_length * sizeof(widechar));
    if (!inbuf) {
        free(outbuf);
        return 0;
    }
    
    // Convert byte stream to widechar array
    size_t widechar_count = size / sizeof(widechar);
    for (size_t i = 0; i < widechar_count && i < max_length; i++) {
        inbuf[i] = ((widechar)data[i * sizeof(widechar)] << 8) | 
                   (i * sizeof(widechar) + 1 < size ? data[i * sizeof(widechar) + 1] : 0);
    }
    
    // Call lou_charSize()
    int char_size = lou_charSize();
    
    // Use a fixed table list for conversion
    const char *tableList = "en-us-g2.ctb";
    
    // First, try to get typeform for emphasis class
    formtype typeform = lou_getTypeformForEmphClass(tableList, "italic");
    
    // Call lou_charToDots() with the input data
    int result = lou_charToDots(tableList, inbuf, outbuf, 
                                (int)widechar_count, 0);
    
    // Try to find and free a table file
    char *table_file = lou_findTable("en-us-g2.ctb");
    if (table_file) {
        lou_freeTableFile(table_file);
    }
    
    // Clean up
    free(inbuf);
    free(outbuf);
    
    // Call lou_free() at the end
    lou_free();
    
    return 0;
}