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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["lou_listTables","lou_freeTableFiles","lou_charSize","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_freeTableFiles","lou_charToDots","lou_dotsToChar","lou_findTable","lou_translate","lou_freeTableFile","lou_freeTableFiles","lou_free"],"critical_calls":["lou_listTables","lou_charSize","lou_getTypeformForEmphClass","lou_charToDots","lou_dotsToChar","lou_findTable","lou_translate","lou_freeTableFile","lou_freeTableFiles","lou_free"],"visited":0}
/**/


// Include necessary headers for liblouis
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
        return 0;
    }

    // 1. List available tables
    char **tables = lou_listTables();
    if (!tables) {
        return 0;
    }

    // Use the first available table if exists
    const char *table_to_use = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        if (tables[i][0] != '\0') {
            table_to_use = tables[i];
            break;
        }
    }

    if (!table_to_use) {
        lou_freeTableFiles(tables);
        return 0;
    }

    // 2. Get character size
    int char_size = lou_charSize();

    // 3. Get typeform for emphasis class
    // Use a simple emphasis class - check if "italic" exists
    formtype typeform = lou_getTypeformForEmphClass(table_to_use, "italic");
    // Note: We'll use this later in translate

    // 4. Prepare input buffer for conversion
    // We'll use the input data as wide characters
    // Calculate how many widechars we have
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        lou_freeTableFiles(tables);
        return 0;
    }

    // Create input buffer
    widechar *inbuf = (widechar *)data;  // Cast directly - data is const
    widechar *inbuf_copy = (widechar *)malloc(size);
    if (!inbuf_copy) {
        lou_freeTableFiles(tables);
        return 0;
    }
    memcpy(inbuf_copy, inbuf, size);

    // 5. Convert characters to dots (braille)
    widechar *dots_buf = (widechar *)malloc(widechar_count * sizeof(widechar));
    if (dots_buf) {
        int dots_result = lou_charToDots(
            table_to_use, 
            inbuf_copy, 
            dots_buf, 
            widechar_count, 
            0  // mode
        );
        (void)dots_result; // Use result to avoid unused variable warning
    }

    // 6. Convert dots back to characters
    if (dots_buf) {
        widechar *back_buf = (widechar *)malloc(widechar_count * sizeof(widechar));
        if (back_buf) {
            int back_result = lou_dotsToChar(
                table_to_use, 
                dots_buf, 
                back_buf, 
                widechar_count, 
                0  // mode
            );
            (void)back_result; // Use result to avoid unused variable warning
            free(back_buf);
        }
        free(dots_buf);
    }

    // 7. Find a table using query
    char *found_table = lou_findTable("en");
    if (found_table) {
        // Use the found table for translation if different
        const char *translation_table = found_table;
        
        // Prepare for translation
        int inlen = widechar_count;
        int outlen = widechar_count * 2; // Allow expansion
        widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        
        // Allocate arrays for positions
        int *outputPos = (int *)malloc(outlen * sizeof(int));
        int *inputPos = (int *)malloc(inlen * sizeof(int));
        int *cursorPos = (int *)malloc(inlen * sizeof(int));
        char *spacing = (char *)malloc(outlen * sizeof(char));
        
        if (outbuf && outputPos && inputPos && cursorPos && spacing) {
            // Initialize arrays
            memset(spacing, 0, outlen * sizeof(char));
            for (int i = 0; i < outlen; i++) outputPos[i] = i;
            for (int i = 0; i < inlen; i++) inputPos[i] = i;
            for (int i = 0; i < inlen; i++) cursorPos[i] = i;
            
            // Perform translation
            int translate_result = lou_translate(
                translation_table,
                inbuf_copy,
                &inlen,
                outbuf,
                &outlen,
                &typeform,  // Use the typeform we got earlier
                spacing,
                outputPos,
                inputPos,
                cursorPos,
                0  // mode
            );
            (void)translate_result; // Use result to avoid unused variable warning
        }
        
        // Cleanup
        if (outbuf) free(outbuf);
        if (outputPos) free(outputPos);
        if (inputPos) free(inputPos);
        if (cursorPos) free(cursorPos);
        if (spacing) free(spacing);
        
        // 8. Free the found table
        lou_freeTableFile(found_table);
    }

    // 9. Cleanup resources
    free(inbuf_copy);
    lou_freeTableFiles(tables);
    
    // 10. Final cleanup
    lou_free();

    return 0;
}