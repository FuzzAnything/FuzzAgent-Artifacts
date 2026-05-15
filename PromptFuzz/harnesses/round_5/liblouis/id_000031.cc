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
//<ID> 650
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 5
//<Quality> {"density":8,"unique_branches":{"getToken":[[383,7,383,18,0,0,4,0]],"compileRule":[[2967,2,2967,16,0,0,4,0],[3055,7,3055,26,0,0,4,0],[3487,8,3487,49,0,0,4,0]],"compileSwap":[[1661,6,1661,50,0,0,4,0]]},"library_calls":["lou_listTables","lou_freeTableFiles","lou_freeTableFiles","lou_compileString","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_getTableInfo","lou_freeTableInfo","lou_logFile","lou_freeEmphClasses","lou_freeTableFiles","lou_logEnd"],"critical_calls":["lou_listTables","lou_compileString","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_getTableInfo","lou_freeTableInfo","lou_logFile","lou_freeEmphClasses","lou_freeTableFiles","lou_logEnd"],"visited":0}
/**/


// Include necessary headers for liblouis
extern "C" {
    // Declare the liblouis functions we'll use
    char **lou_listTables();
    formtype lou_getTypeformForEmphClass(const char *tableList, const char *emphClass);
    void lou_logFile(const char *filename);
    const char **lou_getEmphClasses(const char *tableList);
    char *lou_getTableInfo(const char *table, const char *key);
    int lou_compileString(const char *tableList, const char *inString);
    void lou_freeTableFiles(char **tables);
    void lou_freeEmphClasses(const char **classes);
    void lou_freeTableInfo(char *info);
    void lou_logEnd();
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have minimum data to work with
    if (size < 2) {
	return 0;
	}
	
    
    // 1. List available tables
    char **tables = lou_listTables();
    if (!tables || !tables[0]) {
        if (tables) lou_freeTableFiles(tables);
        return 0;
    }
    
    // Use the first table for our operations
    const char *table = tables[0];
    
    // 2. Create a null-terminated string from input data for compilation
    // We'll use a portion of the input as the string to compile
    size_t compile_str_size = size > 1024 ? 1024 : size;
    char *compile_string = (char *)malloc(compile_str_size + 1);
    if (!compile_string) {
        lou_freeTableFiles(tables);
        return 0;
    }
    memcpy(compile_string, data, compile_str_size);
    compile_string[compile_str_size] = '\0';
    
    // 3. Compile the string with the table
    // This is the main event: compiling a braille table string
    lou_compileString(table, compile_string);
    
    // 4. Get emphasis classes for the table
    const char **emph_classes = lou_getEmphClasses(table);
    if (emph_classes && emph_classes[0]) {
        // 5. Get typeform for the first emphasis class
        // Use a portion of input data as emphasis class name
        size_t emph_size = size > 512 ? 512 : size;
        char *emph_class = (char *)malloc(emph_size + 1);
        if (emph_class) {
            memcpy(emph_class, data, emph_size);
            emph_class[emph_size] = '\0';
            lou_getTypeformForEmphClass(table, emph_class);
            free(emph_class);
        }
    }
    
    // 6. Get table info with a key from input data
    // Use portion of input as key
    size_t key_size = size > 256 ? 256 : size;
    char *key = (char *)malloc(key_size + 1);
    if (key) {
        memcpy(key, data, key_size);
        key[key_size] = '\0';
        char *table_info = lou_getTableInfo(table, key);
        if (table_info) {
            lou_freeTableInfo(table_info);
        }
        free(key);
    }
    
    // 7. Set up logging to a file
    lou_logFile("output_file");
    
    // 8. Clean up
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    free(compile_string);
    lou_freeTableFiles(tables);
    
    // End logging
    lou_logEnd();
    
    assert_file_name_closed("output_file");
	return 0;
}