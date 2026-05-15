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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"parseQuery":[[648,7,648,14,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_indexTables","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_findTable","lou_getTable","lou_charToDots","lou_freeTableFile","lou_freeTableFiles","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_indexTables","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_findTable","lou_getTable","lou_charToDots","lou_freeTableFile","lou_freeTableFiles","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Include liblouis headers

// Log callback function
static void  id_000030_logCallback(logLevels level, const char *message) {
    // Simple log callback that does nothing
    (void)level;
    (void)message;
}

// Table resolver function
static char ** id_000030_tableResolver(const char *table, const char *base) {
    // Return NULL to indicate no custom resolution
    (void)table;
    (void)base;
    return nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0; // Need at least some data
    }

    // 1. Register log callback
    lou_registerLogCallback( id_000030_logCallback);

    // 2. Register table resolver
    lou_registerTableResolver( id_000030_tableResolver);

    // 3. Index some example tables
    const char *tables[] = {"en-us-g2.ctb", "en-ueb-g1.ctb", nullptr};
    lou_indexTables(tables);

    // 4. Find tables using part of input as query
    // Use first min(100, size-1) bytes as query string
    size_t query_len = (size - 1) > 100 ? 100 : (size - 1);
    std::string query((const char*)data, query_len);
    char **found_tables = lou_findTables(query.c_str());
    
    // 5. Get table info for first table if available
    if (found_tables && found_tables[0]) {
        char *table_info = lou_getTableInfo(found_tables[0], fuzz_str_1);
        if (table_info) {
            lou_freeTableInfo(table_info);
        }
    }

    // 6. Find a single table
    char *single_table = lou_findTable(query.c_str());

    // 7. Get table pointer
    const char *table_to_use = single_table ? single_table : "en-us-g2.ctb";
    const void *table_ptr = lou_getTable(table_to_use);

    // 8. Convert input to dots using lou_charToDots
    // Use remaining data as widechar input
    size_t remaining_size = size - query_len;
    if (remaining_size >= 2) { // Need at least 2 bytes for one widechar
        // Ensure we have an even number of bytes for widechars
        size_t widechar_count = remaining_size / 2;
        if (widechar_count > 0) {
            // Convert bytes to widechars
            std::vector<widechar> inbuf(widechar_count);
            std::vector<widechar> outbuf(widechar_count);
            
            memcpy(inbuf.data(), data + query_len, widechar_count * 2);
            
            int mode = 0; // Default mode
            int result = lou_charToDots(table_to_use, inbuf.data(), outbuf.data(), 
                                       static_cast<int>(widechar_count), fuzz_int32_t_2);
            (void)result; // Result not used in fuzzing
        }
    }

    // 9. Clean up
    if (found_tables) {
        // Free each string in the array
        for (char **ptr = found_tables; *ptr; ptr++) {
            lou_freeTableFile(*ptr);
        }
        lou_freeTableFiles(found_tables);
    }
    
    if (single_table) {
        lou_freeTableFile(single_table);
    }

    // 10. Final cleanup
    lou_free();

    return 0;
}