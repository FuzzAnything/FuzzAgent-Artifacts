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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"listDir":[[1005,6,1005,30,0,0,4,1]],"listFiles":[[1032,46,1032,72,0,0,4,1],[1039,7,1039,30,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_setDataPath","lou_findTables","lou_setDataPath","lou_freeTableFiles","lou_setDataPath","lou_freeTableFiles","lou_setDataPath","lou_dotsToChar","lou_backTranslateString","lou_hyphenate","lou_freeTableFiles","lou_setDataPath"],"critical_calls":["lou_getDataPath","lou_setDataPath","lou_findTables","lou_dotsToChar","lou_backTranslateString","lou_hyphenate","lou_freeTableFiles","lou_setDataPath"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs in a sequence to achieve a realistic event:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	  // Need at least some data to process

    // 1. Get current data path
    char *original_path = lou_getDataPath();
    
    // 2. Temporarily set data path to a test location
    lou_setDataPath(fuzz_str_1);
    
    // 3. Find available tables using a query
    char **tables = lou_findTables("en-*");
    if (!tables) {
        lou_setDataPath(original_path);
        return 0;
    }
    
    // Use the first found table if available
    const char *table_to_use = tables[0] ? tables[0] : "en-us-comp8.ctb";
    
    // 4. Process input data as dots (braille) to convert to characters
    // The input data is treated as widechar dots
    int dots_length = size / 2;  // Each widechar is 2 bytes
    if (dots_length == 0) {
        lou_freeTableFiles(tables);
        lou_setDataPath(original_path);
        return 0;
    }
    
    // Allocate buffers
    widechar *dots_input = static_cast<widechar*>(malloc(size));
    widechar *chars_output = static_cast<widechar*>(malloc(size * 2));
    
    if (!dots_input || !chars_output) {
        free(dots_input);
        free(chars_output);
        lou_freeTableFiles(tables);
        lou_setDataPath(original_path);
        return 0;
    }
    
    // Copy input data as dots (widechar)
    memcpy(dots_input, data, size);
    
    // 5. Convert dots to characters
    int chars_result = lou_dotsToChar(table_to_use, dots_input, chars_output, dots_length, fuzz_int32_t_2);
    
    if (chars_result > 0) {
        int chars_length = chars_result;
        
        // 6. Back-translate the characters to dots
        int inlen = chars_length;
        int outlen = size / sizeof(widechar);
        widechar *backtrans_output = static_cast<widechar*>(malloc(size));
        formtype *typeform = static_cast<formtype*>(calloc(chars_length, sizeof(formtype)));
        char *spacing = static_cast<char*>(calloc(chars_length, sizeof(char)));
        
        if (backtrans_output && typeform && spacing) {
            int backtrans_result = lou_backTranslateString(
                table_to_use,
                chars_output,
                &inlen,
                backtrans_output,
                &outlen,
                typeform,
                spacing,
                fuzz_int32_t_3
            );
            
            // 7. Hyphenate the original dots input
            if (backtrans_result > 0) {
                char *hyphens = static_cast<char*>(malloc(dots_length));
                if (hyphens) {
                    int hyphen_result = lou_hyphenate(
                        table_to_use,
                        dots_input,
                        dots_length,
                        hyphens,
                        fuzz_int32_t_4
                    );
                    free(hyphens);
                }
            }
        }
        
        free(backtrans_output);
        free(typeform);
        free(spacing);
    }
    
    // Cleanup
    free(dots_input);
    free(chars_output);
    lou_freeTableFiles(tables);
    
    // Restore original data path
    lou_setDataPath(original_path);
    
    return 0;
}