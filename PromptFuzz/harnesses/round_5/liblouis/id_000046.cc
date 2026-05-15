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
//<ID> 828
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_setDataPath","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_logFile","lou_dotsToChar","lou_freeTableFiles","lou_logEnd"],"critical_calls":["lou_setDataPath","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_logFile","lou_dotsToChar","lou_freeTableFiles","lou_logEnd"],"visited":0}
/**/


// Include the necessary headers for liblouis

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, set the data path for liblouis tables
    // Using a fixed path for fuzzing - liblouis will use its default if this fails
    char *data_path = lou_setDataPath(".");
    if (data_path) {
        // data_path contains the previous data path, which we don't need
        // According to docs, we should free this if we want to avoid memory leaks
        // but lou_setDataPath returns a pointer to static data, so don't free
    }
    
    // 2. Find tables using a query from the input data
    // We'll use the first part of input as query, up to 100 bytes or first null byte
    char query[101] = {0};
    size_t query_len = size < 100 ? size : 100;
    for (size_t i = 0; i < query_len; i++) {
        if (data[i] == 0) {
            query_len = i;
            break;
        }
        query[i] = data[i];
    }
    query[query_len] = '\0';
    
    char **tables = lou_findTables(query);
    
    // 3. Get table info if we found any tables
    if (tables && tables[0]) {
        // Use a fixed key for getting table info
        const char *table = tables[0];
        char *table_info = lou_getTableInfo(table, "locale");
        if (table_info) {
            // We could log this info, but for fuzzing we just consume it
            lou_freeTableInfo(table_info);
        }
    }
    
    // 4. Set up logging to a file
    lou_logFile("liblouis_fuzz.log");
    
    // 5. Convert dots to characters using the input data
    // We need to interpret the remaining data as widechar dots
    if (size > query_len + 1) {
        size_t dots_data_start = query_len + 1;
        size_t dots_data_size = size - dots_data_start;
        
        // We need at least 2 bytes for one widechar
        if (dots_data_size >= 2) {
            // Calculate how many widechars we can process
            int length = dots_data_size / sizeof(widechar);
            
            // Allocate buffers
            widechar *inbuf = (widechar*)(data + dots_data_start);
            widechar *outbuf = (widechar*)malloc(length * sizeof(widechar));
            
            if (outbuf) {
                // Use the first found table if available, otherwise use a default
                const char *tableList = tables && tables[0] ? tables[0] : "en-us-g2.ctb";
                
                // Call dotsToChar with the input data
                int result = lou_dotsToChar(tableList, inbuf, outbuf, length, 0);
                
                // Clean up
                free(outbuf);
            }
        }
    }
    
    // 6. Free the table files list if it was allocated
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Additional cleanup
    lou_logEnd();  // Close the log file
    
    return 0;  // Always return 0
}