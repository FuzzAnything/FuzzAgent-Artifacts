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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 1.5, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_getDataPath","lou_setDataPath","lou_findTable","lou_compileString","lou_hyphenate","lou_charToDots","lou_readCharFromFile","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerLogCallback","lou_getDataPath","lou_setDataPath","lou_findTable","lou_compileString","lou_hyphenate","lou_charToDots","lou_readCharFromFile","lou_freeTableFile","lou_free"],"visited":1}
/**/


// Include liblouis headers
extern "C" {
}

// Simple log callback function
static void  id_000010_log_callback(logLevels level, const char *message) {
    // Empty callback - just to satisfy API requirement
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return for insufficient data
    if (size < 4) {
	return 0;
	}
	
    
    // Variables for resource management
    char *old_path = nullptr;
    char *table_path = nullptr;
    char *found_table = nullptr;
    widechar *wide_buf = nullptr;
    widechar *dots_buf = nullptr;
    char *hyphens = nullptr;
    FILE *in_file = nullptr;
    
    // 1. Register log callback
    lou_registerLogCallback( id_000010_log_callback);
    
    // 2. Get and set data path
    old_path = lou_getDataPath();
    if (old_path) {
        // Use part of input as new path
        size_t path_len = size > 256 ? 256 : size / 4;
        char *new_path = (char *)malloc(path_len + 1);
        if (new_path) {
            memcpy(new_path, data, path_len > size ? size : path_len);
            new_path[path_len] = '\0';
            char *temp = lou_setDataPath(new_path);
            if (temp) free(temp);
            free(new_path);
        }
    }
    
    // 3. Find a table using part of input as query
    size_t query_len = size > 128 ? 128 : size / 8;
    char *query = (char *)malloc(query_len + 1);
    if (query) {
        memcpy(query, data + (size > 100 ? 100 : 0), 
               query_len > (size - 100) ? (size - 100) : query_len);
        query[query_len] = '\0';
        found_table = lou_findTable(query);
        free(query);
    }
    
    // 4. Use a fixed table list for compilation
    const char *tableList = "en-us-g2.ctb";
    
    // 5. Compile a table string using input data
    if (size > 50) {
        char *compile_str = (char *)malloc(50 + 1);
        if (compile_str) {
            memcpy(compile_str, data, 50);
            compile_str[50] = '\0';
            lou_compileString(tableList, compile_str);
            free(compile_str);
        }
    }
    
    // 6. Process the input as widechar data for hyphenation and charToDots
    // Convert input to widechar (2 bytes per character)
    size_t wide_len = size / 2;
    if (wide_len > 0) {
        wide_buf = (widechar *)malloc(wide_len * sizeof(widechar));
        if (wide_buf) {
            // Convert bytes to widechars
            for (size_t i = 0; i < wide_len && i * 2 + 1 < size; i++) {
                wide_buf[i] = (data[i * 2] << 8) | data[i * 2 + 1];
            }
            
            // Allocate hyphen array
            hyphens = (char *)malloc(wide_len + 1);
            if (hyphens) {
                // Call hyphenate API
                lou_hyphenate(tableList, wide_buf, wide_len, hyphens, 0);
            }
            
            // Allocate buffer for dots conversion
            dots_buf = (widechar *)malloc(wide_len * sizeof(widechar));
            if (dots_buf) {
                // Call charToDots API
                lou_charToDots(tableList, wide_buf, dots_buf, wide_len, 0);
            }
        }
    }
    
    // 7. Create a FILE* from input data using fmemopen
    in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read a character from the file (though we don't use the result)
        int mode = 0;
        lou_readCharFromFile("input_file", &mode);
        assert_file_closed(&in_file);;
    }
    
    // 8. Clean up allocated resources
    if (old_path) free(old_path);
    if (found_table) lou_freeTableFile(found_table);
    if (wide_buf) free(wide_buf);
    if (dots_buf) free(dots_buf);
    if (hyphens) free(hyphens);
    
    // 9. Free liblouis resources
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}