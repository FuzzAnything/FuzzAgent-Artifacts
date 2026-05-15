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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"lou_registerTableResolver":[[4880,6,4880,15,0,0,4,0]]},"library_calls":["lou_registerTableResolver","lou_listTables","lou_freeTableFiles","lou_free","lou_indexTables","lou_checkTable","lou_freeTableFiles","lou_free","lou_getTableInfo","lou_freeTableInfo","lou_freeTableFiles","lou_free","lou_compileString","lou_freeTableFiles","lou_free","lou_freeTableFiles","lou_free","lou_translate","lou_translatePrehyphenated","lou_backTranslateString","lou_hyphenate","lou_dotsToChar","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerTableResolver","lou_listTables","lou_indexTables","lou_checkTable","lou_getTableInfo","lou_freeTableInfo","lou_compileString","lou_translate","lou_translatePrehyphenated","lou_backTranslateString","lou_hyphenate","lou_dotsToChar","lou_freeTableFiles","lou_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Early exit for too small inputs
    if (size < sizeof(widechar) * 2) {
	return 0;
	}
	

    // 1. Register table resolver (using default NULL)
    lou_registerTableResolver(NULL);

    // 2. List available tables
    char **tables = lou_listTables();
    if (!tables || !tables[0]) {
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }

    // 3. Index tables
    lou_indexTables(const_cast<const char **>(tables));

    // 4. Check first table
    if (lou_checkTable(tables[0]) != 0) {
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }

    // 5. Get table info
    char *info = lou_getTableInfo(tables[0], fuzz_str_1);
    if (info) {
        lou_freeTableInfo(info);
    }

    // 6. Create FILE* from input data for hypothetical table compilation
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        lou_freeTableFiles(tables);
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. Compile a simple test string (use first 100 bytes if available)
    const char compile_str[] = "patterns test\n";
    lou_compileString(tables[0], fuzz_str_2);

    // 8. Prepare translation buffers from input data
    // Use first half of data as input buffer (as widechar array)
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count < 2) {
        assert_file_closed(&in_file);;
        lou_freeTableFiles(tables);
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }

    widechar *inbuf = (widechar *)data;
    int inlen = static_cast<int>(widechar_count / 2); // Use half for input
    
    // Allocate output buffers
    widechar *outbuf = (widechar *)malloc(sizeof(widechar) * inlen * 2);
    int outlen = inlen * 2;
    
    formtype *typeform = (formtype *)calloc(inlen, sizeof(formtype));
    char *spacing = (char *)calloc(inlen, sizeof(char));
    int *outputPos = (int *)calloc(inlen, sizeof(int));
    int *inputPos = (int *)calloc(inlen, sizeof(int));
    int *cursorPos = (int *)calloc(1, sizeof(int));
    char *inputHyphens = (char *)calloc(inlen, sizeof(char));
    char *outputHyphens = (char *)calloc(inlen * 2, sizeof(char));

    if (!outbuf || !typeform || !spacing || !outputPos || !inputPos || 
        !cursorPos || !inputHyphens || !outputHyphens) {
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        free(cursorPos);
        free(inputHyphens);
        free(outputHyphens);
        assert_file_closed(&in_file);;
        lou_freeTableFiles(tables);
        lou_free();
        assert_file_closed(&in_file);
	return 0;
    }

    // 9. Regular translation
    lou_translate(tables[0], inbuf, &inlen, outbuf, &outlen, 
                  typeform, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_3);

    // 10. Prehyphenated translation
    // Reset lengths
    inlen = static_cast<int>(widechar_count / 2);
    outlen = inlen * 2;
    
    lou_translatePrehyphenated(tables[0], inbuf, &inlen, outbuf, &outlen,
                               typeform, spacing, outputPos, inputPos,
                               cursorPos, inputHyphens, outputHyphens, 0);

    // 11. Back translation
    lou_backTranslateString(tables[0], inbuf, &inlen, outbuf, &outlen,
                            typeform, spacing, 0);

    // 12. Hyphenation
    char *hyphens = (char *)malloc(inlen);
    if (hyphens) {
        lou_hyphenate(tables[0], inbuf, inlen, hyphens, fuzz_int32_t_4);
        free(hyphens);
    }

    // 13. Dots to char conversion
    // Use second half of data for dots input
    widechar *dots_in = inbuf + inlen;
    int dots_len = static_cast<int>((widechar_count - inlen));
    if (dots_len > 0) {
        widechar *dots_out = (widechar *)malloc(sizeof(widechar) * dots_len);
        if (dots_out) {
            lou_dotsToChar(tables[0], dots_in, dots_out, dots_len, fuzz_int32_t_5);
            free(dots_out);
        }
    }

    // 14. Create output file for logging
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
    }

    // 15. Cleanup
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(inputHyphens);
    free(outputHyphens);
    assert_file_closed(&in_file);;
    lou_freeTableFiles(tables);
    lou_free();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}