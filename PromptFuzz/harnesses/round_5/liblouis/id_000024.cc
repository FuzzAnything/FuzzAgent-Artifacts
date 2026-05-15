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
//<ID> 554
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_indexTables","lou_getTableInfo","lou_freeTableInfo","lou_registerTableResolver","lou_backTranslateString","lou_dotsToChar","lou_free"],"critical_calls":["lou_indexTables","lou_getTableInfo","lou_freeTableInfo","lou_registerTableResolver","lou_backTranslateString","lou_dotsToChar","lou_free"],"visited":0}
/**/


extern "C" {
}

// Dummy table resolver that returns NULL for all requests
static char ** id_000024_dummy_table_resolver(const char *table, const char *base) {
    return NULL;
}

// Helper function to create a table list from input data
static std::string  id_000024_create_table_list_from_data(const uint8_t *data, size_t size) {
    if (size < 2) return "en-us-g2.ctb";
    
    // Use first few bytes to select from common tables
    const char* tables[] = {
        "en-us-g2.ctb",
        "en-ueb-g2.ctb",
        "en-gb-g2.ctb",
        "de-de-g2.ctb",
        "fr-bfu-g2.ctb"
    };
    
    size_t table_index = data[0] % (sizeof(tables) / sizeof(tables[0]));
    return std::string(tables[table_index]);
}

// Helper function to create widechar buffer from input data
static std::vector<widechar>  id_000024_create_widechar_buffer(const uint8_t *data, size_t size) {
    std::vector<widechar> buffer;
    
    // Convert bytes to widechars (2 bytes per widechar)
    for (size_t i = 0; i + 1 < size; i += 2) {
        widechar wc = (data[i] << 8) | data[i + 1];
        if (wc == 0) wc = ' '; // Replace null with space
        buffer.push_back(wc);
    }
    
    // Add null terminator if needed
    if (!buffer.empty() && buffer.back() != 0) {
        buffer.push_back(0);
    }
    
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
        return 0;
    }

    // 1. Create table list from input data
    std::string table_list =  id_000024_create_table_list_from_data(data, size);
    const char* table_list_cstr = table_list.c_str();
    
    // 2. Index the tables
    const char* tables_to_index[] = {table_list_cstr, NULL};
    lou_indexTables(tables_to_index);
    
    // 3. Get table information
    char* table_info = lou_getTableInfo(table_list_cstr, "language");
    if (table_info) {
        // Could use table_info here
        lou_freeTableInfo(table_info);
    }
    
    // 4. Register a table resolver
    lou_registerTableResolver( id_000024_dummy_table_resolver);
    
    // 5. Create widechar buffer from input data for processing
    std::vector<widechar> inbuf =  id_000024_create_widechar_buffer(data, size);
    if (inbuf.empty()) {
        return 0;
    }
    
    // 6. Prepare for back-translation (braille to text)
    int inlen = inbuf.size() - 1; // Exclude null terminator
    std::vector<widechar> outbuf(inlen * 4); // Allocate enough space
    int outlen = outbuf.size();
    
    // 7. Perform back-translation
    int mode = 0;
    int back_trans_result = lou_backTranslateString(
        table_list_cstr,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        NULL,  // typeform
        NULL,  // spacing
        mode
    );
    
    // 8. Prepare for dotsToChar conversion
    // We'll use the first part of input data as dots pattern
    size_t dots_length = size < 100 ? size : 100;
    std::vector<widechar> dots_inbuf(dots_length);
    std::vector<widechar> dots_outbuf(dots_length);
    
    // Convert bytes to widechar for dots input
    for (size_t i = 0; i < dots_length; i++) {
        dots_inbuf[i] = data[i] % 256; // Braille dots are 0-255
    }
    
    // 9. Perform dotsToChar conversion
    int dots_result = lou_dotsToChar(
        table_list_cstr,
        dots_inbuf.data(),
        dots_outbuf.data(),
        dots_length,
        mode
    );
    
    // 10. Clean up resources
    lou_free();
    
    return 0;
}