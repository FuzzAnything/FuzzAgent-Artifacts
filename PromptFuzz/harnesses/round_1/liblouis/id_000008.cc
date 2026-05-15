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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 1.3333334, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_logFile","lou_getDataPath","lou_compileString","lou_dotsToChar","lou_charToDots","lou_translatePrehyphenated","lou_logEnd"],"critical_calls":["lou_logFile","lou_getDataPath","lou_compileString","lou_dotsToChar","lou_charToDots","lou_translatePrehyphenated","lou_logEnd"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<146) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // First, set up logging to a file
    lou_logFile(fuzz_str_1);
    
    // Get and store the data path (though we may not use it directly)
    char *data_path = lou_getDataPath();
    
    // Define a simple braille table string for compilation
    // Using a minimal valid table that can be compiled
    const char *table_list = "tests/tables/unicode.dis";
    const char *table_string = 
        "unicode.dis\n"
        "noback context \\s 0\n"
        "noback \\s 0";
    
    // Compile the table string
    int compile_result = lou_compileString(table_list, fuzz_str_2);
    
    // Create a FILE* from the input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free(data_path);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Calculate how many widechars we have (2 bytes each)
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        assert_file_closed(&in_file);;
        free(data_path);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate buffers for the input dots (from fuzzer data)
    widechar *inbuf_dots = (widechar *)malloc(widechar_count * sizeof(widechar));
    if (!inbuf_dots) {
        assert_file_closed(&in_file);;
        free(data_path);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the widechars from the file
    size_t read_count = fread(inbuf_dots, sizeof(widechar), widechar_count, in_file);
    assert_file_closed(&in_file);;
    
    // Adjust count based on actual read
    widechar_count = read_count;
    if (widechar_count == 0) {
        free(inbuf_dots);
        free(data_path);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate output buffer for dotsToChar conversion
    widechar *outbuf_chars = (widechar *)malloc(widechar_count * sizeof(widechar));
    if (!outbuf_chars) {
        free(inbuf_dots);
        free(data_path);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Convert dots to characters using lou_dotsToChar
    int dots_to_char_result = lou_dotsToChar(
        fuzz_str_3, 
        inbuf_dots, 
        outbuf_chars, 
        widechar_count, 
        fuzz_int32_t_4  // mode
    );
    
    // Allocate buffer for charToDots conversion (back to dots)
    widechar *outbuf_dots_again = (widechar *)malloc(widechar_count * sizeof(widechar));
    if (!outbuf_dots_again) {
        free(inbuf_dots);
        free(outbuf_chars);
        free(data_path);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Convert characters back to dots using lou_charToDots
    int char_to_dots_result = lou_charToDots(
        fuzz_str_5,
        outbuf_chars,
        outbuf_dots_again,
        widechar_count,
        0  // mode
    );
    
    // Set up for translatePrehyphenated function
    int inlen = widechar_count;
    int outlen = widechar_count * 2;  // Allow for expansion
    
    // Allocate output buffer for translation
    widechar *outbuf_translated = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf_translated) {
        free(inbuf_dots);
        free(outbuf_chars);
        free(outbuf_dots_again);
        free(data_path);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate arrays for typeform, spacing, positions, and hyphens
    formtype *typeform = (formtype *)calloc(widechar_count, sizeof(formtype));
    char *spacing = (char *)calloc(widechar_count, sizeof(char));
    int *outputPos = (int *)calloc(widechar_count, sizeof(int));
    int *inputPos = (int *)calloc(widechar_count, sizeof(int));
    int *cursorPos = (int *)calloc(1, sizeof(int));
    char *inputHyphens = (char *)calloc(widechar_count, sizeof(char));
    char *outputHyphens = (char *)calloc(widechar_count, sizeof(char));
    
    // Initialize hyphens with a simple pattern (every other position)
    for (size_t i = 0; i < widechar_count; i++) {
        inputHyphens[i] = (i % 2 == 0) ? '1' : '0';
    }
    
    // Call translatePrehyphenated with the character buffer
    int translate_result = lou_translatePrehyphenated(
        fuzz_str_6,
        outbuf_chars,           // Use the characters from dotsToChar
        &inlen,
        outbuf_translated,
        &outlen,
        typeform,
        spacing,
        outputPos,
        inputPos,
        cursorPos,
        inputHyphens,
        outputHyphens,
        0  // mode
    );
    
    // Create an output file to write results (optional, for completeness)
    FILE *out_file = fopen("fuzz_output.txt", "wb");
    if (out_file) {
        // Write the translated output
        fwrite(outbuf_translated, sizeof(widechar), outlen, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    free(inbuf_dots);
    free(outbuf_chars);
    free(outbuf_dots_again);
    free(outbuf_translated);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(inputHyphens);
    free(outputHyphens);
    free(data_path);
    
    // End logging
    lou_logEnd();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}