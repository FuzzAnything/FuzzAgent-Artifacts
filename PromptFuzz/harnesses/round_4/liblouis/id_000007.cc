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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"parseQuery":[[613,10,613,45,0,0,4,0],[645,6,645,19,0,0,4,1]]},"library_calls":["lou_compileString","lou_getEmphClasses","lou_freeEmphClasses","lou_findTable","lou_freeTableFile","lou_getTableInfo","lou_freeTableInfo","lou_logEnd","lou_logEnd","lou_logEnd","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_translate","lou_logEnd"],"critical_calls":["lou_compileString","lou_getEmphClasses","lou_freeEmphClasses","lou_findTable","lou_freeTableFile","lou_getTableInfo","lou_freeTableInfo","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_translate","lou_logEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<121) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 4) {
        return 0;
    }

    // 1. Create a table from input data using lou_compileString
    // Use first part of data as table string
    size_t table_len = size / 4;
    char *table_str = (char *)malloc(table_len + 1);
    if (!table_str) {
        return 0;
    }
    memcpy(table_str, data, table_len);
    table_str[table_len] = '\0';
    
    const char *table_list = "fuzz_table";
    int compile_result = lou_compileString(fuzz_str_1, table_str);
    free(table_str);

    // 2. Get emphasis classes for the table
    const char **emph_classes = lou_getEmphClasses(fuzz_str_2);
    if (emph_classes) {
        // Free the emphasis classes
        // Note: Actual function name from provided APIs is lou_freeEmphClasses
        // but the signature shows: void lou_freeEmphClasses(const char **)
        lou_freeEmphClasses(emph_classes);
    }

    // 3. Find a table (using partial data as query)
    size_t query_len = size / 8;
    if (query_len > 0) {
        char *query = (char *)malloc(query_len + 1);
        if (query) {
            memcpy(query, data + table_len, query_len);
            query[query_len] = '\0';
            char *found_table = lou_findTable(query);
            if (found_table) {
                lou_freeTableFile(found_table);
            }
            free(query);
        }
    }

    // 4. Get table information
    char *table_info = lou_getTableInfo(fuzz_str_3, fuzz_str_4);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }

    // 5. Prepare input buffer for character operations
    // Use remaining data for wide character operations
    size_t widechar_data_len = size - table_len - query_len;
    if (widechar_data_len < 4) {
        lou_logEnd();
        return 0;
    }

    // Convert bytes to widechar (assuming 2 bytes per widechar)
    int widechar_len = widechar_data_len / 2;
    if (widechar_len == 0) {
        lou_logEnd();
        return 0;
    }

    widechar *inbuf = (widechar *)malloc(widechar_len * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(widechar_len * sizeof(widechar));
    char *hyphens = (char *)malloc(widechar_len + 1);
    
    if (!inbuf || !outbuf || !hyphens) {
        free(inbuf);
        free(outbuf);
        free(hyphens);
        lou_logEnd();
        return 0;
    }

    // Copy data to widechar buffer
    for (int i = 0; i < widechar_len; i++) {
        size_t offset = table_len + query_len + i * 2;
        if (offset + 1 < size) {
            inbuf[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            inbuf[i] = 0;
        }
    }

    // 6. Convert characters to dots
    int mode = 0;
    int char_to_dots_result = lou_charToDots(fuzz_str_5, inbuf, outbuf, widechar_len, fuzz_int32_t_6);

    // 7. Convert dots back to characters
    widechar *dots_to_char_out = (widechar *)malloc(widechar_len * sizeof(widechar));
    if (dots_to_char_out) {
        int dots_to_char_result = lou_dotsToChar(fuzz_str_7, outbuf, dots_to_char_out, widechar_len, fuzz_int32_t_8);
        free(dots_to_char_out);
    }

    // 8. Hyphenate the input
    int hyphenate_result = lou_hyphenate(fuzz_str_9, inbuf, widechar_len, hyphens, fuzz_int32_t_10);

    // 9. Translate the input
    int inlen = widechar_len;
    int outlen = widechar_len * 2; // Allocate extra space for output
    widechar *trans_outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(widechar_len * sizeof(formtype));
    char *spacing = (char *)malloc(widechar_len);
    int *output_pos = (int *)malloc(outlen * sizeof(int));
    int *input_pos = (int *)malloc(widechar_len * sizeof(int));
    int *cursor_pos = (int *)malloc(sizeof(int));
    
    if (trans_outbuf && typeform && spacing && output_pos && input_pos && cursor_pos) {
        // Initialize with some values
        for (int i = 0; i < widechar_len; i++) {
            typeform[i] = 0;
            spacing[i] = 0;
            input_pos[i] = i;
        }
        for (int i = 0; i < outlen; i++) {
            output_pos[i] = i;
        }
        *cursor_pos = 0;
        
        int translate_result = lou_translate(fuzz_str_11, inbuf, &inlen, trans_outbuf, 
                                           &outlen, typeform, spacing, output_pos, 
                                           input_pos, cursor_pos, fuzz_int32_t_12);
    }

    // Clean up
    free(inbuf);
    free(outbuf);
    free(hyphens);
    free(trans_outbuf);
    free(typeform);
    free(spacing);
    free(output_pos);
    free(input_pos);
    free(cursor_pos);

    // 10. End logging
    lou_logEnd();

    return 0;
}