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
//<ID> 474
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_indexTables","lou_getEmphClasses","lou_freeEmphClasses","lou_registerTableResolver","lou_getTable","lou_readCharFromFile","lou_translateString","lou_free"],"critical_calls":["lou_indexTables","lou_getEmphClasses","lou_freeEmphClasses","lou_registerTableResolver","lou_getTable","lou_readCharFromFile","lou_translateString","lou_free"],"visited":1}
/**/


// Include liblouis headers

// Custom table resolver function
static char ** id_000020_custom_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns NULL - this is just for demonstration
    (void)table;
    (void)base;
    return nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
        return 0; // Need at least some data
    }

    // 1. Create a FILE* from the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create an output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used

    // 4. Extract table list from input data
    // Find null terminator in the data for table list
    size_t table_list_len = 0;
    while (table_list_len < size && data[table_list_len] != 0) {
        table_list_len++;
    }
    
    if (table_list_len >= size) {
        // No null terminator found, use first half as table list
        table_list_len = size / 2;
    }
    
    char *table_list = (char *)malloc(table_list_len + 1);
    if (!table_list) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    memcpy(table_list, data, table_list_len);
    table_list[table_list_len] = '\0';
    
    // 5. Calculate remaining data for other operations
    size_t remaining_offset = table_list_len + 1;
    if (remaining_offset > size) {
        remaining_offset = table_list_len;
    }
    
    size_t remaining_size = size - remaining_offset;
    const uint8_t *remaining_data = data + remaining_offset;

    // 6. Call lou_indexTables
    const char *tables_array[] = {table_list, nullptr};
    lou_indexTables(tables_array);

    // 7. Call lou_getEmphClasses
    const char **emph_classes = lou_getEmphClasses(table_list);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }

    // 8. Call lou_registerTableResolver
    lou_registerTableResolver( id_000020_custom_table_resolver);

    // 9. Call lou_getTable
    const void *table = lou_getTable(table_list);
    (void)table; // Mark as used

    // 10. Call lou_readCharFromFile
    int mode = 0;
    int read_result = lou_readCharFromFile("input_file", &mode);
    (void)read_result; // Mark as used

    // 11. Prepare data for lou_translateString
    // Convert remaining data to widechar format
    if (remaining_size >= sizeof(widechar)) {
        size_t widechar_count = remaining_size / sizeof(widechar);
        std::vector<widechar> inbuf(widechar_count);
        std::vector<widechar> outbuf(widechar_count * 2); // Allocate extra space
        
        // Copy data to widechar buffer
        memcpy(inbuf.data(), remaining_data, 
               std::min(remaining_size, widechar_count * sizeof(widechar)));
        
        int inlen = static_cast<int>(widechar_count);
        int outlen = static_cast<int>(outbuf.size());
        formtype typeform = 0;
        char spacing = 0;
        
        // 12. Call lou_translateString
        lou_translateString(table_list, inbuf.data(), &inlen, 
                           outbuf.data(), &outlen, &typeform, &spacing, 0);
    }

    // 13. Write some data to output file
    if (remaining_size > 0) {
        fwrite(remaining_data, 1, std::min(remaining_size, (size_t)1024), out_file);
    }

    // 14. Clean up
    free(table_list);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file we created
    remove("output_file");
    
    // 15. Call lou_free to clean up liblouis resources
    lou_free();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}