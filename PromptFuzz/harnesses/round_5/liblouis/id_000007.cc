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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":2,"unique_branches":{},"library_calls":["lou_setDataPath","lou_registerTableResolver","lou_version","lou_free","lou_findTables","lou_compileString","lou_hyphenate","lou_dotsToChar","lou_freeTableFiles","lou_free"],"critical_calls":["lou_setDataPath","lou_registerTableResolver","lou_version","lou_findTables","lou_compileString","lou_hyphenate","lou_dotsToChar","lou_freeTableFiles","lou_free"],"visited":0}
/**/


// Include necessary headers for liblouis

// Dummy table resolver function
static char ** id_000007_dummy_table_resolver(const char *table, const char *base) {
    // For fuzzing, just return NULL
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Set data path (use current directory as a safe default)
    char *old_path = lou_setDataPath(".");
    
    // 2. Register table resolver
    lou_registerTableResolver( id_000007_dummy_table_resolver);
    
    // 3. Get version
    const char *version = lou_version();
    
    // 4. Find tables using part of input as query
    // Ensure we have at least 1 byte for query
    size_t query_len = size > 100 ? 100 : size;
    char *query = (char *)malloc(query_len + 1);
    if (!query) {
        lou_free();
        return 0;
    }
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    char **tables = lou_findTables(query);
    free(query);
    
    // Create a simple table list for other operations
    const char *table_list = "en-us-g2.ctb";
    
    // 5. Compile a table string using part of input
    // Use a different segment of data for compilation
    size_t compile_offset = query_len;
    if (compile_offset < size) {
        size_t compile_len = size - compile_offset;
        if (compile_len > 0) {
            char *compile_string = (char *)malloc(compile_len + 1);
            if (compile_string) {
                memcpy(compile_string, data + compile_offset, compile_len);
                compile_string[compile_len] = '\0';
                
                // Try to compile (may fail with arbitrary input, that's OK)
                lou_compileString(table_list, compile_string);
                
                free(compile_string);
            }
        }
    }
    
    // 6. Hyphenate using part of input as text
    // We'll use the middle section for text input
    size_t text_offset = size / 3;
    if (text_offset < size) {
        size_t text_len = size - text_offset;
        if (text_len > 0) {
            // Convert bytes to widechars (2 bytes per widechar)
            int widechar_len = text_len / sizeof(widechar);
            if (widechar_len > 0) {
                widechar *inbuf = (widechar *)malloc(widechar_len * sizeof(widechar));
                char *hyphens = (char *)malloc(widechar_len);
                
                if (inbuf && hyphens) {
                    // Copy data as widechars
                    memcpy(inbuf, data + text_offset, widechar_len * sizeof(widechar));
                    
                    // Try hyphenation
                    lou_hyphenate(table_list, inbuf, widechar_len, hyphens, 0);
                    
                    free(inbuf);
                    free(hyphens);
                } else {
                    if (inbuf) free(inbuf);
                    if (hyphens) free(hyphens);
                }
            }
        }
    }
    
    // 7. Dots to char conversion
    // Use the last section for dots input
    size_t dots_offset = (size * 2) / 3;
    if (dots_offset < size) {
        size_t dots_len = size - dots_offset;
        if (dots_len > 0) {
            int dots_widechar_len = dots_len / sizeof(widechar);
            if (dots_widechar_len > 0) {
                widechar *dots_in = (widechar *)malloc(dots_widechar_len * sizeof(widechar));
                widechar *dots_out = (widechar *)malloc(dots_widechar_len * sizeof(widechar));
                
                if (dots_in && dots_out) {
                    memcpy(dots_in, data + dots_offset, dots_widechar_len * sizeof(widechar));
                    
                    // Try dots to char conversion
                    lou_dotsToChar(table_list, dots_in, dots_out, dots_widechar_len, 0);
                    
                    free(dots_in);
                    free(dots_out);
                } else {
                    if (dots_in) free(dots_in);
                    if (dots_out) free(dots_out);
                }
            }
        }
    }
    
    // Clean up
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Free any remaining resources
    lou_free();
    
    return 0;
}