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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"resolveSubtable":[[4672,8,4672,17,0,0,4,0]]},"library_calls":["lou_readCharFromFile","lou_logFile","lou_checkTable","lou_setDataPath","lou_listTables","lou_freeTableFiles","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_free"],"critical_calls":["lou_readCharFromFile","lou_logFile","lou_checkTable","lou_setDataPath","lou_listTables","lou_freeTableFiles","lou_getEmphClasses","lou_freeEmphClasses","lou_getTable","lou_free"],"visited":0}
/**/


// Include necessary headers for liblouis
extern "C" {
    // Declare the liblouis functions we need
    int lou_readCharFromFile(const char * fileName, int * mode);
    void lou_logFile(const char * filename);
    int lou_checkTable(const char * tableList);
    char *lou_setDataPath(const char * path);
    char **lou_listTables();
    const char **lou_getEmphClasses(const char * tableList);
    const void *lou_getTable(const char * tableList);
    void lou_freeTableFiles(char ** tables);
    void lou_freeEmphClasses(const char ** emphClasses);
    void lou_free();
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<1050) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_2, fuzz_char_array_size_2, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_4, fuzz_char_array_size_4, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_5, fuzz_char_array_size_5, fdp);
	
		const char * fuzzer_var_lou_checkTable_2_0 = fuzz_char_array_2;
		const char * fuzzer_var_lou_setDataPath_3_0 = fuzz_char_array_3;
		const char * fuzzer_var_lou_getEmphClasses_6_0 = fuzz_char_array_4;
		const char * fuzzer_var_lou_getTable_8_0 = fuzz_char_array_5;//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // 1. Call lou_readCharFromFile
    int mode = 0;
    if (size > 0) {
        int result = lou_readCharFromFile("input_file", &mode);
        // Result is ignored as per fuzzing convention
    }
    
    // 2. Call lou_logFile
    lou_logFile(fuzz_str_1);
    
    // 3. Call lou_checkTable
    // Use the first few bytes as a table name (truncated if needed)
    char tableName[256] = {0};
    size_t tableNameLen = size < 255 ? size : 255;
    if (tableNameLen > 0) {
        memcpy(tableName, data, tableNameLen);
        tableName[tableNameLen] = '\0';
        lou_checkTable(fuzzer_var_lou_checkTable_2_0);
    }
    
    // 4. Call lou_setDataPath
    // Use a simple path based on input data
    char path[256] = {0};
    if (size > 0) {
        size_t pathLen = size < 255 ? size : 255;
        memcpy(path, data, pathLen);
        path[pathLen] = '\0';
        char *oldPath = lou_setDataPath(fuzzer_var_lou_setDataPath_3_0);
        if (oldPath) {
            // oldPath is allocated by liblouis, but we don't need to free it
            // as setDataPath returns a pointer to static data
        }
    }
    
    // 5. Call lou_listTables
    char **tables = lou_listTables();
    if (tables) {
        // Free the table list when done
        lou_freeTableFiles(tables);
    }
    
    // 6. Call lou_getEmphClasses
    if (tableName[0] != '\0') {
        const char **emphClasses = lou_getEmphClasses(fuzzer_var_lou_getEmphClasses_6_0);
        if (emphClasses) {
            lou_freeEmphClasses(emphClasses);
        }
    }
    
    // 7. Call lou_getTable
    if (tableName[0] != '\0') {
        const void *table = lou_getTable(fuzzer_var_lou_getTable_8_0);
        // table pointer is owned by liblouis, don't free it
    }
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    // Optional: Call lou_free to release liblouis resources
    lou_free();
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}