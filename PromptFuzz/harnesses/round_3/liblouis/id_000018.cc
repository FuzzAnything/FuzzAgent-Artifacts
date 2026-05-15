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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 2
//<Quality> {"density":2,"unique_branches":{"lou_getTypeformForEmphClass":[[5263,42,5263,66,0,0,4,0],[5264,7,5264,55,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_readCharFromFile","lou_logPrint","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_hyphenate","lou_logPrint","lou_freeEmphClasses"],"critical_calls":["lou_registerLogCallback","lou_readCharFromFile","lou_logPrint","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_hyphenate","lou_logPrint","lou_freeEmphClasses"],"visited":0}
/**/


// Custom log callback function
static void  id_000018_my_log_callback(logLevels level, const char *message) {
    // Simple callback that does nothing - just to demonstrate the API
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<49) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. First, we need to write the input data to a file so lou_readCharFromFile can read it
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
        assert_file_closed(&input_file);
	return 0;
    }
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    // 2. Register a log callback
    lou_registerLogCallback( id_000018_my_log_callback);
    
    // 3. Use a fixed table for testing - en-us-g2.ctb is common in liblouis
    const char *tableList = "en-us-g2.ctb";
    
    // 4. Read character from the file we created
    int mode = 0;
    int char_result = lou_readCharFromFile("input_file", &mode);
    
    // 5. Log something about what we're doing
    lou_logPrint("Fuzzing liblouis with hyphenation and emphasis APIs");
    
    // 6. Get emphasis classes for the table
    const char **emph_classes = lou_getEmphClasses(fuzz_str_1);
    
    // 7. If we have emphasis classes, get typeform for the first one
    formtype typeform = 0;
    if (emph_classes && emph_classes[0]) {
        typeform = lou_getTypeformForEmphClass(fuzz_str_2, emph_classes[0]);
    }
    
    // 8. Now use the input data for hyphenation
    // Convert the input data to widechar format for liblouis
    // We need at least some data to work with
    if (size >= 2) {
        // Calculate how many widechars we can have (each widechar is 2 bytes)
        size_t widechar_count = size / 2;
        if (widechar_count > 0) {
            std::vector<widechar> inbuf(widechar_count);
            std::vector<char> hyphens(widechar_count + 1); // +1 for safety
            
            // Copy data to widechar buffer (simple byte-to-widechar conversion)
            // Note: This is a simplification - real usage would properly encode the text
            for (size_t i = 0; i < widechar_count; i++) {
                inbuf[i] = (data[i * 2] << 8) | data[i * 2 + 1];
            }
            
            // Call hyphenate with the data
            int hyphen_result = lou_hyphenate(
                fuzz_str_3, 
                inbuf.data(), 
                static_cast<int>(widechar_count), 
                hyphens.data(), 
                fuzz_int32_t_4  // mode = 0 for default
            );
            
            // The hyphens array now contains hyphenation points
            // We could log something about the result
            if (hyphen_result == 0) {
                lou_logPrint("Hyphenation completed");
            }
        }
    }
    
    // 9. Clean up all allocated resources
    
    // Free emphasis classes if they were allocated
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // Remove the temporary input file
    remove("input_file");
    
    // Note: We don't need to free the table list as it's a string literal
    
    assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;  // Non-zero return values are reserved for future use
}