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
//<ID> 915
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"back_selectRule":[[885,29,885,62,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_listTables","lou_freeTableFiles","lou_findTable","lou_checkTable","lou_compileString","lou_logPrint","lou_freeTableFile","lou_translateString","lou_hyphenate","lou_backTranslate","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerLogCallback","lou_listTables","lou_freeTableFiles","lou_findTable","lou_checkTable","lou_compileString","lou_logPrint","lou_translateString","lou_hyphenate","lou_backTranslate","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Register a log callback first
    static bool callback_registered = false;
    if (!callback_registered) {
        lou_registerLogCallback([](logLevels level, const char *msg) {
            // Simple callback that does nothing
        });
        callback_registered = true;
    }
    
    // 2. List available tables
    char **tables = lou_listTables();
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // 3. Find a table using part of input as query
    // Use first 32 bytes or less as query string
    size_t query_len = size > 32 ? 32 : size;
    char *query = (char *)malloc(query_len + 1);
    if (!query) {
	return 0;
	}
	
    
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    char *found_table = lou_findTable(query);
    free(query);
    
    // 4. Check if a table was found and is valid
    const char *table_to_use = "en-us-g2.ctb"; // Default fallback
    if (found_table && lou_checkTable(found_table) == 0) {
        table_to_use = found_table;
    }
    
    // 5. Compile a table string (using a simple test pattern)
    const char *compile_string = "table test\n";
    lou_compileString(table_to_use, fuzz_str_1);
    
    // 6. Log a message
    lou_logPrint("Fuzzing liblouis");
    
    // 7. Prepare input buffer for translation
    // Convert data to widechar (assuming UTF-16 input)
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        if (found_table) lou_freeTableFile(found_table);
        return 0;
    }
    
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));
    
    // 8. Perform translation
    int inlen = static_cast<int>(widechar_count);
    std::vector<widechar> outbuf(inlen * 2); // Allocate larger buffer for output
    int outlen = static_cast<int>(outbuf.size());
    formtype *typeform = nullptr;
    char *spacing = nullptr;
    int mode = 0;
    
    int translate_result = lou_translateString(
        fuzz_str_2,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform,
        spacing,
        fuzz_int32_t_3
    );
    
    // 9. Perform hyphenation on the input
    if (translate_result != 0) {
        std::vector<char> hyphens(widechar_count);
        int hyphen_result = lou_hyphenate(
            fuzz_str_4,
            inbuf.data(),
            inlen,
            hyphens.data(),
            fuzz_int32_t_5
        );
    }
    
    // 10. Perform back translation
    if (translate_result != 0 && outlen > 0) {
        int back_inlen = outlen;
        std::vector<widechar> back_outbuf(back_inlen * 2);
        int back_outlen = static_cast<int>(back_outbuf.size());
        int *outputPos = nullptr;
        int *inputPos = nullptr;
        int *cursorPos = nullptr;
        
        int back_result = lou_backTranslate(
            fuzz_str_6,
            outbuf.data(),
            &back_inlen,
            back_outbuf.data(),
            &back_outlen,
            typeform,
            spacing,
            outputPos,
            inputPos,
            cursorPos,
            fuzz_int32_t_7
        );
    }
    
    // 11. Clean up
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // 12. Free liblouis resources
    lou_free();
    
    return 0;
}