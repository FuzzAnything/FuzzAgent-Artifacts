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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"parseCharsInternal":[[1377,24,1377,38,0,0,4,0]],"compileTable":[[5058,9,5058,56,0,0,4,0]],"compileCharDef":[[2651,6,2651,41,0,0,4,0]]},"library_calls":["lou_listTables","lou_checkTable","lou_freeTableFiles","lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_freeTableFiles","lou_freeTableFiles","lou_translateString","lou_translatePrehyphenated","lou_freeTableFiles","lou_free"],"critical_calls":["lou_listTables","lou_checkTable","lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_translateString","lou_translatePrehyphenated","lou_freeTableFiles","lou_free"],"visited":0}
/*Here's a fuzzer that uses the specified liblouis APIs:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // 1. List available tables
    char **tables = lou_listTables();
    if (!tables) {
	return 0;
	}
	
    
    // Find a valid table to use
    const char *table_to_use = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        if (lou_checkTable(tables[i]) == 0) {
            table_to_use = tables[i];
            break;
        }
    }
    
    if (!table_to_use) {
        lou_freeTableFiles(tables);
        return 0;
    }
    
    // 2. Find tables using query from input data
    size_t query_len = std::min(size, (size_t)50);
    std::vector<char> query_buf(query_len + 1);
    memcpy(query_buf.data(), data, query_len);
    query_buf[query_len] = '\0';
    
    char **found_tables = lou_findTables(query_buf.data());
    lou_freeTableFiles(found_tables);
    
    // 3. Get emphasis classes
    const char **emph_classes = lou_getEmphClasses(table_to_use);
    if (emph_classes) {
        // 4. Get typeform for an emphasis class if available
        for (int i = 0; emph_classes[i] != nullptr; i++) {
            if (strlen(emph_classes[i]) > 0) {
                formtype typeform = lou_getTypeformForEmphClass(table_to_use, emph_classes[i]);
                (void)typeform; // Use to avoid unused variable warning
                break;
            }
        }
        lou_freeEmphClasses(emph_classes);
    }
    
    // 5. Prepare input buffer for translation
    size_t usable_size = size - query_len;
    if (usable_size < 4) {
        lou_freeTableFiles(tables);
        return 0;
    }
    
    // Convert input bytes to widechar array
    size_t max_chars = usable_size / sizeof(widechar);
    if (max_chars == 0) {
        lou_freeTableFiles(tables);
        return 0;
    }
    
    std::vector<widechar> inbuf(max_chars);
    size_t chars_to_copy = std::min(max_chars, usable_size / sizeof(widechar));
    memcpy(inbuf.data(), data + query_len, chars_to_copy * sizeof(widechar));
    
    int inlen = static_cast<int>(chars_to_copy);
    int outlen = inlen * 2; // Allocate double for safety
    
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform_buf(inlen);
    std::vector<char> spacing_buf(inlen);
    
    // Initialize buffers
    for (int i = 0; i < inlen; i++) {
        typeform_buf[i] = 0;
        spacing_buf[i] = 0;
    }
    
    // 6. Translate string
    int result1 = lou_translateString(
        table_to_use,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform_buf.data(),
        spacing_buf.data(),
        0
    );
    
    // 7. Prepare for pre-hyphenated translation
    if (result1 > 0 && outlen > 0) {
        // Reset inlen to original value
        inlen = static_cast<int>(chars_to_copy);
        int outlen2 = outlen;
        
        std::vector<int> outputPos(outlen2);
        std::vector<int> inputPos(inlen);
        std::vector<int> cursorPos(inlen);
        std::vector<char> inputHyphens(inlen);
        std::vector<char> outputHyphens(outlen2);
        
        // Initialize hyphen arrays
        for (int i = 0; i < inlen; i++) {
            inputHyphens[i] = (i % 3 == 0) ? '1' : '0';
        }
        for (int i = 0; i < outlen2; i++) {
            outputHyphens[i] = '0';
        }
        
        // 8. Translate pre-hyphenated
        int result2 = lou_translatePrehyphenated(
            table_to_use,
            inbuf.data(),
            &inlen,
            outbuf.data(),
            &outlen2,
            typeform_buf.data(),
            spacing_buf.data(),
            outputPos.data(),
            inputPos.data(),
            cursorPos.data(),
            inputHyphens.data(),
            outputHyphens.data(),
            0
        );
        
        (void)result2; // Use to avoid unused variable warning
    }
    
    // 9. Clean up
    lou_freeTableFiles(tables);
    lou_free();
    
    return 0;
}