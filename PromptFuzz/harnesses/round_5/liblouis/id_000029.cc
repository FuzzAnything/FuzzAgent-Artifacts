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
//<ID> 647
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["lou_getDataPath","lou_indexTables","lou_indexTables","lou_findTable","lou_getTable","lou_getTableInfo","lou_backTranslateString","lou_hyphenate","lou_freeTableFile","lou_free"],"critical_calls":["lou_getDataPath","lou_indexTables","lou_indexTables","lou_findTable","lou_getTable","lou_getTableInfo","lou_backTranslateString","lou_hyphenate","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Include necessary headers for liblouis
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < sizeof(unsigned short) * 2) {
        return 0;
    }

    // 1. Get data path
    char *data_path = lou_getDataPath();
    
    // 2. Index tables - call twice as requested
    const char *tables[] = {"en-us-g2.ctb", "en-ueb-g2.ctb", NULL};
    lou_indexTables(tables);
    lou_indexTables(tables);  // Second call as specified
    
    // 3. Find a table using input data as query
    // Use part of input as table query (safe copy)
    char table_query[256];
    size_t query_len = size < 255 ? size : 255;
    memcpy(table_query, data, query_len);
    table_query[query_len] = '\0';
    
    char *found_table = lou_findTable(table_query);
    if (!found_table) {
        // If no table found, use a default
        found_table = strdup("en-us-g2.ctb");
    }
    
    // 4. Get table handle
    const char *table_list = found_table;
    const void *table_handle = lou_getTable(table_list);
    
    // 5. Get table info
    char *table_info = lou_getTableInfo(found_table, "locale");
    
    // 6. Prepare input buffer for back translation
    // Convert input data to widechar format
    size_t widechar_count = size / sizeof(unsigned short);
    unsigned short *inbuf = (unsigned short *)data;
    int inlen = widechar_count;
    
    // Allocate output buffer for back translation
    unsigned short *outbuf = (unsigned short *)malloc(widechar_count * sizeof(unsigned short));
    int outlen = widechar_count;
    
    // 7. Perform back translation
    int back_translate_result = lou_backTranslateString(
        table_list,
        inbuf,
        &inlen,
        outbuf,
        &outlen,
        NULL,  // typeform
        NULL,  // spacing
        0      // mode
    );
    
    // 8. Perform hyphenation on the original input
    char *hyphens = (char *)malloc(widechar_count);
    int hyphenate_result = lou_hyphenate(
        table_list,
        inbuf,
        inlen,
        hyphens,
        0      // mode
    );
    
    // 9. Clean up allocated resources
    free(outbuf);
    free(hyphens);
    
    if (table_info) {
        free(table_info);
    }
    
    // 10. Free table file and cleanup
    lou_freeTableFile(found_table);
    lou_free();
    
    return 0;
}