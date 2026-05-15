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
//<ID> 496
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 14
//<Quality> {"density":3,"unique_branches":{"compileRule":[[3206,3,3206,23,0,0,4,0],[3369,8,3369,14,0,0,4,1],[3739,8,3739,43,0,0,4,0],[3782,8,3783,41,0,0,4,0],[4075,8,4075,62,0,0,4,0]],"lou_getTypeformForEmphClass":[[5263,42,5263,66,0,0,4,0],[5264,7,5264,55,0,0,4,0],[5264,7,5264,55,0,0,4,1]],"compileSwap":[[1671,7,1671,55,0,0,4,0]],"pattern_compile_expression":[[727,2,727,10,0,0,4,0],[729,7,729,32,0,0,4,1]],"lou_findTable":[[1069,6,1069,17,0,0,4,1]],"compilePassOpcode":[[2295,3,2295,10,0,0,4,0]],"lou_listTables":[[1181,6,1181,17,0,0,4,1]]},"library_calls":["lou_logFile","lou_registerLogCallback","lou_indexTables","lou_listTables","lou_freeTableFiles","lou_findTable","lou_freeTableFile","lou_compileString","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_hyphenate","lou_readCharFromFile","lou_logEnd","lou_free"],"critical_calls":["lou_logFile","lou_registerLogCallback","lou_indexTables","lou_listTables","lou_freeTableFiles","lou_findTable","lou_freeTableFile","lou_compileString","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_hyphenate","lou_readCharFromFile","lou_logEnd","lou_free"],"visited":0}
/**/


extern "C" {
}

// Define log callback function type
typedef void (*logcallback)(logLevels, const char*);

// Simple log callback function
void  id_000023_simple_log_callback(logLevels level, const char* message) {
    // Do nothing for fuzzing - just a stub
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<70) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if data is too small for meaningful operations
    if (size < 10) {
        return 0;
    }

    // Setup: Create a temporary file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Set up logging
    lou_logFile(fuzz_str_1);
    lou_registerLogCallback( id_000023_simple_log_callback);

    // 2. Index tables - create a static table list
    const char* tables_to_index[] = {"en-us-g2.ctb", "en-ueb-g2.ctb", nullptr};
    lou_indexTables(tables_to_index);

    // 3. List available tables
    char** table_list = lou_listTables();
    if (table_list) {
        // Free the table list when done
        lou_freeTableFiles(table_list);
    }

    // 4. Find a table using part of the input as query
    // Use first 50 bytes or less if input is smaller
    size_t query_len = size > 50 ? 50 : size;
    char* query = (char*)malloc(query_len + 1);
    if (query) {
        memcpy(query, data, query_len);
        query[query_len] = '\0';
        char* found_table = lou_findTable(query);
        if (found_table) {
            lou_freeTableFile(found_table);
        }
        free(query);
    }

    // 5. Compile a table string
    // Use middle section of input for compilation
    size_t compile_len = size > 100 ? 100 : size / 2;
    size_t compile_offset = size / 4;
    if (compile_offset + compile_len > size) {
        compile_offset = 0;
    }
    
    char* compile_string = (char*)malloc(compile_len + 1);
    if (compile_string) {
        memcpy(compile_string, data + compile_offset, compile_len);
        compile_string[compile_len] = '\0';
        
        // Try to compile the string as a table
        lou_compileString("en-us-g2.ctb", compile_string);
        free(compile_string);
    }

    // 6. Get emphasis classes and typeform
    const char** emph_classes = lou_getEmphClasses(fuzz_str_2);
    if (emph_classes) {
        // Try to get typeform for each emphasis class
        for (int i = 0; emph_classes[i] != nullptr; i++) {
            formtype typeform = lou_getTypeformForEmphClass(fuzz_str_3, emph_classes[i]);
            (void)typeform; // Use to avoid unused variable warning
        }
        lou_freeEmphClasses(emph_classes);
    }

    // 7. Hyphenate input text
    // Convert input data to widechar for hyphenation
    size_t max_widechars = size / sizeof(widechar);
    if (max_widechars > 0) {
        std::vector<widechar> inbuf(max_widechars);
        std::vector<char> hyphens(max_widechars);
        
        // Convert bytes to widechars (simple conversion)
        for (size_t i = 0; i < max_widechars && i * sizeof(widechar) < size; i++) {
            inbuf[i] = 0;
            for (size_t j = 0; j < sizeof(widechar) && (i * sizeof(widechar) + j) < size; j++) {
                inbuf[i] |= (data[i * sizeof(widechar) + j] << (8 * j));
            }
        }
        
        // Perform hyphenation
        int inlen = static_cast<int>(max_widechars);
        lou_hyphenate(fuzz_str_4, inbuf.data(), inlen, hyphens.data(), fuzz_int32_t_5);
    }

    // 8. Try to read a character from the input file
    int mode = 0;
    lou_readCharFromFile("input_file", &mode);

    // Cleanup
    assert_file_closed(&in_file);;
    lou_logEnd();
    lou_free();

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}