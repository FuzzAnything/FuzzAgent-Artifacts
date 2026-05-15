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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_findTables","lou_getDataPath","lou_indexTables","lou_checkTable","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_free"],"critical_calls":["lou_findTables","lou_getDataPath","lou_indexTables","lou_checkTable","lou_getTypeformForEmphClass","lou_freeTableFiles","lou_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use a portion of the input as query string for finding tables
    // Ensure we have at least 1 byte for null-termination
    if (size < 1) {
	return 0;
	}
	
    
    // Create query string from input data (null-terminated)
    size_t query_len = size > 100 ? 100 : size; // Limit query length
    char *query = static_cast<char*>(malloc(query_len + 1));
    if (!query) {
	return 0;
	}
	
    
    memcpy(query, data, query_len);
    query[query_len] = '\0'; // Ensure null termination
    
    // Call lou_findTables with the query
    char **found_tables = lou_findTables(query);
    
    // Get data path
    char *data_path = lou_getDataPath();
    
    // Index tables if any were found
    if (found_tables) {
        lou_indexTables(const_cast<const char**>(found_tables));
        
        // Check the first table if available
        if (found_tables[0]) {
            lou_checkTable(found_tables[0]);
            
            // Use part of input as emphasis class (if enough data remains)
            size_t emph_offset = query_len;
            if (emph_offset < size) {
                size_t emph_len = size - emph_offset;
                if (emph_len > 50) emph_len = 50; // Limit emphasis class length
                
                char *emph_class = static_cast<char*>(malloc(emph_len + 1));
                if (emph_class) {
                    memcpy(emph_class, data + emph_offset, emph_len);
                    emph_class[emph_len] = '\0';
                    
                    // Get typeform for emphasis class
                    formtype typeform = lou_getTypeformForEmphClass(found_tables[0], emph_class);
                    (void)typeform; // Prevent unused variable warning
                    
                    free(emph_class);
                }
            }
        }
    }
    
    // Clean up allocated resources
    
    // Free found tables
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }
    
    // Free query string
    free(query);
    
    // Free data path (if allocated by library)
    if (data_path) {
        free(data_path);
    }
    
    // Call lou_free to release library resources
    lou_free();
    
    return 0;
}