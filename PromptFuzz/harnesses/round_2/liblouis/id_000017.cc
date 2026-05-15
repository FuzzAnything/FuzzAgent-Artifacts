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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 6
//<Quality> {"density":5,"unique_branches":{"parseQuery":[[613,10,613,45,0,0,4,0],[645,6,645,19,0,0,4,1]],"lou_findTable":[[1077,7,1077,23,0,0,4,0],[1079,8,1079,17,0,0,4,0],[1079,8,1079,17,0,0,4,1],[1084,6,1084,15,0,0,4,0]]},"library_calls":["lou_findTable","lou_registerTableResolver","lou_getTypeformForEmphClass","lou_translate","lou_hyphenate","lou_translatePrehyphenated","lou_backTranslate","lou_freeTableFile","lou_free"],"critical_calls":["lou_findTable","lou_registerTableResolver","lou_getTypeformForEmphClass","lou_translate","lou_hyphenate","lou_translatePrehyphenated","lou_backTranslate","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Include liblouis headers
extern "C" {
}

// Custom table resolver function
static char ** id_000017_custom_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns NULL (use default behavior)
    (void)table;
    (void)base;
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Parse input data structure:
    // [2 bytes: inlen] [inlen*2 bytes: widechar input] [rest: table name]
    size_t offset = 0;
    if (offset + 2 > size) {
	return 0;
	}
	
    
    uint16_t inlen_raw;
    memcpy(&inlen_raw, data + offset, 2);
    offset += 2;
    
    // Limit input length for safety
    int inlen = inlen_raw % 256;
    if (inlen <= 0) inlen = 1;
    
    if (offset + inlen * sizeof(widechar) > size) {
        return 0;
    }
    
    // Extract widechar input
    std::vector<widechar> inbuf(inlen);
    memcpy(inbuf.data(), data + offset, inlen * sizeof(widechar));
    offset += inlen * sizeof(widechar);
    
    // Extract table name (null-terminated string)
    if (offset >= size) {
	return 0;
	}
	
    std::string table_query(reinterpret_cast<const char*>(data + offset), 
                           size - offset);
    if (table_query.empty()) {
        table_query = "en-us-g2.ctb";
    }
    
    // Find table using lou_findTable
    char *found_table = lou_findTable(table_query.c_str());
    const char *table_to_use = found_table ? found_table : "en-us-g2.ctb";
    
    // Register table resolver
    lou_registerTableResolver( id_000017_custom_table_resolver);
    
    // Get typeform for emphasis class
    formtype typeform = lou_getTypeformForEmphClass(table_to_use, "emphasis");
    
    // Prepare buffers for translation
    int outlen = inlen * 2;
    std::vector<widechar> outbuf(outlen);
    std::vector<char> spacing(inlen);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(inlen);
    int cursorPos = 0;
    int mode = 0;
    
    // Call lou_translate
    int translate_result = lou_translate(
        table_to_use,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform,
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        &cursorPos,
        mode
    );
    
    // Prepare hyphenation arrays
    std::vector<char> hyphens(inlen);
    std::vector<char> inputHyphens(inlen);
    std::vector<char> outputHyphens(outlen);
    
    // Call lou_hyphenate
    int hyphenate_result = lou_hyphenate(
        table_to_use,
        inbuf.data(),
        inlen,
        hyphens.data(),
        mode
    );
    
    // Call lou_translatePrehyphenated
    int prehyphenated_result = lou_translatePrehyphenated(
        table_to_use,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform,
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        &cursorPos,
        inputHyphens.data(),
        outputHyphens.data(),
        mode
    );
    
    // Prepare for back translation
    int back_inlen = outlen;
    std::vector<widechar> back_outbuf(inlen);
    int back_outlen = inlen;
    std::vector<int> back_outputPos(back_outlen);
    std::vector<int> back_inputPos(back_inlen);
    int back_cursorPos = 0;
    
    // Call lou_backTranslate
    int backtranslate_result = lou_backTranslate(
        table_to_use,
        outbuf.data(),
        &back_inlen,
        back_outbuf.data(),
        &back_outlen,
        &typeform,
        spacing.data(),
        back_outputPos.data(),
        back_inputPos.data(),
        &back_cursorPos,
        mode
    );
    
    // Cleanup
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // Free any internal resources
    lou_free();
    
    return 0;
}