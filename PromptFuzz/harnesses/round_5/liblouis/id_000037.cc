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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.5, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_listTables","lou_freeTableFiles","lou_findTables","lou_freeTableFiles","lou_checkTable","lou_free","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_getTableInfo","lou_free","lou_free","lou_free","lou_free","lou_translateString","lou_backTranslateString","lou_free"],"critical_calls":["lou_registerLogCallback","lou_listTables","lou_freeTableFiles","lou_findTables","lou_freeTableFiles","lou_checkTable","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_getTableInfo","lou_translateString","lou_backTranslateString","lou_free"],"visited":1}
/**/


// Include liblouis headers
extern "C" {
    const char *lou_version();
    int lou_charSize();
    int lou_translateString(const char *tableList, const widechar *inbuf, int *inlen, 
                            widechar *outbuf, int *outlen, formtype *typeform, 
                            char *spacing, int mode);
    int lou_backTranslateString(const char *tableList, const widechar *inbuf, int *inlen,
                                widechar *outbuf, int *outlen, formtype *typeform,
                                char *spacing, int mode);
    const void *lou_getTable(const char *tableList);
    int lou_checkTable(const char *tableList);
    void lou_registerLogCallback(logcallback callback);
    void lou_setLogLevel(logLevels level);
    char **lou_listTables();
    void lou_freeTableFiles(char **tables);
    char **lou_findTables(const char *query);
    const char **lou_getEmphClasses(const char *tableList);
    void lou_freeEmphClasses(const char **classes);
    char *lou_getTableInfo(const char *table, const char *key);
    void lou_free();
}

// Simple log callback function
void  id_000037_log_callback(logLevels level, const char *message) {
    // Do nothing for fuzzing to avoid I/O
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Register log callback first
    lou_registerLogCallback( id_000037_log_callback);
    
    // List available tables
    char **tables = lou_listTables();
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Extract a query string from input (first 100 bytes or less)
    size_t query_len = size > 100 ? 100 : size;
    char *query = (char *)malloc(query_len + 1);
    if (!query) {
	return 0;
	}
	
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    // Find tables based on query
    char **found_tables = lou_findTables(query);
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    // Use a fixed table for testing (English Grade 2 braille)
    const char *table_name = "en-us-g2.ctb";
    
    // Check if table is valid
    int table_check = lou_checkTable(table_name);
    if (table_check != 0) {
        free(query);
        lou_free();
        return 0;
    }
    
    // Get emphasis classes for table
    const char **emph_classes = lou_getEmphClasses(table_name);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // Get internal table representation
    const void *table_ptr = lou_getTable(table_name);
    (void)table_ptr; // Use table_ptr to avoid unused variable warning
    
    // Get table info using query as key
    char *table_info = lou_getTableInfo(table_name, query);
    if (table_info) {
        free(table_info);
    }
    
    // Prepare translation buffers using remaining input data
    size_t remaining_size = size - query_len;
    if (remaining_size < 8) {
        free(query);
        lou_free();
        return 0;
    }
    
    // Convert byte data to widechar array for translation
    const uint8_t *translation_data = data + query_len;
    size_t translation_size = remaining_size;
    
    // Ensure we have at least 2 widechars (4 bytes)
    if (translation_size < 4) {
        free(query);
        lou_free();
        return 0;
    }
    
    // Input buffer (widechar is 2 bytes)
    size_t widechar_count = translation_size / 2;
    widechar *inbuf = (widechar *)malloc(widechar_count * sizeof(widechar));
    if (!inbuf) {
        free(query);
        lou_free();
        return 0;
    }
    
    memcpy(inbuf, translation_data, widechar_count * sizeof(widechar));
    int inlen = widechar_count;
    
    // Output buffer (allocate larger size for expansion)
    int outlen = inlen * 4;
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        free(query);
        lou_free();
        return 0;
    }
    
    // Typeform and spacing arrays (optional)
    formtype *typeform = nullptr;
    char *spacing = nullptr;
    
    // Forward translation
    int trans_result = lou_translateString(table_name, inbuf, &inlen, outbuf, 
                                           &outlen, typeform, spacing, 0);
    (void)trans_result; // Use result to avoid unused variable warning
    
    // Prepare for back-translation using forward translation output
    int back_inlen = outlen;
    int back_outlen = back_inlen * 4;
    widechar *back_outbuf = (widechar *)malloc(back_outlen * sizeof(widechar));
    if (back_outbuf) {
        // Back translation
        int back_result = lou_backTranslateString(table_name, outbuf, &back_inlen,
                                                  back_outbuf, &back_outlen,
                                                  typeform, spacing, 0);
        (void)back_result; // Use result to avoid unused variable warning
        free(back_outbuf);
    }
    
    // Cleanup
    free(inbuf);
    free(outbuf);
    free(query);
    
    // Free liblouis resources
    lou_free();
    
    return 0;
}