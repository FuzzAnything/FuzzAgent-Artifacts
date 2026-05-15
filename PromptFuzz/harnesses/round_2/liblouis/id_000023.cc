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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"matchFeatureLists":[[478,16,478,18,0,0,4,1]],"matchLanguageTags":[[387,7,387,46,0,0,4,0],[397,9,397,12,0,0,4,0]]},"library_calls":["lou_findTables","lou_registerTableResolver","lou_findTable","lou_getTableInfo","lou_getTable","lou_charToDots","lou_dotsToChar","lou_freeTableInfo","lou_logFile","lou_freeTableFiles","lou_freeTableFile","lou_logEnd"],"critical_calls":["lou_findTables","lou_registerTableResolver","lou_findTable","lou_getTableInfo","lou_getTable","lou_charToDots","lou_dotsToChar","lou_freeTableInfo","lou_logFile","lou_freeTableFiles","lou_freeTableFile","lou_logEnd"],"visited":2}
/**/


// liblouis header
extern "C" {
}

// Custom resolver function
char ** id_000023_my_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns a single table path
    static char *result[] = {(char*)"en-us-comp8.ctb", nullptr};
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small for meaningful processing
    if (size < 2) {
	return 0;
	}
	
    
    // Create a null-terminated query string from input data
    char *query = (char *)malloc(size + 1);
    if (!query) {
	return 0;
	}
	
    memcpy(query, data, size);
    query[size] = '\0';
    
    // 1. Find multiple tables using query
    char **tables = lou_findTables(query);
    
    // 2. Register table resolver
    lou_registerTableResolver( id_000023_my_table_resolver);
    
    // 3. Find a single table
    char *single_table = lou_findTable(query);
    
    if (tables && tables[0]) {
        // 4. Get table info for the first table found
        char *table_info = lou_getTableInfo(tables[0], "language");
        
        // 5. Get the table object
        const void *table_obj = lou_getTable(tables[0]);
        
        // Use part of data for character operations
        size_t usable_size = size / 2;
        if (usable_size < 1) usable_size = 1;
        
        // Allocate buffers for character/dots conversion
        widechar *inbuf = (widechar *)malloc(usable_size * sizeof(widechar));
        widechar *outbuf = (widechar *)malloc(usable_size * sizeof(widechar));
        
        if (inbuf && outbuf) {
            // Fill input buffer with data (converting bytes to widechars)
            size_t wlen = usable_size / sizeof(widechar);
            if (wlen < 1) wlen = 1;
            
            for (size_t i = 0; i < wlen && i < usable_size; i++) {
                inbuf[i] = (widechar)data[i % size];
            }
            
            // 6. Convert characters to dots
            int mode = 0;
            int dots_result = lou_charToDots(tables[0], inbuf, outbuf, wlen, mode);
            
            if (dots_result) {
                // 7. Convert dots back to characters
                widechar *backbuf = (widechar *)malloc(usable_size * sizeof(widechar));
                if (backbuf) {
                    int back_result = lou_dotsToChar(tables[0], outbuf, backbuf, wlen, mode);
                    free(backbuf);
                }
            }
            
            free(inbuf);
            free(outbuf);
        }
        
        // 8. Free table info if obtained
        if (table_info) {
            lou_freeTableInfo(table_info);
        }
    }
    
    // 9. Set log file
    FILE *log_file = fopen("fuzz_log.txt", "w");
    if (log_file) {
        assert_file_closed(&log_file);;
        lou_logFile("fuzz_log.txt");
    }
    
    // 10. Free table files (multiple tables)
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // 11. Free single table result
    if (single_table) {
        lou_freeTableFile(single_table);
    }
    
    // 12. End logging
    lou_logEnd();
    
    // 13. Free allocated memory
    free(query);
    
    assert_file_closed(&log_file);
	return 0;
}