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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 3
//<Quality> {"density":4,"unique_branches":{"noCompbrlAhead":[[1827,43,1827,53,0,0,4,1]],"lou_hyphenate":[[4079,40,4079,55,0,0,4,0]],"lou_translatePrehyphenated":[[1416,6,1416,26,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_logFile","lou_getTableInfo","lou_logEnd","lou_translate","lou_hyphenate","lou_translatePrehyphenated","lou_logEnd"],"critical_calls":["lou_registerLogCallback","lou_logFile","lou_getTableInfo","lou_translate","lou_hyphenate","lou_translatePrehyphenated","lou_logEnd"],"visited":1}
/**/


// Include liblouis headers
extern "C" {
}

// Custom log callback function
static void  id_000004_log_callback(logLevels level, const char *message) {
    // Simple callback that does nothing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum size for processing
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Setup logging
    lou_registerLogCallback( id_000004_log_callback);
    lou_logFile("fuzz.log");
    
    // 2. Use a fixed table for consistency (en-us-g2.ctb is common in liblouis)
    const char *tableList = "en-us-g2.ctb";
    
    // 3. Get table info (call lou_getTableInfo)
    char *tableInfo = lou_getTableInfo(tableList, "locale");
    if (tableInfo) {
        free(tableInfo); // Free the returned string
    }
    
    // 4. Prepare input buffer (convert data to widechar)
    // Use half of size for widechars (since widechar is 2 bytes)
    int inlen = static_cast<int>(size / 2);
    if (inlen <= 0) {
        lou_logEnd();
        return 0;
    }
    
    std::vector<widechar> inbuf(inlen);
    memcpy(inbuf.data(), data, inlen * sizeof(widechar));
    
    // 5. First translation with lou_translate
    std::vector<widechar> outbuf1(inlen * 2); // Allocate larger output buffer
    int outlen1 = static_cast<int>(outbuf1.size());
    formtype *typeform = nullptr;
    char *spacing = nullptr;
    int *outputPos = nullptr;
    int *inputPos = nullptr;
    int *cursorPos = nullptr;
    
    int trans_result = lou_translate(
        tableList,
        inbuf.data(),
        &inlen,
        outbuf1.data(),
        &outlen1,
        typeform,
        spacing,
        outputPos,
        inputPos,
        cursorPos,
        0  // mode
    );
    
    // 6. Hyphenation with lou_hyphenate
    std::vector<char> hyphens(inlen);
    int hyphen_result = lou_hyphenate(
        tableList,
        inbuf.data(),
        inlen,
        hyphens.data(),
        0  // mode
    );
    
    // 7. Translate with hyphenation using lou_translatePrehyphenated
    std::vector<widechar> outbuf2(inlen * 2);
    int outlen2 = static_cast<int>(outbuf2.size());
    std::vector<char> inputHyphens(inlen);
    std::vector<char> outputHyphens(inlen * 2);
    
    // Initialize hyphens arrays
    for (int i = 0; i < inlen; i++) {
        inputHyphens[i] = (i % 3 == 0) ? '1' : '0'; // Simple pattern
    }
    
    int prehyphen_result = lou_translatePrehyphenated(
        tableList,
        inbuf.data(),
        &inlen,
        outbuf2.data(),
        &outlen2,
        typeform,
        spacing,
        outputPos,
        inputPos,
        cursorPos,
        inputHyphens.data(),
        outputHyphens.data(),
        0  // mode
    );
    
    // 8. Create a FILE* from input data for additional processing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some data from the file
        char read_buffer[100];
        fread(read_buffer, 1, sizeof(read_buffer), in_file);
        assert_file_closed(&in_file);;
    }
    
    // 9. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some results
        if (trans_result > 0) {
            fwrite(outbuf1.data(), sizeof(widechar), outlen1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 10. End logging
    lou_logEnd();
    
    // All APIs have been called at least once:
    // - lou_registerLogCallback
    // - lou_logFile
    // - lou_getTableInfo
    // - lou_translate
    // - lou_hyphenate
    // - lou_translatePrehyphenated
    // - lou_logEnd
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}