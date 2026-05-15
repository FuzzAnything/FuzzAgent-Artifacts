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
//<ID> 882
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"deallocateRuleNames":[[1621,9,1621,18,0,0,4,0],[1621,9,1621,18,0,0,4,1]],"freeTranslationTable":[[4936,6,4936,18,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_findTables","lou_free","lou_checkTable","lou_freeTableFiles","lou_free","lou_freeTableFiles","lou_free","lou_compileString","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_freeTableFiles","lou_free","lou_freeTableFiles","lou_free","lou_translateString","lou_hyphenate","lou_backTranslateString","lou_translatePrehyphenated","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_findTables","lou_checkTable","lou_compileString","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_translateString","lou_hyphenate","lou_backTranslateString","lou_translatePrehyphenated","lou_freeTableFiles","lou_free"],"visited":0}
/**/


// Include necessary headers for liblouis

// Custom log callback function
static void  id_000047_my_log_callback(logLevels level, const char *message) {
    // Do nothing in fuzzing context
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for minimal input requirements
    if (size < 32) {
	return 0;
	}
	
    
    // Register log callback first
    lou_registerLogCallback( id_000047_my_log_callback);
    
    // 1. Parse input data for table query
    // Find null-terminated table query in first part of data
    size_t query_len = 0;
    while (query_len < size && data[query_len] != 0) {
        query_len++;
    }
    if (query_len >= size - 1) {
	return 0;
	}
	
    
    char *table_query = (char *)malloc(query_len + 1);
    memcpy(table_query, data, query_len);
    table_query[query_len] = 0;
    
    // 2. Find tables using the query
    char **found_tables = lou_findTables(table_query);
    free(table_query);
    
    if (!found_tables) {
        lou_free();
        return 0;
    }
    
    // Find first valid table for further operations
    const char *table_to_use = nullptr;
    for (int i = 0; found_tables[i] != nullptr; i++) {
        if (lou_checkTable(found_tables[i]) == 0) {
            table_to_use = found_tables[i];
            break;
        }
    }
    
    if (!table_to_use) {
        lou_freeTableFiles(found_tables);
        lou_free();
        return 0;
    }
    
    // 3. Compile a simple table string (using remaining data)
    size_t table_string_offset = query_len + 1;
    if (table_string_offset >= size) {
        lou_freeTableFiles(found_tables);
        lou_free();
        return 0;
    }
    
    // Use a small portion of data as table string
    size_t table_string_len = (size - table_string_offset > 256) ? 256 : size - table_string_offset;
    char *table_string = (char *)malloc(table_string_len + 1);
    memcpy(table_string, data + table_string_offset, table_string_len);
    table_string[table_string_len] = 0;
    
    // Try to compile the string (may fail, that's OK)
    lou_compileString(table_to_use, table_string);
    free(table_string);
    
    // 4. Get emphasis classes
    const char **emph_classes = lou_getEmphClasses(table_to_use);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // 5. Get table pointer
    const void *table_ptr = lou_getTable(table_to_use);
    (void)table_ptr; // Use to avoid unused variable warning
    
    // 6. Prepare text input for translation/hyphenation
    size_t text_offset = table_string_offset + table_string_len;
    if (text_offset >= size) {
        lou_freeTableFiles(found_tables);
        lou_free();
        return 0;
    }
    
    size_t text_data_size = size - text_offset;
    size_t widechar_count = text_data_size / sizeof(widechar);
    if (widechar_count == 0) {
        lou_freeTableFiles(found_tables);
        lou_free();
        return 0;
    }
    
    // Allocate buffers for text operations
    widechar *inbuf = (widechar *)malloc(widechar_count * sizeof(widechar));
    memcpy(inbuf, data + text_offset, widechar_count * sizeof(widechar));
    
    int inlen = (int)widechar_count;
    int outlen = inlen * 2; // Conservative estimate
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    
    // 7. Translate string
    formtype *typeform = (formtype *)calloc(inlen, sizeof(formtype));
    char *spacing = (char *)calloc(inlen, sizeof(char));
    int translate_result = lou_translateString(table_to_use, inbuf, &inlen, 
                                               outbuf, &outlen, typeform, spacing, 0);
    (void)translate_result; // Result may be 0 or negative, both acceptable
    
    // 8. Hyphenate
    char *hyphens = (char *)malloc(inlen * sizeof(char));
    int hyphen_result = lou_hyphenate(table_to_use, inbuf, inlen, hyphens, 0);
    (void)hyphen_result;
    
    // 9. Back translate (using original input)
    int back_inlen = inlen;
    int back_outlen = outlen;
    widechar *back_outbuf = (widechar *)malloc(back_outlen * sizeof(widechar));
    int back_result = lou_backTranslateString(table_to_use, inbuf, &back_inlen,
                                              back_outbuf, &back_outlen, 
                                              typeform, spacing, 0);
    (void)back_result;
    
    // 10. Translate pre-hyphenated (more complex translation)
    int pre_inlen = inlen;
    int pre_outlen = outlen;
    widechar *pre_outbuf = (widechar *)malloc(pre_outlen * sizeof(widechar));
    int *outputPos = (int *)calloc(pre_outlen, sizeof(int));
    int *inputPos = (int *)calloc(pre_inlen, sizeof(int));
    int *cursorPos = (int *)calloc(1, sizeof(int));
    char *inputHyphens = (char *)calloc(pre_inlen, sizeof(char));
    char *outputHyphens = (char *)calloc(pre_outlen, sizeof(char));
    
    // Initialize input hyphens with pattern from data
    for (int i = 0; i < pre_inlen && i < 10; i++) {
        inputHyphens[i] = (data[i] % 2) ? '-' : ' ';
    }
    
    int pre_result = lou_translatePrehyphenated(table_to_use, inbuf, &pre_inlen,
                                                pre_outbuf, &pre_outlen,
                                                typeform, spacing, outputPos,
                                                inputPos, cursorPos,
                                                inputHyphens, outputHyphens, 0);
    (void)pre_result;
    
    // Cleanup all allocated resources
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(hyphens);
    free(back_outbuf);
    free(pre_outbuf);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(inputHyphens);
    free(outputHyphens);
    
    lou_freeTableFiles(found_tables);
    lou_free();
    
    return 0;
}