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
//<ID> 598
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"analyzeTable":[[713,7,713,12,0,0,4,0]]},"library_calls":["lou_registerTableResolver","lou_findTables","lou_findTable","lou_getTableInfo","lou_getTypeformForEmphClass","lou_translate","lou_getTable","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_logFile","lou_logPrint","lou_logEnd","lou_freeTableInfo","lou_freeTableFiles","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerTableResolver","lou_findTables","lou_findTable","lou_getTableInfo","lou_getTypeformForEmphClass","lou_translate","lou_getTable","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_logFile","lou_logPrint","lou_logEnd","lou_freeTableInfo","lou_freeTableFiles","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Custom table resolver function
extern "C" char **custom_resolver(const char *table, const char *base) {
    // Return a static list of tables
    static char *tables[] = {
        (char *)"en-us-g2.ctb",
        (char *)"en-ueb-g2.ctb",
        (char *)"de-de-g2.ctb",
        nullptr
    };
    return tables;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Use input data as FILE* using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use to avoid unused variable warning
    
    // 3. Register custom table resolver
    lou_registerTableResolver(custom_resolver);
    
    // 4. Find tables using part of input data as query
    char *query = (char *)malloc(size/2 + 1);
    if (!query) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    memcpy(query, data, size/2);
    query[size/2] = '\0';
    
    char **tables = lou_findTables(query);
    
    // 5. Find individual table
    char *found_table = lou_findTable(query);
    
    // 6. Get table information
    char *table_info = nullptr;
    if (found_table) {
        table_info = lou_getTableInfo(found_table, "language");
    }
    
    // 7. Get typeform for emphasis class
    formtype typeform = 0;
    if (found_table) {
        // Use part of data as emphasis class
        char emph_class[50] = {0};
        size_t emph_size = size < 50 ? size : 50;
        memcpy(emph_class, data + size/2, emph_size);
        typeform = lou_getTypeformForEmphClass(found_table, emph_class);
    }
    
    // 8. Prepare translation buffers using input data
    widechar *inbuf = nullptr;
    widechar *outbuf = nullptr;
    int *inlen = nullptr;
    int *outlen = nullptr;
    formtype *typeform_arr = nullptr;
    char *spacing = nullptr;
    int *outputPos = nullptr;
    int *inputPos = nullptr;
    int *cursorPos = nullptr;
    
    if (size >= 20 && found_table) {
        // Convert input data to widechar for translation
        size_t widechar_count = size / sizeof(widechar);
        if (widechar_count > 0) {
            inbuf = (widechar *)malloc(widechar_count * sizeof(widechar));
            outbuf = (widechar *)malloc(widechar_count * 2 * sizeof(widechar));
            inlen = (int *)malloc(sizeof(int));
            outlen = (int *)malloc(sizeof(int));
            typeform_arr = (formtype *)malloc(widechar_count * sizeof(formtype));
            spacing = (char *)malloc(widechar_count * 2 * sizeof(char));
            outputPos = (int *)malloc(widechar_count * 2 * sizeof(int));
            inputPos = (int *)malloc(widechar_count * sizeof(int));
            cursorPos = (int *)malloc(sizeof(int));
            
            if (inbuf && outbuf && inlen && outlen && typeform_arr && 
                spacing && outputPos && inputPos && cursorPos) {
                
                // Copy data as widechar
                memcpy(inbuf, data, widechar_count * sizeof(widechar));
                *inlen = (int)widechar_count;
                *outlen = (int)(widechar_count * 2);
                
                // Initialize arrays
                for (size_t i = 0; i < widechar_count; i++) {
                    typeform_arr[i] = typeform;
                    spacing[i] = 0;
                }
                
                // 9. Perform translation
                lou_translate(found_table, inbuf, inlen, outbuf, outlen, 
                            typeform_arr, spacing, outputPos, inputPos, 
                            cursorPos, 0);
                
                // 10. Get internal table structure
                const void *internal_table = lou_getTable(found_table);
                (void)internal_table; // Use to avoid warning
                
                // 11. Perform charToDots conversion
                widechar *dots_buf = (widechar *)malloc(widechar_count * sizeof(widechar));
                if (dots_buf) {
                    lou_charToDots(found_table, inbuf, dots_buf, *inlen, 0);
                    
                    // 12. Perform dotsToChar conversion
                    widechar *back_buf = (widechar *)malloc(widechar_count * sizeof(widechar));
                    if (back_buf) {
                        lou_dotsToChar(found_table, dots_buf, back_buf, *inlen, 0);
                        free(back_buf);
                    }
                    free(dots_buf);
                }
                
                // 13. Perform hyphenation
                char *hyphens = (char *)malloc(widechar_count * sizeof(char));
                if (hyphens) {
                    lou_hyphenate(found_table, inbuf, *inlen, hyphens, 0);
                    free(hyphens);
                }
            }
        }
    }
    
    // 14. Set log file and perform logging
    lou_logFile("liblouis_fuzz_log.txt");
    lou_logPrint("Fuzzing test completed");
    lou_logEnd();
    
    // 15. Clean up all allocated resources
    if (table_info) lou_freeTableInfo(table_info);
    if (tables) lou_freeTableFiles(tables);
    if (found_table) lou_freeTableFile(found_table);
    
    free(query);
    assert_file_closed(&in_file);;
    
    // Free translation buffers
    if (inbuf) free(inbuf);
    if (outbuf) free(outbuf);
    if (inlen) free(inlen);
    if (outlen) free(outlen);
    if (typeform_arr) free(typeform_arr);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
    if (cursorPos) free(cursorPos);
    
    // 16. Free library resources
    lou_free();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}