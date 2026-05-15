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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_readCharFromFile","lou_compileString","lou_translateString","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_findTable","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerLogCallback","lou_readCharFromFile","lou_compileString","lou_translateString","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_findTable","lou_freeTableFile","lou_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to process input data:

*/


// Include liblouis headers
extern "C" {
}

// Custom log callback function
static void  id_000016_log_callback(logLevels level, const char *msg) {
    // Simple callback that does nothing to avoid output during fuzzing
    (void)level;
    (void)msg;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
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
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Register log callback
    lou_registerLogCallback( id_000016_log_callback);
    
    // 2. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Write data to a file for lou_readCharFromFile
    FILE *temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // 3. Read character from file using lou_readCharFromFile
    int mode = 0;
    int char_result = lou_readCharFromFile("input_file", &mode);
    (void)char_result; // Result may be -1 for EOF
    
    // 4. Compile a simple braille table from string
    // Create a minimal valid braille table
    const char *table_string = 
        "table test\n"
        "include en-us-g1.ctb\n"
        "locale en\n";
    
    int compile_result = lou_compileString("test", table_string);
    (void)compile_result; // Result may be 0 or error
    
    // 5. Prepare input buffer from data
    // Convert data to widechar array (liblouis uses 16-bit wide chars)
    size_t wchar_count = size / sizeof(widechar);
    if (wchar_count == 0) wchar_count = 1;
    
    std::vector<widechar> inbuf(wchar_count);
    std::vector<widechar> outbuf(wchar_count * 2); // Extra space for output
    std::vector<char> hyphens(wchar_count);
    std::vector<char> spacing(wchar_count);
    std::vector<formtype> typeform(wchar_count, 0);
    
    // Copy data to input buffer (handle possible partial widechar)
    size_t copy_size = std::min(size, wchar_count * sizeof(widechar));
    memcpy(inbuf.data(), data, copy_size);
    
    // 6. Use lou_translateString
    int inlen = static_cast<int>(wchar_count);
    int outlen = static_cast<int>(outbuf.size());
    const char *table_list = "en-us-g1.ctb,test";
    
    int translate_result = lou_translateString(
        table_list,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform.data(),
        spacing.data(),
        0
    );
    (void)translate_result;
    
    // 7. Use lou_charToDots
    std::vector<widechar> dots_out(wchar_count);
    int dots_result = lou_charToDots(
        table_list,
        inbuf.data(),
        dots_out.data(),
        static_cast<int>(wchar_count),
        0
    );
    (void)dots_result;
    
    // 8. Use lou_dotsToChar
    std::vector<widechar> chars_out(wchar_count);
    int chars_result = lou_dotsToChar(
        table_list,
        dots_out.data(),
        chars_out.data(),
        static_cast<int>(wchar_count),
        0
    );
    (void)chars_result;
    
    // 9. Use lou_hyphenate
    int hyphen_result = lou_hyphenate(
        table_list,
        inbuf.data(),
        static_cast<int>(wchar_count),
        hyphens.data(),
        0
    );
    (void)hyphen_result;
    
    // 10. Try to find and free a table file
    char *table_file = lou_findTable("en-us-g1.ctb");
    if (table_file) {
        lou_freeTableFile(table_file);
    }
    
    // 11. Clean up resources
    assert_file_closed(&in_file);;
    
    // Remove temporary file
    remove("input_file");
    
    // 12. Call lou_free to clean up liblouis internal state
    lou_free();
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}