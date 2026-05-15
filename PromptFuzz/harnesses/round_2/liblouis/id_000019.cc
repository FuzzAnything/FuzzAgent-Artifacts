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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 3
//<Quality> {"density":3,"unique_branches":{"lou_dotsToChar":[[4163,7,4163,25,0,0,4,1],[4164,5,4164,39,0,0,4,0],[4169,7,4169,24,0,0,4,1]]},"library_calls":["lou_readCharFromFile","lou_getDataPath","lou_indexTables","lou_getTypeformForEmphClass","lou_dotsToChar","lou_hyphenate","lou_free"],"critical_calls":["lou_readCharFromFile","lou_getDataPath","lou_indexTables","lou_getTypeformForEmphClass","lou_dotsToChar","lou_hyphenate","lou_free"],"visited":0}
/**/


// Include liblouis headers (assuming they're available)

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < sizeof(widechar) || size > 1024 * 1024) {
        return 0; // Skip too small or too large inputs
    }

    // 1. Write input data to a file for lou_readCharFromFile
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
	assert_file_closed(&input_file);
	return 0;
	}
	
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;

    // 2. Use lou_readCharFromFile to read from the file
    int mode = 0;
    widechar read_char = lou_readCharFromFile("input_file", &mode);
    
    // 3. Get data path using lou_getDataPath
    char *data_path = lou_getDataPath();
    
    // 4. Index some tables - using a simple table list
    const char *tables[] = {"en-us-g2.ctb", NULL};
    lou_indexTables(tables);

    // 5. Get typeform for emphasis class
    const char *table_list = "en-us-g2.ctb";
    const char *emph_class = "emphasis";
    formtype typeform = lou_getTypeformForEmphClass(table_list, emph_class);

    // 6. Use lou_dotsToChar - convert dot patterns to characters
    // Convert part of input data to widechar array
    size_t dots_length = size / sizeof(widechar);
    if (dots_length > 0) {
        std::vector<widechar> inbuf(dots_length);
        std::vector<widechar> outbuf(dots_length);
        
        // Copy data to widechar buffer
        memcpy(inbuf.data(), data, std::min(size, dots_length * sizeof(widechar)));
        
        lou_dotsToChar(table_list, inbuf.data(), outbuf.data(), 
                      std::min(static_cast<int>(dots_length), 100), mode);
    }

    // 7. Use lou_hyphenate - hyphenate input text
    // Prepare input buffer for hyphenation
    if (size >= sizeof(widechar)) {
        size_t hyphen_length = std::min(size / sizeof(widechar), static_cast<size_t>(100));
        std::vector<widechar> hyphen_buf(hyphen_length);
        std::vector<char> hyphens(hyphen_length + 1);
        
        memcpy(hyphen_buf.data(), data, hyphen_length * sizeof(widechar));
        
        lou_hyphenate(table_list, hyphen_buf.data(), hyphen_length, 
                     hyphens.data(), mode);
    }

    // 8. Clean up - free resources
    lou_free();
    
    // Free data path if it was allocated
    // Note: lou_getDataPath returns a pointer that should not be freed according to documentation
    
    // Remove temporary file
    remove("input_file");

    assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}