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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 4
//<Quality> {"density":6,"unique_branches":{"compileTable":[[5038,8,5038,63,0,0,4,0]],"analyzeTable":[[724,6,724,44,0,0,4,1]],"_lou_backTranslate":[[217,6,217,19,0,0,4,0]],"compileFile":[[4913,6,4913,44,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_registerTableResolver","lou_checkTable","lou_checkTable","lou_getTable","lou_indexTables","lou_backTranslate","lou_backTranslateString","lou_charToDots","lou_free"],"critical_calls":["lou_getDataPath","lou_registerTableResolver","lou_checkTable","lou_checkTable","lou_getTable","lou_indexTables","lou_backTranslate","lou_backTranslateString","lou_charToDots","lou_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to achieve back-translation of braille output data:

*/


extern "C" {
}

// Custom table resolver function
static char ** id_000009_custom_table_resolver(const char *table, const char *base) {
    // Simple resolver that just returns the table name as-is
    static char *table_array[2];
    static char table_name[256];
    
    if (table && strlen(table) < sizeof(table_name) - 1) {
        strcpy(table_name, table);
        table_array[0] = table_name;
        table_array[1] = nullptr;
        return table_array;
    }
    return nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
        return 0; // Need at least some data
    }

    // 1. Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Could use this path for table lookup if needed
    }

    // 2. Register custom table resolver
    lou_registerTableResolver( id_000009_custom_table_resolver);

    // 3. Prepare table list - use a simple built-in table
    const char *table_list = "en-us-g1.ctb";

    // 4. Check if the table is valid
    int table_check = lou_checkTable(table_list);
    if (table_check != 0) {
        // Table might not be available, try another
        table_list = "tests/tables/sample.ctb";
        table_check = lou_checkTable(table_list);
    }

    // 5. Get table handle
    const void *table_handle = lou_getTable(table_list);

    // 6. Index tables (requires a null-terminated array)
    const char *tables_to_index[] = {table_list, nullptr};
    lou_indexTables(tables_to_index);

    // 7. Prepare input buffer for back-translation
    // We'll interpret the input data as widechar (braille output)
    // Ensure we have enough data for at least a few characters
    if (size < sizeof(widechar) * 2) {
        return 0;
    }

    // Convert input to widechar array
    size_t max_widechars = size / sizeof(widechar);
    widechar *inbuf = (widechar *)data;
    int inlen = (max_widechars > 100) ? 100 : max_widechars; // Limit input size
    
    // Allocate output buffers
    int outlen = inlen * 2; // Back-translation might produce more characters
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
	return 0;
	}
	
    
    formtype *typeform = (formtype *)calloc(inlen, sizeof(formtype));
    char *spacing = (char *)calloc(inlen, sizeof(char));
    
    // Position arrays
    int *outputPos = (int *)calloc(inlen, sizeof(int));
    int *inputPos = (int *)calloc(inlen, sizeof(int));
    int *cursorPos = (int *)calloc(inlen, sizeof(int));

    // 8. Perform back-translation using both APIs
    int back_translate_result = 0;
    int back_translate_string_result = 0;
    
    if (outbuf && typeform && spacing && outputPos && inputPos && cursorPos) {
        // Reset lengths for each call
        int inlen1 = inlen;
        int outlen1 = outlen;
        
        // Try lou_backTranslate
        back_translate_result = lou_backTranslate(
            table_list, 
            inbuf, 
            &inlen1, 
            outbuf, 
            &outlen1, 
            typeform, 
            spacing, 
            outputPos, 
            inputPos, 
            cursorPos, 
            0
        );

        // Reset for next call
        int inlen2 = inlen;
        int outlen2 = outlen;
        
        // Try lou_backTranslateString
        back_translate_string_result = lou_backTranslateString(
            table_list,
            inbuf,
            &inlen2,
            outbuf,
            &outlen2,
            typeform,
            spacing,
            0
        );

        // 9. If back-translation succeeded, try to convert to dots
        if (back_translate_result > 0 || back_translate_string_result > 0) {
            // Use the output from back-translation as input to charToDots
            int dots_length = (back_translate_result > 0) ? outlen1 : outlen2;
            if (dots_length > 0) {
                widechar *dots_buf = (widechar *)malloc(dots_length * sizeof(widechar));
                if (dots_buf) {
                    int dots_result = lou_charToDots(
                        table_list,
                        outbuf,
                        dots_buf,
                        dots_length,
                        0
                    );
                    free(dots_buf);
                }
            }
        }
    }

    // 10. Clean up allocated resources
    if (outbuf) free(outbuf);
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
    if (cursorPos) free(cursorPos);

    // 11. Free liblouis resources
    lou_free();

    return 0;
}