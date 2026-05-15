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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 26
//<Quality> {"density":2,"unique_branches":{"compileTable":[[5047,32,5047,41,0,0,4,1],[5048,9,5048,52,0,0,4,1],[5052,7,5052,23,0,0,4,1],[5053,8,5053,58,0,0,4,1],[5057,32,5057,41,0,0,4,0],[5057,32,5057,41,0,0,4,1],[5058,9,5058,56,0,0,4,1],[5068,7,5068,23,0,0,4,1]],"compileRule":[[3066,7,3066,13,0,0,4,0]],"_lou_getCharForDots":[[662,6,662,11,0,0,4,1]],"getTable":[[5221,8,5221,35,0,0,4,1],[5234,8,5234,31,0,0,4,1]],"lou_getTypeformForEmphClass":[[5262,6,5262,12,0,0,4,1],[5263,18,5263,38,0,0,4,0],[5263,42,5263,66,0,0,4,0],[5263,42,5263,66,0,0,4,1],[5264,7,5264,55,0,0,4,1]],"_lou_getTranslationTable":[[5146,6,5146,11,0,0,4,0],[5147,7,5147,28,0,0,4,1]],"lou_dotsToChar":[[4160,6,4160,19,0,0,4,1],[4160,23,4160,34,0,0,4,1],[4161,14,4161,24,0,0,4,0],[4161,14,4161,24,0,0,4,1],[4163,7,4163,25,0,0,4,0],[4164,5,4164,39,0,0,4,1],[4169,7,4169,24,0,0,4,0]]},"library_calls":["lou_readCharFromFile","lou_getDataPath","lou_indexTables","lou_getTypeformForEmphClass","lou_dotsToChar","lou_hyphenate","lou_hyphenate","lou_free"],"critical_calls":["lou_readCharFromFile","lou_getDataPath","lou_indexTables","lou_getTypeformForEmphClass","lou_dotsToChar","lou_hyphenate","lou_hyphenate","lou_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if insufficient data
    if (size < 2) {
        return 0;
    }

    // 1. Write input data to a file for lou_readCharFromFile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a temporary file name
    const char *filename = "input_file";
    FILE *temp_file = fopen(filename, "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Copy data to temp file
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;

    // 2. Call lou_readCharFromFile
    int mode = 0;
    int read_result = lou_readCharFromFile(filename, &mode);

    // 3. Get data path
    char *data_path = lou_getDataPath();

    // 4. Index tables - using a minimal table list
    const char *tables[] = {"en-us-g2.ctb", NULL}; // Common English braille table
    lou_indexTables(tables);

    // 5. Get typeform for emphasis class
    formtype typeform = lou_getTypeformForEmphClass("en-us-g2.ctb", "emphasis");

    // 6. Process with lou_dotsToChar
    // Use first half of data as dots (converted to widechar)
    size_t dots_length = size / 2;
    if (dots_length > 0) {
        widechar *dots_buf = new widechar[dots_length];
        widechar *char_buf = new widechar[dots_length];
        
        // Convert bytes to widechar (simplified - assumes bytes are valid dots)
        for (size_t i = 0; i < dots_length; i++) {
            dots_buf[i] = static_cast<widechar>(data[i]);
        }
        
        int dots_result = lou_dotsToChar("en-us-g2.ctb", dots_buf, char_buf, dots_length, 0);
        
        // 7. Hyphenate the converted characters
        if (dots_length > 0) {
            char *hyphens = new char[dots_length + 1];
            int hyphen_result = lou_hyphenate("en-us-g2.ctb", char_buf, dots_length, hyphens, 0);
            delete[] hyphens;
        }
        
        delete[] dots_buf;
        delete[] char_buf;
    }

    // 8. Process remaining data with lou_hyphenate directly
    if (size > dots_length && size - dots_length > 0) {
        size_t remaining_length = size - dots_length;
        widechar *remaining_buf = new widechar[remaining_length];
        char *remaining_hyphens = new char[remaining_length + 1];
        
        // Convert remaining bytes to widechar
        for (size_t i = 0; i < remaining_length; i++) {
            remaining_buf[i] = static_cast<widechar>(data[dots_length + i]);
        }
        
        int hyphen_result = lou_hyphenate("en-us-g2.ctb", remaining_buf, remaining_length, remaining_hyphens, 0);
        
        delete[] remaining_buf;
        delete[] remaining_hyphens;
    }

    // 9. Cleanup
    lou_free();
    
    // Clean up temporary file
    remove(filename);
    assert_file_closed(&in_file);;
    
    // No need to free data_path as it's internal to liblouis
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}