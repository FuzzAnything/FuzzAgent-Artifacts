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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_getDataPath","lou_indexTables","lou_findTables","lou_checkTable","lou_getTypeformForEmphClass","lou_getTableInfo","lou_readCharFromFile","lou_freeTableFiles","lou_freeTableInfo","lou_free"],"critical_calls":["lou_getDataPath","lou_indexTables","lou_findTables","lou_checkTable","lou_getTypeformForEmphClass","lou_getTableInfo","lou_readCharFromFile","lou_freeTableFiles","lou_freeTableInfo","lou_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize liblouis data structures
    char* data_path = nullptr;
    char** table_list = nullptr;
    char* table_info = nullptr;
    formtype typeform = 0;
    int table_check_result = 0;
    
    // Create input file from the fuzzing data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (as per requirement #5, though not directly used here)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used to avoid warnings
    (void)out_fd; // Mark as used to avoid warnings
    
    // 1. Get the current data path
    data_path = lou_getDataPath();
    if (data_path) {
        // Could optionally log or use data_path
        // For fuzzing, we'll just verify it's not null
    }
    
    // 2. Index tables - we need a NULL-terminated array of tables
    // Since we don't have actual table names from input, we'll create a minimal array
    const char* dummy_tables[] = {"en-us-g2.ctb", nullptr};
    lou_indexTables(dummy_tables);
    
    // 3. Find tables using the input data as query
    // We'll use the first part of input as query string (null-terminated)
    char* query = nullptr;
    if (size > 0) {
        query = (char*)malloc(size + 1);
        if (query) {
            memcpy(query, data, size);
            query[size] = '\0';  // Null-terminate
            
            // Find tables with the query
            table_list = lou_findTables(query);
            
            free(query);
        }
    }
    
    // 4. Check tables if we found any
    if (table_list && table_list[0]) {
        table_check_result = lou_checkTable(table_list[0]);
        
        // 5. Get typeform for emphasis class if table check succeeded
        if (table_check_result == 0) {
            // Use a dummy emphasis class name
            const char* emph_class = "italic";
            typeform = lou_getTypeformForEmphClass(table_list[0], fuzz_str_1);
            
            // Try to get table info as well
            table_info = lou_getTableInfo(table_list[0], fuzz_str_2);
        }
    }
    
    // 6. Additional processing: try to read character from the input file
    // This consumes the input data through liblouis API
    if (in_file) {
        int mode = 0;
        int char_result = lou_readCharFromFile("input_file", &mode);
        (void)char_result; // Result may be -1 if file doesn't exist, which is OK for fuzzing
    }
    
    // 7. Write some output to the output file based on our findings
    if (out_file) {
        if (table_list && table_list[0]) {
            fprintf(out_file, "Found table: %s\n", table_list[0]);
            fprintf(out_file, "Table check result: %d\n", table_check_result);
            fprintf(out_file, "Typeform for italic: %u\n", typeform);
        }
        if (data_path) {
            fprintf(out_file, "Data path: %s\n", data_path);
        }
    }
    
    // 8. Cleanup resources
    // Free table list if we got one
    if (table_list) {
        lou_freeTableFiles(table_list);
    }
    
    // Free table info if we got one
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    // Close files
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Call lou_free to clean up liblouis internal resources
    lou_free();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}