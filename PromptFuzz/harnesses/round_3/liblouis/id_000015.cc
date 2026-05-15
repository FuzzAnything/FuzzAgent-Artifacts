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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"analyzeTable":[[724,6,724,44,0,0,4,1]],"lou_getEmphClasses":[[5090,6,5090,12,0,0,4,0]]},"library_calls":["lou_setDataPath","lou_registerTableResolver","lou_readCharFromFile","lou_checkTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_getTableInfo","lou_freeTableInfo","lou_hyphenate","lou_translate","lou_backTranslate","lou_backTranslateString","lou_free"],"critical_calls":["lou_setDataPath","lou_registerTableResolver","lou_readCharFromFile","lou_checkTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_getTableInfo","lou_freeTableInfo","lou_hyphenate","lou_translate","lou_backTranslate","lou_backTranslateString","lou_free"],"visited":1}
/**/


// Include liblouis headers

// Custom table resolver function
static char ** id_000015_test_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns a static table path
    static char *table_paths[] = {(char *)"en-us-g1.ctb", nullptr};
    return table_paths;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<174) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
        return 0; // Not enough data for meaningful fuzzing
    }

    // Initialize liblouis
    lou_setDataPath(fuzz_str_1);
    
    // 1. Register table resolver
    lou_registerTableResolver( id_000015_test_table_resolver);
    
    // 2. Create input file from the fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Read character from file using lou_readCharFromFile
    int mode = 0;
    lou_readCharFromFile("input_file", &mode);
    
    // 4. Prepare table list and buffers for translation
    const char *tableList = "en-us-g1.ctb";
    
    // Check if table is valid
    if (lou_checkTable(fuzz_str_2) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 5. Get emphasis classes
    const char **emphClasses = lou_getEmphClasses(fuzz_str_3);
    if (emphClasses) {
        // 6. Get typeform for first emphasis class if available
        if (emphClasses[0]) {
            formtype typeform = lou_getTypeformForEmphClass(fuzz_str_4, emphClasses[0]);
            (void)typeform; // Use to avoid unused variable warning
        }
        lou_freeEmphClasses(emphClasses);
    }
    
    // 7. Get table info
    char *tableInfo = lou_getTableInfo(fuzz_str_5, fuzz_str_6);
    if (tableInfo) {
        lou_freeTableInfo(tableInfo);
    }
    
    // 8. Prepare input buffer from fuzzing data
    // Convert first part of data to widechars for processing
    int inlen = (size / 2 < 100) ? size / 2 : 100;
    if (inlen < 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    widechar *inbuf = new widechar[inlen];
    for (int i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)data[i];
    }
    
    // 9. Hyphenate the input
    char *hyphens = new char[inlen];
    lou_hyphenate(fuzz_str_7, inbuf, inlen, hyphens, fuzz_int32_t_8);
    
    // 10. Translate the input
    int outlen = inlen * 2;
    widechar *outbuf = new widechar[outlen];
    int actual_inlen = inlen;
    int actual_outlen = outlen;
    formtype *typeform_array = new formtype[inlen];
    char *spacing = new char[inlen];
    int *outputPos = new int[inlen];
    int *inputPos = new int[inlen];
    int *cursorPos = new int[inlen];
    
    // Initialize arrays
    for (int i = 0; i < inlen; i++) {
        typeform_array[i] = 0;
        spacing[i] = 0;
        outputPos[i] = inputPos[i] = cursorPos[i] = 0;
    }
    
    // Perform translation
    lou_translate(fuzz_str_9, inbuf, &actual_inlen, outbuf, &actual_outlen,
                  typeform_array, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_10);
    
    // 11. Backtranslate the result
    int back_inlen = actual_outlen;
    int back_outlen = inlen;
    widechar *back_outbuf = new widechar[back_outlen];
    
    lou_backTranslate(fuzz_str_11, outbuf, &back_inlen, back_outbuf, &back_outlen,
                     typeform_array, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_12);
    
    // 12. Also test backTranslateString
    int str_back_inlen = actual_outlen;
    int str_back_outlen = inlen;
    widechar *str_back_outbuf = new widechar[str_back_outlen];
    
    lou_backTranslateString(fuzz_str_13, outbuf, &str_back_inlen, str_back_outbuf,
                           &str_back_outlen, typeform_array, spacing, fuzz_int32_t_14);
    
    // 13. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some results to output file
        fwrite(outbuf, sizeof(widechar), actual_outlen, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 14. Cleanup - free all allocated resources
    delete[] inbuf;
    delete[] hyphens;
    delete[] outbuf;
    delete[] typeform_array;
    delete[] spacing;
    delete[] outputPos;
    delete[] inputPos;
    delete[] cursorPos;
    delete[] back_outbuf;
    delete[] str_back_outbuf;
    
    assert_file_closed(&in_file);;
    
    // 15. Free liblouis resources
    lou_free();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}