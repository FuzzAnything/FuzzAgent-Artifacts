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
//<ID> 693
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 21
//<Quality> {"density":9,"unique_branches":{"compileRule":[[3488,8,3488,45,0,0,4,0],[3721,8,3722,28,0,0,4,0],[3753,8,3753,43,0,0,4,0],[3756,9,3756,11,0,0,4,0],[3782,8,3783,41,0,0,4,0],[3920,12,3920,13,0,0,4,1]],"pattern_compile_expression":[[868,2,868,10,0,0,4,0],[871,7,871,30,0,0,4,1],[874,7,874,31,0,0,4,1],[885,7,885,59,0,0,4,1],[891,15,891,37,0,0,4,0],[891,15,891,37,0,0,4,1],[892,8,892,33,0,0,4,1],[901,7,901,10,0,0,4,1]],"_lou_showString":[[162,46,162,72,0,0,4,1],[199,8,199,66,0,0,4,0]],"hexValue":[[1240,32,1240,48,0,0,4,1],[1242,32,1242,48,0,0,4,1]],"parseDots":[[1454,4,1454,12,0,0,4,0],[1489,8,1489,16,0,0,4,0]],"printSource":[[803,7,803,18,0,0,4,1]]},"library_calls":["lou_compileString","lou_getTableInfo","lou_freeTableInfo","lou_checkTable","lou_translate","lou_translatePrehyphenated","lou_backTranslateString","lou_hyphenate","lou_dotsToChar","lou_free"],"critical_calls":["lou_compileString","lou_getTableInfo","lou_freeTableInfo","lou_checkTable","lou_translate","lou_translatePrehyphenated","lou_backTranslateString","lou_hyphenate","lou_dotsToChar","lou_free"],"visited":5}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<1059) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeCharArray(char, fuzz_char_array_1, fuzz_char_array_size_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_4, fuzz_char_array_size_4, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_5, fuzz_char_array_size_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_7, fuzz_char_array_size_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	
		const char * fuzzer_var_lou_compileString_0_0 = fuzz_char_array_1;
		const char * fuzzer_var_lou_translatePrehyphenated_5_0 = fuzz_char_array_4;
		const char * fuzzer_var_lou_backTranslateString_6_0 = fuzz_char_array_5;
		const char * fuzzer_var_lou_dotsToChar_8_0 = fuzz_char_array_7;//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Read table list from input (first line up to newline or end)
    char tableList[256] = {0};
    fgets(tableList, sizeof(tableList) - 1, in_file);
    
    // Remove newline if present
    size_t tableLen = strlen(tableList);
    if (tableLen > 0 && tableList[tableLen - 1] == '\n') {
        tableList[tableLen - 1] = '\0';
    }
    
    // If no table specified, use a default
    if (tableLen == 0 || tableList[0] == '\0') {
        strcpy(tableList, "en-us-g2.ctb");
    }

    // Try to compile table from string (using remaining input data)
    long remaining = size - ftell(in_file);
    if (remaining > 0) {
        char *compileString = (char *)malloc(remaining + 1);
        if (compileString) {
            fread(compileString, 1, remaining, in_file);
            compileString[remaining] = '\0';
            lou_compileString(fuzzer_var_lou_compileString_0_0, compileString);
            free(compileString);
        }
    }

    assert_file_closed(&in_file);;

    // Get table info
    char *tableInfo = lou_getTableInfo(tableList, fuzz_str_2);
    if (tableInfo) {
        lou_freeTableInfo(tableInfo);
    }

    // Check if table is valid
    if (lou_checkTable(tableList) != 0) {
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Prepare input buffer from data
    std::vector<widechar> inbuf;
    size_t maxChars = size / sizeof(widechar);
    if (maxChars > 1024) maxChars = 1024;
    
    if (maxChars > 0) {
        inbuf.resize(maxChars);
        memcpy(inbuf.data(), data, maxChars * sizeof(widechar));
    } else {
        // If no widechars, create some test data
        inbuf = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    }

    int inlen = inbuf.size();
    std::vector<widechar> outbuf(inlen * 2);
    int outlen = outbuf.size();
    
    std::vector<formtype> typeform(inlen, 0);
    std::vector<char> spacing(inlen, 0);
    std::vector<int> outputPos(inlen * 2, 0);
    std::vector<int> inputPos(inlen * 2, 0);
    std::vector<int> cursorPos(1, 0);
    std::vector<char> hyphens(inlen, 0);
    std::vector<char> inputHyphens(inlen, 0);
    std::vector<char> outputHyphens(inlen * 2, 0);

    // 1. Translate
    int translateResult = lou_translate(
        tableList,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform.data(),
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        fuzz_int32_t_3
    );

    // 2. Translate pre-hyphenated
    if (translateResult > 0 && outlen > 0) {
        int inlen2 = inlen;
        int outlen2 = outbuf.size();
        lou_translatePrehyphenated(
            fuzzer_var_lou_translatePrehyphenated_5_0,
            inbuf.data(),
            &inlen2,
            outbuf.data(),
            &outlen2,
            typeform.data(),
            spacing.data(),
            outputPos.data(),
            inputPos.data(),
            cursorPos.data(),
            inputHyphens.data(),
            outputHyphens.data(),
            0
        );
    }

    // 3. Back translate string
    if (translateResult > 0 && outlen > 0) {
        int backInlen = outlen;
        int backOutlen = inbuf.size();
        std::vector<widechar> backOutbuf(backOutlen);
        lou_backTranslateString(
            fuzzer_var_lou_backTranslateString_6_0,
            outbuf.data(),
            &backInlen,
            backOutbuf.data(),
            &backOutlen,
            typeform.data(),
            spacing.data(),
            0
        );
    }

    // 4. Hyphenate
    int hyphenResult = lou_hyphenate(
        tableList,
        inbuf.data(),
        inlen,
        hyphens.data(),
        fuzz_int32_t_6
    );

    // 5. Dots to char (using output from translate as input)
    if (translateResult > 0 && outlen > 0) {
        std::vector<widechar> dotsToCharOut(outlen);
        int dotsResult = lou_dotsToChar(
            fuzzer_var_lou_dotsToChar_8_0,
            outbuf.data(),
            dotsToCharOut.data(),
            outlen,
            fuzz_int32_t_8
        );
    }

    // Clean up
    lou_free();
    
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}