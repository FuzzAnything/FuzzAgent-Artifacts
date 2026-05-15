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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_charSize","lou_checkTable","lou_findTable","lou_charToDots","lou_getTypeformForEmphClass","lou_freeTableFile","lou_free"],"critical_calls":["lou_charSize","lou_checkTable","lou_findTable","lou_charToDots","lou_getTypeformForEmphClass","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data for basic operations
    if (size < 4) {
        return 0;
    }

    // 1. Call lou_charSize() at least once
    int char_size = lou_charSize();
    (void)char_size; // Use the result to avoid unused variable warning

    // 2. Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Read table list from input data (first part as string)
    // Find null terminator or use first 256 bytes for table name
    size_t table_name_len = 0;
    for (size_t i = 0; i < size && i < 256; ++i) {
        if (data[i] == 0) {
            table_name_len = i;
            break;
        }
    }
    
    // If no null terminator found, use first 256 bytes
    if (table_name_len == 0) {
        table_name_len = size < 256 ? size : 256;
    }
    
    char *table_list = (char *)malloc(table_name_len + 1);
    if (!table_list) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    memcpy(table_list, data, table_name_len);
    table_list[table_name_len] = '\0';
    
    // 4. Check if table exists
    int table_check = lou_checkTable(table_list);
    
    // 5. Find table file (if available)
    char *table_file = lou_findTable(table_list);
    
    // 6. Prepare input buffer for lou_charToDots
    // Use remaining data after table name as widechar input
    size_t widechar_data_start = table_name_len + 1;
    if (widechar_data_start >= size) {
        widechar_data_start = table_name_len;
    }
    
    size_t widechar_data_size = size - widechar_data_start;
    size_t widechar_count = widechar_data_size / sizeof(widechar);
    
    if (widechar_count > 0) {
        // Create input buffer
        const widechar *inbuf = reinterpret_cast<const widechar *>(data + widechar_data_start);
        
        // Create output buffer
        widechar *outbuf = (widechar *)malloc(widechar_count * sizeof(widechar));
        if (outbuf) {
            // 7. Call lou_charToDots
            int mode = 0; // Default mode
            int result = lou_charToDots(table_list, inbuf, outbuf, widechar_count, mode);
            (void)result; // Use result to avoid warning
            
            free(outbuf);
        }
    }
    
    // 8. Call lou_getTypeformForEmphClass
    // Use a sample emphasis class name
    const char *emph_class = "emphasis";
    formtype typeform = lou_getTypeformForEmphClass(table_list, emph_class);
    (void)typeform; // Use result to avoid warning
    
    // 9. Free table file if found
    if (table_file) {
        lou_freeTableFile(table_file);
    }
    
    // 10. Clean up
    free(table_list);
    assert_file_closed(&in_file);;
    
    // 11. Call lou_free() at the end
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}