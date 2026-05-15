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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":2,"unique_branches":{},"library_calls":["lou_getDataPath","lou_indexTables","lou_findTables","lou_freeTableFiles","lou_checkTable","lou_getTypeformForEmphClass","lou_free"],"critical_calls":["lou_getDataPath","lou_indexTables","lou_findTables","lou_freeTableFiles","lou_checkTable","lou_getTypeformForEmphClass","lou_free"],"visited":0}
/**/


extern "C" {
}

// Helper function to safely extract strings from fuzzer input
static std::string  id_000005_extract_string(const uint8_t *data, size_t &offset, size_t size) {
    if (offset >= size) return "";
    
    // Find null terminator or end of input
    size_t start = offset;
    size_t len = 0;
    while (offset + len < size && data[offset + len] != 0) {
        len++;
    }
    
    std::string result(reinterpret_cast<const char*>(data + start), len);
    
    // Skip the null terminator if present
    offset += len + 1;
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Initialize offset for reading data
    size_t offset = 0;
    
    // Extract strings from fuzzer input
    // First string: query for table search
    std::string query =  id_000005_extract_string(data, offset, size);
    if (query.empty() && offset > 0) {
        // If the string was empty but we had data, reset offset
        offset = 0;
        query = std::string(reinterpret_cast<const char*>(data), 
                           std::min(size, (size_t)100));
    }
    
    // Second string: table list for typeform checking
    std::string table_list =  id_000005_extract_string(data, offset, size);
    if (table_list.empty() && offset < size) {
        table_list = std::string(reinterpret_cast<const char*>(data + offset), 
                                std::min(size - offset, (size_t)100));
    }
    
    // Third string: emphasis class name
    std::string emph_class = "emphasis"; // Default value
    if (offset < size) {
        std::string temp =  id_000005_extract_string(data, offset, size);
        if (!temp.empty()) {
            emph_class = temp;
        }
    }
    
    // 1. Get current data path (API 1/6)
    char *data_path = lou_getDataPath();
    if (data_path) {
        // data_path is owned by liblouis, don't free it
    }
    
    // 2. Index tables (API 2/6)
    // Create a null-terminated array of table names
    std::vector<const char*> tables_to_index;
    if (!table_list.empty()) {
        // Split table_list by comma (common format in liblouis)
        size_t start = 0;
        size_t end = table_list.find(',');
        while (end != std::string::npos) {
            std::string table = table_list.substr(start, end - start);
            if (!table.empty()) {
                tables_to_index.push_back(strdup(table.c_str()));
            }
            start = end + 1;
            end = table_list.find(',', start);
        }
        // Add the last table
        std::string last_table = table_list.substr(start);
        if (!last_table.empty()) {
            tables_to_index.push_back(strdup(last_table.c_str()));
        }
        
        // Add null terminator to the array
        tables_to_index.push_back(nullptr);
        
        // Index the tables
        lou_indexTables(tables_to_index.data());
        
        // Free duplicated strings
        for (size_t i = 0; i < tables_to_index.size() - 1; ++i) {
            free(const_cast<char*>(tables_to_index[i]));
        }
    }
    
    // 3. Find tables matching query (API 3/6)
    char **found_tables = nullptr;
    if (!query.empty()) {
        found_tables = lou_findTables(query.c_str());
        
        // Process found tables
        if (found_tables) {
            for (int i = 0; found_tables[i] != nullptr; ++i) {
                // Could check each table individually
                // lou_checkTable(found_tables[i]);
            }
            // Free the table list
            lou_freeTableFiles(found_tables);
        }
    }
    
    // 4. Check if table is valid (API 4/6)
    if (!table_list.empty()) {
        int table_valid = lou_checkTable(table_list.c_str());
        // table_valid is 0 if valid, non-zero if invalid
        // We don't need to do anything with the result for fuzzing
    }
    
    // 5. Get typeform for emphasis class (API 5/6)
    if (!table_list.empty() && !emph_class.empty()) {
        formtype typeform = lou_getTypeformForEmphClass(table_list.c_str(), 
                                                       emph_class.c_str());
        // typeform is a bitmask, no need to process for fuzzing
    }
    
    // 6. Free liblouis resources (API 6/6)
    lou_free();
    
    // Note: lou_free() frees internal liblouis structures.
    // External allocations (like strdup) were already freed.
    
    return 0;
}