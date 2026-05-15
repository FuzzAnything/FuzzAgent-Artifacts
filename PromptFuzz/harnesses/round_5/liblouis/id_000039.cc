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
//<ID> 771
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"for_selectRule":[[2229,12,2229,40,0,0,4,0]]},"library_calls":["lou_indexTables","lou_getTable","lou_logFile","lou_getTableInfo","lou_freeTableInfo","lou_checkTable","lou_translateString","lou_dotsToChar","lou_backTranslate","lou_logEnd","lou_free"],"critical_calls":["lou_indexTables","lou_getTable","lou_logFile","lou_getTableInfo","lou_freeTableInfo","lou_checkTable","lou_translateString","lou_dotsToChar","lou_backTranslate","lou_logEnd","lou_free"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Initialize resources
    char *table_info = nullptr;
    const void *table_handle = nullptr;
    widechar *outbuf = nullptr;
    widechar *back_outbuf = nullptr;
    char *spacing = nullptr;
    formtype *typeform = nullptr;
    int *outputPos = nullptr;
    int *inputPos = nullptr;
    int *cursorPos = nullptr;
    
    // 1. Index tables
    const char *tables[] = {"en-us-g2.ctb", "en-ueb-g2.ctb", nullptr};
    lou_indexTables(tables);
    
    // 2. Get table handle
    const char *table_list = "en-us-g2.ctb";
    table_handle = lou_getTable(table_list);
    
    // 3. Set log file
    lou_logFile("fuzz.log");
    
    // 4. Get table info
    table_info = lou_getTableInfo(table_list, "locale");
    if (table_info) {
        lou_freeTableInfo(table_info);
        table_info = nullptr;
    }
    
    // 5. Check table validity
    int table_check = lou_checkTable(table_list);
    
    // Parse input data for translation
    // First 2 bytes: input length for translation (max 1024)
    uint16_t trans_len = 0;
    if (size >= 2) {
        trans_len = (data[0] << 8) | data[1];
        trans_len = trans_len % 1024; // Limit to reasonable size
    }
    
    // Calculate remaining data for translation input
    size_t trans_data_start = 2;
    size_t trans_data_size = (size - trans_data_start) / sizeof(widechar);
    if (trans_data_size > trans_len) {
        trans_data_size = trans_len;
    }
    
    if (trans_data_size > 0) {
        // Prepare input buffer for translation
        std::vector<widechar> inbuf(trans_data_size);
        memcpy(inbuf.data(), data + trans_data_start, trans_data_size * sizeof(widechar));
        
        // Allocate output buffer (2x input size as per liblouis convention)
        outbuf = new widechar[trans_data_size * 2];
        int outlen = trans_data_size * 2;
        int inlen = trans_data_size;
        
        // 6. Translate string
        lou_translateString(table_list, inbuf.data(), &inlen, 
                           outbuf, &outlen, typeform, spacing, 0);
        
        // 7. Dots to char conversion
        // Use the translated output as input for dotsToChar
        if (outlen > 0) {
            widechar *dots_outbuf = new widechar[outlen];
            int dots_result = lou_dotsToChar(table_list, outbuf, dots_outbuf, outlen, 0);
            delete[] dots_outbuf;
        }
        
        // 8. Back translation
        // Use original input for back translation
        if (trans_data_size > 0) {
            int back_inlen = trans_data_size;
            int back_outlen = trans_data_size * 2;
            back_outbuf = new widechar[back_outlen];
            
            lou_backTranslate(table_list, inbuf.data(), &back_inlen, 
                             back_outbuf, &back_outlen, typeform, spacing,
                             outputPos, inputPos, cursorPos, 0);
        }
    }
    
    // 9. Clean up
    if (outbuf) {
        delete[] outbuf;
    }
    if (back_outbuf) {
        delete[] back_outbuf;
    }
    
    // End logging and free resources
    lou_logEnd();
    lou_free();
    
    return 0;
}