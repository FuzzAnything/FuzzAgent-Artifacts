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
//<ID> 651
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["lou_listTables","lou_freeTableFiles","lou_logFile","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_getTableInfo","lou_freeTableInfo","lou_compileString","lou_freeTableFiles","lou_logEnd"],"critical_calls":["lou_listTables","lou_logFile","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_getTableInfo","lou_freeTableInfo","lou_compileString","lou_freeTableFiles","lou_logEnd"],"visited":0}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. First, list available tables
    char **tables = lou_listTables();
    if (!tables) {
        return 0;
    }

    // 2. Create a temporary file with input data for logging
    FILE *log_file = fopen("input_file", "wb");
    if (!log_file) {
        lou_freeTableFiles(tables);
        assert_file_closed(&log_file);
	return 0;
    }
    fwrite(data, 1, size, log_file);
    assert_file_closed(&log_file);;
    
    // 3. Set the log file to our input file
    lou_logFile("input_file");
    
    // 4. Process each table
    for (int i = 0; tables[i] != nullptr; i++) {
        const char *table = tables[i];
        
        // 5. Get emphasis classes for this table
        const char **emph_classes = lou_getEmphClasses(table);
        if (emph_classes) {
            // 6. For each emphasis class, get typeform
            for (int j = 0; emph_classes[j] != nullptr; j++) {
                formtype typeform = lou_getTypeformForEmphClass(table, emph_classes[j]);
                (void)typeform; // Use the result to avoid unused variable warning
            }
            // 7. Free emphasis classes
            lou_freeEmphClasses(emph_classes);
        }
        
        // 8. Get table info for a specific key
        // Use a portion of input data as key (mod 256 to ensure printable)
        if (size > 0) {
            char key[2] = {static_cast<char>(data[0] % 256), '\0'};
            char *table_info = lou_getTableInfo(table, key);
            if (table_info) {
                lou_freeTableInfo(table_info);
            }
        }
        
        // 9. Compile a string using this table
        // Use input data as the string to compile (null-terminated)
        if (size > 0) {
            // Create null-terminated string from input data
            std::vector<char> compile_string(size + 1);
            memcpy(compile_string.data(), data, size);
            compile_string[size] = '\0';
            
            int result = lou_compileString(table, compile_string.data());
            (void)result; // Use the result to avoid unused variable warning
        }
    }
    
    // 10. Clean up
    lou_freeTableFiles(tables);
    
    // 11. Remove temporary log file
    remove("input_file");
    
    // 12. End logging
    lou_logEnd();
    
    assert_file_closed(&log_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}