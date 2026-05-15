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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 2.5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_registerTableResolver","lou_setDataPath","lou_listTables","lou_indexTables","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_checkTable","lou_freeTableFiles","lou_freeTableFiles","lou_checkTable","lou_setDataPath","lou_free"],"critical_calls":["lou_registerTableResolver","lou_setDataPath","lou_listTables","lou_indexTables","lou_findTables","lou_getTableInfo","lou_freeTableInfo","lou_checkTable","lou_freeTableFiles","lou_freeTableFiles","lou_checkTable","lou_setDataPath","lou_free"],"visited":1}
/*Here's a C++ fuzz driver for liblouis that utilizes the specified APIs to simulate table management operations:

*/


// Custom table resolver function
static char ** id_000011_custom_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns NULL - mimics default behavior
    return NULL;
}

// Helper function to read null-terminated string from data
static std::string  id_000011_readStringFromData(const uint8_t *data, size_t size, size_t &offset) {
    std::string result;
    while (offset < size && data[offset] != 0) {
        result.push_back(static_cast<char>(data[offset]));
        offset++;
    }
    offset++; // Skip null terminator
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data

    size_t offset = 0;
    
    // 1. Register custom table resolver
    lou_registerTableResolver( id_000011_custom_table_resolver);
    
    // 2. Set data path using input data
    std::string dataPath =  id_000011_readStringFromData(data, size, offset);
    if (!dataPath.empty() && offset < size) {
        char *oldPath = lou_setDataPath(dataPath.c_str());
        // oldPath is owned by liblouis, don't free it
    }
    
    // 3. List available tables
    char **tables = lou_listTables();
    
    // 4. Index tables if any were found
    if (tables != NULL) {
        lou_indexTables(const_cast<const char **>(tables));
        
        // 5. Find tables using query from input data
        if (offset < size) {
            std::string query =  id_000011_readStringFromData(data, size, offset);
            if (!query.empty()) {
                char **foundTables = lou_findTables(query.c_str());
                
                // 6. Check a table (use first from list if available)
                if (foundTables != NULL && foundTables[0] != NULL) {
                    const char *tableToCheck = foundTables[0];
                    
                    // Try to get table info
                    char *tableInfo = lou_getTableInfo(tableToCheck, fuzz_str_1);
                    if (tableInfo != NULL) {
                        lou_freeTableInfo(tableInfo);
                    }
                    
                    // Check the table
                    int checkResult = lou_checkTable(tableToCheck);
                    (void)checkResult; // Use result to avoid unused warning
                    
                    lou_freeTableFiles(foundTables);
                }
            }
        }
        
        // 7. Free allocated table list
        lou_freeTableFiles(tables);
    }
    
    // 8. Additional table operations using the rest of the input data
    if (offset < size) {
        // Create a FILE* from remaining data
        size_t remaining = size - offset;
        const uint8_t *remainingData = data + offset;
        
        FILE *in_file = fmemopen((void *)remainingData, remaining, "rb");
        if (in_file != NULL) {
            // Get file descriptor for potential future use
            int fd = fuzz_fileno(in_file);
            (void)fd; // Use to avoid unused warning
            
            // Read data from the file for table checking
            fseek(in_file, 0, SEEK_END);
            long file_size = ftell(in_file);
            fseek(in_file, 0, SEEK_SET);
            
            if (file_size > 0) {
                std::vector<char> buffer(file_size + 1);
                size_t bytes_read = fread(buffer.data(), 1, file_size, in_file);
                buffer[bytes_read] = '\0';
                
                // Try to use the file content as a table list
                if (bytes_read > 0) {
                    int checkResult = lou_checkTable(buffer.data());
                    (void)checkResult;
                }
            }
            
            assert_file_closed(&in_file);;
        }
    }
    
    // 9. Set data path back to default using empty string
    lou_setDataPath(fuzz_str_2);
    
    // 10. Cleanup liblouis resources
    lou_free();
    
    return 0;
}