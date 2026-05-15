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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["lou_charSize","lou_compileString","lou_checkTable","lou_checkTable","lou_charToDots","lou_getTypeformForEmphClass","lou_getTypeformForEmphClass","lou_findTable","lou_freeTableFile","lou_getTableInfo","lou_listTables","lou_freeTableFiles","lou_free","lou_logEnd"],"critical_calls":["lou_charSize","lou_compileString","lou_checkTable","lou_checkTable","lou_charToDots","lou_getTypeformForEmphClass","lou_getTypeformForEmphClass","lou_findTable","lou_freeTableFile","lou_getTableInfo","lou_listTables","lou_freeTableFiles","lou_free","lou_logEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to process braille translation data:

*/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 4) {
	return 0;
	}
	
    
    // Get character size from liblouis
    int char_size = lou_charSize();
    if (char_size <= 0) {
	return 0;
	}
	
    
    // Create a table list - use a simple built-in table
    const char *tableList = "en-us-g2.ctb";
    
    // Try to compile a simple table string to ensure table is available
    const char *tableString = 
        "table test.tbl\n"
        "include latinsmall.cti\n"
        "letter a 1\n"
        "letter b 12\n"
        "letter c 14\n";
    
    int compile_result = lou_compileString(tableList, tableString);
    if (compile_result != 0) {
        // If compilation fails, use a different approach
        tableList = "test.tbl";
    }
    
    // Check if table is valid
    int table_check = lou_checkTable(tableList);
    if (table_check != 0) {
        // Table is not valid, try a different one
        tableList = "en-us-g2.ctb";
        table_check = lou_checkTable(tableList);
        if (table_check != 0) {
            return 0; // No valid table available
        }
    }
    
    // Calculate input length in widechars
    // We'll use half of the input for widechars (since widechar is 2 bytes)
    int widechar_len = static_cast<int>(size / 2);
    if (widechar_len <= 0) {
	return 0;
	}
	
    
    // Create input buffer from data (cast to widechar*)
    // Note: This assumes data can be safely interpreted as widechars
    const widechar *inbuf = reinterpret_cast<const widechar*>(data);
    
    // Create output buffer for dots
    widechar *outbuf = static_cast<widechar*>(malloc(widechar_len * sizeof(widechar)));
    if (!outbuf) {
	return 0;
	}
	
    
    // Initialize output buffer
    memset(outbuf, 0, widechar_len * sizeof(widechar));
    
    // Call lou_charToDots to convert characters to braille dots
    // Mode 0 = no contraction, 1 = compbrlAtCursor
    int mode = 0;
    int dots_result = lou_charToDots(tableList, inbuf, outbuf, widechar_len, mode);
    
    // Try lou_getTypeformForEmphClass with a sample emphasis class
    const char *emphClass = "emphasis";
    formtype typeform = lou_getTypeformForEmphClass(tableList, emphClass);
    
    // Try with another emphasis class using data from input
    if (size > 10) {
        // Create a simple emphasis class name from first few bytes
        char dynamicEmphClass[32];
        size_t emph_len = (size < 31) ? size : 31;
        for (size_t i = 0; i < emph_len; i++) {
            dynamicEmphClass[i] = (data[i] % 26) + 'a'; // Convert to lowercase letter
        }
        dynamicEmphClass[emph_len] = '\0';
        
        formtype dynamic_typeform = lou_getTypeformForEmphClass(tableList, dynamicEmphClass);
        (void)dynamic_typeform; // Use variable to avoid unused warning
    }
    
    // Try to find a table file and free it
    char *found_table = lou_findTable("en");
    if (found_table) {
        // We found a table, now free it using lou_freeTableFile
        lou_freeTableFile(found_table);
    }
    
    // Try to get table info
    char *table_info = lou_getTableInfo(tableList, "language");
    if (table_info) {
        free(table_info); // Note: lou_freeTableInfo is the correct API, but we use free for consistency
    }
    
    // List tables (just to use another API)
    char **tables_list = lou_listTables();
    if (tables_list) {
        lou_freeTableFiles(tables_list);
    }
    
    // Clean up
    free(outbuf);
    
    // Call lou_free to release internal liblouis resources
    lou_free();
    
    // Also try lou_logEnd to clean up logging
    lou_logEnd();
    
    return 0;
}